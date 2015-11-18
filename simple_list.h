#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"


enum{
	Input,
	Delete,
	List,
	Quit
};

enum{
	TAIL,
	HEAD
};

typedef struct num_list{
	int num;
	int index;
	struct list_head my_list;

}numl;

/*item could be head or member*/
typedef struct multi_lists{
	numl *item;

}multil;

int add_section(multil **mlr_ptr, multil **all, int start, int stop, int *len_of_mlr, int *all_len, int *s_mlr, int*s_all);
int input_handler(char input[10],int *op);
void show_all_lists(multil* mlr, int len_of_mlr);
int init(multil **mlr_ptr,multil **all, int *all_len,int *s_all);
