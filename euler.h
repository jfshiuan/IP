/*
 * Used for Fleury's algorithm.
 * Source: http://www.geeksforgeeks.com/fleurys-algorithm-for-printing-eulerian-path
 */

#include <iostream>
#include <algorithm>
#include <list>
using namespace std;

class Euler
{
   private:
      int N;
      list<int> *adj;
      list<int> *adjcopy;
      list<int> * tour;

      int DFSCountCopy(int v, bool visited[]);
      void createCopy(int u, int v);

   public:
      Euler(int N);
      ~Euler();

      void addEdge(int u, int v);
      void removeEdge(int u, int v);
      std::list<int> * getTour();
      void getTourUtil(int u);
      int DFSCount(int v, bool visited[]);
      bool isValidNextEdge(int u, int v);
};
