#include "MST.h"

MST::MST(float** input, int size)
{
   adjacentMatrix = input;
   key = new int[size];   
   mstSet = new bool[size];  
   parent = new int[size];
   parent2 = new int[size];
   parent3 = new int[size];
   map = new std::multimap<int, int>;
   combined = new std::multimap<int, int>;

   for(int i=0; i<size; i++)
   {
      parent[i]=-1;
      parent2[i]=-1;
      parent3[i]=-1;
   }

   N = size;
}

MST::~MST()
{
   delete [] key;
   delete [] mstSet;
   delete [] parent;
   delete [] parent2;
   delete [] parent3;
   delete map;
   delete combined;
}


//use Prim's algorithm or Kruskal algorithm. Copied from 'http://www.geeksforgeeks.org/greedy-algorithms-set-5-prims-minimum-spanning-tree-mst-2/'
void MST::makeTree()
{ 
   // Initialize all keys as INFINITE
   for (int i = 0; i < N; i++)
      key[i] = INT_MAX, mstSet[i] = false;

   // Always include first 1st vertex in MST.
   key[0] = 0;     // Make key 0 so that this vertex is picked as first vertex
   parent[0] = -1; // First node is always root of MST 

   // The MST will have V vertices
   for (int count = 0; count < N-1; count++)
   {
      // Pick thd minimum key vertex from the set of vertices
      // not yet included in MST
      int u = minKey(key, mstSet);

      // Add the picked vertex to the MST Set
      mstSet[u] = true;

      // Update key value and parent index of the adjacent vertices of
      // the picked vertex. Consider only those vertices which are not yet
      // included in MST
      for (int v = 0; v < N; v++)
         // mstSet[v] is false for vertices not yet included in MST
         // Update the key only if adjacentMatrix[u][v] is smaller than key[v]
         if (adjacentMatrix[u][v] && mstSet[v] == false && adjacentMatrix[u][v] <  key[v])
            parent[v]  = u, key[v] = adjacentMatrix[u][v];
   }
}

// A utility function to find the vertex with minimum key value, from
// the set of vertices not yet included in MST
int MST::minKey(int key[], bool mstSet[])
{
   // Initialize min value
   int min = INT_MAX, min_index;

   for (int v = 0; v < N; v++)
      if (mstSet[v] == false && key[v] < min)
         min = key[v], min_index = v;

   return min_index;
}

// A utility function to print the constructed MST stored in parent[]
void MST::printMST() {
   cout<<endl;
   cout<<"Minimum spanning tree from the adjacency matrix"<<endl;
   cout<<"Edge   Weight"<<endl;
   for (int i = 1; i < N; i++) {
      cout<<parent[i]<<" - "<<i<<"  "<<adjacentMatrix[i][parent[i]]<<endl;
   }
   cout<<"\nTSP2:\n";
   for (int i = 0; i < N; i++) {
      cout<<parent2[i]<<" - "<<i<<"  "<<adjacentMatrix[i][parent2[i]]<<endl;
   }
   cout<<"\nTSP1_5:\n";
   for (int i = 0; i < N; i++) {
      cout<<parent3[i]<<" - "<<i<<"  "<<adjacentMatrix[i][parent3[i]]<<endl;
   }
}


float MST::calcSum(int option)
{
   float sum = 0.0;

   if(option == MST_1)
   {
      for(int i=1; i<N; i++)
      {
         sum = sum + adjacentMatrix[i][parent[i]];
      }
   }

   else if(option == TSP2)
   {
      for(int i=0; i<N; i++)
      {
         sum = sum + adjacentMatrix[i][parent2[i]];
      }
   }

   else if(option == TSP1_5)
   {
      for(int i=0; i<N; i++)
      {
         sum = sum + adjacentMatrix[i][parent3[i]];
      }
   }

   return sum;
}



void MST::createMap()
{
   for(int i=1; i<N; i++)
   {
      map->insert(std::pair<int, int>(parent[i], i));
      map->insert(std::pair<int, int>(i, parent[i]));
   }
}


void MST::makeTSP2() 
{
   bool visited [N];
   bool isParent [N];

   for(int i =0; i<N; i++)
   {
      isParent[i]=false;
      visited[i]=false;
   }

   std::stack<int> vertexStack;
   vertexStack.push(0);
   int node=0;
   int previous=-1;

   while(!vertexStack.empty())
   {
      node=vertexStack.top();
      visited[node]=true;

      if(previous!=-1)
      {
         parent2[node]=previous;
         isParent[previous]=true;
      }

      previous=node;
      vertexStack.pop();
      std::pair<std::multimap<int, int>::iterator, std::multimap<int, int>::iterator> ret;
      ret=map->equal_range(node);

      for(std::multimap<int, int>::iterator it=ret.first ; it!=ret.second; ++it)
      {
         if(!visited[it->second])
         { 
            vertexStack.push(it->second);
         }
      }
   }

   parent2[0]=previous;
}


void MST::makeTSP1_5()
{
   bool isOdd [N];
   int count=0;
   int numOdd=0;
   std::pair<std::multimap<int, int>::iterator, std::multimap<int, int>::iterator> ret;

   for(int i=0; i<N; i++)
   {
      count=0;
      ret=map->equal_range(i);

      for(std::multimap<int, int>::iterator it=ret.first ; it!=ret.second; ++it)
      {
         count++;
      }

      if(count%2==1) //is odd
      {
         isOdd[i]=true;
         numOdd++;
      }

      else
      {
         isOdd[i]=false;
      }
   }


   int oddVertices[numOdd];
   int index=0;

   for(int i=0; i<N; i++)
   {
      if(isOdd[i])
      {
         oddVertices[index] =i;
         index++;
      }
   }


   struct PerfectMatching::Options options;
   options.verbose=false;
   int i, e, node_num = numOdd, edge_num=numOdd*(numOdd-1)/2;
   int * edges;
   int * weights;
   PerfectMatching *pm = new PerfectMatching(node_num, edge_num);
   LoadInput(node_num, edge_num, edges, weights, oddVertices, numOdd);

   for(e=0; e<edge_num; e++)
   {
      pm->AddEdge(edges[2*e], edges[2*e+1], weights[e]);
   }

   pm->options = options;
   pm->Solve();


   combined = new std::multimap<int, int>(*map);
   int match=-1;

   for(int i=0; i<node_num; i++)
   {
      match=pm->GetMatch(i);
      std::pair<std::multimap<int, int>::iterator, std::multimap<int, int>::iterator> ret;
      ret = combined->equal_range(oddVertices[i]);
      combined->insert(std::pair<int, int>(oddVertices[i], oddVertices[match]));
      combined->insert(std::pair<int, int>(oddVertices[match], oddVertices[i]));
   }

   delete pm;
   delete [] edges;
   delete [] weights;

   Euler euler(N);

   for(std::multimap<int, int>::iterator it=combined->begin(); it!=combined->end(); ++it)
   {
      if(it->first > it->second)
      {
         euler.addEdge(it->first, it->second);
      }
   }

   std::list<int> *tour;
   tour = euler.getTour();
   bool visited[N];

   for(int i=0; i<N; i++)
   {
      visited[i]=false;
   }

   int previous=-1;
   for(std::list<int>::iterator it=tour->begin(); it!=tour->end(); ++it)
   {
      if(!visited[*it])
      {
         visited[*it]=true;
         parent3[*it]=previous;
         previous=*it;
      }
   }

   parent3[*(tour->begin())] = previous;

   delete tour;
}




void MST::LoadInput(int& node_num, int& edge_num, int*& edges, int*& weights, int * oddVertices, int numOdd)
{
   int e = 0;
   node_num=numOdd;
   edge_num=numOdd*(numOdd-1)/2;
   edges = new int[2*edge_num];
   weights = new int[edge_num];

   for(int i=0; i <numOdd; i++)
   {
      for(int j=i+1; j<numOdd; j++)
      {
         edges[2*e] = i;
         edges[2*e+1]=j;
         weights[e] = adjacentMatrix[oddVertices[i]][oddVertices[j]];
         e++;
      }
   }

   if(e!=edge_num)
   {
      cout<<"the number of edges is wrong"<<endl;
      exit(1);
   }
}


void MST::PrintMatching(int node_num, PerfectMatching* pm, int *oddVertices)
{
   int i, j;

   for(i=0; i<node_num; i++)
   {
      j=pm->GetMatch(i);

      if(i<j)
      {
         printf("%d %d\n", oddVertices[i], oddVertices[j]);
      }
   }
}
