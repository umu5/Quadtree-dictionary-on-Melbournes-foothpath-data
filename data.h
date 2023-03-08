/*-------------------------------------------------------------- 
..Project: Assignment 2
  data.h :
         = the interface of module data of the project
----------------------------------------------------------------*/

#ifndef _DATA_H_
#define _DATA_H_
#include "dictionary.h"

// constants definitions >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define TOT_FIELDS 19
#define MAX_LINE 512
#define MAX_FIELD 128
#define ATTRIBUTE_LEN 20
#define LAST_ATTRIBUTE "end_lon"
#define INITIAL 5
#define NOTDOUBLE (-1)
#define MAXPRECISION (-2)

// functions definitions >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

// reading from the `fp` line by line it stores each record in a prticular list node
list_t *build_dict(FILE *fp, char header[][ATTRIBUTE_LEN], list_t *list);

// creates the space to store struct data
data_t *create_data();

// creates a 2d array header thats stores the attrubutes name used in dataset
void header_lst(char *line, char header[][ATTRIBUTE_LEN]);

// stores the attrubute value in its respective attrubute field in struct data
void insert_data(data_t *data, char *field, int count);

// prints the data  in a required manner
void print_data(data_t *data, char header_lst[][ATTRIBUTE_LEN], FILE *f_out);

// sort the array by foothpath id 
void sort_arr(data_t **arr, int size);

// adds the array of pointers to data_t from `node` into `base_arr`
void merge_arrays(data_t** base_arr, node_t *node, int base_i);

#endif
