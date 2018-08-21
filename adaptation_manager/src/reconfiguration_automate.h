//TODO Insert header comment

#ifndef RECONFIGURATION_AUTOMATE_H
#define RECONFIGURATION_AUTOMATE_H

#include "adaptation_manager_node.h"

Step_out doStep (Step_in e);
void do1( Step_in e);
std::vector< std::vector<std::string> > initialise_sortie( int n, int m, Step_out s);


Step_in entree0( std::vector<std::string> C3);
Step_in entree1( std::vector<std::string> C3);
Step_in entree( std::vector<std::string> C3, std::vector<std::string> lignes);
Step_out sortie();


#endif