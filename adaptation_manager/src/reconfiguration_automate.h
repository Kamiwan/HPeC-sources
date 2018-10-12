//TODO Insert header comment

#ifndef RECONFIGURATION_AUTOMATE_H
#define RECONFIGURATION_AUTOMATE_H

#include "adaptation_manager_node.h"

Step_out doStep (Step_in e);
void do1( Step_in e);
std::vector< std::vector<std::string> > initialise_sortie( int n, int m, Step_out s);

Step_out sortie();


#endif