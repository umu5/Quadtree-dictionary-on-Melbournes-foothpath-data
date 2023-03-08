/*-------------------------------------------------------------- 
..Project: Assignment 2
  main.c :  
          = the implementation of module main of the project

To run the main program used  the command ./dicttype stage datafile outputfile bottomX bottomY topX topY
----------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "data.h"
#include "dict3.h"
#include "dict4.h"

#define STAGE3 3
#define STAGE4 4
// number of total attrubutes used in given data set
#define TOT_FIELDS 19


int main(int argc, char *argv[]) {
    char *ptr;
    list_t *list;
    // pointer to PR_Quadtree
    node_t *dict;
    rectangle_t root_bounds;
    // storing the bottom-left and top-right corners for the root node
    root_bounds.bottom_x = strtold(argv[4], &ptr); 
    root_bounds.bottom_y = strtold(argv[5], &ptr);
    root_bounds.top_x = strtold(argv[6], &ptr); 
    root_bounds.top_y = strtold(argv[7], &ptr);

    // create the linked list to store footpath data
    list = create_list();
    // 2D array to store attribute names from foothpath data
    char header[TOT_FIELDS][ATTRIBUTE_LEN];
    // creates the empty root node for quad tree
    dict = create_node(root_bounds);

    FILE *fp = fopen(argv[2], "r");
    FILE *f_out = fopen(argv[3], "w");
    // build a linked list by reading `fp` and stores each record in a particular list node
    list = build_dict(fp, header, list);
    // traverses the linked list and store pointers to footpath data in Quadtree
    traverse_lst(dict, list, header, f_out);

    // Bases on the stage it runs some particular functions associated to that stage
    if (atoi(argv[1]) == STAGE3) {
        read_input(dict, fp, header, f_out);
    }
    else if (atoi(argv[1]) == STAGE4) {
        read_input_2(dict, fp, header, f_out);
    }
    // all done close the output file and free the allocated memory
    fclose(f_out);
    list_free(list);
    tree_free(dict);

    return 0;
}
