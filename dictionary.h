/*-------------------------------------------------------------- 
..Project: Assignment 2
  dictionary.h :  
         = the interface of module dictionary of the project
----------------------------------------------------------------*/

#ifndef _DICTIONARY_H_
#define _DICTIONARY_H_

// constants definitions >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define ATTRIBUTE_LEN 20
#define INITIAL 5

// type definitions >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
typedef struct data data_t;
typedef struct list_node l_node_t;
typedef struct list list_t;
typedef struct rectangle rectangle_t;
typedef struct point2D point2D_t;
typedef struct node node_t;
typedef struct nodes_arr nodes_arr_t;

struct data {
    int footpath_id, mcc_id, mccid_int, statusid, streetid, street_group;
    double deltaz, distance, grade1in, rlmax, rlmin, start_lat, start_lon;
    double end_lat, end_lon;
    char *address, *clue_sa, *asset_type, *segside;
};

struct list_node {
    data_t *data;
    l_node_t *next;
};

struct list {
    l_node_t *head;
    l_node_t *tail;
    int size;
};

struct rectangle {
    long double bottom_x;
    long double bottom_y;
    long double top_x;
    long double top_y;
};

struct point2D {
    double latitude;
    double longitude;
};

// node for PR_Quadtree
struct node {
    rectangle_t bounds;
    node_t *NW;
    node_t *NE;
    node_t *SE;
    node_t *SW;
    point2D_t footpath_locn;
    data_t **data;
    int size;
    int arr_size;
    int leaf;
};

struct nodes_arr {
    data_t **data_arr;
    int size;
    int arr_size;
};

// functions definitions >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

// append the list to the foot of the list
list_t *list_append(list_t *list, data_t *data);

// creates the empty list and returns the pointer to that list
list_t *create_list();

// frees the memory that was alloacted to store the footpath data, nodes of list, and list_t
void list_free(list_t *list);

// creates the node for quadtree and store the given bounds in that node to specify nodes
// rectangular area
node_t *create_node(rectangle_t bounds);

// creates the 4 childs nodes by taking parent node
void create_childs(node_t *dict, rectangle_t bounds);

// inserts the array that contains the pointer to foothpath into quadtree
int tree_insert(node_t *dict, data_t **arr, double x, double y, int size);

// returns 1 if coordinate x & y are within rectangular bounds
int in_rectangle(rectangle_t bounds, double x, double y);

// frees the memory used by quad tree
void tree_free(node_t *dict);

// traverses the linked-list and store eeach footpath data pointer into quadtree
void traverse_lst(node_t *tree, list_t *list, char header_lst[][ATTRIBUTE_LEN], FILE *f_out);

#endif
