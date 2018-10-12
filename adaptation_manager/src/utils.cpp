//TODO insert header comment here

#include "utils.h"


long elapse_time_u (struct timeval *end, struct timeval *start ) 
{
	long seconds = end->tv_sec - start->tv_sec ;
	long micro_seconds = end->tv_usec - start->tv_usec ;
	if (micro_seconds < 0)
	{
		seconds -= 1;
	}
	return (seconds * 1000000) + abs (micro_seconds) ;
}

long time_micros (struct timeval *end, struct timeval *start ) 
{
	if (end != NULL && start != NULL )
		return  (((double) elapse_time_u (end, start)) / 1000 ) ;
	return -1;
}


double cpuload ( ){
        FILE *fp;
        char buffer[100];
        fp = popen("top -b -d 1 -n5 | grep Cpu | awk '{printf \"%.2f:\", $8}'","r");
        fscanf(fp,"%s", buffer);
	int closureReturn = pclose(fp);
	std::cout << "Processus properly killed ? " << closureReturn << std::endl;

        int len, i, j = 0, nb = 0 ;
        double value, sum = 0;
        char stock [7];

        len = strlen (buffer);
        for (i = 0; i < len ; i++){
                if (buffer [i] == '\0'|| buffer [i] == '\n')
                        break;
                else {
                        if (buffer [i] == ':'){
                                //printf ("%s\n", stock); 
								j = 0;
                                if(nb > 0) {
                                        value = atof ( stock );
                                        sum+= value;
                                }
                                nb++;
                                continue;
                        }
                        stock[j++] = buffer [i];
                }
        } nb -- ;
        return ((nb > 0)? sum / nb : -1);
}


vector<string> readfile(const char* path){
    std::ifstream fichier(path); 
    vector<string> lignes;
    int i(0); 
   	string temp;
    if ( fichier ) 
    { 
        while ( std::getline( fichier, temp) ) 
        { 
		lignes.push_back(temp);
        }
	}
	else
    {
        cout << "ERREUR: Impossible d'ouvrir le fichier en lecture." << endl;
    }  
	fichier.close();
   
   	/*cout << "il y a "<< lignes.size() << " lignes dans le fichier" << endl;
   	for (size_t i = 0; i < lignes.size(); ++i)
       cout << "Ligne " << i << " : " << lignes[i] <<  endl;
	*/
	return lignes;
}

inline bool isInteger(const std::string & s)
{
   if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false ;

   char * p ;
   strtol(s.c_str(), &p, 10) ;

   return (*p == 0) ;
}
