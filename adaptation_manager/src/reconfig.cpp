//*****************
/* author : El Mehdi ABDALI    Laboratory: Institut Pascal, CNRS UMR 6602, FRANCE        */
/* contact information : el_mehdi.abdali@uca.fr  , elmehdi.abdali@gmail.com              */

#include "reconfig.h"

/* start trigger module */
void start_reconfiguration(int fd_mem, int bitstream_address, int region_id)
{
   int param_address;
   int start_done_word = 0x80000000;

   start_done_word = start_done_word + (0x30000000 & (region_id<<28)) + (0x0FFFFFFF & bitstream_address);
   int *pointer_param= &start_done_word;

   setimg((uchar*)pointer_param, fd_mem, 0, LW_HPS2FPGA_AXI_MASTER, 1, 4);
}

/* read done module */
int reconfiguration_done(int fd_mem)
{
   int param_address;
   int start_done_word;
   int *pointer_param= &start_done_word;
   int value;

   getimg((uchar*)pointer_param, fd_mem, 0, LW_HPS2FPGA_AXI_MASTER, 1, 4);
   
   if ((*pointer_param & 0x40000000) == 2)
         value=1;
   else
         value=0;

   return value;
}
//******************