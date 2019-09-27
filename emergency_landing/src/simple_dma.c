#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "stdint.h"
#include <unistd.h>
#include <stdbool.h>
#include "simple_dma.h"


void stop_dma(volatile unsigned int *dma_base_iomem)
{
	volatile uint32_t val=0;
	*(dma_base_iomem+SIMPLE_DMA_CSR_OFST)=SIMPLE_DMA_CSR_STOP;
	while (val==0)
	{
		val = *(dma_base_iomem+SIMPLE_DMA_STATUS_OFST);
		val &= SIMPLE_DMA_STATUS_STOPPED;
	}
	//reset status register
	*(dma_base_iomem+SIMPLE_DMA_STATUS_OFST)=0;
	//reset control register
	*(dma_base_iomem+SIMPLE_DMA_CSR_OFST)=0;

}

void reset_dma(volatile unsigned int *dma_base_iomem)
{
	volatile uint32_t val;
	val = *(dma_base_iomem+SIMPLE_DMA_STATUS_OFST);
	val &= SIMPLE_DMA_STATUS_BUSY;
	if (val != 0)
	{
		*(dma_base_iomem+SIMPLE_DMA_CSR_OFST)=SIMPLE_DMA_CSR_RESET;
		while (val==0)
		{
			val = *(dma_base_iomem+SIMPLE_DMA_STATUS_OFST);
			val &= SIMPLE_DMA_STATUS_FLUSH;
		}
		//reset status register
		*(dma_base_iomem+SIMPLE_DMA_STATUS_OFST)=0;
		//reset control register
		*(dma_base_iomem+SIMPLE_DMA_CSR_OFST)=0;
	 } 
	
}


void start_dma(volatile unsigned int *dma_base_iomem,unsigned int addr,unsigned int length)
{
	 unsigned int val=0;
	*(dma_base_iomem+SIMPLE_DMA_ADDRESS_OFST)=addr;
	*(dma_base_iomem+SIMPLE_DMA_LENGTH_OFST)=length;
	*(dma_base_iomem+SIMPLE_DMA_CSR_OFST)=SIMPLE_DMA_CSR_GO;
	while (val==0)
	{
		val = *(dma_base_iomem+SIMPLE_DMA_STATUS_OFST);
		val &= SIMPLE_DMA_STATUS_BUSY;
	}
	
}


bool transfer_done(volatile unsigned int *dma_base_iomem)
{
	volatile uint32_t val=0;
	while (val==0)
	{
		val = *(dma_base_iomem+SIMPLE_DMA_STATUS_OFST);
		val &= SIMPLE_DMA_STATUS_DONE;
	}
	return true;
}
