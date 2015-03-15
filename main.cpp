#include "common.h"
#include "point.h"
#include "MST.h"
#include <cmath>
#include <time.h>

/*
   This project is a starter code and wrappers for CSE101W15 Implementation project.

   point.h - uniform random pointset generator

   MST.h - minimum spanning tree

   PerfectMatching.h - interface to min cost perfect matching code 

   -------------------------------------
   PerfectMatching is from the paper:

   Vladimir Kolmogorov. "Blossom V: A new implementation of a minimum cost perfect matching algorithm."
   In Mathematical Programming Computation (MPC), July 2009, 1(1):43-67.

sourcecode : pub.ist.ac.at/~vnk/software/blossom5-v2.05.src.tar.gz

*/


int main()
{
   set< pair<int,int> > generatedPointset;
   float** adjacentMatrix;
   int W, H, N;
   Point * pointset;

clock_t start=0;
clock_t end=0;
clock_t start2=0;
clock_t start3=0;
clock_t end2=0;
clock_t end3=0;


   W = 100;
   H = 100;
   N = 100;
   int numTrials=100;
   cout<<"W: "<<W<<" H: "<<H<<" N:"<<N<<endl;


   float sum [numTrials];
   float sumTSP2[numTrials];
   float sumTSP1_5[numTrials];
   float mean=0.0;
   float meanTSP2=0.0;
   float meanTSP1_5=0.0;

   MST * mst;

   double time1=0;
   double time2=0;
   double time3=0;

   for(int i=0; i<numTrials; i++)
   {
      pointset = new Point;
      pointset->generatePoint(W, H, N);
      //pointset->printPointset();

      mst = new MST(pointset->getAdjacentMatrix(), N);
      start=clock();
      mst->makeTree();
      end=clock();
      start2=clock();
      mst->createMap();
      mst->makeTSP2();
      end2=clock();
      start3=clock();
      mst->makeTSP1_5();
      end3=clock();


time1= time1 + (double)(end-start)/CLOCKS_PER_SEC/(double)numTrials;
time2= time2 + (double)(end2-start2)/CLOCKS_PER_SEC/(double)numTrials;
time3= time3 + (double)(end3-start3)/CLOCKS_PER_SEC/(double)numTrials;

      //mst->printMST();
      sum[i]= mst->calcSum(MST_1);
      sumTSP2[i] = mst->calcSum(TSP2);
      sumTSP1_5[i] = mst->calcSum(TSP1_5);
      mean = mean + sum[i];
      meanTSP2 = meanTSP2 + sumTSP2[i];
      meanTSP1_5 = meanTSP1_5 + sumTSP1_5[i];
      delete pointset;
      delete mst;
   }

   mean = mean/ (float)numTrials;
   meanTSP2 = meanTSP2 / (float)numTrials;
   meanTSP1_5 = meanTSP1_5 / (float)numTrials;

   float deviation = 0.0;
   float deviationTSP2= 0.0;
   float deviationTSP1_5=0.0;

   for(int i=0; i<numTrials; i++)
   {
      deviation = deviation + pow(sum[i] - mean, 2);
      deviationTSP2 = deviationTSP2 + pow(sumTSP2[i] - meanTSP2, 2);
      deviationTSP1_5 = deviationTSP1_5 + pow(sumTSP1_5[i] - meanTSP1_5, 2);
   }


   deviation = sqrt(deviation / ((float)(numTrials-1)));
   deviationTSP2 = sqrt(deviationTSP2 / ((float)(numTrials-1)));
   deviationTSP1_5 = sqrt(deviationTSP1_5 / ((float)(numTrials-1)));
   cout<<"mean: "<<mean<<"\n";
   cout<<"standard deviation: "<<deviation<<"\n";
   cout<<"meanTSP2: "<<meanTSP2<<"\n";
   cout<<"standard deviation TSP2: "<<deviationTSP2<<"\n";
   cout<<"meanTSP1_5: "<<meanTSP1_5<<"\n";
   cout<<"standard deviation TSP1_5: "<<deviationTSP1_5<<"\n";

printf("%f seconds\n", time1);
printf("%f seconds\n", time2);
printf("%f seconds\n", time3);

   return 0;
}
