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
 * File   : policy_iteration.cpp, 
 * Copyright (C) 2019 Lab-STICC Laboratory
 * Author(s) :  Chabha Hireche, chabha.hireche@gmail.com
 * 				Erwan Moréac, erwan.moreac@univ-ubs.fr (EM)
 * Created on: July 17, 2019
 * 
 *************************************************************************************/

#include "mdp_global.hpp"


// fonction inversion de matrice
void findInverse_PI(float B[MATRIX_SIZE][MATRIX_SIZE], float mat_inverse[MATRIX_SIZE][MATRIX_SIZE]){

	 float matrix[MATRIX_SIZE][2*MATRIX_SIZE], ratio,a;

	    for(int i = 0; i < MATRIX_SIZE; i++){
	        for(int j = MATRIX_SIZE; j < 2*MATRIX_SIZE; j++){
	        	matrix[i][j-MATRIX_SIZE]=B[i][j-MATRIX_SIZE];
	            if(i==(j-MATRIX_SIZE))
	                matrix[i][j] = 1.0;
	            else
	                matrix[i][j] = 0.0;
	        }
	    }

	    for(int i = 0; i < MATRIX_SIZE; i++){
	        for(int j = 0; j < MATRIX_SIZE; j++){
	            if(i!=j){
	                ratio = matrix[j][i]/matrix[i][i];

	                for(int k = 0; k < 2*MATRIX_SIZE; k++){
	                    matrix[j][k] -= ratio * matrix[i][k];
	                }
	            }
	        }
	    }

	    for(int i = 0; i < MATRIX_SIZE; i++){
	        a = matrix[i][i];
	        for(int j = 0; j < 2*MATRIX_SIZE; j++){
	            matrix[i][j] /= a;
	        }
	    }
	   // printf("The inverse matrix is: \n");
	    for(int i = 0; i < MATRIX_SIZE; i++){
	        for(int j = MATRIX_SIZE; j < 2*MATRIX_SIZE; j++){
	          	// printf("%.2f", matrix[i][j]);
            	// printf("\t");//
	        	mat_inverse[i][j-MATRIX_SIZE]=matrix[i][j];
	        }
	    }
}


// Produit de matrices
void ProduitMat_PI(float P[MATRIX_SIZE][MATRIX_SIZE][NB_ACTION], float Vprev[MATRIX_SIZE][1], float discount, float produit_vect[MATRIX_SIZE][NB_ACTION]){
	float Res1[MATRIX_SIZE][NB_ACTION];

	for(int a=0; a<NB_ACTION; a++){
		for(int i=0; i<MATRIX_SIZE; i++){
			Res1[i][a]=0; produit_vect[i][a]=0;
			for(int k=0; k<MATRIX_SIZE; k++){
				Res1[i][a] += P[i][k][a]*Vprev[k][0];
				produit_vect[i][a] = Res1[i][a]*discount;
			}
		}
	}
}

// Fonction Max
void max_PI(float Q[MATRIX_SIZE][NB_ACTION], float V[MATRIX_SIZE][1], float policy[MATRIX_SIZE][1]){

	float max;
	for(int i=0; i<MATRIX_SIZE; i++){
		 max = Q[i][0];
		for(int j=0; j<NB_ACTION; j++){
			if(Q[i][j] >= max){
				V[i][0] = Q[i][j];
				//policy0[i][0] = j;
			}
			else {
				V[i][0] = max;
				//policy0[i][0] = j;
			}
			max = V[i][0];
		}
	}
	for(int i=0; i<MATRIX_SIZE; i++){
		for(int j=0; j<NB_ACTION; j++){
			if (Q[i][j] == V[i][0])
				policy[i][0] = j;
		}
	}
}


void Compute_Ppolicy_PRpolicy_PI(float P[MATRIX_SIZE][MATRIX_SIZE][NB_ACTION], float R[MATRIX_SIZE][NB_ACTION], float policy[MATRIX_SIZE][1], float Ppolicy[MATRIX_SIZE][MATRIX_SIZE], float PRpolicy[MATRIX_SIZE][1] ){
	int indice;
	//float PR[MATRIX_SIZE][NB_ACTION];
	for(int a=0; a<NB_ACTION; a++){
		for(int i=0; i<MATRIX_SIZE; i++){
			indice = policy[i][0];

			for(int j=0; j<MATRIX_SIZE; j++){
				if(indice == a){
					Ppolicy[i][j] = P[i][j][a];
					PRpolicy[i][0] = R[i][a];
				}
			}
		}
	}
	// Affichage de Ppolicy et PRpolicy
	/*
		for(int i=0; i<MATRIX_SIZE; i++){
			for(int j=0; j<MATRIX_SIZE; j++){
				printf("Ppolicy : %f \n", Ppolicy[i][j]);
				printf("PRpolicy : %f \n", PRpolicy[i][0]);
			}
		}
	*/
}

void Mdp_eval_policy_matrix_PI(float P[MATRIX_SIZE][MATRIX_SIZE][NB_ACTION], float R[MATRIX_SIZE][NB_ACTION], float discount, float policy[MATRIX_SIZE][1], float Vpolicy[MATRIX_SIZE][1]){
	float mat_diag[MATRIX_SIZE][MATRIX_SIZE];
	float A[MATRIX_SIZE][MATRIX_SIZE];
	float B[MATRIX_SIZE][MATRIX_SIZE];
	float mat_inverse[MATRIX_SIZE][MATRIX_SIZE];
	float Ppolicy[MATRIX_SIZE][MATRIX_SIZE], PRpolicy[MATRIX_SIZE][1];

	// construction de la matrice diagonale
	for(int i=0; i<MATRIX_SIZE; i++){
		for(int j=0; j<MATRIX_SIZE; j++){
			if (i==j)
				mat_diag[i][j] = 1;
			else
				mat_diag[i][j] = 0;
		}
	}

	//construction de la matrice transition et reward associ�e � la politique
	Compute_Ppolicy_PRpolicy_PI(P, R, policy, Ppolicy, PRpolicy);

	for(int i=0; i<MATRIX_SIZE; i++){
		for(int j=0; j<MATRIX_SIZE; j++){
			A[i][j] = discount* Ppolicy[i][j];
			B[i][j] = mat_diag[i][j] - A[i][j];
		}
	}

	//calcul de l'inverse de la matrice B
	findInverse_PI(B,mat_inverse);

	//calcul des nouvelles values functions Vpolicy
	for (int i=0; i<MATRIX_SIZE; i++) {
		Vpolicy[i][0]=0;
		for(int k=0; k<MATRIX_SIZE; k++){
			Vpolicy[i][0] += mat_inverse[i][k]*PRpolicy[k][0];
		}
	}
		//Affichage Vpolicy

	//	printf("affichage de Vpolicy : \n");
/*	printf("Valeurs de Vpolicy : \n");
		for(int i=0; i<MATRIX_SIZE; i++){
			printf("%f \n", Vpolicy[i][0]);
		}*/
}


// Bellman Operator
void bellman_eq_PI(float P[MATRIX_SIZE][MATRIX_SIZE][NB_ACTION], float R[MATRIX_SIZE][NB_ACTION], float discount, float Vpolicy[MATRIX_SIZE][1], float V[MATRIX_SIZE][1], float policy[MATRIX_SIZE][1])
{
	float PR[MATRIX_SIZE][NB_ACTION];
	float Q[MATRIX_SIZE][NB_ACTION];
	float Res[MATRIX_SIZE][NB_ACTION];

	// Initialiser PR Rewards
	for(int i=0; i<MATRIX_SIZE; i++){
		for(int j=0; j<NB_ACTION; j++){
			PR[i][j] = R[i][j];
		}
	}

	// Initialiser la politique

	ProduitMat_PI(P,Vpolicy,discount,Res);


	for(int a=0; a<NB_ACTION; a++){
		for(int i=0; i<MATRIX_SIZE; i++){
			Q[i][a] = PR[i][a] + Res[i][a];
		}
	}

	for(int i=0; i<MATRIX_SIZE; i++){
		for(int a=0; a<NB_ACTION; a++){
			max_PI(Q, V, policy);
			 //printf("\tV==> %f", V[i][0]); printf("\tpolicy0==> %f\n", policy[i][0]);
		}
	//	printf("\n");
	}
}

// Fonction de r�solution avec l'algo policy_iteration
void Mdp_Policy_Iteration_PI(float P[MATRIX_SIZE][MATRIX_SIZE][NB_ACTION], float R[MATRIX_SIZE][NB_ACTION], float discount, float Next_Policy[MATRIX_SIZE][1], float V[MATRIX_SIZE][1]){
	float policyInit[MATRIX_SIZE][1];
	//float Next_Policy[MATRIX_SIZE][1];
	//float PR[MATRIX_SIZE][NB_ACTION];
	float Vpolicy[MATRIX_SIZE][1];

	// initialisation de Vprev � zero !!!!!! Vprev = Vpolicy
		for(int i=0; i<MATRIX_SIZE; i++){
			Vpolicy[i][0] = 0.0;
		}


	//initialisation de la politique policy0
	bellman_eq_PI(P, R, discount, Vpolicy, V, policyInit);

	//Affichage politique initiale
/*	for(int i=0; i<MATRIX_SIZE; i++){
		printf("Init Policy %f \n", policy0[i][0]);
		printf("Init V %f \n", V[i][0]);
	}
*/
	int nb_iter = 5;

	for(int iter=0; iter<nb_iter; iter++){
		Mdp_eval_policy_matrix_PI(P, R, discount, policyInit, Vpolicy);

		bellman_eq_PI(P, R, discount, Vpolicy, V, Next_Policy);

		for(int i=0; i<MATRIX_SIZE; i++){
			policyInit[i][0] = Next_Policy[i][0];
		}
	}
	/*for (int i=0; i<MATRIX_SIZE; i++) {
			//printf("Value fonction: %f \n", V[i][0]);
			printf("Politique : %f \n", Next_Policy[i][0]);

		}*/
}
