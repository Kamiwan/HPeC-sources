#include <ros/ros.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "std_msgs/String.h"


#include <errno.h>
#include <sys/types.h>
#include <sys/inotify.h>
#include <unistd.h>

#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )


#include <boost/thread.hpp>
#include "std_msgs/Int32.h"
#include "std_msgs/Float32.h"



#include <iostream>
#include <ostream>
#include <fstream>
#include <string>


using namespace std; 
boost::shared_ptr<ros::Publisher> notify_from_MM_pub;


//lecture table c3, texe des fonctions
vector<string> readfile1(const char* path){
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
   } fichier.close();
   //cout << "il y a "<< lignes.size() << " lignes dans le fichier" << endl;
 
   /* for (size_t i = 0; i < lignes.size(); ++i)
    {
       cout << "Ligne " << i << " : " << lignes[i] <<  endl; 
	}*/
    return lignes;
}
vector<string> readcpufile(const char* path){
   std::ifstream fichier(path); 
   vector<string> lignes ; 
   int i(0); 
   string temp;
    if ( fichier ) 
    {
        while ( std::getline( fichier, temp, ' ') ) 
        { 
		 lignes.push_back(temp);
        }  
    }  
	else
   {
      cout << "ERREUR: Impossible d'ouvrir le fichier en lecture." << endl;
   } fichier.close();

    for (size_t i = 0; i < lignes.size(); ++i)
    {
       cout << "Ligne " << i << " : " << lignes[i] <<  endl; 
	}
    return lignes;
}
 
 //**********************mise a jour de table c3
 void modifyC3_QOS_TEXE(const char* path, vector<string> C3,vector<string> lignes){ 
       ofstream file(path, ios::out); // path to table c3
   if ( file ) 
    {  
         for(int j=0;j<C3.size();j++){
                 for(int m=0;m<lignes.size();m=m+3){
                 if(C3[j]==lignes[m]){
                 if(C3[j+2] != lignes[m+1]){ 
                         C3[j+2]= lignes[m+1]; //modif de Texec des taches
                 }
                 if(C3[j+5] != lignes[m+2]){ 
                         C3[j+5]= lignes[m+2]; //modif de qos des taches
                 }        
                 }
                 }
         }
           //ecriture dans table c3 en ecrasant la data
           for (int i = 0; i < C3.size(); ++i)
           {   
            file <<C3[i]<<endl; 
           }
         file.close();  
    }  else{
                cerr << "Impossible d'ouvrir le fichier !" << endl;
                }
 }
 //*************** savoir laquelle des taches nn realisable
 void know(vector<string> achievable_tab){
   int i;
   for(i=0;i<achievable_tab.size();i++){
           if(achievable_tab[i] == "0"){
                   cout << "la tache :"<< achievable_tab[i-1]<< " n'est pas realisable, achievable_value :"<< achievable_tab[i]<<endl;
          }
   }
 }
 //**********changemment de table c3 pour taches non realisables
  void update_table_c3(){
          const char*path1 ="./src/parameters/tableC3(2).txt";
          const char*path2 ="./src/parameters/time_qos_task.txt"; 
       vector<string> C3, lignes;
          C3 = readfile1(path1);

    //lecture des Texec de S_L et autres /MODIF DE C3
   lignes = readfile1(path2);
 
  modifyC3_QOS_TEXE(path1,C3,lignes);

  }
 //***************************
 void achievable_Callback(const std_msgs::Int32::ConstPtr& msg1)
{  
       std_msgs::Int32 msg;
 ROS_ERROR("[RECU][ADAPTATION_MANAGER][Achievable_VALUE OF TASKS]");
 ROS_ERROR("[%d]",msg1->data);
 vector<string> achievable_tab = readfile1("./src/parameters/achievable_tab.txt");
   know(achievable_tab);
   update_table_c3();
  //changemment de table c3 pour taches non realisables ; func utilisant la func Know();
  ROS_ERROR("CHANGEMENT DE TABLE C3, ENVOIE DE NOTIF A AUTOMATE EN COURS...");
   msg.data = 1;
 notify_from_MM_pub->publish(msg); 

 }
 //******************
void cpu_load_Callback(const std_msgs::Float32::ConstPtr& load){
   ROS_ERROR("[CPU][LOAD] : [%f]",load->data);
}
//***********************
int main (int argc, char ** argv)
{
ros::init(argc, argv, "mission_manager");
ros::NodeHandle n;
//************** reception de la liste des taches nn realisables, publiée par l'automate ...
ros::Subscriber achievable_sub = n.subscribe("/achievable_topic", 1000, achievable_Callback);
notify_from_MM_pub= boost::make_shared<ros::Publisher>( 
			         n.advertise<std_msgs::Int32>("/notify_from_MM_topic", 1000));

ros::Subscriber cpu_sub = n.subscribe("/cpu_load_topic", 1000, cpu_load_Callback); 

const char*path0 ="./src/parameters/tableC3(2).txt";
const char*path4 ="./src/parameters/cpu.txt";
 vector<string> C3, cpu;
  

ROS_ERROR("[mission_manager][listening]");
 

ros::Rate loop_rate1(10);
 while(ros::ok()){
   
//lecture de C3 apres modif des Qos et Texec
 ROS_ERROR("TABLE C3");
 C3 = readfile1(path0);

 ros::spin();

loop_rate1.sleep(); 

}
}
























