/*-------------------------------------------------------------- 
..Project: Assignment 2
  dict3.c :  
          = the implementation of module dict3 of the project
----------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include "dict3.h"
#include "data.h"


// function `read_input` takes pointer to root node of quadtree `dict`, pointer to a 
// file `f_in`,a 2D array `header`(that contains the attributes name from given dataset),
// and a pointer to an output file `f_out`. It reads the input from `f_in` line by line
// and inputs that query into function `tree_search`
void read_input(node_t *dict, FILE *f_in, char header[][ATTRIBUTE_LEN], FILE *f_out) {
    char *query;
    char *ptr;
    double x;
    double y;
    assert(dict);

    query = (char*)malloc((MAX_FIELD + 1)*sizeof(char));
    assert(query);

    // reads each line from the input file
    while(scanf(" %[^\n]", query) == 1) {
        fprintf(f_out, "%s\n", query);
        printf("%s -->", query);
        // uses function strtok to store the longitude and latitude into x and y 
        // respectively to search for foothpaths data from the quadtree using function
        // tree_search
        char *attributes = strtok(query, " ");
        x = strtod(attributes, &ptr);
        attributes = strtok(NULL, " ");
        y = strtod(attributes, &ptr);
        tree_search(dict, x, y,  header, f_out);
        printf("\n");
    }
    free(query);
    query = NULL;
}


// functions takes a pointer to root node of quadtree `dict` and a longitude (x) and 
// latitude (y) pairs. Also it takes a 2D array header, and a pointer to output file 
// `f_out`. It uses a recursive approcah to search the footpath data from the quadtree
int tree_search(node_t *dict, double x, double y, char header[][ATTRIBUTE_LEN], FILE *f_out) {

    // This is a leaf node check(base case) whether the given longitude and latitude are within
    // the rectangular bounds of leaf node
    if (dict->leaf == 1 && in_rectangle(dict->bounds, x, y)) {
        // print the footpath data stored in leaf node 
        if (dict->data != NULL) {
            print_dict3(dict, header, f_out);
            return 1;
        }
        return 0;
    }
    // Otherwise, move to NE quadrant and recursively call the tree_search function
    else if (dict->NE != NULL && in_rectangle(dict->NE->bounds, x, y)) {
        printf(" NE");
        tree_search(dict->NE, x, y, header, f_out);
        return 1;
    }
    // Move to NW quadrant and recursively call the tree_search function
    else if (dict->NW != NULL && in_rectangle(dict->NW->bounds, x, y)) {
        printf(" NW");
        tree_search(dict->NW, x, y, header, f_out);
        return 1;
    }
    // move to SW quadrant and recursively call the tree_search function
    else if (dict->SW != NULL && in_rectangle(dict->SW->bounds, x, y)) {
        printf(" SW");
        tree_search(dict->SW, x, y, header, f_out);
        return 1;
    }
    // move to SE quadrant and recursively call the tree_search function
    else if (dict->SE != NULL && in_rectangle(dict->SE->bounds, x, y)) {
        printf(" SE");
        tree_search(dict->SE, x, y, header, f_out);
        return 1;
    }
    return 0;
}


// function takes the leaf node of quadtree `node` and prints the footpaths data
// stored in that node
void print_dict3(node_t* node, char header_lst[][ATTRIBUTE_LEN], FILE *f_out) {
    int j;
    // if data array is has more than 1 foothpaths than sort the array by footpath id
    if (node->size > 1) {
        sort_arr(node->data, node->size);
    }
    // print each footpath data
    for (j = 0; j < node->size; j++) { 
        data_t *data = node->data[j];
        print_data(data, header_lst, f_out);
    }
}
