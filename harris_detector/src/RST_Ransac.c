// -------------------------------------------------------
// RANSAC_rotation_translation
// -------------------------------------------------------
//Includes
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>
#include <time.h>
#include "image.h"


typedef float Homography[3][3] ;
typedef float SkewMatrix[2] ;
typedef float ParameterVector[4] ;


 //random
int rand_a_b(int a, int b);
//calcul RST
void RST_estimation(Point point1[2], Point point2[2], Homography H);
//Affichage et debug
void affichage_homography(Homography H);


int rand_a_b(int a, int b)
{
    return rand()%(b-a) +a;
}


// Estimation RST
//point1, point2 = 2 pairs of point matches (cartesian coordinates)
//point1: 2 interests point of reference image
//point2: 2 interests point of current image
//point1[0] matches point2[0]
//point1[1] matches point2[1]
void RST_estimation(Point point1[2], Point point2[2], Homography H)
{

  SkewMatrix MM,MMi;
  float detM;
  int delta[2];
  float theta[4];

  //2 sub
  MM[0] =  point1[0].X - point1[1].X;
  MM[1] = point1[0].Y -  point1[1].Y;
  //determinant M : 1 add, 2 mult
  detM = MM[0]*MM[0] + MM[1]*MM[1];
  //inverse M: 2 div
  MMi[0]=MM[0]/detM;
  MMi[1]=MM[1]/detM;
  //2 sub
  delta[0] = point2[0].X-point2[1].X;
  delta[1] = point2[0].Y-point2[1].Y;
   //compute parameters
   //1 add, 2 mult
  theta[0] = MMi[0]*delta[0] + MMi[1]*delta[1];
   //1 sub, 2 mult
  theta[1] = MMi[0]*delta[1] - MMi[1]*delta[0];
   // 1 add, 1 sub, 2 mult
  theta[2] = point2[1].X - theta[0]* point1[1].X + theta[1]*point1[1].Y;
   //2 sub, 2 mult
  theta[3] = point2[1].Y - theta[0]* point1[1].Y - theta[1]*point1[1].X;

  H[0][0]= H[1][1]= theta[0];
  H[0][1]=-theta[1];
  H[0][2]= theta[2];
  H[1][0]= theta[1];
  H[1][2]= theta[3];
  H[2][0]= H[2][1]= 0;
  H[2][2]=1;
 }

void print_homography(Homography H)
{
  int i,y;
  for(i = 0; i<3; i++)
  {
    for(y = 0; y<3; y++)
    {
      printf("| %f |",H[i][y]);
    }
    printf("\n");
  }
}

float distanceBetween2Points(Point p1,Point p2)
{
      float d=sqrt((p2.X-p1.X)*(p2.X-p1.X)+  (p2.Y-p1.Y)*(p2.Y-p1.Y));
      return d;
}


int squareDistanceBetween2Points(Point p1,Point p2)
{
      int d=(p2.X-p1.X)*(p2.X-p1.X)+  (p2.Y-p1.Y)*(p2.Y-p1.Y);
      return d;
}

Point warpTransform(Point p1,Homography H)
{
    Point p2;

    p2.X = H[0][0]*(float)p1.X+H[0][1]*(float)p1.Y+H[0][2];
    p2.Y = H[1][0]*(float)p1.X+H[1][1]*(float)p1.Y+H[1][2];

    return p2;
}


//Main
void RST_RANSAC(Point *point1,Point *point2,int N_SAMPLES,int threshold,float *theta,int *tx,int *ty ){

  //Variable main
  Homography H,bestH;
  Point point1Pair[2];
  Point point2Pair[2];
  Point point2Estimate;
  float errorPoint;
  int rand1,rand2;
  int ite,nbIterations=500;
  float p=0.99; /* probabiblity of success */
  int n_inliers,n,max_inliers;
  long MSE,min_MSE;
  float f;


  printf("RANSAC RST\n");
  max_inliers=0;
  min_MSE=LONG_MAX;
  for (ite=0;ite< nbIterations;ite++)
  {
        //printf("Iteration %d ...\n",ite);
        // Estimation: take 2 pairs of models randomly
        rand1 = rand()%N_SAMPLES;
        rand2 = rand()%N_SAMPLES;
        //debug printf("rand 1 %d, rand 2 %d \n",rand1,rand2);
        point1Pair[0]=point1[rand1];
        point1Pair[1]=point1[rand2];
        point2Pair[0]=point2[rand1];
        point2Pair[1]=point2[rand2];

        //compute the homography Hcurr from the 2 points pairs
        RST_estimation(point1Pair,point2Pair,H);
        //for each pairs, count the number of inliers which has the distance di < threshold and compute the Mean Square Error
        n_inliers=0;MSE=0;
        for (n=0;n<N_SAMPLES;n++)
        {
            point2Estimate=warpTransform(point1[n],H);
            errorPoint=squareDistanceBetween2Points(point2[n],point2Estimate);
            if (errorPoint<threshold) {
                    n_inliers++;
                    MSE+=errorPoint;
            }
        }
        //update best H
        if ((n_inliers>=max_inliers) && (MSE<=min_MSE))
        {
            max_inliers=n_inliers;
            min_MSE=MSE;
            memcpy(bestH,H,sizeof(H));
            //printf("ite %d n_inliers %d MSE %ld \n",ite,n_inliers,MSE);
            //probability that a point is an outlier
            //e = 1.0 - (float)n_inliers/(float)N_SAMPLES;
            //printf("e %f \n",e);
            //probability that a point is an inlier
            f = (float)n_inliers/(float)N_SAMPLES;
            //printf("probability that a point is an inlier f= %f \n",f);
            //nbIterations = log(1.0-p)/log(1.0-(1.0-e)*(1.0-e));
            nbIterations = log(1.0-p)/log(1.0-f*f);
            //printf("nbIterations %d \n",nbIterations);
        }
  }
 
  printf("Results after %d iterations  : %d inliers %ld MSE\n",ite,max_inliers,min_MSE);
    // Affichage
   printf("Best Homography Matrix :\n");
   print_homography(bestH);
  *theta=atan2(bestH[1][0],bestH[0][0]);
  *tx=(int)(bestH[0][2]+0.5);
  *ty=(int)(bestH[1][2]+0.5);
  printf("Estimate rotation phi : %f\n",*theta);
  printf("Estimate translation X : %d\n",*tx);
  printf("Estimate translation Y : %d\n",*ty);
}
