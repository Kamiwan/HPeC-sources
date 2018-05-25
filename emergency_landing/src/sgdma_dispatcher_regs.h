/**
 * \file sgdma_dispatcher_regs.h
 *
 * \description Register address for the Modular SGDMA Dispatcher
 *
 *
 * \copyright Critical Link LLC 2013
 */
#ifndef SGDMA_DISPATCHER_REGS_H
#define SGDMA_DISPATCHER_REGS_H

#define CSR_STATUS_REG                      (0x0)
#define CSR_CONTROL_REG                      (0x4)

#define DESCRIPTOR_READ_ADDRESS_REG                      (0x0)
#define DESCRIPTOR_WRITE_ADDRESS_REG                     (0x1)
#define DESCRIPTOR_LENGTH_REG                            (0x2)
#define DESCRIPTOR_CONTROL_STANDARD_REG                  (0x3)

typedef union
{
	struct
	{
		unsigned int tx_chan			: 8;
		unsigned int gen_sop    		: 1;
		unsigned int gen_eop   			: 1;
		unsigned int park_rd			: 1;	
		unsigned int park_wr			: 1;	
		unsigned int end_on_eop			: 1;	
		unsigned int reserved_1			: 1;	
		unsigned int tx_cmplte_mask		: 1;	
		unsigned int early_term_mask	: 1;	
		unsigned int tx_error_mask		: 8;	
		unsigned int early_done			: 1;	
		unsigned int reserved_2			: 6;	
		unsigned int go					: 1;	
	} msBits;
	unsigned int mnWord;
} tuSGDMADescCtrl;

struct tsSGDMADescriptor
{
	unsigned int read_addr;
	unsigned int write_addr;
	unsigned int length;
	tuSGDMADescCtrl control;
};

typedef union
{
	struct
	{ unsigned int busy 			: 1;
		unsigned int desc_buf_empty	: 1;
		unsigned int desc_buf_full 	: 1;
		unsigned int desc_res_empty : 1;
		unsigned int desc_res_full 	: 1;
		unsigned int stopped 		: 1;
		unsigned int resetting 		: 1;
		unsigned int stop_on_err 	: 1;
		unsigned int stop_on_early 	: 1;
		unsigned int irq 			: 1;
		unsigned int reserved 		: 22;
	} msBits;
	unsigned int mnWord;
} tuSgdmaStatus;

typedef union
{
	struct
	{
		unsigned int stop_dispatcher 	: 1;
		unsigned int reset_dispatcher	: 1;
		unsigned int stop_on_err 		: 1;
		unsigned int stop_on_early 		: 1;
		unsigned int gl_int_en 			: 1;
		unsigned int stop_desc 			: 1;
		unsigned int reserved 			: 1;
	} msBits;
	unsigned int mnWord;
} tuSgdmaCtrl;


#endif /*SGDMA_DISPATCHER_REGS_H*/

