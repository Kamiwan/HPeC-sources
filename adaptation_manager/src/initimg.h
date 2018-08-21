//TODO Insert header comment

#ifndef INITIMG_H
#define INITIMG_H

#include <stdio.h>
#include <string.h>
#include <sys/mman.h> //EM, TODO: tell to Medhi it was missing this include in its file
#include "fpga_header.h"
#include <opencv2/opencv.hpp>


//EM, TODO: Ask to Medhi where these define are normally declared
//#define PROT_WRITE 0
//#define MAP_SHARED 0

int setimg(uchar* imgptr, int fd_mem,unsigned int mem_str_adrss, unsigned int mem_cntrl_adrss,size_t img_rws,size_t img_clmns);

int getimg(uchar* imgptr, int fd_mem,int mem_str_adrss, int mem_cntrl_adrss,size_t img_rws,size_t img_clmns);

int get_array_mem(int* pageptr, int fd_mem,int mem_str_adrss, int mem_cntrl_adrss);

#endif