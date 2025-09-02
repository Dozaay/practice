/*
 * In this file, you'll implement Dijkstra's algorithm to find the least
 * expensive paths in the graph defined in `airports.dat`.  Don't forget to
 * include your name and @oregonstate.edu email address below.
 *
 * Name:David Velasco Perez
 * Email:velascod@oregonstate.edu
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "pq.h"

#define DATA_FILE "airports.dat"
#define START_NODE 0

// This struct represents current node path  
struct path{
    int prev;
    int node;
    int cost;
};

/*
 * Function name: create_adj_mat(n_nodes, n_edges)
 * Description: This function allocates an n_nodes x n_nodes  adjacency
    matrix and fills it with edge values( mat[node_i][node_j] = cost_i_j)
 * Params: 
    n_nodes - number of nodes in file
    n_edges - number of edges in file
 * Returns: returns a pointer to the newly created adjacency matrix 
 * */
int** create_adj_mat(int n_nodes, int n_edges){
    // allocate n_nodes x n_nodes matrix 
    int** adj_mat = malloc(n_nodes * sizeof(int*));
    for (int i = START_NODE; i < n_nodes; i++){
        adj_mat[i] = calloc(n_nodes, sizeof(int));
    }
    
    FILE* file = fopen(DATA_FILE, "r");
    fscanf(file, "%*d %*d");
    
    // insert values from file into matrix
    for(int i = START_NODE; i < n_edges; i++){
        int n_i, n_j, cost;
        fscanf(file, "%d %d %d", &n_i, &n_j, &cost);
        adj_mat[n_i][n_j] = cost;
    }
    fclose(file);
    return adj_mat;
}

/*
 * Function name: path_create(n_nodes)
 * Description: This function creates a path array of n_nodes and fills it with base values
 * Params: number of nodes in this path
 * Returns: a pointer to the array of path P
 * */
struct path *path_create(int n_nodes){
    struct path* P  = malloc(n_nodes * sizeof(struct path));
    for (int i = 0 ; i < n_nodes; i++){
        P[i].prev = 0;
        P[i].node = i;
        P[i].cost = INT_MAX;
    
    }
    return P;
}

/*
 * Function name: path_free(paths)
 * Description: This function frees the path struct
 * Params: 
    paths - desired path to be free'd
 * Returns: void function so there is no return
 * */
void path_free(struct path* paths){
    free(paths); 
}


/* 
 * Function name: single_path(int prev, int node, int cost)
 * Description: This function creates the path at desired node, with whatever 
    cost you want
 * Params:
    prev - previous path node of current node 
    node - current node 
    cost - cost you wish to have at that node
 * Returns: Returns a pointer to newly created path 
 * */
struct path *single_path(int prev, int node, int cost){
    
    struct path* path = malloc(sizeof(struct path));
    path->prev = prev;
    path->node = node;
    path->cost = cost;
    return path;
}

/*
 * Function name: update_path(pq, paths, adj_mat, n_nodes)
 * Description: this function updates the path with the values of the adj_matrix of least cost.
    Using the priority queue, this will calculate the least cost from node 0 up to n_nodes
 * Params:
    pq - prioity queue used to to determines paths of least cost
    paths - path filled with updated values of path with least cost  
    adj_mat - matrix filled all edge values
    n_nodes - used for iterating through the adjacency matrix
 * Returns: void function so return type
 * */
void update_path(struct pq* pq, struct path* paths, int** adj_mat, int n_nodes){
    while(!pq_isempty(pq)){
        struct path *curr = (struct path*)pq_remove_first(pq);
        // check if the curr node has been visited or not
        if(paths[curr->node].cost == INT_MAX){
            paths[curr->node] = *curr;
            // loop through all nodes to find neighbor of curr nodes
            for(int i = 0; i < n_nodes; i++){
                // check if there is an edge between curr node and node i
                if(adj_mat[curr->node][i] != 0){
                    
                    int new_cost = curr->cost + adj_mat[curr->node][i];
                    // if new cost is less than curr cost, insert neighbor
                    // path into pq with new cost as the priority
                    if(new_cost < paths[i].cost){
                        struct path* neighbor = single_path(curr->node, i, new_cost);
                        pq_insert(pq, neighbor, neighbor->cost);
                    }

                }

            }

        }
        free(curr);
    }
}

/*
 * Function name: print_path(paths, n_nodes)
 * Description: this function prints the values of the path at each node up until n_nodes
 * Params:
    paths - desired path to print the values of 
    n_nodes - used to iterate through path of n_nodes
 * Returns: void function so no return type
 * */
void print_path(struct path* paths, int n_nodes){
    for (int i = START_NODE; i < n_nodes; i++){
        printf("\nCost to node %d: %d -- Previous node : %d ",i , paths[i].cost , paths[i].prev);
    }

}

/*
 * Function name: dijkstra(adj_mat, n_nodes)
 * Description:This function implements Dijkstra's 
    algorithm to find the shortest paths from the
    start node to all other nodes in the graph.
 * Params: 
    adj_mat - adjacecny matrix containg the file values
    n_nodes - number of nodes in the graph 
 * Return: void return type
 * */
void dijkstra(int **adj_mat, int n_nodes){
    //initialize paths
    struct path *paths = path_create(n_nodes);

    // initialize pq
    struct pq* pq = pq_create();
    struct path* start = single_path(0, START_NODE, 0); 
    pq_insert(pq, start, start->cost);
    
    //update paths
    update_path(pq, paths, adj_mat, n_nodes); 
    
    //print paths
    print_path(paths, n_nodes);

    path_free(paths);
    pq_free(pq);
    
    //


}

int main(int argc, char const *argv[]) {
	/*
	 * Open file and read the first two int: num of nodes, num of edges
	 */
	int n_nodes, n_edges;

	FILE* file = fopen(DATA_FILE, "r");
    fscanf(file, " %d %d ", &n_nodes, &n_edges);
    
    fclose(file);
    
    int** adj_mat = create_adj_mat(n_nodes, n_edges);
   
	/*
     *
	 * Write your code here to find the
	 * least-cost paths from node 0 to all other nodes in the graph.  Make sure
	 * to print out the least-cost paths you find at the end.
	 *
 	 */

    dijkstra(adj_mat, n_nodes);
    
    for(int i = 0; i < n_nodes; i++){
        free(adj_mat[i]);
    }
    free(adj_mat);
 
    return 0;
}
