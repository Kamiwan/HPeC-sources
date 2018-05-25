/**
 * \file sgdma_dispatcher.cpp
 *
 * \description SGDMA Dispatcher interface class
 *
 *
 * \copyright Critical Link LLC 2013
 */
#include "sgdma_dispatcher.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include "csr_map.h"
#include <iostream>

/**  
 * Empty Constructor
 *
 */
tcSGDMADispatcher::tcSGDMADispatcher()
{
	//Doing nothing, must use init() method before using this object
}

/**  
 * Constructor
 *
 * \param CtrlRegsAddr physical address of control register slave port
 * \param DescRegsAddr physical address of descriptor slave port
 * \param RespRegsAddr physical address of ST->MM Response slave port (set to zero if not used)
 * \param EnhanceMode set to true to use enhanced descriptors.
 */
tcSGDMADispatcher::tcSGDMADispatcher(unsigned int CtrlRegsAddr, 
					unsigned int DescRegsAddr,
					unsigned int RespRegsAddr, 
					bool EnhancedMode)
: mbEnhanced(EnhancedMode)
, mpCsr(NULL)
, mpDescriptors(NULL)
, mpResponse(NULL)
{
	// Memory map in the dispatcher's CSR 
	mpCsr = new tcCSRMap(CtrlRegsAddr);
	// Memory map in the dispatcher's Discriptor Reg
	mpDescriptors = new tcCSRMap(DescRegsAddr);
	// Memory map in the dispatcher's Response Reg,
	// if it has one (only write master have one)
	if(RespRegsAddr != 0)
		mpResponse = new tcCSRMap(RespRegsAddr);
}

/**
 *   Destructor.
 */
tcSGDMADispatcher::~tcSGDMADispatcher(void)
{
	if(mpCsr)
		delete mpCsr;
	if(mpDescriptors)
		delete mpDescriptors;
	if(mpResponse)
		delete mpResponse;
}


/**
 * Inits a descriptor from empty constructor
 *
 * \param CtrlRegsAddr physical address of control register slave port
 * \param DescRegsAddr physical address of descriptor slave port
 * \param RespRegsAddr physical address of ST->MM Response slave port (set to zero if not used)
 * \param EnhanceMode set to true to use enhanced descriptors.
 */
void tcSGDMADispatcher::init(unsigned int CtrlRegsAddr, 
					unsigned int DescRegsAddr,
					unsigned int RespRegsAddr, 
					bool EnhancedMode)
{
	// Memory map in the dispatcher's CSR 
	mpCsr = new tcCSRMap(CtrlRegsAddr);
	// Memory map in the dispatcher's Discriptor Reg
	mpDescriptors = new tcCSRMap(DescRegsAddr);
	// Memory map in the dispatcher's Response Reg,
	// if it has one (only write master have one)
	if(RespRegsAddr != 0)
		mpResponse = new tcCSRMap(RespRegsAddr);
}



/**
 * Writes a descriptor to the dispatcher
 *
 * \param descriptor the descriptor that will be added
 * \return 0 on success
 */
int tcSGDMADispatcher::WriteDescriptor(tsSGDMADescriptor& descriptor)
{
	
	if(mpDescriptors == NULL)
		return -1;

	// Write the descriptor to the dispatcher
	mpDescriptors->WriteCtrlReg(DESCRIPTOR_READ_ADDRESS_REG, descriptor.read_addr);	
	mpDescriptors->WriteCtrlReg(DESCRIPTOR_WRITE_ADDRESS_REG, descriptor.write_addr);
	mpDescriptors->WriteCtrlReg(DESCRIPTOR_LENGTH_REG, descriptor.length);
	// The control reg needs to be written last because it has the go bit, which posts
	// the descriptor to the dispatcher
	if(mbEnhanced)
	{
		// TODO: implement
	}
	else
	{
		mpDescriptors->WriteCtrlReg(DESCRIPTOR_CONTROL_STANDARD_REG, descriptor.control.mnWord);
	}

	return 0;
}

/**
 * Display descriptor parameters
 *
 * \param descriptor the descriptor that will be added
 * \return 0 on success
 */
int tcSGDMADispatcher::DisplayDescriptor(tsSGDMADescriptor& descriptor)
{
	if(mpDescriptors == NULL)
		return -1;
	std::cout << "Read addr: " << descriptor.read_addr << std::endl;
	std::cout << "Write addr: " << descriptor.write_addr << std::endl;
	std::cout << "Length: " << descriptor.length << std::endl;

	std::cout << "Parameter tx_chan: " << descriptor.control.msBits.tx_chan << std::endl;
	std::cout << "Parameter gen_sop: " << descriptor.control.msBits.gen_sop << std::endl;
	std::cout << "Parameter gen_eop: " << descriptor.control.msBits.gen_eop << std::endl;
	std::cout << "Parameter park_rd: " << descriptor.control.msBits.park_rd << std::endl;
	std::cout << "Parameter park_wr: " << descriptor.control.msBits.park_wr << std::endl;
	std::cout << "Parameter end_on_eop: " << descriptor.control.msBits.end_on_eop << std::endl;
	std::cout << "Parameter reserved_1: " << descriptor.control.msBits.reserved_1 << std::endl;
	std::cout << "Parameter tx_cmplte_mask: " << descriptor.control.msBits.tx_cmplte_mask << std::endl;
	std::cout << "Parameter early_term_mask: " << descriptor.control.msBits.early_term_mask << std::endl;
	std::cout << "Parameter tx_error_mask: " << descriptor.control.msBits.tx_error_mask << std::endl;
	std::cout << "Parameter early_done: " << descriptor.control.msBits.early_done << std::endl;
	std::cout << "Parameter reserved_2: " << descriptor.control.msBits.reserved_2 << std::endl;
	std::cout << "Parameter go: " << descriptor.control.msBits.go << std::endl;

	return 0;
}

/**
 * Reads and returns the status register of the Dispatcher
 *
 * \return the status register
 */
tuSgdmaStatus tcSGDMADispatcher::GetStatusReg()
{
	tuSgdmaStatus status;
	status.mnWord = mpCsr->ReadCtrlReg(CSR_STATUS_REG);
	return status;
}


void tcSGDMADispatcher::SetControlReg(int value)
{
	mpCsr->WriteCtrlReg(CSR_CONTROL_REG,value);
}
