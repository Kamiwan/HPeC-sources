//TODO Insert header comment

#ifndef RECONFIG_H
#define RECONFIG_H

#include "adaptation_manager_node.h"

void start_reconfiguration(int fd_mem, int bitstream_address, int region_id);
int reconfiguration_done(int fd_mem);

#endif