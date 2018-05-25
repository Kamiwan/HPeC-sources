/**
 * \file csr_map.h 
 *
 * \description Generic class for CSR memory mapping.
 *
 *
 * \copyright Critical Link LLC 2013
 */
#ifndef CSR_MAP_H
#define CSR_MAP_H

#include <stdint.h>

class tcCSRMap 
{
public:
	tcCSRMap(unsigned int RegsAddr);
	virtual ~tcCSRMap(void);

	unsigned int ReadCtrlReg(int offset)
	{
		volatile unsigned int* preg = (volatile unsigned int*)mpRegMapMM;
		return preg[offset];
	}

	void WriteCtrlReg(int offset, unsigned int value)
	{
		volatile unsigned int* preg = (volatile unsigned int*)mpRegMapMM;
		preg[offset] = value;
	}

	void	*mpRegMapMM;  //!< memory mapped pointer to registers
};

#endif
