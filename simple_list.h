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
	struct list_head my_list;

}numl;

typedef struct multi_lists{
	numl *head;
}multil;

int add_section(multil **mlr_ptr, int start, int stop, int *len_of_mlr);
int input_handler(char input[10],int *op);
int show_all_lists(multil* mlr, int len_of_mlr);
