/**
 * \file csr_map.cpp 
 *
 * \description Implementation file for generic register mapping.
 *
 * \copyright Critical Link LLC 2013
 */
#include "csr_map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

tcCSRMap::tcCSRMap(unsigned int RegsAddr)
: mpRegMapMM(NULL)
{
	int fd;

	/* map in the raw image buffer area */
	if ((fd = open("/dev/mem", O_RDWR)) < 0)
	{
		perror("/dev/mem");
	}
	else
	{
		void *lpmem;

		lpmem = mmap(0, 4096, PROT_READ|PROT_WRITE, MAP_SHARED, fd, RegsAddr);
		if (lpmem == MAP_FAILED)
		{
			perror("mmap register buffers");
			close(fd);
			return;
		}
		mpRegMapMM = lpmem;
	}
}

tcCSRMap::~tcCSRMap(void)
{
	if (mpRegMapMM)
		munmap(mpRegMapMM, 32);
}

