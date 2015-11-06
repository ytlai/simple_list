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

typedef struct num_list{
	int num;
	struct list_head my_list;

}numl;

int add_section(numl *mlr,start,stop,int *len_of_mlr){

	int i,j=0;
	numl *ql = calloc(len_of_mlr,sizeof(numl));
	numl *pos;

	/*Create new list first*/
	struct list_head mylist = INIT_LIST_HEAD(&mylist);
	numl new_nl={start,mylist};
	*len_of_mlr=*len_of_mlr+1;
	mlr = realloc(*len_of_mlr,sizeof(numl));
	(mlr+*len_of_mlr-1) = &new_nl;
	for(i=start+1;i++;i<(stop-start))
		add_element(&new_nl,i);

	/*Find qualified lists for section to insert */
	for(i=0;i<*len_of_mlr;i++){
		if (start>=(mlr+i)->num){
				pos=list_last_entry(&((mlr+i)->mylist),typeof(numl),my_list);
				//ori: [4-5] insert: [4-6]
				if(stop<=pos->num || stop==(pos->num)+1){
					(ql+j++)=pos;
				}
		}
		else if ((mlr+i)->num==(stop+1)){
			//ori: [4-5] insert: [1-3]
			(ql+j++) = (mlr+i);
		}
	}

	//merge_lists(ql,j);


	return 0;
}

int add_element(numl *root, int num){

	struct list_head mylist = INIT_LIST_HEAD(&mylist);
	numl new_nl={num,mylist};
	list_add_tail(&(new_nl.my_list), &(root->my_tlist));

	return 0;
}

int merge_list(numl *mlr, int len){


	return 0;
}

int input_handler(char input[10],int *op){

	char act[8]={0};
	char start[4]={0};
	char stop[4]={0};
	int i,space=0,comma=0,base,flag=1,errcode,_act=100,_page;



	for(i=0;i<strlen(input);i++){


		if(input[i]=='\0'||input[i]=='\n') break;
		if(input[i]==' ') ++space;
		else {
			if (space<1) act[i]=input[i];

			else {
				if (flag){
					base=i;
					flag=0;
				}
				if(input[i]==',') ++comma;
					if(comma<1)
						start[i-base]=input[i];
					else
						stop[i-base]=input[i];


			}
		}

	}

	/*if (space==0)
		base=i;
	act[base-space] = '\0';*/


	errcode=0;
	if(!strcasecmp(act,"Input")) _act=0;
	else if (!strcasecmp(act,"Delete")) _act=1;
	else if (!strcasecmp(act,"List")) _act=2;
	else if (!strcasecmp(act,"Quit")) _act=3;
	else
		errcode=1;

	if(_act==0 || _act==1)
		if(strlen(start)==0 || strlen(stop)==0)
			errcode=2;


	op[0]=_act;
	op[1]=atoi(start);
	op[1]=atoi(stop);

	return errcode;


}
