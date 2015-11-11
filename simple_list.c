#include "simple_list.h"

int add_element(numl *root, int num, int i){

	struct list_head mylist = LIST_HEAD_INIT(mylist);
	numl new_nl={num,mylist};

	root[i]=new_nl;
	INIT_LIST_HEAD(&(root[i].my_list));

	if(i!=0)
		list_add_tail(&(root[i].my_list), &(root->my_list));
	return 0;
}

int do_del_mlr(multil *mlr,int begin,int *len_of_mlr){

	int i;
	for(i=begin;i<(*len_of_mlr-1);i++)
		mlr[i] = mlr[i+1];

	*len_of_mlr=*len_of_mlr-1;

	return 0;
}


int do_merge(int type, numl *old, numl *new ){

	switch(type){
	case TAIL:
		list_add_tail( &(new->my_list),&(old->my_list));
		break;
	case HEAD:
		list_add( &(new->my_list),&(old->my_list));
		break;
	default:
		break;

	}


	return 0;
}

/*
int check_merge(multil *mlr, int *len_of_mlr, int begin){

	int i, start, stop, flag = 0;
	numl  *pos, *tmp;


	start = (mlr+begin)->head.num;
	pos = list_last_entry(&(((mlr+begin)->head).my_list),typeof(numl),my_list);
	stop = pos->num;



	for(i=0;i<*len_of_mlr;i++){

		if (i==begin)
			continue;

		pos=list_last_entry(&(((mlr+i)->head).my_list),typeof(numl),my_list);
		if(start==(mlr+i)->head.num && stop==(pos->num)){
					flag=1;
					break;
		}
		else if(start<(mlr+i)->head.num && stop>(pos->num)){
					//list i: [4-5] list begin: [3-7]
					flag=2;
					break;
				}

		else if (start>=(mlr+i)->head.num && start<=(pos->num+1)){
				//list i: [4-7] list begin: [5-9] || [8-9]
				if(stop>(pos->num)){
					for(tmp=&((mlr+i)->head);;tmp = list_next_entry(tmp,my_list)){
						if (tmp->num>=start && tmp->num<=pos->num)
							list_del(&tmp->my_list);
					}
					do_merge(TAIL,&((mlr+i)->head),&((mlr+begin)->head));
					flag =1;
					break;
				}

		}
		else if(start < (mlr+i)->head.num) {
					//list i: [4-7], list begin: [1-5] || [1-3]
				if ((stop+1) >=(mlr+i)->head.num){
					for(tmp=&((mlr+begin)->head);;tmp = list_next_entry(tmp,my_list)){
						if (tmp->num >=(mlr+i)->head.num && tmp->num<=stop)
							list_del(&tmp->my_list);
					}
					do_merge(HEAD,&((mlr+i)->head),&((mlr+begin)->head));
					flag =1;
					break;
				}
		}

	}

	switch(flag){

	case 1:
		do_del_mlr(mlr,begin,len_of_mlr);
		break;
	case 2:
		do_del_mlr(mlr,i,len_of_mlr);
		break;
	}

	if(flag)
		return 1;


	return 0;
}

*/
int add_section(multil **mlr_ptr, int start, int stop, int *len_of_mlr){

	int i,j,flag=0;
	numl *pos, *root;
	multil *mlr = *mlr_ptr;


	/*Find one of the qualified list for section to insert */

	for(i=0;i<*len_of_mlr;i++){

		pos=list_last_entry(&(((mlr+i)->head)->my_list),typeof(numl),my_list);

		// if section is perfectly matched, do nothing
		if(start==(mlr+i)->head->num && stop==(pos->num))
			return 0;



		else if(start<(mlr+i)->head->num && stop>(pos->num)){
			//ori: [4-5] insert: [3-7]

			struct list_head mylist = LIST_HEAD_INIT(mylist);
			numl new_nl ={start,mylist};

			root =calloc((stop-start+1),sizeof(numl));
			root[0]= new_nl;
			INIT_LIST_HEAD(&(root[0].my_list));
			mlr[i].head = root;

			for(j=start+1;j<stop+1;j++)
				add_element(root,j,j-start);
			break;
		}
		//Todo: fix sth wrong
		else if (start>=(mlr+i)->head->num && start<=(pos->num+1)){
				//ori: [4-7] insert: [5-9] || [8-9]
				if(stop>(pos->num)){

					struct list_head mylist = LIST_HEAD_INIT(mylist);
					numl new_nl ={(1+pos->num),mylist};

					root =calloc((stop-(1+pos->num)+1),sizeof(numl));
					root[0]= new_nl;
					INIT_LIST_HEAD(&(root[0].my_list));

					for(j=(2+pos->num);j<(stop+1);j++)
						add_element(root,j,j-start);

					do_merge(TAIL,&((mlr+i)->head),root);
				}
				break;
		}
		/*
		else if(start < (mlr+i)->head.num) {
			//ori: [4-7], insert: [1-5]
			if ((stop+1) >= (mlr+i)->head.num){

				new_nl.num = start;
				new_nl.my_list = mylist;
				INIT_LIST_HEAD(&(new_nl.my_list));
				for(j=(start+1);j<(mlr+i)->head.num;j++)
					add_element(&new_nl,j);
				do_merge(HEAD,&((mlr+i)->head),&new_nl);
				}
				break;
		}
		*/

		else
			++flag;


	}



	if(flag==*len_of_mlr){

		struct list_head mylist = LIST_HEAD_INIT(mylist);
		numl new_nl ={start,mylist};

		mlr = realloc(mlr,*len_of_mlr);
		root =calloc((stop-start+1),sizeof(numl));
		root[0]=new_nl;
		INIT_LIST_HEAD(&(root[0].my_list));
		mlr[i].head = root;


		for(j=start+1;j<stop+1;j++){
			add_element(root,j,j-start);

		}



		*len_of_mlr= *len_of_mlr+1;
		*mlr_ptr = mlr;
	}

	else{

	//Todo: check merge;

	*mlr_ptr = mlr;
	i=0;
	//while(check_merge(mlr,len_of_mlr,i))
		++i;

	}




	return 0;
}

int show_all_lists(multil* mlr, int len_of_mlr){

	int i;
	numl *pos;
	for(i=0;i<len_of_mlr;i++){
		printf("List %d:\t",i);
		if(!i){
			pos=list_first_entry(&(((mlr+i)->head)->my_list),numl,my_list);
			printf("%d, ",pos->num);
		}
		list_for_each_entry(pos,&(((mlr+i)->head)->my_list),my_list){
			printf("%d, ",pos->num);
		}

		printf("\n");
	}

}


int input_handler(char input[10],int *op){

	char act[8]={0};
	char start[4]={0};
	char stop[4]={0};
	int i,space=0,comma=0,base,flag=1,flag_comma=1,errcode,_act=100;



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
				else{
					if(comma<1){
						start[i-base]=input[i];
						//printf("start %c  %c \n",start[i-base],input[i]);
					}
					else{
						if(flag_comma){
							base=i;
							flag_comma=0;
						}
						stop[i-base]=input[i];
						//printf("stop %c  %c %d\n",stop[i-base],input[i],i-base);
					}
				}

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
	op[2]=atoi(stop);
	return errcode;


}
