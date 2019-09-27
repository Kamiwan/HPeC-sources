#ifndef SIMPLE_DMA_H
#define SIMPLE_DMA_H


#define SIMPLE_DMA_STATUS_OFST  0x0	
#define SIMPLE_DMA_ADDRESS_OFST  0x1	
#define SIMPLE_DMA_LENGTH_OFST	0x2	
#define SIMPLE_DMA_CSR_OFST    0x3	


/* control 
	Bit 0 --> clear (soft reset)
	Bit 1 --> stop (stop dma)
	Bit 2 --> Go (starts transfer)
	Bit 3 --> I_EN (enable interrupt when all Reads complete)
*/


#define SIMPLE_DMA_CSR_IE 0x8
#define SIMPLE_DMA_CSR_GO 0x4
#define SIMPLE_DMA_CSR_STOP 0x2
#define SIMPLE_DMA_CSR_RESET 0x1



/**************************************************************
 Status register (and interrupt)
 	  Bit 0 --> Done (DMA has completed transfer)
	  Bit 1 --> Busy (DMA is currently tranfering data)
	  Bit 2 --> Early_done (DMA has completed read transfer)
	  Bit 3 --> Stopped (DMA is currently stopped)
	  Bit 4 --> Flush (DMA is currently flushed)

 **************************************************************/

#define SIMPLE_DMA_STATUS_DONE 0x1
#define SIMPLE_DMA_STATUS_BUSY 0x2
#define SIMPLE_DMA_STATUS_STOPPED 0x8
#define SIMPLE_DMA_STATUS_FLUSH 0x10



void stop_dma(volatile unsigned int *dma_base_iomem);
void reset_dma(volatile unsigned int *dma_base_iomem);
void start_dma(volatile unsigned int *dma_base_iomem,unsigned int addr,unsigned int length);
bool transfer_done(volatile unsigned int *dma_base_iomem);

#endif
