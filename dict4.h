/*-------------------------------------------------------------- 
..Project: Assignment 2
  dict4.h :  
         = the interface of module dict4 of the project
----------------------------------------------------------------*/

#ifndef _DICT4_H_
#define _DICT4_H_
#include "data.h"

// functions definitions >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

// reads the range query and inputs this into function dict4_search
void read_input_2(node_t *dict, FILE *f_in, char header[][ATTRIBUTE_LEN], FILE *f_out);

// using recursive approach finds the foothpaths that falls within range query
int dict4_search(node_t *dict, rectangle_t* query, nodes_arr_t *arr);

// printd the foothpaths that falls within range query
void print_dict4(nodes_arr_t* arr, char header[][ATTRIBUTE_LEN], FILE *f_out);

// returns 0 if foothpath is same to the preceding foothpath in the `arr`
int is_unique(data_t **arr, data_t *data, int index, int last_index);

// returns 1 if two rectangles overlap
int rect_overlap(rectangle_t *bounds1, rectangle_t *bounds2);

#endif


