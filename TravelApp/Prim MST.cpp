#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include<iostream>
 
using namespace std;
// A structure to represent a node in adjacency list
struct AdjListNode
{
    int dest;
    int Junction_id;
    int light_time;
    int build_time;
    int traverse_time;
    struct AdjListNode* next;
};
 
// A structure to represent an adjacency liat
struct AdjList
{	int scr;
	int junction_id;
	int light_time;
    struct AdjListNode *head;  // pointer to head node of list
};
 
// A structure to represent a graph. A graph is an array of adjacency lists.
// Size of array will be V (number of vertices in graph)
struct Graph
{
    int V;
    struct AdjList* array;
};
 
// A utility function to create a new adjacency list node
AdjListNode* newAdjListNode(int dest, int junction_id,int light_time, int build_time, int traverse_time)
{
    AdjListNode* newNode =new AdjListNode;
    newNode->dest = dest;
    newNode->Junction_id = junction_id;
    newNode->light_time=light_time;
    newNode->build_time=build_time;
    newNode->traverse_time=traverse_time;
    newNode->next = NULL;
    return newNode;
}
 
// A utility function that creates a graph of V vertices
struct Graph* createGraph(int V, int v[][2])
{
    Graph* graph = new Graph;
    graph->V = V;
 
    // Create an array of adjacency lists.  Size of array will be V
    graph->array = new AdjList[100];
 
     // Initialize each adjacency list as empty by making head as NULL
    for (int i = 0; i < V; i++)
	{		graph->array[i].scr=i;
			graph->array[i].junction_id=v[i][0];
			graph->array[i].light_time=v[i][1];
	        graph->array[i].head = NULL;
 
	}

    return graph;
}

Graph *AddJunction(Graph *graph,int x,int y) 
{  int v=graph->V;
	graph->array[v].scr=v;
	graph->array[v].junction_id=x;
	graph->array[v].light_time=y;
	graph->array[v].head = NULL;
	graph->V=graph->V+1;
	//cout<<"done";
	return graph;	
}

// Adds an edge to an undirected graph
Graph *addEdge(Graph* graph, int edge[])
{
    // Add an edge from src to dest.  A new node is added to the adjacency
    // list of src.  The node is added at the begining
    int dest=-1,src=-1;
    int V=graph->V; //cout<<"doubt check V is "<<V<<endl;
	for(int i=0;i<V;i++)
	if(graph->array[i].junction_id==edge[0])
	{src=i; //cout<<"Source is "<<src<<endl; 
	 break;
	}
	for(int i=0;i<V;i++)
	if(graph->array[i].junction_id==edge[1])
	{dest=i;// cout<<"Destination is "<<dest; 
	 break;
	}
	if(src==-1 || dest==-1)
	{//cout<<"Unsuccessful edge addition";
		return graph;
	}	
	if(src!=-1 && dest!=-1)
	{
	AdjListNode* newNode = newAdjListNode(dest,graph->array[dest].junction_id,graph->array[dest].light_time,edge[2],edge[3] );
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;
 
    // Since graph is undirected, add an edge from dest to src also
    newNode = newAdjListNode(src,graph->array[src].junction_id,graph->array[src].light_time,edge[2],edge[3] );
    newNode->next = graph->array[dest].head;
    graph->array[dest].head = newNode;
	}
	return graph;
	
}
 
void DemolishRoad(Graph *graph,int x,int y)
{   int V=graph->V;//cout<<"entres function\n";
	for(int i=0;i<V;i++)
{
		if(graph->array[i].junction_id==x)
	{	AdjListNode *p,*q;
		//cout<<"node found\n";
		p= graph->array[i].head;
	    if(p==NULL) return;
	    if(p->next==NULL && p->Junction_id==y)
	    {graph->array[i].head=NULL; free(p); return;
		}
		while(p!=NULL)
	    { if(p->Junction_id==y)
	    	{q->next=p->next;
	    	free(p);
			//cout<<"Demolished\n";
			break;
			}
			q=p; p=p->next;
		}
		
	}
	if(graph->array[i].junction_id==y)
	{	AdjListNode *p,*q;
		//cout<<"node found\n";
		p= graph->array[i].head;
	    if(p==NULL) return;
	    if(p->next==NULL && p->Junction_id==x)
	    {graph->array[i].head=NULL; free(p); return;
		}
		while(p!=NULL)
	    { if(p->Junction_id==x)
	    	{q->next=p->next;
	    	free(p);
			//cout<<"Demolished\n";
			break;
			}
			q=p; p=p->next;
		}
		
	}
}
	
 } 
 
// Structure to represent a min heap node
struct MinHeapNode
{
    int  v;
    int key;
};
 
// Structure to represent a min heap
struct MinHeap
{
    int size;      // Number of heap nodes present currently
    int capacity;  // Capacity of min heap
    int *pos;     // This is needed for decreaseKey()
    struct MinHeapNode **array;
};
 
// A utility function to create a new Min Heap Node
struct MinHeapNode* newMinHeapNode(int v, int key)
{
    struct MinHeapNode* minHeapNode =
           (struct MinHeapNode*) malloc(sizeof(struct MinHeapNode));
    minHeapNode->v = v;
    minHeapNode->key = key;
    return minHeapNode;
}
 
// A utilit function to create a Min Heap
struct MinHeap* createMinHeap(int capacity)
{
    struct MinHeap* minHeap =
         (struct MinHeap*) malloc(sizeof(struct MinHeap));
    minHeap->pos = (int *)malloc(capacity * sizeof(int));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array =
         (struct MinHeapNode**) malloc(capacity * sizeof(struct MinHeapNode*));
    return minHeap;
}
 
// A utility function to swap two nodes of min heap. Needed for min heapify
void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b)
{
    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}
 
// A standard function to heapify at given idx
// This function also updates position of nodes when they are swapped.
// Position is needed for decreaseKey()
void minHeapify(struct MinHeap* minHeap, int idx)
{
    int smallest, left, right;
    smallest = idx;
    left = 2 * idx + 1;
    right = 2 * idx + 2;
 
    if (left < minHeap->size &&
        minHeap->array[left]->key < minHeap->array[smallest]->key )
      smallest = left;
 
    if (right < minHeap->size &&
        minHeap->array[right]->key < minHeap->array[smallest]->key )
      smallest = right;
 
    if (smallest != idx)
    {
        // The nodes to be swapped in min heap
        MinHeapNode *smallestNode = minHeap->array[smallest];
        MinHeapNode *idxNode = minHeap->array[idx];
 
        // Swap positions
        minHeap->pos[smallestNode->v] = idx;
        minHeap->pos[idxNode->v] = smallest;
 
        // Swap nodes
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);
 
        minHeapify(minHeap, smallest);
    }
}
 
// A utility function to check if the given minHeap is ampty or not
int isEmpty(struct MinHeap* minHeap)
{
    return minHeap->size == 0;
}
 
// Standard function to extract minimum node from heap
struct MinHeapNode* extractMin(struct MinHeap* minHeap)
{
    if (isEmpty(minHeap))
        return NULL;
 
    // Store the root node
    struct MinHeapNode* root = minHeap->array[0];
 
    // Replace root node with last node
    struct MinHeapNode* lastNode = minHeap->array[minHeap->size - 1];
    minHeap->array[0] = lastNode;
 
    // Update position of last node
    minHeap->pos[root->v] = minHeap->size-1;
    minHeap->pos[lastNode->v] = 0;
 
    // Reduce heap size and heapify root
    --minHeap->size;
    minHeapify(minHeap, 0);
 
    return root;
}
 
// Function to decreasy key value of a given vertex v. This function
// uses pos[] of min heap to get the current index of node in min heap
void decreaseKey(struct MinHeap* minHeap, int v, int key)
{
    // Get the index of v in  heap array
    int i = minHeap->pos[v];
 
    // Get the node and update its key value
    minHeap->array[i]->key = key;
 
    // Travel up while the complete tree is not hepified.
    // This is a O(Logn) loop
    while (i && minHeap->array[i]->key < minHeap->array[(i - 1) / 2]->key)
    {
        // Swap this node with its parent
        minHeap->pos[minHeap->array[i]->v] = (i-1)/2;
        minHeap->pos[minHeap->array[(i-1)/2]->v] = i;
        swapMinHeapNode(&minHeap->array[i],  &minHeap->array[(i - 1) / 2]);
 
        // move to parent index
        i = (i - 1) / 2;
    }
}
 
// A utility function to check if a given vertex
// 'v' is in min heap or not
bool isInMinHeap(struct MinHeap *minHeap, int v)
{
   if (minHeap->pos[v] < minHeap->size)
     return true;
   return false;
}


void MergeSort( int a[][2],  int l,  int r)
{
	if (l < r)
    {   int m = (r+l)/2;
        MergeSort(a, l, m);
        MergeSort(a, m+1, r);
		//////Merging Starts
					   int n1=m-l+1, n2=r-m;
					 	int L[n1][2], R[n2][2];
					 
					    for(  int i=0;i<n1;i++)
						{  L[i][0] = a[l + i][0];
						   L[i][1] = a[l + i][1];
						}   
						for ( int j = 0; j < n2; j++)
					    { R[j][0] = a[m+1+j][0];
					      R[j][1] = a[m+1+j][1];
						}    
					 
					    int i=0,j=0,k=l; 
					    while (i < n1 && j < n2)
					    {
					        if (L[i][0] < R[j][0])
					        {	a[k][0] = L[i][0]; 
								a[k][1] = L[i][1];
					            i++;
					        }
					        else if (L[i][0] > R[j][0])
					        {	a[k][0] = R[j][0];
					            a[k][1] = R[j][1];
					            j++;
					        }
					        else if (L[i][0] == R[j][0])
					        if(L[i][1] <= R[j][1])
					        {	a[k][0] = L[i][0]; 
								a[k][1] = L[i][1];
					            i++;
					        }
					        else{	a[k][0] = R[j][0];
					            a[k][1] = R[j][1];
					            j++;
					        }
					        
					        k++;
					    }
					    while (i < n1)
					    {
					        a[k][0] = L[i][0]; a[k][1] = L[i][1];
					        i++;
					        k++;
					    }
					    while (j < n2)
					    {	a[k][1] = R[j][1];
					        a[k][0] = R[j][0];
					        j++; k++;
					    }
					
		//////Merging Ends
	}
}




 
// A utility function used to print the constructed MST
void printArr(int arr[], int n, Graph *G)
{   //cout<<"Parent array is ";
	//for(int i=0;i<n;i++)
	//cout<<arr[i]<<"  ";
	//cout<<endl<<"no of edges is "<<n-1<<"\n"; 
	cout<<n-1<<" ";
	int weight=0;
    for (int i = 1; i < n; ++i)
    {   //cout<<"Value of i is "<<i;
		int V=G->V; int j=arr[i];
		AdjListNode *r =G->array[j].head;
    	while(r!=NULL)
    	{
    		if(r->Junction_id==G->array[i].junction_id)
    		{	//cout<<"weight = "<<weight;
				weight=weight+r->build_time;
    		    //cout<<"+ "<<r->build_time<<" = "<<weight<<endl;
			 break;
			}
    		r=r->next;
    		
		//	cout<<"Value of V is "<<V<<" in while loop"<<endl;
		}
    //	cout<<"IN for loop"<<endl;
		}	
    cout<<weight<<" ";
    int a[n-1][2];
	for (int i = 1; i < n; ++i)
    if(G->array[arr[i]].junction_id <G->array[i].junction_id)
     { a[i-1][0]=G->array[arr[i]].junction_id; 
	   a[i-1][1]=G->array[i].junction_id;}
    else 
     {a[i-1][1]=G->array[arr[i]].junction_id;a[i-1][0]=G->array[i].junction_id;
	   }  	
   	MergeSort(a,0,n-2);
	   for(int i=0;i<n-1;i++)
   	cout<<a[i][0]<<" "<<a[i][1]<<" ";
     	
    	

}
 
// The main function that constructs Minimum Spanning Tree (MST)
// using Prim's algorithm
void PrimMST(struct Graph* graph)
{
    int V = graph->V;// Get the number of vertices in graph
    int parent[V];   // Array to store constructed MST
    int key[V];      // Key values used to pick minimum weight edge in cut
 
    // minHeap represents set E
    struct MinHeap* minHeap = createMinHeap(V);
 
    // Initialize min heap with all vertices. Key value of
    // all vertices (except 0th vertex) is initially infinite
    for (int v = 1; v < V; ++v)
    {
        parent[v] = -1;
        key[v] = INT_MAX;
        minHeap->array[v] = newMinHeapNode(v, key[v]);
        minHeap->pos[v] = v;
    }
 
    // Make key value of 0th vertex as 0 so that it
    // is extracted first
    key[0] = 0;
    minHeap->array[0] = newMinHeapNode(0, key[0]);
    minHeap->pos[0]   = 0;
 
    // Initially size of min heap is equal to V
    minHeap->size = V;
 
    // In the followin loop, min heap contains all nodes
    // not yet added to MST.
    while (!isEmpty(minHeap))
    {
        // Extract the vertex with minimum key value
        struct MinHeapNode* minHeapNode = extractMin(minHeap);
        int u = minHeapNode->v; // Store the extracted vertex number
 
        // Traverse through all adjacent vertices of u (the extracted
        // vertex) and update their key values
        struct AdjListNode* pCrawl = graph->array[u].head;
        while (pCrawl != NULL)
        {
            int v = pCrawl->dest;
 
            // If v is not yet included in MST and weight of u-v is
            // less than key value of v, then update key value and
            // parent of v
            if (isInMinHeap(minHeap, v) && pCrawl->build_time < key[v])
            {
                key[v] = pCrawl->build_time;
                parent[v] = u;
                decreaseKey(minHeap, v, key[v]);
            }
            pCrawl = pCrawl->next;
        }
    }
 
    // print edges of MST
    printArr(parent, V, graph);
}
 
// Driver program to test above functions
int main()
{
    // Let us create the graph given in above fugure
    int V,E,Q,fun;
    cin>>V>>E;
    int vertex[V][2],edge[E][4];
    for(int i=0;i<V;i++)
    cin>>vertex[i][0]>>vertex[i][1];
    for(int i=0;i<E;i++)
    cin>>edge[i][0]>>edge[i][1]>>edge[i][2]>>edge[i][3];
    Graph* graph = createGraph(V,vertex);
    for(int i=0;i<E;i++)
    {int Edge[4];
    	for(int j=0;j<4;j++)
    	Edge[j]=edge[i][j];
    	graph=addEdge(graph,Edge);
    }
    cin>>Q;
	for(int i=0;i<Q;i++)
    { 	//cout<<i+1<<". ";
	    cin>>fun;
        
    	switch(fun)
		{ 
		  
		  case 1:  int x,y; cin>>x>>y; graph=AddJunction(graph,x,y); break;
		  case 2:  int edg[4]; cin>>edg[0]>>edg[1]>>edg[2]>>edg[4]; graph= addEdge(graph,edg); break;  							
		  case 3:  int a,b; cin>>a>>b; DemolishRoad(graph,a,b); break;
		  case 4:  PrimMST(graph); break;
		 // case 5:  break;
		  default: break;	
		           
		}
	}
    
 
    
}