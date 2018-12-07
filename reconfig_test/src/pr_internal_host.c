#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "stdint.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

#define ALT_PR_DATA_OFST		0x00
#define ALT_PR_CSR_OFST			0x01
#define BIT(n) (UINT32_C(1) << (n))    

#define ALT_PR_CSR_PR_START		0x1
#define ALT_PR_CSR_STATUS_SFT		2
#define ALT_PR_CSR_STATUS_MSK		(7 << ALT_PR_CSR_STATUS_SFT)
#define ALT_PR_CSR_STATUS_NRESET	(0 << ALT_PR_CSR_STATUS_SFT)
#define ALT_PR_CSR_STATUS_PR_ERR	(1 << ALT_PR_CSR_STATUS_SFT)
#define ALT_PR_CSR_STATUS_CRC_ERR	(2 << ALT_PR_CSR_STATUS_SFT)
#define ALT_PR_CSR_STATUS_BAD_BITS	(3 << ALT_PR_CSR_STATUS_SFT)
#define ALT_PR_CSR_STATUS_PR_IN_PROG	(4 << ALT_PR_CSR_STATUS_SFT)
#define ALT_PR_CSR_STATUS_PR_SUCCESS	(5 << ALT_PR_CSR_STATUS_SFT)
#define ALT_PR_ADDRESSS_OFFSET	0x40000
#define TIMEOUT_US 10
#define LW_HPS2FPGA_AXI_MASTER  0xFF200000

enum fpga_mgr_states {
	/* default FPGA states */
	FPGA_MGR_STATE_UNKNOWN,
	FPGA_MGR_STATE_POWER_OFF,
	FPGA_MGR_STATE_POWER_UP,
	FPGA_MGR_STATE_RESET,

	/* getting an image for loading */
	FPGA_MGR_STATE_FIRMWARE_REQ,
	FPGA_MGR_STATE_FIRMWARE_REQ_ERR,

	/* write sequence: init, write, complete */
	FPGA_MGR_STATE_WRITE_INIT,
	FPGA_MGR_STATE_WRITE_INIT_ERR,
	FPGA_MGR_STATE_WRITE,
	FPGA_MGR_STATE_WRITE_ERR,
	FPGA_MGR_STATE_WRITE_COMPLETE,
	FPGA_MGR_STATE_WRITE_COMPLETE_ERR,

	/* fpga is programmed and operating */
	FPGA_MGR_STATE_OPERATING,
};


static enum fpga_mgr_states pr_controler_state(volatile unsigned int *pr_base_iomem)
{

	volatile uint32_t val;
	val = *(pr_base_iomem+ALT_PR_CSR_OFST);
	enum fpga_mgr_states ret = FPGA_MGR_STATE_UNKNOWN;
			
	const char *err = "unknown";
	
	val &= ALT_PR_CSR_STATUS_MSK;
	printf("pr_controler csr :  %x \n",val);

	switch (val) {
	case ALT_PR_CSR_STATUS_NRESET:
		printf("pr_controler power_up or nreset :  %x !!!\n",val);
		return FPGA_MGR_STATE_RESET;

	case ALT_PR_CSR_STATUS_PR_ERR:
		err = "pr error";
		printf("pr_controler error :  %s !!!\n",err);
		ret = FPGA_MGR_STATE_WRITE_ERR;
		break;

	case ALT_PR_CSR_STATUS_CRC_ERR:
		err = "crc error";
		printf("pr_controler error :  %s !!!\n",err);
		ret = FPGA_MGR_STATE_WRITE_ERR;
		break;

	case ALT_PR_CSR_STATUS_BAD_BITS:
		err = "bad bits";
		printf("pr_controler error :  %s !!!\n",err);
		ret = FPGA_MGR_STATE_WRITE_ERR;
		break;

	case ALT_PR_CSR_STATUS_PR_IN_PROG:
		printf("pr_controler in progress :  %s !!!\n",err);
		return FPGA_MGR_STATE_WRITE;

	case ALT_PR_CSR_STATUS_PR_SUCCESS:
		printf("pr_controler success :  %s !!!\n",err);
		return FPGA_MGR_STATE_OPERATING;

	default:
		break;
	}
	return ret;

}

long fsize(FILE *fp){
    fseek(fp, 0L, SEEK_END);
    long sz=ftell(fp);
    fseek(fp,0,SEEK_SET); //go back to where we were
    return sz;
}


char* load_bitstream(char* bitstream_path, long *fileSize)
{
    FILE* file = fopen(bitstream_path, "rb");
    if(file == NULL)
    {
        printf("ERROR: could not open %s file\n",bitstream_path);
		exit (1);
    }

    *fileSize = fsize(file);
    printf("\n FileSize=%ld",*fileSize);

    char *buffer = (char *)malloc(*fileSize + 1);
    size_t test = fread(buffer, 1,*fileSize,file);
    printf("\n nb read=%ld\n",test);
	
    fclose(file);
    return buffer;
}


unsigned int * pr_controler_init(int fd_mem)
{
	unsigned int *pr_base_iomem;
	
	/* map pr_controler to user address space  */
	pr_base_iomem = (unsigned int *)mmap(NULL, 4096, (PROT_READ | PROT_WRITE), 
							MAP_SHARED, fd_mem, LW_HPS2FPGA_AXI_MASTER+ALT_PR_ADDRESSS_OFFSET); 
	if (pr_base_iomem == MAP_FAILED)
	{
		printf("ERROR: pr controller mmap() failed...\n");
		exit (1);
	}
	return pr_base_iomem;
}


int pr_controler_release(unsigned int *pr_base_iomem)
{
	if (munmap(pr_base_iomem, 4096) != 0)
	{
		printf("ERROR: munmap() failed...\n");
	}
}

int pr_controler_start(volatile unsigned int *pr_base_iomem)
{
	volatile uint32_t val;

	val = *(pr_base_iomem+ALT_PR_CSR_OFST);
	if (val & ALT_PR_CSR_PR_START) {
		printf("warning : Partial Reconfiguration already started\n");
		if (pr_controler_state(pr_base_iomem)!=ALT_PR_CSR_STATUS_PR_IN_PROG) 
			return -1;
	}
	//start	
	// *(pr_base_iomem+ALT_PR_CSR_OFST)=val | ALT_PR_CSR_PR_START; 
	*(pr_base_iomem+0x1)=0x1; 
	val = *(pr_base_iomem+ALT_PR_CSR_OFST);
	if (val & ALT_PR_CSR_PR_START) 
		printf("Partial Reconfiguration started\n");
	 /* poll the status register until FPGA_MGR_STATE_WRITE : ALT_PR_CSR_STATUS_PR_IN_PROG  */
	while (pr_controler_state(pr_base_iomem)!=FPGA_MGR_STATE_WRITE)
	{
		usleep(1);
	}
	return 0;
}

int pr_controler_write(volatile unsigned int *pr_base_iomem,const char *buf, long count)
{

	uint32_t *buffer_32 = (uint32_t *)buf;
	long i = 0;

	if (count <= 0)
		return -1;
		
	/* Write out the complete 32-bit chunks */
	while (count >= sizeof(uint32_t)) {
		*(pr_base_iomem)=buffer_32[i++];
		count -= sizeof(uint32_t);
	}

	/* Write out remaining non 32-bit chunks */
	switch (count) {
	case 3:
		*(pr_base_iomem)=buffer_32[i++] & 0x00ffffff;
		break;
	case 2:
		*(pr_base_iomem)=buffer_32[i++] & 0x0000ffff;
		break;
	case 1:
		*(pr_base_iomem)=buffer_32[i++] & 0x000000ff;
		break;
	case 0:
		break;
	default:
		/* This will never happen */
		return -1;
	}
	if (pr_controler_state(pr_base_iomem)==	FPGA_MGR_STATE_WRITE_ERR)
		return -1;	
	return 0;

}


void pr_controler_write_complete(volatile unsigned int *pr_base_iomem)
{
	int done_or_error=0;
	
	while (done_or_error==0)
	{
		switch(pr_controler_state(pr_base_iomem))
		{
		case FPGA_MGR_STATE_WRITE_ERR:		
			done_or_error=1;

		case FPGA_MGR_STATE_OPERATING:
			printf("successful partial reconfiguration\n");
			done_or_error=1;

		default:
			break;
		}
	}
}



void FPGA_reconfiguration(int fd_mem,char *rbf_path)
{
	unsigned int *pr_base_iomem;
	char *bitstream;
	long count;
	/* map pr_controler to user address space  */
	pr_base_iomem=pr_controler_init(fd_mem);
	/* write partial bitstream file in HPS DDR memory */
    	bitstream=load_bitstream(rbf_path,&count);
	if (pr_controler_start(pr_base_iomem)==0)
	{
		pr_controler_write(pr_base_iomem,bitstream,count);
		pr_controler_write_complete(pr_base_iomem);
		pr_controler_release(pr_base_iomem);
	}
}
