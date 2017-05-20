/*
 *  This program will download an undirected graph from a text file
 *  which we refer a the data file.  The graph is assumed to have
 *  node values 0, 1, 2, ... N-1, where N is the number of nodes.
 *  The program will output a spanning tree of the graph
 *
 *  The following is an example of the contents of a data file 
 *  with N=6 nodes and E=6 undirected edges, where the edges
 *  are (0,3), (1,2), (4,2), (5,3), (0,4), (2,5)
 *  
 *  6
 *  7
 *  0 3
 *  1 2
 *  4 2
 *  5 3
 *  0 4
 *  2 5
 *
 *  The first line is the number of nodes, the second line is
 *  the number of edges, and the subsequent lines are the edges.
 *
 *  To run the program type
 *
 *  ./a.out <data file>
 *
 *  The output will be 
 *
 *  (0,4)
 *  (2,4) 
 *  (1,2)
 *  (0,3)
 *  (2,5)
 *
 *  which are the edges of the spanning tree
 *
 *  Currently the program will input the graph from the data file
 *  and then store the topology in an adjacency list.  
 *
 *  You must complete the program by having it compute a spanning
 *  tree and then displaying on the console.  Basically,
 *  you must implement 'spanning_tree'
 */

#include <stdlib.h>
#include <stdio.h>

#define MAX_NODES 1000
struct ListNode{
   int node;
   struct ListNode * next;
};

void list_insert_node(struct ListNode **list, int node);
void list_display_all(struct ListNode **adj_list, int num_nodes);
void list_delete_all(struct ListNode ** adj_list, int num_nodes);
void spanning_tree(struct ListNode **adj_list, int num_nodes);

int main(int argc, char *argv[])
{

   FILE * fp;   
   int i;

   if (argc != 2) {
      printf("Usage:  a.out <file name>\n");
      return 0;
   }
   fp = fopen(argv[1],"r");

   int num_nodes;
   int num_edges;

   fscanf(fp,"%d", &num_nodes);  
   fscanf(fp,"%d", &num_edges);

   /* Create adjacency list */
   struct ListNode ** adj_list;

   adj_list = (struct ListNode **) malloc(num_nodes*sizeof(struct ListNode *));
   for (i=0; i<num_nodes; i++) {
      adj_list[i] = NULL;
   }

   /* Load the adjacency list with the graph */
   int node1, node2;

   printf("Edges:\n");
   for (i=0; i<num_edges; i++) {
      fscanf(fp,"%d %d", &node1, &node2); 
      printf("(%d %d)\n",node1,node2);
      list_insert_node(&adj_list[node1],node2);
      list_insert_node(&adj_list[node2],node1);
   }

   /* Display the adjacency list on the console */
   list_display_all(adj_list, num_nodes);

   spanning_tree(adj_list, num_nodes);

   list_delete_all(adj_list, num_nodes);

}

/* 
 * Implement this function that computes a spanning tree
 * and displays its edges
 */
void spanning_tree(struct ListNode **adj_list, int num_nodes)
{
   //printf("\nImplement this function to compute a spanning tree\n");
   //printf("and display its edges\n");
   printf("\nSpanning tree edges:\n");
   int i=0;
   int set[MAX_NODES];
   for (i=0; i<MAX_NODES; i++) {
      set[i] = 0;
   }
   set[0] = 1;
   int count = 1;
   int j = 0;
   while(count < num_nodes) {
      struct ListNode *p;
      for (j = 0; j<num_nodes; j++) {
         if (count == num_nodes) break;
         if (set[j] == 1) {
            for(p=adj_list[j]; p!=NULL; p=p->next) {
               if(set[p->node] == 0) {
                  set[p->node] = 1;
                  count ++;
                  printf("(%d %d)\n", j, p->node);
               }
            }
         }
      }
   }

}

void list_display_all(struct ListNode **adj_list, int num_nodes)
{
   struct ListNode *p;
   int i;

   printf("\nAdjacency List:\n");
   for (i=0; i<num_nodes; i++) {
      printf("Node %d:",i);
      for (p=adj_list[i]; p!=NULL; p=p->next) {
         printf("-> %d", p->node);
      }
      printf("\n");
   }

}


/*
 * Inserts 'node' in a linked list.
 */
void list_insert_node(struct ListNode **list, int node)
{

   struct ListNode ** p;

   for (p=list; *p!=NULL; p=&((*p)->next)); 
   *p = (struct ListNode *) malloc(sizeof(struct ListNode));
   (*p)->node = node;
   (*p)->next = NULL;
}


/*
 * Delete the adjacency list
 */
void list_delete_all(struct ListNode ** adj_list, int num_nodes)
{
   int i;
   struct ListNode * p;
   struct ListNode * temp;

   /* Delete all the nodes */
   for (i=0; i<num_nodes; i++) {
      p = adj_list[i];
      while (p != NULL) {
         temp = p;
         p = p->next;
         free(temp);
      }
   }

   /* Delete the adjacency list array */
   free(adj_list);

   printf("\nDeleted the adjacency list\n");
}

