/* author : El Mehdi ABDALI    Laboratory: Institut Pascal, CNRS UMR 6602, FRANCE        */
/* contact information : el_mehdi.abdali@univ.bpclermont.fr  , elmehdi.abdali@gmail.com  */
/*											 */
/*											 */
/* setimg function writes an image into DDR memory                                       */
/* usage : initconfg(FILE* file, int fd_mem,int mem_str_adrss, int mem_cntrl_adrss)      */
/* FILE* file : binary file pointer                                                      */
/* int fd_mem : pointer to file sys/mem                                                  */
/* int mem_str_adrss : start address, the binary file physical address in target memory  */
/* int mem_cntrl_adrss : physical target memory address into HPS address space           */

#include "initimg.h"

int setimg(uchar* imgptr, int fd_mem,unsigned int mem_str_adrss, unsigned int mem_cntrl_adrss,size_t img_rws,size_t img_clmns)
{
 
    void *bridge_map;
    off_t mem_base = mem_cntrl_adrss;
    volatile int *int_mem_index;
    int page_cntr=0, int_word_adrss=mem_str_adrss;
    int shft_data=0;
    int rdcounter=0;
 
    while(rdcounter<((img_rws*img_clmns)/4)){

       /* mapping of the bridge address to the PAGE_SIZE and remapping the bridge address after */
       if(page_cntr==0){
          bridge_map = mmap(NULL, PAGE_SIZE, PROT_WRITE, MAP_SHARED, fd_mem, mem_base+int_word_adrss);
	  if (bridge_map == MAP_FAILED) {
	     perror("mmap");
             goto exit_sate;
	  }
       }

       int_mem_index = (int *) (bridge_map + page_cntr); 
       *int_mem_index = *(int*)(imgptr+shft_data);
       shft_data=shft_data+4;
       
       int_word_adrss=int_word_adrss+4;
       rdcounter++;
       /* unmap the PAGE_SIZE once all data inside are correclty read */
       if(page_cntr==(PAGE_SIZE-4)){
          if (munmap(bridge_map, PAGE_SIZE) < 0) {
             perror("munmap");
             goto exit_sate;
          }

          /*map agin the memory*/
          page_cntr=0;
       }
       else
          page_cntr=page_cntr+4;
    }

    /* unmap the previously mapped memory pages*/
    if (munmap(bridge_map, PAGE_SIZE) < 0) {
       perror("munmap");
    }
    exit_sate:
    return 0;
}



/** @author : El Mehdi ABDALI    Laboratory: Institut Pascal, CNRS UMR 6602, FRANCE      */
/* contact information : el_mehdi.abdali@univ.bpclermont.fr  , elmehdi.abdali@gmail.com  */
/*											 */
/*											 */
/* getimg function reads an image from DDR memory                                        */
/* usage : initconfg(FILE* file, int fd_mem,int mem_str_adrss, int mem_cntrl_adrss)      */
/* FILE* file : binary file pointer                                                      */
/* int fd_mem : pointer to file sys/mem                                                  */
/* int mem_str_adrss : start address, the binary file physical address in target memory  */
/* int mem_cntrl_adrss : physical target memory address into HPS address space           */



int getimg(uchar* imgptr, int fd_mem,int mem_str_adrss, int mem_cntrl_adrss,size_t img_rws,size_t img_clmns)
{
 
    void *bridge_map;
    off_t mem_base = mem_cntrl_adrss;
    volatile int *int_mem_index;
    int page_cntr=0, int_word_adrss=mem_str_adrss;
    int shft_data=0;
    int rdcounter=0;
    while(rdcounter<((img_rws*img_clmns)/4)){

       /* mapping of the bridge address to the PAGE_SIZE and remapping the bridge address after */
       if(page_cntr==0)
       {
          bridge_map = mmap(NULL, PAGE_SIZE, PROT_WRITE, MAP_SHARED, fd_mem, mem_base+int_word_adrss);
       	  if (bridge_map == MAP_FAILED) 
          {
	     perror("mmap");
             goto exit_sate;
	  }
       }

       int_mem_index = (int *) (bridge_map + page_cntr);
       *(int*)(imgptr+shft_data)=*int_mem_index ;

       //printf(" the pointer valus is %d\n", *int_mem_index);
       shft_data=shft_data+4;
       int_word_adrss=int_word_adrss+4;
       rdcounter++;
      
       /* unmap the PAGE_SIZE one all data inside are correclty read */
       if(page_cntr==(PAGE_SIZE-4)){
          if (munmap(bridge_map, PAGE_SIZE) < 0) {
             perror("munmap");
             goto exit_sate;
          }

          /*map agin the memory*/
          page_cntr=0;
       }
       else
          page_cntr=page_cntr+4;
    }

    /* unmap the previously mapped memory pages*/
    if (munmap(bridge_map, PAGE_SIZE) < 0) {
       perror("munmap");    } 
    exit_sate:
    return 0;
}




/** @author : El Mehdi ABDALI    Laboratory: Institut Pascal, CNRS UMR 6602, FRANCE      */
/* contact information : el_mehdi.abdali@univ.bpclermont.fr  , elmehdi.abdali@gmail.com  */
/*											 */
/*											 */
/* getimg function reads an image from DDR memory                                        */
/* usage : initconfg(FILE* file, int fd_mem,int mem_str_adrss, int mem_cntrl_adrss)      */
/* FILE* file : binary file pointer                                                      */
/* int fd_mem : pointer to file sys/mem                                                  */
/* int mem_str_adrss : start address, the binary file physical address in target memory  */
/* int mem_cntrl_adrss : physical target memory address into HPS address space           */

int get_array_mem(int* pageptr, int fd_mem,int mem_str_adrss, int mem_cntrl_adrss)
{
 
    void *bridge_map;
    off_t mem_base = mem_cntrl_adrss;
    volatile int *int_mem_index;
    int int_word_adrss=mem_str_adrss;


       /* mapping of the bridge address to the PAGE_SIZE and remapping the bridge address after */
          bridge_map = mmap(NULL, PAGE_SIZE, PROT_WRITE, MAP_SHARED, fd_mem, mem_base+int_word_adrss);
       	  if (bridge_map == MAP_FAILED) {
	     perror("mmap");
             goto exit_sate;
	     }

       int_mem_index = (int *) (bridge_map);
       *pageptr=*int_mem_index;    


       /* unmap the previously mapped memory page*/
       if (munmap(bridge_map, PAGE_SIZE) < 0) {
         perror("munmap");    }


    exit_sate:
    return 0;
}








