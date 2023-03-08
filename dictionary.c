/*-------------------------------------------------------------- 
..Project: Assignment 2
  dictionary.c :  
          = the implementation of module dictionary of the project
----------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "dictionary.h"


/* function used from assignment 1 written by Muhammad Usman (1229086) */
// `list_append` takes the pointer to a list_t and a pointer to data_t `data`.
// It appends the list by adding a new node at the foot of the list. It stores
// `data` in that appending node. Returns pointer to that `list`
list_t *list_append(list_t *list, data_t *data) {

    // new node that will be used to append the list
    l_node_t *node = (l_node_t*)malloc(sizeof(*node));
    assert(list != NULL && node != NULL);

    l_node_t *old_tail;
    node->data = data;
    node->next = NULL;
    // This is the first ever insertion to `list`
    if (list->tail == NULL) {
        list->tail = list->head = node;
    }
    // appending to the list with new node
    else {
        old_tail = list->tail;
        old_tail->next = node;
        list->tail = node;
    }
    // list->size keeps track of the size of the list
    list->size += 1;
    return list;
}


/* function used from assignment 1 written by Muhammad Usman (1229086) */
// function creates an empty list and returns the pointer to that list
list_t *create_list(){

    list_t *empty_list = (list_t*)malloc(sizeof(*empty_list));
    assert(empty_list);
    empty_list->head = empty_list->tail = NULL;
    empty_list->size = 0;
    return empty_list;

}


/* function used from assignment 1 written by Muhammad Usman (1229086) */
// `list_free` takes the pointer to list_t `list`. It traverses the whole list. In this 
// process it frees the memory used to store string data types in each node. Then frees the
// memory that was allocated for data_t and then frees that particular node. In the end it 
// frees the space that was allocated to store list_t.
void list_free(list_t *list) {
    assert(list);

    if (list->size > 0) {
        l_node_t *curr;
        curr = list->head;
        l_node_t *old_head;
        // traverses the whole list to free malloced memory
        // also frees the memory used to store string type attribute
        while(curr) {
            if (curr->data->address != NULL){
                free(curr->data->address);
            }
            if (curr->data->clue_sa != NULL) {
                free(curr->data->clue_sa);
            }   
            if (curr->data->asset_type != NULL) {
                free(curr->data->asset_type);
            }
            if (curr->data->segside != NULL) {
                free(curr->data->segside);
            }
            // free the memory allocated for data_t
            free(curr->data);
            old_head = curr;
            curr = list->head = curr->next;
            // free the memory that was allocated for node_t
            free(old_head);
            old_head = NULL;
        }
    }
    free(list);
    list = NULL;
}


// takes the argument `bounds` of type rectange_t and creates a node for Quadtree.
// it stores the bounds in the node
node_t *create_node(rectangle_t bounds) {
    node_t *temp;
    temp = (node_t*)malloc(sizeof(*temp));
    assert(temp);
    temp->NE = temp->NW = temp->SE = temp->SW = NULL;
    temp->bounds = bounds;
    temp->data = NULL;
    temp->leaf = 1;
    temp->size = 0;
    temp->arr_size = 0;

    return temp;
}


// function takes the parent node `dict` and `bounds`. it creates the childs for 
// parent node that is SW, NW, NE, SE child nodes. it partitions the given `bounds` i.e rectangle
// to store calculated bounds into child nodes.
void create_childs(node_t *dict, rectangle_t bounds) {
    assert(dict);
    rectangle_t temp;

    // bounds calculation for North West quadrant
    temp.bottom_x = bounds.bottom_x; temp.bottom_y = (bounds.bottom_y + bounds.top_y)/2;
    temp.top_x = (bounds.bottom_x + bounds.top_x)/2; temp.top_y = bounds.top_y;
    // create the node and store the calculated bounds
    dict->NW = create_node(temp);

    // bounds calculation for North East quadrant
    temp.bottom_x = (bounds.bottom_x + bounds.top_x)/2; temp.bottom_y = (bounds.bottom_y + bounds.top_y)/2;
    temp.top_x = bounds.top_x; temp.top_y = bounds.top_y;
    // create the node and store the calculated bounds
    dict->NE = create_node(temp);

    // bounds calculation for South West quadrant
    temp.bottom_x = bounds.bottom_x; temp.bottom_y = bounds.bottom_y;
    temp.top_x = (bounds.bottom_x + bounds.top_x)/2; temp.top_y = (bounds.bottom_y + bounds.top_y)/2;
    // create the node and store the calculated bounds
    dict->SW = create_node(temp);

    // bounds calculation for South East quadrant
    temp.bottom_x = (bounds.bottom_x + bounds.top_x)/2; temp.bottom_y = bounds.bottom_y;
    temp.top_x = bounds.top_x; temp.top_y = (bounds.bottom_y + bounds.top_y)/2;
    // create the node and store the calculated bounds
    dict->SE = create_node(temp);
}


// functions takes a root node for Quadtree `tree`, a pointer to linked-list that contains
// the footpath data `list`, a header_lst that contains footpaths attributes names, and a 
// pointer to output file. It traverses the linked-list and for each data creates an array 
// of pointer to footpath data. Then it calls tree_insert function to store that array.
void traverse_lst(node_t *tree, list_t *list, char header_lst[][ATTRIBUTE_LEN], FILE *f_out) {
    l_node_t *curr;
    assert(list && tree);

    curr = list->head;
    while(curr) {
        // array created to store pointer to footpath data
        data_t **arr = (data_t**)malloc(INITIAL*sizeof(*arr));
        assert(arr);
        arr[0] = curr->data;
        // calls tree_insert to store array at a particular location specified by
        // footpath's start longitude and start latitude
        tree_insert(tree, arr, curr->data->start_lon, curr->data->start_lat, 1);

        // second array created to store footpaths data this time at a location
        // specified by its end longitude and end latitude
        data_t **arr2 = (data_t**)malloc(INITIAL*sizeof(*arr2));
        assert(arr2);
        arr2[0] = curr->data;
        tree_insert(tree, arr2, curr->data->end_lon, curr->data->end_lat, 1);

        // done with this footpath move to next footpath
        curr = curr->next;
    }
}


// function takes root node of Quad tree `dict` and an array `arr` of pointer
// to a footpath data that needs to be inserted into the quadtree. Arguments
// `x` is a longitude and `y` is latitude for that footpath. It can either be
// start or end pair depends on the calling function. It helps specifying the 
// position to store the footpath data at a particular location. `size` is the 
// number of foothpath pointers in `arr`. 
// functions adopts a recursive approach to find a node suitable to store pointer to
// footpath data
int tree_insert(node_t *dict, data_t **arr, double x, double y, int size) {
    assert(dict && arr);

    // This is a leaf node can call function `in_rectangle` to check whther the 
    // longitude and latitude falls in the bounds of the node.
    if (dict->leaf == 1 && in_rectangle(dict->bounds, x, y)) {
        // there isn't any data array store in the leaf node so we can store `arr` here
        if (dict->data == NULL) {
            dict->data = arr;
            // This is the first time ever we are storing `arr` so initialise arr_size of this
            // node to INITIAL because `arr` has a maximum capacity to store INITIAL data pointer at start
            dict->arr_size = INITIAL;
            dict->footpath_locn.longitude = x;
            dict->footpath_locn.latitude = y;
            // This keeps record of the current footpaths pointer in that array stored which is stored here
            dict->size = size;
            return 1;
        } 
        // If footpath data that needs to be stored has same longitude and latitude to currently
        // stored data in node just append dict->data array with this footpath pointer
        else if (dict->footpath_locn.longitude == x && dict->footpath_locn.latitude == y) {
            // check if dict->arr_size has reaced its limit so realloc to create more memory for dict->data array
            if (dict->size >= dict->arr_size) {
                int new_size = dict->arr_size*2;
                dict->data = (data_t**)realloc(dict->data, new_size*sizeof(*(dict->data)));
                dict->arr_size = new_size;
            }
            // now append dict->data array with foothpath pointer
            dict->data[dict->size] = arr[0];
            dict->size++;
            free(arr);
            return 1;
        } 
        // if there is already stored array of pointers and the longitude and latitude of the data pointer
        // that needs to be stored does not match with stored data pointers further divide this node to create
        // childs
        else {
            create_childs(dict, dict->bounds);
            // now this node is no longer a leaf node it is an internal node 
            dict->leaf = 0;
            // now insert again the already inserted array of foothpath pointers from internal to child
            tree_insert(dict, dict->data, dict->footpath_locn.longitude, dict->footpath_locn.latitude, dict->size);
            // also we have shifted array to its child so assign value of NULL to dict->data
            dict->data = NULL;
            // now insert the new data array
            tree_insert(dict, arr, x, y, size);
            return 1;
        } 
    }
    // search through the NW node if foothpath is within NW nodes bound
    else if (dict->NW != NULL && in_rectangle(dict->NW->bounds, x, y)) {
        tree_insert(dict->NW, arr, x, y, size);
        return 1;
    }
    // search through the NE node if foothpath is within NE nodes bound
    else if (dict->NE != NULL && in_rectangle(dict->NE->bounds, x, y)) {
        tree_insert(dict->NE, arr, x, y, size);
        return 1;
    }
    // search through the SW node if foothpath is within SW nodes bound
    else if (dict->SW != NULL && in_rectangle(dict->SW->bounds, x, y)) {
        tree_insert(dict->SW, arr, x, y, size);
        return 1;
    }
    // search through the SE node if foothpath is within SE nodes bound
    else if (dict->SE != NULL && in_rectangle(dict->SE->bounds, x, y)) {
        tree_insert(dict->SE, arr, x, y, size);
        return 1;
    }
    // failed to store that array of footpath pointer in a node hence return 0
    return 0;  
}


// function takes the `bounds` of type rectangle_t and x,y coordinate pairs.
// It checks whether the coordinates are in the rectangle defined by this bound
int in_rectangle(rectangle_t bounds, double x, double y) {
    if (x >= bounds.bottom_x && y <= bounds.top_y) {
        if (x <= bounds.top_x && y >= bounds.bottom_y) {
            return 1;
        }
    }
    return 0;
}


// uses recursive approach to free the Quadtree it takes the root node to quadtree
void tree_free(node_t *dict) {
    // this is leaf node so just free this memory allocated to this node
    if (dict->leaf == 1) {
        // check if leaf node containes an array of pointers to footpath so just free that array
        if (dict->data != NULL) {
            free(dict->data);
            dict->data = NULL;
        }
        free(dict);
        dict = NULL;
        return;
    }
    // Recursively free each quadrant nodes 
    tree_free(dict->SW);
    tree_free(dict->NW);
    tree_free(dict->NE);
    tree_free(dict->SE);
    // now free the root node
    free(dict);
}
