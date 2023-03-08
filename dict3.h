/*-------------------------------------------------------------- 
..Project: Assignment 2
  dict3.h :  
         = the interface of module dict3 of the project
----------------------------------------------------------------*/

#ifndef _DICT3_H_
#define _DICT3_H_
#include "data.h"

// functions definitions >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

// reads the input from `f_in` and inputs each read line to  helper function `tree_search`
void read_input(node_t *dict, FILE *f_in, char header[][ATTRIBUTE_LEN], FILE *f_out);

// allows the lookup of data by longitude (x) and latitude (y) pairs.
int tree_search(node_t *dict, double x, double y, char header[][ATTRIBUTE_LEN], FILE *f_out);

// print the footpath(s) data stored in the provide `node`
void print_dict3(node_t* node, char header_lst[][ATTRIBUTE_LEN], FILE *f_out);

#endif
