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
 * File   : tb_mdp_global.hpp, 
 * Copyright (C) 2019 Lab-STICC Laboratory
 * Author(s) :  Chabha Hireche, chabha.hireche@gmail.com
 * 				Erwan Moréac, erwan.moreac@univ-ubs.fr (EM)
 * Created on: July 17, 2019
 * 
 *************************************************************************************/

#ifndef TB_MDP_GLOBAL_HPP
#define TB_MDP_GLOBAL_HPP

#include "mdp_global.hpp"

#include <map>
#include <iostream>
#include <string>
#include <iterator>
#include <fstream>
#include <vector>
#include <algorithm>

#define PATH_PROB_VECTOR 	"./src/parameters/mdp_prob_vector.txt"
#define PATH_REWARD_VECTOR  "./src/parameters/mdp_reward_vector.txt"

void global_mission_spec_tb(float proba_vect[PROBA_VECT_SIZE], float reward_vect[REWARD_VECT_SIZE],
							 float P[MATRIX_SIZE][MATRIX_SIZE][NB_ACTION], float R[MATRIX_SIZE][NB_ACTION]);

void Mdp_Global_tb(float proba_vect[PROBA_VECT_SIZE], float reward_vect[REWARD_VECT_SIZE], 
                    float discount, float policy[MATRIX_SIZE][1], float V[MATRIX_SIZE][1]);

int main_tb(float policy[MATRIX_SIZE][1], float V[MATRIX_SIZE][1]);

std::map<std::string, float> read_mdp_file(const char* path);
std::vector<std::string> read_whole_file(const char* path);
void write_value_file(const char* path, const std::string& key, float value);
void write_whole_map_file(const char* path, const std::map<std::string, float>& data_map);

std::map<std::string, int> build_index_reward_map();
std::map<std::string, int> build_index_prob_map();

#endif