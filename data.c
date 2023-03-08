/*-------------------------------------------------------------- 
..Project: Assignment 2
  data.c :  
          = the implementation of module data of the project
----------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include "data.h"
#include "dictionary.h"

#define NOTDOUBLE (-1)
#define MAXPRECISION (-2)

/* This static arary copied from assignment 1 solutions provided by COMP20003 team */
// used to achieve precision for the printed footpath data
static int fieldPrecision[] = {NOTDOUBLE, NOTDOUBLE, NOTDOUBLE, 
    NOTDOUBLE, 2, 2, 1, NOTDOUBLE, NOTDOUBLE, 
    2, 2, NOTDOUBLE, NOTDOUBLE, NOTDOUBLE, NOTDOUBLE, 
    MAXPRECISION, MAXPRECISION, MAXPRECISION, MAXPRECISION};


/* function used from assignment 1 written by Muhammad Usman (1229086) */
// `function` takes a pointer to a file `fp`, a 2D array `header`, and a 
// a pointer to a list_t `list`. functions operates on `fp` using fgets()
// in a while loop. Within each iteration it manually iteartes on a line 
// charachter by charachter to store attributes in the nodes of a linked-list.
// It returns a pointer to a list_t
list_t *build_dict(FILE *fp, char header[][ATTRIBUTE_LEN], list_t *list) {
    char line[MAX_LINE + 2];
    int j=0;

    // gets the header line and store the attributes name in 2D array header
    fgets(line, MAX_LINE, fp);
    header_lst(line, header);

    while (fgets(line, MAX_LINE, fp) != NULL) {
        // helper function `create_data` mallocs the space to store data_t
        data_t *data = create_data();
        int i =0;
        // count keep tracks of the index of the attribute.
        int count = 0;

        // manually iterating through each line
        while (i <= (strlen(line) - 2)) {
            // it handles the special cases where attrubute values starts
            // with quotaion mark.
            if (line[i+1] == '\"' && line[i] == ',') {
                j = 0;
                // `field` used to store the attrubute value
                char field[MAX_FIELD + 1];
                // to skip pass the comma and the quotaion mark
                i+=2;
                // iterates until find the ending quoation mark of an attribute
                // value. during iteration it stores charachters in `field`.
                while (line[i] != '\"') {
                    field[j] = line[i];
                    j++;
                    i++;
                }
                field[j] = '\0';
                // Now insert that `field` into its respective attribute name
                // in struct data_t
                insert_data(data, field, count);
                count += 1;
                // Now move to the next preceding comma before the next attribute
                i++;
            }
            // deals with usual cases where attribute values does not starts with 
            // quoation marks.
            else if (line[i] == ',' || i == 0) {
                // if i != 0 means this is not first attribute so skip past one value to
                // avoid storing comma
                if (i != 0) {
                    i++;
                }
                j = 0;
                char field[MAX_FIELD + 1];
                // iterate until find the preceding comma of next attribute vale
                while (line[i] != ',') {
                    // storing the charachters in `field` from `line`
                    field[j] = line[i];
                    j++;
                    i++;
                    // special case need to break the while loop if reached the end of the line
                    // while dealing with last attribute value
                    if (line[i] == '\0' || line[i] == '\n') {
                        break;
                    }
                }
                field[j] = '\0';
                // Now insert that `field` into the its respective attribute name
                // in struct data_t
                insert_data(data, field, count);
                count += 1;
            }
        }
        // done dealing with single line now time to store data into list 
        list_append(list, data);

    }
    fclose(fp);
    return list;
}


/* function used from assignment 1 written by Muhammad Usman (1229086) */
// function creates the space to store data_t and returns pointer to it.
data_t *create_data() {
    data_t *data = (data_t*)malloc(sizeof(*data));
    assert(data);
    return data;
}


/* function used from assignment 1 written by Muhammad Usman (1229086) */
// function takes the string line and store the attributes name in 2D array
// header
void header_lst(char *line, char header[][ATTRIBUTE_LEN]) {
    int i=0;

    char *attributes = strtok(line, ",");
    // iterate and store each attribute name in header splitting line 
    // on comma delimiter
    while (attributes != NULL) {
        strcpy(header[i], attributes);
        attributes = strtok(NULL, ",");
        i++;
    }
    // manually store the last attribute to avoid storing comma
    strcpy(header[18], LAST_ATTRIBUTE);
}


/* function used from assignment 1 written by Muhammad Usman (1229086) */
// function takes pointer to data_t, a string `field`, and a `count` of type int
// Comparing the count with indexes it helps to store the value `field` into 
// its respective attrubute in struct `data`.
void insert_data(data_t *data, char *field, int count) {

    // field needs to be stored in int type attribute
    if (count == 0 || count == 7 || count == 8 || count == 12 || count == 13 || count == 14) {
        if (count == 0) {data->footpath_id = atoi(field);}
        else if (count == 7) {data->mcc_id = atoi(field);}
        else if (count == 8) {data->mccid_int = atoi(field);}
        else if (count == 12) {data->statusid = atoi(field);}
        else if (count == 13) {data->streetid = atoi(field);}
        else {data->street_group = atoi(field);}
    }
    // field need to be stored in string type attirbute. So, in this 
    // it does mallocs to create the space to store field.
    else if (count == 1 || count == 2 || count == 3|| count == 11) {
        int size = strlen(field) + 1;

        if (count == 1) {
            data->address = (char*)malloc(size*sizeof(char));
            assert(data->address);
            strcpy(data->address, field);
        }
        else if (count == 2) {
            data->clue_sa = (char*)malloc(size*sizeof(char));
            assert(data->clue_sa);
            strcpy(data->clue_sa, field);
        }
        else if (count == 3) {
            data->asset_type = (char*)malloc(size*sizeof(char));
            assert(data->asset_type);
            strcpy(data->asset_type, field);
        }
        else {
            data->segside = (char*)malloc(size*sizeof(char));
            assert(data->segside);
            strcpy(data->segside, field);
        }
    }
    // field need to be stored in double data type attribute
    else {
        double var = atof(field);
        if (count == 4) {data->deltaz = var;}
        else if (count == 5) {data->distance = var;}
        else if (count == 6) {data->grade1in = var;}
        else if (count == 9) {data->rlmax = var;}
        else if (count == 10) {data->rlmin = var;}
        else if (count == 15) {data->start_lat = var;}
        else if (count == 16) {data->start_lon = var;}
        else if (count == 17) {data->end_lat = var;}
        else {data->end_lon = var;}
    }
}


/* function used from assignment 1 written by Muhammad Usman (1229086) */
// function takes pointer to data_t `data`, a 2D array `header`(that contains the 
// attributes name from given dataset), and pointer to an output file `f_out`.
// It prints the output to f_out in required manner. It manually prints each attribute
// value
void print_data(data_t *data, char header_lst[][ATTRIBUTE_LEN], FILE *f_out) {
    int i=0;
    fprintf(f_out, "--> ");
    fprintf(f_out, "%s: ", header_lst[i]); fprintf(f_out, "%d || ", data->footpath_id); i++;
    fprintf(f_out, "%s: ", header_lst[i]); fprintf(f_out, "%s || ", data->address); i++;
    fprintf(f_out, "%s: ", header_lst[i]); fprintf(f_out, "%s || ", data->clue_sa); i++;
    fprintf(f_out, "%s: ", header_lst[i]); fprintf(f_out, "%s || ", data->asset_type); i++;
    fprintf(f_out, "%s: ", header_lst[i]); fprintf(f_out, "%.*f || ", fieldPrecision[i], data->deltaz); i++;
    fprintf(f_out, "%s: ", header_lst[i]); fprintf(f_out, "%.*f || ", fieldPrecision[i], data->distance); i++;
    fprintf(f_out, "%s: ", header_lst[i]); fprintf(f_out, "%.*f || ", fieldPrecision[i], data->grade1in); i++;
    fprintf(f_out, "%s: ", header_lst[i]); fprintf(f_out, "%d || ", data->mcc_id); i++;
    fprintf(f_out, "%s: ", header_lst[i]); fprintf(f_out, "%d || ", data->mccid_int); i++;
    fprintf(f_out, "%s: ", header_lst[i]); fprintf(f_out, "%.*f || ", fieldPrecision[i], data->rlmax); i++;
    fprintf(f_out, "%s: ", header_lst[i]); fprintf(f_out, "%.*f || ", fieldPrecision[i], data->rlmin); i++;
    fprintf(f_out, "%s: ", header_lst[i]); fprintf(f_out, "%s || ", data->segside); i++;
    fprintf(f_out, "%s: ", header_lst[i]); fprintf(f_out, "%d || ", data->statusid); i++;
    fprintf(f_out, "%s: ", header_lst[i]); fprintf(f_out, "%d || ", data->streetid); i++;
    fprintf(f_out, "%s: ", header_lst[i]); fprintf(f_out, "%d || ", data->street_group); i++;
    fprintf(f_out, "%s: ", header_lst[i]); fprintf(f_out, "%.*f || ", fieldPrecision[i], data->start_lat); i++;
    fprintf(f_out, "%s: ", header_lst[i]); fprintf(f_out, "%.*f || ", fieldPrecision[i], data->start_lon); i++;
    fprintf(f_out, "%s: ", header_lst[i]); fprintf(f_out, "%.*f || ", fieldPrecision[i], data->end_lat); i++;
    fprintf(f_out, "%s: ", header_lst[i]); fprintf(f_out, "%.*f || ", fieldPrecision[i], data->end_lon); i++;
    fprintf(f_out, "\n");
}


// Sort the given array on the basis of footpath id using insertion sort algorithm
void sort_arr(data_t **arr, int size) {
    data_t *temp;
    int i, j;
    for (i = 0; i < size; i++) {
        for (j = i-1; j >= 0; j--) {
            // if precending element is greater in  magnitude so swap it 
            // with next element.
            if (arr[j+1]->footpath_id < arr[j]->footpath_id) {
                temp = arr[j+1];
                arr[j+1] = arr[j];
                arr[j] = temp;
            }
        }
    }

}


// function takes an array of pointers to data_t `base_arr` and a quadtree node
// also it takes base_i that is the size of `base_arr`. It itterated through the
// array of pointers to data_t stored in `node` and add these pointers to `base_arr`
void merge_arrays(data_t** base_arr, node_t *node, int base_i) {
    int i, index;
    index = base_i;
    data_t **temp_arr = node->data;

    for (i=0; i < node->size; i++) {
        base_arr[index] = temp_arr[i];
        index++;
    }
}
