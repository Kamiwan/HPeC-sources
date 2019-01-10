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
