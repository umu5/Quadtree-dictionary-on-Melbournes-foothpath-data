/*-------------------------------------------------------------- 
..Project: Assignment 2
  dict4.c :  
          = the implementation of module dict4 of the project
----------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include "dict4.h"
#include "data.h"


// function `read_input_2` takes pointer to a root node of quadtree `node`,
// pointer to a file `f_in`, a 2D array `header`(that contains the attributes
// name from given dataset), and a pointer to an output file `f_out`.
// It reads the input from `f_in` line by line and inputs that query into 
// function `dict4_search`. It then prints the footpaths within the range query
void read_input_2(node_t *dict, FILE *f_in, char header[][ATTRIBUTE_LEN], FILE *f_out) {
    char *query;
    char *ptr;
    assert(dict);

    query = (char*)malloc((MAX_FIELD + 1)*sizeof(char));
    assert(query);
    // reads each line from the input file
    while(scanf(" %[^\n]", query) == 1) {
        fprintf(f_out, "%s\n", query);
        printf("%s -->", query);
        // creates read_bounds and using strtok function stores the bottom and
        // and top coordinate pairs of range query into read_bounds 
        rectangle_t *read_bounds = (rectangle_t*)malloc(sizeof(*read_bounds));

        char *attributes = strtok(query, " ");
        read_bounds->bottom_x = strtold(attributes, &ptr);

        attributes = strtok(NULL, " ");
        read_bounds->bottom_y = strtold(attributes, &ptr);

        attributes = strtok(NULL, " ");
        read_bounds->top_x = strtold(attributes, &ptr);

        attributes = strtok(NULL, " ");
        read_bounds->top_y = strtold(attributes, &ptr);

        // creates an array of nodes_arr_t type to store the matched footpaths pointers
        // within the range query
        nodes_arr_t *arr = (nodes_arr_t*)malloc(sizeof(*arr));
        arr->data_arr = (data_t**)malloc(INITIAL*(sizeof(*(arr->data_arr))));
        arr->arr_size = INITIAL;
        arr->size = 0;

        // search the foothpaths within the range query and store there pointers
        // into `arr`
        dict4_search(dict, read_bounds, arr);
        // now print the footpaths stored in `arr->data_arr`
        print_dict4(arr, header, f_out);
        // all done for this query now free the space used by `arr` and `read_bounds`
        free(arr->data_arr);
        free(arr);
        free(read_bounds);
        printf("\n");
    }
    free(query);
    query = NULL;
}


// function takes the pointer to root node of quad tree `dict`, a pointer to 
// range query `query, and a pointer to nodes_arr_t `arr`. It recursively search
// through the quadtree and stores the footpaths that falls within the range query
// into the `arr`
int dict4_search(node_t *dict, rectangle_t* query, nodes_arr_t *arr) {

    // This is the leaf node and it has some stored data that falls within the range query
    // so store those foothpath pointer into `arr`
    if (dict->leaf == 1 && dict->data != NULL 
        && in_rectangle(*query, dict->footpath_locn.longitude, dict->footpath_locn.latitude)) {
        // if `arr->data_arr` size is not enough to store the array of foothpath pointer stored 
        // in this node than create more space for `arr->data_arr` using realloc
        if ((arr->size + dict->size) >= arr->arr_size) {
            int new_size;
            if (dict->size != 0) {
                new_size = 2*((dict->size)+(arr->arr_size));
            } else {
                new_size = arr->arr_size*2;
            }
            arr->data_arr = (data_t**)realloc(arr->data_arr, new_size*sizeof(*(arr->data_arr)));
            assert(arr->data_arr);
            arr->arr_size = new_size;     
        }
        // calls the function merge_arrays to append `arr->data_arr` with the stored foothpath
        // pointers in this node
        merge_arrays(arr->data_arr, dict, arr->size);
        arr->size += dict->size; 
        return 1;
    }
    // also perform the recursive search through the SW node of root node if falls 
    // within the range query
    if (dict->SW != NULL && rect_overlap(query, &(dict->SW->bounds))) {
        // if the this node is a leaf node and and contains some footpaths 
        // data then print this quadrant to stdout
        if (dict->SW->leaf == 1 && dict->SW->data==NULL);
        else {
            printf(" SW");}
        dict4_search(dict->SW, query, arr);

    }
    // also perform the recursive search through the NW node of root node if falls 
    // within the range query
    if (dict->NW != NULL && rect_overlap(query, &(dict->NW->bounds))) {
        // if the this node is a leaf node and and contains some footpaths 
        // data then print this quadrant to stdout
        if (dict->NW->leaf == 1 && dict->NW->data==NULL);
        else {
            printf(" NW");}
        dict4_search(dict->NW, query, arr);

    }
    // also perform the recursive search through the NE node of root node if falls 
    // within the range query
    if (dict->NE != NULL && rect_overlap(query, &(dict->NE->bounds))) {
        // if the this node is a leaf node and and contains some footpaths 
        // data then print this quadrant to stdout
        if (dict->NE->leaf == 1 && dict->NE->data==NULL);
        else {
            printf(" NE");}
         dict4_search(dict->NE, query, arr);

    }
    // also perform the recursive search through the SE node of root node if falls 
    // within the range query
    if (dict->SE != NULL && rect_overlap(query, &(dict->SE->bounds))) {
        // if the this node is a leaf node and and contains some footpaths 
        // data then print this quadrant to stdout
        if (dict->SE->leaf == 1 && dict->SE->data==NULL);
        else {
            printf(" SE");}
        dict4_search(dict->SE, query, arr);

    }
    return 0;
}


// function takes the pointer to `arr` of type  nodes_arr_t, a 2D array `header`
// (that contains the attributes name from given dataset), pointer to output file
// `f_out`. It prints the unique footpaths stored in `arr->data_arr` to `f_out`
void print_dict4(nodes_arr_t* arr, char header[][ATTRIBUTE_LEN], FILE *f_out) {
    int i;
    // sort the arr->data_arr by footpath Id
    sort_arr(arr->data_arr, arr->size);
    // itterate through the arr->data_arr and prints the footpath to f_out if 
    // that particular footpath isn't printed yet
    for (i=0; i < arr->size; i++) {
        if (is_unique(arr->data_arr, arr->data_arr[i], i, arr->size-1)) {
            print_data(arr->data_arr[i], header, f_out);
        }
    }
}


// function takes the sorted array `arr` that containes pointer to foothpaths. Array 
// argument `arr` sorted by foothpath id. it returns 0 if a foothpath pointer 
// is same to preceding element in the array, otherwise 1
int is_unique(data_t **arr, data_t *data, int index, int last_index) {

    // data is unique because only one data point in the array
    if (last_index < 1) {
        return 1;
    }
    // now safe to move forward as array have more then one elements.
    // need to check if preceding data point is same
    if (index <= last_index && index > 0) {
        if (arr[index-1]->footpath_id == data->footpath_id) {
            return 0;
        }
    }
    // no preceding match found so data can be printed
    return 1;
}


// functions takes two argument of type rectange_t and returns 1 
// if both these recatngles overlap
int rect_overlap(rectangle_t *bounds1, rectangle_t *bounds2) {

    // if rectangle with bounds2 is on right and doesnot overlap
    if (bounds1->top_x < bounds2->bottom_x) {
        return 0;
    }
    // if rectangle with bounds2 is on left and doesnot overlap
    if (bounds1->bottom_x > bounds2->top_x) {
        return 0;
    }
    // if rectangle with bounds2 is on top and doesnot overlap
    if (bounds1->top_y < bounds2->bottom_y) {
        return 0;
    }
    // if rectangle with bounds2 is below and doesnot overlap
    if (bounds1->bottom_y > bounds2->top_y) {
        return 0;
    }
    return 1;
}
