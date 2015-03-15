#include "common.h"
#include "Minmatching/PerfectMatching.h"
#include "euler.h"

#pragma once

class MST {
   public:
      float** adjacentMatrix;
      int* parent; //Array to store constructed MST
      int* parent2;
      int* parent3;
      int* key; //Key values used to pick minimum weight edge in cut
      bool* mstSet; //To represent set of vertices not yet included in MST
      int N; //the size of pointset
      std::multimap<int, int> * map;
      std::multimap<int, int> * combined;

      MST(float** adjacentMatrix, int size);
      ~MST();

      //deliverable a
      void makeTree();
      void printMST();
      void createMap();

      //deliverable b
      void makeTSP2();

      //deliverable c
      void makeTSP1_5();
      void LoadInput(int& node_num, int& edge_num, int*&edges, int*& weights, int *oddVertices, int numOdd);
      void PrintMatching(int node_num, PerfectMatching* pm, int *oddVertices);
      float calcSum(int option);
      int minKey(int key[], bool mstSet[]);
};
