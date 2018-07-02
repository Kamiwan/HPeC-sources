/**
 * \file sgdma_dispatcher.h
 *
 * \description SGDMA Dispatcher interface class
 *
 *
 * \copyright Critical Link LLC 2013
 */
#ifndef SGDMA_DISPATCHER_H
#define SGDMA_DISPATCHER_H
#include "stdio.h"
#include "sgdma_dispatcher_regs.h"


class tcCSRMap;
/**
 *  The tcSDGMADispatcher class wraps a user space control class around the 
 *  Scatter Gather DMA master Avalon core in ST->MM or MM->ST mode.
 */
class tcSGDMADispatcher
{
public:
	tcSGDMADispatcher();
	tcSGDMADispatcher(unsigned int CtrlRegsAddr, 
						unsigned int DescRegsAddr,
		          		unsigned int RespRegsAddr, 
						bool EnhancedMode = false); 
						
	~tcSGDMADispatcher(void);

	void init(unsigned int CtrlRegsAddr, 
						unsigned int DescRegsAddr,
		          		unsigned int RespRegsAddr, 
						bool EnhancedMode = false);

	int WriteDescriptor(tsSGDMADescriptor& descriptor);

	int DisplayDescriptor(tsSGDMADescriptor& descriptor);

	tuSgdmaStatus GetStatusReg();
	void SetControlReg(int value);
protected:
	bool	 mbEnhanced; //!< when true, use enhanced descriptors
	tcCSRMap *mpCsr;
	tcCSRMap *mpDescriptors;
	tcCSRMap *mpResponse;
};

#endif
