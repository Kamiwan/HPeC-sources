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

double cpuload()
{
    FILE *fp;
    char buffer[100];
    fp = popen("top -b -d 1 -n5 | grep Cpu | awk '{printf \"%.2f:\", $8}'", "r");
    fscanf(fp, "%s", buffer);
    int closureReturn = pclose(fp);
    std::cout << "Processus properly killed ? " << closureReturn << std::endl;

    int len, i, j = 0, nb = 0;
    double value, sum = 0;
    char stock[7];

    len = strlen(buffer);
    for (i = 0; i < len; i++)
    {
        if (buffer[i] == '\0' || buffer[i] == '\n')
            break;
        else
        {
            if (buffer[i] == ':')
            {
                //printf ("%s\n", stock);
                j = 0;
                if (nb > 0)
                {
                    value = atof(stock);
                    sum += value;
                }
                nb++;
                continue;
            }
            stock[j++] = buffer[i];
        }
    }
    nb--;
    return ((nb > 0) ? sum / nb : -1);
}

std::vector<std::string> readfile(const char* path){
    std::ifstream fichier(path); 
    std::vector<std::string> lignes;
   	std::string temp;
    if ( fichier ) 
    { 
        while ( std::getline( fichier, temp) ) 
        { 
		lignes.push_back(temp);
        }
	}
	else
    {
        std::cout << "ERREUR: Impossible d'ouvrir le fichier en lecture." << std::endl;
    }  
	fichier.close();
   
   	/*cout << "il y a "<< lignes.size() << " lignes dans le fichier" << std::endl;
   	for (size_t i = 0; i < lignes.size(); ++i)
       cout << "Ligne " << i << " : " << lignes[i] <<  std::endl;
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


void write_value_file(const char* path, const std::string& app_name, const int& value)
{
    std::vector<std::string> input_file_content = readfile(path);
    std::ofstream output_file;
    output_file.open(path);
    for(size_t i = 0; i < input_file_content.size(); i++)
    {
        if(input_file_content[i] == app_name)
        {
            output_file << app_name << std::endl;
            output_file << value << std::endl;
            i++; //EM, increment to NOT overwrite value's line
        }
        else
            output_file << input_file_content[i] << std::endl;
    }
    output_file.close();
}

int read_value_file(const char* path, const int& app_name_index)
{
    int res = -1;
    std::ifstream fichier(path); 
    std::string index_app_name_converter[11] = {"contrast_img", "motion_estim_imu", "motion_estim_img"
                                            "search_landing", "obstacle_avoidance", "t_landing", 
                                            "rotoz_s", "rotoz_b", "replanning", "detection",
                                            "tracking"};
    std::string temp;
    while (std::getline( fichier, temp)) 
	    if(temp == index_app_name_converter[app_name_index])
        {
            std::getline( fichier, temp);
            res = std::stoi(temp);
            break;
        }
    fichier.close();
    return res;
}
