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
 * File   : mdp_global.hpp, 
 * Copyright (C) 2019 Lab-STICC Laboratory
 * Author(s) :  Chabha Hireche, chabha.hireche@gmail.com
 * 				Erwan Moréac, erwan.moreac@univ-ubs.fr (EM)
 * Created on: July 17, 2019
 * 
 *************************************************************************************/

#ifndef MDP_GLOBAL_HPP
#define MDP_GLOBAL_HPP

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>

#define MATRIX_SIZE 20
#define NB_ACTION 21
#define PROBA_VECT_SIZE 13
#define REWARD_VECT_SIZE 21

void mdp_global(
			float proba_vect[PROBA_VECT_SIZE],
			float reward_vect[REWARD_VECT_SIZE],
			float discount, float policy[MATRIX_SIZE][1],
			float V[MATRIX_SIZE][1]);


//***************************

void findInverse_PI(float B[MATRIX_SIZE][MATRIX_SIZE],
					float mat_inverse[MATRIX_SIZE][MATRIX_SIZE]);

void ProduitMat_PI(float P[MATRIX_SIZE][MATRIX_SIZE][NB_ACTION],
				   float Vprev[MATRIX_SIZE][1], float discount,
				   float produit_vect[MATRIX_SIZE][NB_ACTION]);

void max_PI(float Q[MATRIX_SIZE][NB_ACTION],
			float V[MATRIX_SIZE][1],
			float policy[MATRIX_SIZE][1]);

void Compute_Ppolicy_PRpolicy_PI(float P[MATRIX_SIZE][MATRIX_SIZE][NB_ACTION],
								float R[MATRIX_SIZE][NB_ACTION], float policy[MATRIX_SIZE][1],
								float Ppolicy[MATRIX_SIZE][MATRIX_SIZE], float PRpolicy[MATRIX_SIZE][1] );

void Mdp_eval_policy_matrix_PI(float P[MATRIX_SIZE][MATRIX_SIZE][NB_ACTION],
							  float R[MATRIX_SIZE][NB_ACTION], float discount,
							  float policy[MATRIX_SIZE][1], float Vpolicy[MATRIX_SIZE][1]);

void bellman_eq_PI(float P[MATRIX_SIZE][MATRIX_SIZE][NB_ACTION],
				   float R[MATRIX_SIZE][NB_ACTION], float discount,
				   float Vpolicy[MATRIX_SIZE][1], float V[MATRIX_SIZE][1],
				   float policy[MATRIX_SIZE][1]);

void Mdp_Policy_Iteration_PI(float P[MATRIX_SIZE][MATRIX_SIZE][NB_ACTION],
							 float R[MATRIX_SIZE][NB_ACTION], float discount,
							 float policy[MATRIX_SIZE][1], float V[MATRIX_SIZE][1]);


//**********************


#endif
