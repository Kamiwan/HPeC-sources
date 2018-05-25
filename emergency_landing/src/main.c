/*************************************************************************************
 * File   : main.c, file for Site Landing Research
 *
 * Copyright (C) 2016 Lab-Sticc Laboratory
 * Author(s) :  Dominique Heller, dominique.heller@univ-ubs.fr
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 *************************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "image.h"
#include <time.h>
#include <unistd.h>

#include <fcntl.h>
#include <sys/mman.h>

#define SDRAM_SPAN   ( 0x04000000 )
#define HW_REGS_SPAN ( 0x00020000 )
#define HWREG_BASE   ( 0xff200000 )


unsigned int cma_base_addr;
void *virtual_base_sdram;
void *virtual_base_hwreg;

//DEGUG only #define WRITE_IMG
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
//source ppm color image 24bits RGB (packed) 
// The video IP cores used for edge detection require the RGB 24 bits of each pixel to be
// word aligned (aka 1 byte of padding per pixel). | unused 8 bits  | red 8 bits | green 8 bits | blue 8 bits |
void memcpy_consecutive_to_padded(unsigned char* from, volatile unsigned int * to, int pixels){
	int i;
	for (i=0;i<pixels;i++){
		*(to+i) = (((unsigned int)*((unsigned char*)(from+i*3+2)))&0xff) | ((((unsigned int)*((unsigned char*)(from+i*3+1)))<<8)&0xff00) | ((((unsigned int)*((unsigned char*)(from+i*3+0)))<<16)&0xff0000);
	}
}

/* 
void memcpy_padded_to_consecutive(volatile unsigned char * from,  unsigned char*  to, int pixels){
	 int i;
	for (i=0;i<pixels;i++){
		(*(to+i))=(*(from+i));
	}
}
*/


int main(int argc, char *argv[])
{
    PPM_IMG img_ibuf_color;
    PGM_IMG img_ibuf_g;
    PGM_IMG img_ibuf_ceh;
    //PGM_IMG img_ibuf_me1;
    PGM_IMG img_ibuf_me;
    PGM_IMG img_ibuf_bw;
    PGM_IMG img_ibuf_ccl;
    PGM_IMG img_ibuf_mop;
    PGM_IMG img_ibuf_mop2;
    PGM_IMG img_ibuf_mop3;
    PGM_IMG img_ibuf_ero;
    PGM_IMG img_ibuf_gs;
    PGM_IMG img_ibuf_sp;
    PGM_IMG img_ibuf_zon;
    PGM_IMG img_ibuf_se;
    PGM_IMG img_ibuf_ms;
    int nb_labels;
    COMPONENT *ccl;
    int hardware = 0;
    int fd;

    FILE* cma_dev_file;
    char cma_addr_string[10];
    time_t start, end;
	
    volatile unsigned int *mem_to_stream_dma=NULL;
    volatile unsigned int *stream_to_mem_dma=NULL;
    volatile unsigned int *mem_to_stream_dma_buffer = NULL;
    volatile unsigned char *stream_to_mem_dma_buffer = NULL;

   struct pixel * data;
   unsigned char * header;


    printf("Main\r\n");
    printf("Running Vision Based Autonomous Landing for ppm color images.\r\n");
    // Check inputs
    if (argc < 3){
	printf("Usage: sudo vbal <PPM filename> <hardware>\n\nhardware: true false\n");
	return 0;
    }
    // Determine hardware mode
    if (argv[2][0] == 't')
    {
    	hardware = 1;

    	//init hardware
    	// Open /dev/mem
    	if( ( fd = open( "/dev/mem", ( O_RDWR | O_SYNC ) ) ) == -1 ) {
        	printf( "ERROR: could not open \"/dev/mem\"...\n" );
        	return( 1 );
    	}	
    	// Open /dev/cma_block
    	if( ( cma_dev_file = fopen("/dev/cma_block", "r") ) == NULL ) {
        	printf( "ERROR: could not open \"/dev/cma_block\"...\n" );
        	close( fd );
        	return( 1 );
    	}
	
	// Determine the CMA block physical address
    	fgets(cma_addr_string, sizeof(cma_addr_string), cma_dev_file);
    	cma_base_addr = strtol(cma_addr_string, NULL, 16);
    	printf("Use CMA memory block at phys addr: 0x%x\n",cma_base_addr);
	
    	fclose(cma_dev_file);
    
    	// get virtual addr that maps to the sdram region (through HPS-FPGA non-lightweight bridge)
    	virtual_base_sdram = mmap( NULL, SDRAM_SPAN, ( PROT_READ | PROT_WRITE ), MAP_SHARED, fd, cma_base_addr );    
    	if( virtual_base_sdram == MAP_FAILED ) {
        	printf( "ERROR: mmap() failed...\n" );
        	close( fd ); 
        	return(1);
    	}
	
	virtual_base_hwreg = mmap( NULL, HW_REGS_SPAN, ( PROT_READ | PROT_WRITE ), MAP_SHARED, fd, HWREG_BASE );    
	if( virtual_base_hwreg == MAP_FAILED ) {
        	printf( "ERROR: mmap() failed...\n" );
        	close( fd ); 
        	return(1);
    	}
	
	mem_to_stream_dma = (volatile unsigned int *)(virtual_base_hwreg + 0x3100);
	stream_to_mem_dma = (volatile unsigned int *)(virtual_base_hwreg + 0x3120);
	mem_to_stream_dma_buffer = (volatile unsigned int *)(virtual_base_sdram);
	stream_to_mem_dma_buffer = (volatile unsigned char *)(virtual_base_sdram + 0x2000000);

    }
    printf("Loading ppm image %s.\r\n",argv[1]);
     img_ibuf_color = read_ppm(argv[1]);
    if (hardware==0)
    {
	    //printf("Rgb2gray...\r\n");
	    printf("Rgb2gray, median filter, canny edge detector, morphological closing in sw...\r\n");
	    // Start measuring time
	    start = clock();
	    img_ibuf_g = rgb2gray(img_ibuf_color);
	    free_ppm(img_ibuf_color);
	    #ifdef WRITE_IMG
        	   write_pgm(img_ibuf_g, "gray.pgm");
	    #endif
	    /* printf("Contrast Enhencement...\r\n");
	    img_ibuf_ceh=contrast_enhancement_gray(img_ibuf_g);
	    #ifdef WRITE_IMG
        	   write_pgm(img_ibuf_ceh, "contrast.pgm");
	    #endif
	    free_pgm(img_ibuf_g); 
	    printf("Median filter : Noise reduction...\r\n");
	    img_ibuf_me1=medianfilter(img_ibuf_ceh);
	    img_ibuf_me=medianfilter(img_ibuf_me1);
	    #ifdef WRITE_IMG
	           write_pgm(img_ibuf_me, "median.pgm");
	    #endif */
	    //free_pgm(img_ibuf_ceh); 
	    img_ibuf_me=medianfilter(img_ibuf_g);
	    #ifdef WRITE_IMG
	           write_pgm(img_ibuf_me, "median.pgm");
	    #endif
	    free_pgm(img_ibuf_g); 
	    //printf("Gaussian Filter...\r\n");
	    img_ibuf_gs=gaussianfilter(img_ibuf_me);
    	    #ifdef WRITE_IMG
	       write_pgm(img_ibuf_gs, "gauss.pgm");
	    #endif
	    //free_pgm(img_ibuf_me1);
	    //printf("Sobel Edge Detection...\r\n");
	    img_ibuf_sp.w = img_ibuf_gs.w;
	    img_ibuf_sp.h = img_ibuf_gs.h;
	    img_ibuf_sp.img = (unsigned char *)malloc(img_ibuf_sp.w * img_ibuf_sp.h * sizeof(unsigned char));
	    img_ibuf_se.w = img_ibuf_gs.w;
	    img_ibuf_se.h = img_ibuf_gs.h;
	    img_ibuf_se.img = (unsigned char *)malloc(img_ibuf_se.w * img_ibuf_se.h * sizeof(unsigned char));
	    sobel(img_ibuf_gs,img_ibuf_sp,img_ibuf_se);
	    free_pgm(img_ibuf_gs);
	    #ifdef WRITE_IMG
		    write_pgm(img_ibuf_sp, "sobel.pgm");
		    write_pgm(img_ibuf_se, "edge.pgm");
	    #endif
	    //printf("Canny Non Maxima Suppression...\r\n");
	    img_ibuf_ms=canny(img_ibuf_sp,img_ibuf_se,20,80);
	    #ifdef WRITE_IMG
	    	write_pgm(img_ibuf_ms, "canny.pgm");
	    #endif
	    //printf("Morphological operator: dilate, erode...\r\n");
	    //dilate
	    img_ibuf_mop=dilate(img_ibuf_ms);
	    #ifdef WRITE_IMG
	    	write_pgm(img_ibuf_mop, "dilate.pgm");
	    #endif
	    img_ibuf_mop2=dilate(img_ibuf_mop);
	    #ifdef WRITE_IMG
	    	write_pgm(img_ibuf_mop2, "dilate2.pgm");
	    #endif
	    img_ibuf_mop3=dilate(img_ibuf_mop2);
	   #ifdef WRITE_IMG
	   	write_pgm(img_ibuf_mop3, "dilate3.pgm");
	    #endif
	   //erode
	   img_ibuf_ero=erode(img_ibuf_mop3);
	   end = clock();
	   //#ifdef WRITE_IMG
	   	write_pgm(img_ibuf_ero, "erode.pgm");
	   //#endif
	   free_pgm(img_ibuf_mop);
	   free_pgm(img_ibuf_mop2);
	   free_pgm(img_ibuf_mop3);
	   free_pgm(img_ibuf_sp);
	   free_pgm(img_ibuf_se);
	   free_pgm(img_ibuf_ms);
     }
     else //harware accelerator
     {


	
	// Image dimensions
	int width =img_ibuf_color.w;
       int height = img_ibuf_color.h; 

 	printf("Rgb2gray, median filter, canny edge detector, morphological closing in hw...\r\n");
	// Start measuring time
	start = clock();
	
	// Turn off DMAs
	*(mem_to_stream_dma+3) = 0;
	*(stream_to_mem_dma+3) = 0;
	
	// Write the image to the mem-to-stream buffer
	memcpy_consecutive_to_padded(img_ibuf_color.img, mem_to_stream_dma_buffer, width*height);
	
	// Configure DMAs to the correct addresses
	*(stream_to_mem_dma+1) = cma_base_addr+0x2000000;
	*(mem_to_stream_dma+1) = cma_base_addr;
	// Turn on the DMAs and start the swaps
	*(stream_to_mem_dma+3) = 4;
	*(mem_to_stream_dma+3) = 4;
	*(mem_to_stream_dma) = 1;
	*(stream_to_mem_dma) = 1;
	// wait until the swap is complete
	while((*(mem_to_stream_dma+3))&0x1 || (*(stream_to_mem_dma+3))&0x1 ){}
	// Write the same addr to the back buffer reg
	*(stream_to_mem_dma+1) = cma_base_addr+0x2000000;
	*(mem_to_stream_dma+1) = cma_base_addr;
		
	// Write to the buffer reg to start swap
	*(mem_to_stream_dma) = 1;
	*(stream_to_mem_dma) = 1;
	 
	// wait until the swap is complete
	while((*(mem_to_stream_dma+3))&0x1 || (*(stream_to_mem_dma+3))&0x1 ){}
	
	// Turn off DMAs
	*(mem_to_stream_dma+3) = 0;
	*(stream_to_mem_dma+3) = 0;
	
	// Copy the edge detected image from the stream-to-mem buffer
    	img_ibuf_ero.w = width;
    	img_ibuf_ero.h = height;
    	//img_ibuf_ero.img = (unsigned char *)malloc(img_ibuf_ero.w * img_ibuf_ero.h * sizeof(unsigned char));
	//memcpy_padded_to_consecutive(stream_to_mem_dma_buffer, (unsigned char*)img_ibuf_ero.img,
	//				 img_ibuf_ero.w*img_ibuf_ero.h);
	img_ibuf_ero.img=(unsigned char *)stream_to_mem_dma_buffer;//zero copy
	end = clock();
	write_pgm(img_ibuf_ero, "erode.pgm");

	/* zero copy if( munmap( virtual_base_sdram, SDRAM_SPAN ) != 0 ) {
        	printf( "ERROR: munmap() failed...\n" );
	        close( fd );
        	return( 1 );
	    }*/
	if( munmap( virtual_base_hwreg, HW_REGS_SPAN ) != 0) {
        	printf( "ERROR: munmap() failed...\n" );
	        close( fd ); 
        	return( 1 );
	}
	close( fd ); 

     }
    //search big enough zones
    int size_zone=10;
    img_ibuf_zon=zone(img_ibuf_ero,size_zone);
    //#ifdef WRITE_IMG
    	write_pgm(img_ibuf_zon, "zone.pgm");
    //#endif
    printf("Connected components labeling...\r\n");
    img_ibuf_ccl=connected_component_labeling(img_ibuf_zon,&nb_labels,&ccl,200,100000);
    printf("Nb connected components = %d \r\n",nb_labels);
    #ifdef WRITE_IMG
    write_pgm(img_ibuf_ccl, "ccl.pgm");
    #endif
    //zero copy free_pgm(img_ibuf_ero);
    if (hardware==1)
    {
    if( munmap( virtual_base_sdram, SDRAM_SPAN ) != 0 ) {
        	printf( "ERROR: munmap() failed...\n" );
	        close( fd );
        	return( 1 );
	    }
    }
    free_pgm(img_ibuf_ccl);
    free_pgm(img_ibuf_zon);
	printf("TIME ELAPSED: %.0f ms\n", ((double) (end - start)) * 1000 / CLOCKS_PER_SEC);
    printf("Exit main...\r\n");
    exit(0);
    return 0;
}
