/*************************************************************************************
 * File   : pr_internal_host.h, file for partial dynamic reconfiguration
 * Copyright (C) 2018 Lab-STICC Laboratory
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
#ifndef PR_INTERNAL_HOST_H
#define PR_INTERNAL_HOST_H

long fsize(FILE *fp);
char* load_bitstream(char* bitstream_path, long *fileSize);

static enum fpga_mgr_states pr_controler_state(volatile unsigned int *pr_base_iomem);
unsigned int * pr_controler_init(int fd_mem);
int pr_controler_release(unsigned int *pr_base_iomem);
int pr_controler_start(volatile unsigned int *pr_base_iomem);
int pr_controler_write(volatile unsigned int *pr_base_iomem,const char *buf, long count);
void pr_controler_write_complete(volatile unsigned int *pr_base_iomem);

void FPGA_reconfiguration(int fd_mem,char *rbf_path);



#endif
