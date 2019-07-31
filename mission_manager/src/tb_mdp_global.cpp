/* 
 * This file is part of the HPeC distribution (https://github.com/Kamiwan/HPeC-sources).
 * Copyright (c) 2018 Lab-STICC Laboratory.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
/*************************************************************************************
 * File   : tb_mdp_global.cpp, 
 * Copyright (C) 2019 Lab-STICC Laboratory
 * Author(s) :  Chabha Hireche, chabha.hireche@gmail.com
 * 				Erwan Moréac, erwan.moreac@univ-ubs.fr (EM)
 * Created on: July 17, 2019
 * 
 *************************************************************************************/
#include "tb_mdp_global.hpp"

void global_mission_spec_tb(float proba_vect[PROBA_VECT_SIZE], float reward_vect[REWARD_VECT_SIZE],
							 float P[MATRIX_SIZE][MATRIX_SIZE][NB_ACTION], float R[MATRIX_SIZE][NB_ACTION]) {


	for (int a=0; a<NB_ACTION; a++) {
		for (int i=0; i<MATRIX_SIZE; i++) {
			for(int j=0; j<MATRIX_SIZE; j++){
				P[i][j][a] = 0.0;
			}
		}
	}
	for (int i=0; i<MATRIX_SIZE; i++) {
		for(int j=0; j<NB_ACTION; j++){
			R[i][j]	= 0.0;
		}
	}

	// Matrices de transitions
	//Take-off action (A1)
	P[0][0][0] = 1-proba_vect[0]; //1-p_sys
	P[0][1][0] = proba_vect[0];	  //p_sys

	//Follow trajectoiry + obstacle detection by LIDAR action (A2+A11)
	P[1][2][1] = (proba_vect[0]-proba_vect[1]); //p_sys-p_obs
	P[1][4][1] = proba_vect[1]; //p_obs
	P[1][0][1] = (1-proba_vect[0]); //1-p_sys

	P[2][3][1] = ((1-proba_vect[2])-proba_vect[1]); //psf-p_obs (psf=1-psi)
	P[2][4][1] = proba_vect[1]; //p_obs
	P[2][2][1] = proba_vect[2]; //psi

	P[3][3][1] = (1-proba_vect[1]); //1-p_obs
	P[3][4][1] = proba_vect[1]; //p_obs

	P[7][2][1] = proba_vect[2]; //psi
	P[7][3][1] = (1-proba_vect[2]); //1-psi = psf
	P[10][2][1] = proba_vect[2]; //psi
	P[10][3][1] = (1-proba_vect[2]); //1-psi=psf

	// Follow trajectoiry + obstacle detection by fusion (A2+A4)
	P[1][2][2] = proba_vect[3]; //p_battery
	P[1][5][2] = (1-proba_vect[3]); //1-p_battery

	P[2][2][2] = (proba_vect[2] * proba_vect[3]); //psi*p_battery
	P[2][3][2] = ((1-proba_vect[2]) * proba_vect[3]); //psf*p_battery
	P[2][5][2] = (1-proba_vect[3]); //1-p_battery

	P[3][3][2] = proba_vect[3]; //p_battery
	P[3][5][2] = (1-proba_vect[3]); //1-p_battery

	P[5][4][2] = proba_vect[1]; //p_obs
	P[5][2][2] = proba_vect[2]; //psi
	P[5][3][2] = ((1-proba_vect[2]) - proba_vect[1]); //psf-p_obs

	P[7][2][2] = proba_vect[2]; //psi
	P[7][3][2] = (1-proba_vect[2]); //1-psi=psf
	P[10][2][2] = proba_vect[2]; //psi
	P[10][3][2] = (1-proba_vect[2]); //1-psi=psf

	//Obstacle Avoidance (A3)
	P[4][6][3] = proba_vect[1]; //p_obs
	P[4][19][3] = (1-proba_vect[1]); //1-p_obs
	P[6][6][3] = proba_vect[1]; //p_obs

	P[6][2][3] = proba_vect[2]; //psi
	P[6][3][3] = ((1-proba_vect[2]) - proba_vect[1]); //psf-p_obs

	// Recherche de zone "T" pour landing (A5)
	P[2][7][4] = proba_vect[4]; //p_app_T
	P[2][8][4] = (1-proba_vect[4]); //1-p_app_T

	P[3][7][4] = proba_vect[4];
	P[3][8][4] = (1-proba_vect[4]);

	P[12][7][4] = proba_vect[4];
	P[12][8][4] = (1-proba_vect[4]);

	P[13][7][4] = proba_vect[4];
	P[13][8][4] = (1-proba_vect[4]);

	P[14][7][4] = proba_vect[4];
	P[14][8][4] = (1-proba_vect[4]);

	P[15][7][4] = proba_vect[4];
	P[15][8][4] = (1-proba_vect[4]);

	P[16][7][4] = proba_vect[4];
	P[16][8][4] = (1-proba_vect[4]);

	P[17][7][4] = proba_vect[4];
	P[17][8][4] = (1-proba_vect[4]);

	P[18][7][4]= proba_vect[4];
	P[18][8][4] = (1-proba_vect[4]);

	// Recherche zone (non "T") pour landing (A6)
	P[8][9][5] = proba_vect[5]; //p_app_z
	P[8][10][5] = (1-proba_vect[5]); //1-p_app_z

	// Landing par servo contr�le (by camera) (A7)
	P[7][11][6] = 1;

	// Replanning + landing (A8)
	P[9][11][7] =1;

	// Detection de cible � traquer (A9)
	P[3][12][8] = proba_vect[6]; //p_target
	P[3][3][8] = (1-proba_vect[6]); //1-p_target

	// V0=V011= V0 + A11 (tracking V0 + LIDAR)
	P[12][13][9] = (proba_vect[7] - proba_vect[1]); //p_v0 - p_obs
	P[12][3][9] = (1-proba_vect[7]); //1-p_v0
	P[12][4][9] = proba_vect[1]; //p_obs

	P[13][13][9] = (proba_vect[7] - proba_vect[1]);
	P[13][3][9] = (1-proba_vect[7]);
	P[13][4][9] =proba_vect[1];

	// V04 = V0+A4 (tracking V0 + fusion)
	P[12][13][10] = (proba_vect[7] - proba_vect[3]);//p_v0-p_battery
	P[12][3][10] = (1-proba_vect[7]); //1-p_v0
	P[13][5][10] = proba_vect[3]; //p_battery

	P[13][13][10] = (proba_vect[7] - proba_vect[3]); //p_v0-p_battery
	P[13][5][10] = proba_vect[3]; //p_battery
	P[13][3][10] = (1-proba_vect[7]); //1-p_v0

	// V111 = V1+A11 (tracking V1 + LIDAR)
	P[12][14][11] = (proba_vect[8] - proba_vect[1]); //p_v1-p_obs
	P[12][3][11] = (1-proba_vect[8]); //1-p_v1
	P[12][4][11] = proba_vect[1]; //p_obs

	P[13][14][11] = (proba_vect[8] - proba_vect[1]);
	P[13][13][11] = (1-proba_vect[8]);
	P[13][4][11] = proba_vect[1];

	P[14][14][11] = (proba_vect[8] - proba_vect[1]);
	P[14][4][11] = proba_vect[1];
	P[14][13][11] = (1-proba_vect[8]);

	//V14 = V1+A4 (tracking V1 + fusion)
	P[12][14][12] = (proba_vect[8] - proba_vect[3]); //p_v1-p_battery
	P[12][3][12] = (1-proba_vect[8]);
	P[12][5][12] = proba_vect[3];

	P[13][14][12] = (proba_vect[8] - proba_vect[3]);
	P[13][13][12] = (1-proba_vect[8]);
	P[13][5][12] = proba_vect[3];

	P[14][14][12] = (proba_vect[8] - proba_vect[3]);
	P[14][13][12] = (1-proba_vect[8]);
	P[14][5][12] = proba_vect[3];

	//V211 = V2+A11 (tracking V2 + LIDAR)
	P[12][15][13] = (proba_vect[9] - proba_vect[1]); //p_v2-p_obs
	P[12][3][13] = (1-proba_vect[9]);
	P[12][4][13] = proba_vect[1];

	P[15][15][13] = (proba_vect[9] - proba_vect[1]);
	P[15][4][13] = proba_vect[1];
	P[15][13][13] = (1-proba_vect[9]);

	P[13][15][13] = (proba_vect[9] - proba_vect[1]);
	P[13][13][13] = (1-proba_vect[9]);
	P[13][4][13] = proba_vect[1];

	//V24 = V2+A4 (tracking V2 + fusion)
	P[12][15][14] = (proba_vect[9] - proba_vect[3]); //p_v2-p_battery
	P[12][3][14] = (1-proba_vect[9]);
	P[12][5][14] = proba_vect[3];

	P[13][15][14] = (proba_vect[9] - proba_vect[3]);
	P[13][13][14] = (1-proba_vect[9]);
	P[13][5][14] = proba_vect[3];

	P[15][15][14] = (proba_vect[9] - proba_vect[3]);
	P[15][13][14] = (1-proba_vect[9]);
	P[15][5][14] = proba_vect[3];

	//V311 = V3+A11 (tracking V3 + LIDAR)
	P[12][16][15] = (proba_vect[10] - proba_vect[1]); //p_v3-p_obs
	P[12][3][15] = (1-proba_vect[10]);
	P[12][4][15] = proba_vect[1];

	P[16][16][15] = (proba_vect[10] - proba_vect[1]);
	P[16][4][15] = proba_vect[1];
	P[16][13][15] = (1-proba_vect[10]);

	P[13][16][15] = (proba_vect[10] - proba_vect[1]);
	P[13][13][15] = (1-proba_vect[10]);
	P[13][4][15] = proba_vect[1];

	//V34 = V3+A4 (tracking V3 + fusion)
	P[12][16][16] = (proba_vect[10] - proba_vect[3]); //p_v3-p_battery
	P[12][3][16] = (1-proba_vect[10]);
	P[12][5][16] = proba_vect[3];

	P[16][16][16] = (proba_vect[10] - proba_vect[3]);
	P[16][13][16] = (1-proba_vect[10]);
	P[16][5][16] = proba_vect[3];

	P[13][16][16] = (proba_vect[10] - proba_vect[3]);
	P[13][13][16] = (1-proba_vect[10]);
	P[13][5][16] = proba_vect[3];

	//V411 = V4+A11 (tracking V4 + LIDAR)
	P[12][17][17] = (proba_vect[11] - proba_vect[1]); //p_v4-p_obs
	P[12][3][17] = (1-proba_vect[11]);
	P[12][4][17] = proba_vect[1];

	P[17][17][17] = (proba_vect[11] - proba_vect[1]);
	P[17][4][17] = proba_vect[1];
	P[17][13][17] = (1-proba_vect[11]);

	P[13][17][17] = (proba_vect[11] - proba_vect[1]);
	P[13][13][17] = (1-proba_vect[11]);
	P[13][4][17] = proba_vect[1];

	//V44 = V4+A4 (tracking V4 + fusion)
	P[12][17][18] = (proba_vect[11] - proba_vect[3]); //p_v4-p_battery
	P[12][3][18] = (1-proba_vect[11]);
	P[12][5][18] = proba_vect[3];

	P[17][17][18] = (proba_vect[11] - proba_vect[3]);
	P[17][13][18] = (1-proba_vect[11]);
	P[17][5][18] = proba_vect[3];

	P[13][17][18] = (proba_vect[11] - proba_vect[3]);
	P[13][13][18] = (1-proba_vect[11]);
	P[13][5][18] = proba_vect[3];

	//V511 = V5+A11 (tracking V5 + LIDAR)
	P[12][18][19] = (proba_vect[12] - proba_vect[1]); //p_v5-p_obs
	P[12][3][19] = (1-proba_vect[12]);
	P[12][4][19] = proba_vect[1];

	P[18][18][19] = (proba_vect[12] - proba_vect[1]);
	P[18][4][19] = proba_vect[1];
	P[18][13][19] = (1-proba_vect[12]);

	P[13][18][19] = (proba_vect[12] - proba_vect[1]);
	P[13][13][19] = (1-proba_vect[12]);
	P[13][4][19] = proba_vect[1];

	//V54 = V5+A4 (tracking V5 + fusion)
	P[12][18][20] = (proba_vect[12] - proba_vect[3]); //p_v5-p_battery
	P[12][3][20] = (1-proba_vect[12]);
	P[12][5][20] = proba_vect[3];

	P[18][18][20] = (proba_vect[12] - proba_vect[3]);
	P[18][13][20] = (1-proba_vect[12]);
	P[18][5][20] = proba_vect[3];

	P[13][18][20] = (proba_vect[12] - proba_vect[3]);
	P[13][13][20] = (1-proba_vect[12]);
	P[13][5][20] = proba_vect[3];

	//***********************************************
	// MATRICES REWARDS
	R[0][0] = reward_vect[0];
	R[1][1] = reward_vect[1]; R[2][1] = reward_vect[1]; R[3][1] = reward_vect[1]; R[7][1] = reward_vect[1]; R[10][1] = reward_vect[1];
	R[1][2] = reward_vect[2]; R[2][2] = reward_vect[2]; R[3][2] = reward_vect[2]; R[5][2] = reward_vect[2]; R[7][2] = reward_vect[2]; R[10][2] = reward_vect[2];
	R[4][3] = reward_vect[3]; R[6][3] = reward_vect[3];
	R[2][4] = reward_vect[4]; R[3][4] = reward_vect[4]; R[12][4] = reward_vect[4]; R[13][4] = reward_vect[4]; R[14][4] = reward_vect[4];
	R[15][4] = reward_vect[4]; R[16][4] = reward_vect[4]; R[17][4] = reward_vect[4]; R[18][4] = reward_vect[4];

	R[8][5] = reward_vect[5];
	R[7][6] = reward_vect[6];
	R[9][7] = reward_vect[7];
	R[3][8] = reward_vect[8];
	R[12][9] = reward_vect[9]; R[13][9] = reward_vect[9];
	R[12][10] = reward_vect[10]; R[13][10] = reward_vect[10];
	R[12][11] = reward_vect[11]; R[13][11] = reward_vect[11]; R[14][11] = reward_vect[11];
	R[12][12] = reward_vect[12]; R[13][12] = reward_vect[12]; R[14][12] = reward_vect[12];
	R[12][13] = reward_vect[13]; R[13][13] = reward_vect[13]; R[15][13] = reward_vect[13];
	R[12][14] = reward_vect[14]; R[13][14] = reward_vect[14]; R[15][14] = reward_vect[14];
	R[12][15] = reward_vect[15]; R[13][15] = reward_vect[15]; R[16][15] = reward_vect[15];
	R[12][16] = reward_vect[16]; R[13][16] = reward_vect[16]; R[16][16] = reward_vect[16];
	R[12][17] = reward_vect[17]; R[13][17] = reward_vect[17]; R[17][17] = reward_vect[17];
	R[12][18] = reward_vect[18]; R[13][18] = reward_vect[18]; R[17][18] = reward_vect[18];
	R[12][19] = reward_vect[19]; R[13][19] = reward_vect[19]; R[18][19] = reward_vect[19];
	R[12][20] = reward_vect[20]; R[13][20] = reward_vect[20]; R[18][20] = reward_vect[20];
}



void Mdp_Global_tb(float proba_vect[PROBA_VECT_SIZE], float reward_vect[REWARD_VECT_SIZE], float discount, float policy[MATRIX_SIZE][1], float V[MATRIX_SIZE][1]){
	float P[MATRIX_SIZE][MATRIX_SIZE][NB_ACTION], R[MATRIX_SIZE][NB_ACTION];


	global_mission_spec_tb(proba_vect, reward_vect, P, R); //spec mission. creation des matrices transitions et rewards

	Mdp_Policy_Iteration_PI(P, R, discount, policy, V); // resolution du mdp mission
}



//--------------------------------------------------------------------
// function main
// EM, renamed to main_tb to compile with mission_manager_node main

int main_tb(){
//input
	float discount = 0.95;
	//float proba_vect[PROBA_VECT_SIZE] = {0.9, 0.5, 0.6, 0.8, 0.5, 0.4, 0.8, 0.9, 0.7207, 0.8467, 0.0, 0.1, 0.7615}; // proba event
	//float reward_vect[REWARD_VECT_SIZE] = {5.0, 40.0, 30.0, 20.0, 20.0, 25.0, 15.0, 15.0, 25.0, 90.0, 85.0, 80.0, 75.0, 60.0, 55.0, 70.0, 65.0, 50.0, 45.0, 40.0, 35.0}; //reward par action
	float policy[MATRIX_SIZE][1];
	float V[MATRIX_SIZE][1];

	// EM, associative memory
	std::map<std::string, int> index_prob_map 	= build_index_prob_map();
	std::map<std::string, int> index_reward_map = build_index_reward_map();
	std::map<std::string, float> prob_map		= read_mdp_file( PATH_PROB_VECTOR );
	std::map<std::string, float> reward_map		= read_mdp_file( PATH_REWARD_VECTOR );

	float proba_vect[PROBA_VECT_SIZE];
	std::map<std::string, int>::iterator it;
	for ( it = index_prob_map.begin(); it != index_prob_map.end(); it++ )
	{
		// EM, get index from index_prob_map with it->second
		// index_prob_map and prob_map have the same key: it->first
		proba_vect[it->second] = prob_map[it->first];
	}
	float reward_vect[REWARD_VECT_SIZE];
	for ( it = index_reward_map.begin(); it != index_reward_map.end(); it++ )
	{
		reward_vect[it->second] = reward_map[it->first];
	}

	float P[MATRIX_SIZE][MATRIX_SIZE][NB_ACTION]; float R[MATRIX_SIZE][NB_ACTION];

	Mdp_Global_tb(proba_vect, reward_vect, discount, policy, V);
	//spec_mission_globale_tb(proba_vect, reward_vect, P, R);

	printf("Resultat Policy : \n");
	for(int i=0; i<MATRIX_SIZE; i++)
	{
		printf("Value Function: %f \n", V[i][0]);
		printf("Politique: %f \n", policy[i][0]);
	}
}



std::map<std::string, float> read_mdp_file(const char* path)
{
    std::ifstream file(path); 
   	std::string temp;
	std::map<std::string, float> output_map;
	std::string delimiter = "=";

    if ( file ) 
    { 
        while ( std::getline( file, temp) ) 
        { 
			if(temp[0] != '#' && temp.size() > 1) // EM, allow comment text in file
			{
				int pos = temp.find(delimiter);
				std::string key   = temp.substr(0, pos);
				// EM, remove extra spaces
				key.erase(std::remove_if(key.begin(), key.end(), isspace), key.end());
				std::string value = temp.substr(pos + delimiter.size());
				output_map.insert(std::make_pair(key, std::stof(value)));
			}
        }
	}
	else
    {
        std::cerr << "ERROR: Cannot open input file." << std::endl;
    }  
	file.close();

	std::cout << "Display MAP :" << std::endl;
	std::map<std::string, float>::iterator it;
	for ( it = output_map.begin(); it != output_map.end(); it++ )
		std::cout << "Key : " << it->first << " ;  Value : " << it->second << std::endl;  

   	return output_map;
}

void write_whole_map_file(const char* path, const std::map<std::string, float>& data_map)
{
    std::vector<std::string> input_file_content = read_whole_file(path);
    std::ofstream output_file;
	if(input_file_content.empty())
	{
		std::cerr << "ERROR in input file reading " << std::endl;
	} else
	{
    	output_file.open(path);
		//EM, wrtie comments before data
    	for(size_t i = 0; i < input_file_content.size(); i++)
    	{
			if(input_file_content[i][0] == '#' || input_file_content[i].size() > 1)
			{
				output_file << input_file_content[i] << std::endl;
			}
    	}
		// EM, write data
		for (auto it = data_map.begin(); it != data_map.end(); it++ )
				output_file <<  it->first << " = " << it->second << std::endl;  

    	output_file.close();
	}
}

void write_value_file(const char* path, const std::string& key, float value)
{
    std::vector<std::string> input_file_content = read_whole_file(path);
    std::ofstream output_file;

	if(input_file_content.empty())
	{
		std::cerr << "ERROR in input file reading " << std::endl;
	} else
	{
    	output_file.open(path);
		char delimiter = '=';
    	for(size_t i = 0; i < input_file_content.size(); i++)
    	{
			if(input_file_content[i][0] != '#' && input_file_content[i].size() > 1)
			{
				std::string current_key = input_file_content[i].substr(0, 
											input_file_content[i].find(delimiter));
				// EM, remove extra spaces
				current_key.erase(std::remove_if(current_key.begin(), current_key.end(), isspace), current_key.end());
				if(current_key == key)
					output_file << key << " = " << value << std::endl;
				else
					output_file << input_file_content[i] << std::endl;
			} else
			{
				output_file << input_file_content[i] << std::endl;
			}
    	}
    	output_file.close();
	}
}



std::vector<std::string> read_whole_file(const char* path)
{
    std::ifstream file(path); 
    std::vector<std::string> lines;
   	std::string temp;
    if ( file ) 
    { 
        while ( std::getline( file, temp) ) 
        { 
			lines.push_back(temp);
        }
	}
	else
    {
        std::cerr << "ERROR: Cannot open input file." << std::endl;
    }  
	file.close();
	return lines;
}





std::map<std::string, int> build_index_prob_map()
{
	std::map<std::string, int> index_map;	
	index_map.insert(std::make_pair("P_Sys", 0));
	index_map.insert(std::make_pair("P_obs", 1));
	index_map.insert(std::make_pair("P_SI", 2));
	index_map.insert(std::make_pair("P_Batterie", 3));
	index_map.insert(std::make_pair("P_zone_T", 4));
	index_map.insert(std::make_pair("P_SL_Area", 5));
	index_map.insert(std::make_pair("P_Target", 6));
	index_map.insert(std::make_pair("P_V0", 7));
	index_map.insert(std::make_pair("P_V1", 8));
	index_map.insert(std::make_pair("P_V2", 9));
	index_map.insert(std::make_pair("P_V3", 10));
	index_map.insert(std::make_pair("P_V4", 11));
	index_map.insert(std::make_pair("P_V5", 12));

	return index_map;
}

std::map<std::string, int> build_index_reward_map()
{
	std::map<std::string, int> index_map;
	index_map.insert(std::make_pair("R_take_off", 0));
	index_map.insert(std::make_pair("R_FT_ODL", 1));
	index_map.insert(std::make_pair("R_FT_ODF", 2));
	index_map.insert(std::make_pair("R_Obs_Avoidance", 3));
	index_map.insert(std::make_pair("R_SLT", 4));
	index_map.insert(std::make_pair("R_SLA", 5));
	index_map.insert(std::make_pair("R_Landing", 6));
	index_map.insert(std::make_pair("R_replanning_landing", 7));
	index_map.insert(std::make_pair("R_targetDetection", 8));
	index_map.insert(std::make_pair("R_track_V0_Lidar", 9));
	index_map.insert(std::make_pair("R_track_V0_fusion", 10));
	index_map.insert(std::make_pair("R_track_V1_Lidar", 11));
	index_map.insert(std::make_pair("R_track_V1_fusion", 12));
	index_map.insert(std::make_pair("R_track_V2_Lidar", 13));
	index_map.insert(std::make_pair("R_track_V2_fusion", 14));
	index_map.insert(std::make_pair("R_track_V3_Lidar", 15));
	index_map.insert(std::make_pair("R_track_V3_fusion", 16));
	index_map.insert(std::make_pair("R_track_V4_Lidar", 17));
	index_map.insert(std::make_pair("R_track_V4_fusion", 18));
	index_map.insert(std::make_pair("R_track_V5_Lidar", 19));
	index_map.insert(std::make_pair("R_track_V5_fusion", 20));

	return index_map;
}

/*int read_value_file(const char* path, const int& app_name_index)
{
    int res = -1;
    std::ifstream file(path); 
    std::string index_app_name_converter[11] = {"contrast_img", "motion_estim_imu", "motion_estim_img"
                                            "search_landing", "obstacle_avoidance", "t_landing", 
                                            "rotoz_s", "rotoz_b", "replanning", "detection",
                                            "tracking"};
    std::string temp;
    while (std::getline( file, temp)) 
	    if(temp == index_app_name_converter[app_name_index])
        {
            std::getline( file, temp);
            res = std::stoi(temp);
            break;
        }
    file.close();
    return res;
}*/

