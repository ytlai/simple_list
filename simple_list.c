#include "simple_list.h"

int add_element(numl *root, int num, multil **all, int *all_len, int *s_all){

	numl *new_member = calloc(1,sizeof(numl));
	multil *mlr_member = *all;
	multil *tmp=NULL;

	if(*all_len <=0){
		mlr_member =  calloc(1+*all_len,sizeof(multil));
		*s_all=*s_all+1;
	}
	else{
		if(*s_all<=*all_len){

			tmp =  realloc(mlr_member, (1+*all_len)*sizeof(multil));
			if (tmp)
				mlr_member = tmp;
			else{
				printf("failed to realloc\n");
				exit(0);

			}
			*s_all=*s_all+1;
		}

	}

	struct list_head mylist = LIST_HEAD_INIT(mylist);
	numl new_nl={num,*all_len,mylist};
	new_member[0]=new_nl;
	INIT_LIST_HEAD(&(new_member->my_list));
	list_add_tail(&(new_member->my_list), &(root->my_list));
	mlr_member[*all_len].item = new_member;
	*all = mlr_member;
	*all_len=*all_len+1;
	return 0;
}

int init(multil **mlr_ptr, multil **all, int *all_len,int *s_all){

	numl *root = calloc(1,sizeof(numl));
	multil *mlr_head = calloc(1,sizeof(multil));
	struct list_head mylist = LIST_HEAD_INIT(mylist);
	numl new_nl={0,0,mylist};

	root[0] = new_nl;
	INIT_LIST_HEAD(&(root->my_list));
	mlr_head[0].item = root;
	*mlr_ptr = mlr_head;
	add_element(root,0,all,all_len, s_all);
	return 0;
}

int do_del_mlr(multil **mlr_ptr,int begin,int *len_of_mlr){

	int i;
	multil *mlr =*mlr_ptr;

	free(mlr[begin].item);

	for(i=begin;i<(*len_of_mlr-1);i++)
		mlr[i].item = mlr[i+1].item;

	*len_of_mlr=*len_of_mlr-1;

	return 0;
}

int do_del_member(multil **all,int begin, int *all_len){

	multil *mlr_member =*all;
	int j;

	free(mlr_member[begin].item);
	for(j=begin;j<(*all_len-1);j++){
		mlr_member[j] = mlr_member[j+1];
		mlr_member[j].item->index = j;
	}

	*all_len = *all_len-1;
	return 0;
}

int do_merge(int type, numl *old, numl *new){

	switch(type){
		case TAIL:
			list_splice_tail(&(new->my_list),&(old->my_list));
			break;
		case HEAD:
			list_splice(&(new->my_list),&(old->my_list));
			break;
		default:
			break;
	}
	return 0;
}

int check_merge(multil **mlr_ptr, multil **all, int *len_of_mlr, int begin, int *all_len){

	int i, j,k,f, start, stop, flag = 0;
	numl  *pos, *tmp, *first;
	multil *mlr = *mlr_ptr;

	start = (mlr+begin)->item->num;
	pos = list_last_entry(&(((mlr+begin)->item)->my_list),typeof(numl),my_list);
	stop = pos->num;

	for(i=0;i<*len_of_mlr;i++){

		if (i==begin)
			continue;

		pos=list_last_entry(&(((mlr+i)->item)->my_list),typeof(numl),my_list);
		first = list_first_entry(&(((mlr+i)->item)->my_list),typeof(numl),my_list);

		if(start==(first->num) && stop==(pos->num)){
			flag=1;
			break;
		}
		else if(start<(first->num) && stop>(pos->num)){
			//list i: [4-5] list begin: [3-7]
			flag=2;
			break;
		}

		else if (start>=(first->num) && start<=(pos->num+1)){
			//list i: [4-7] list begin: [5-9] || [8-9]
			if(stop>(pos->num)){

				j=k=f=0;
				list_for_each_entry(tmp,&(((mlr+i)->item)->my_list),my_list){
					++k;}

				while(1){
					list_for_each_entry(tmp,&(((mlr+i)->item)->my_list),my_list){
					++j;

					if (tmp->num >=start && tmp->num<=stop){
						f=1;
						break;
						}
					}

					if(j==k && f==0)
						break;

					if(f){
						list_del(&(tmp->my_list));
						do_del_member(all,tmp->index,all_len);
						f=0;
					}
					j=0;k=0;
					list_for_each_entry(tmp,&(((mlr+i)->item)->my_list),my_list){
						++k;}
				}

				do_merge(TAIL,((mlr+i)->item),(mlr+begin)->item);
				flag =1;
				break;
			}
		}

		else if(start < (first->num)) {
			//list i: [4-7],[9-13] list begin: [1-5] || [1-3] [0-12]
			if ((stop+1) >=(first->num)){

				j=k=f=0;
				list_for_each_entry(tmp,&(((mlr+begin)->item)->my_list),my_list){
					++k;}

				while(1){
					list_for_each_entry(tmp,&(((mlr+begin)->item)->my_list),my_list){
						++j;
						if (tmp->num >=(first->num) && tmp->num<=stop){
							f=1;
							break;
						}
					}

					if(j==k && f==0)
						break;

					if(f){
						list_del(&(tmp->my_list));
						do_del_member(all,tmp->index,all_len);
						f=0;
					}

					j=0;k=0;
					list_for_each_entry(tmp,&(((mlr+begin)->item)->my_list),my_list){
						++k;}
				}

				do_merge(HEAD,((mlr+i)->item),(mlr+begin)->item);
				flag =1;
				break;
			}
		}
	}
	switch(flag){
		case 1:
			do_del_mlr(mlr_ptr,begin,len_of_mlr);
			break;
		case 2:
			do_del_mlr(mlr_ptr,i,len_of_mlr);
			break;
		default:
			break;
	}
}

int check_consistency(multil **mlr_ptr, int *len_of_mlr, int *s_mlr, int which_head){

	multil *mlr = *mlr_ptr;
	numl *pos, *root, *prev;
	int flag=0;
	pos = list_first_entry(&(((mlr+which_head)->item)->my_list),typeof(numl),my_list);
	prev = pos;

	list_for_each_entry(pos,&(((mlr+which_head)->item)->my_list),my_list){
		if (prev->num==pos->num || (prev->num)+1==(pos->num))
			prev = pos;
		else{
			flag=1;
			break;
		}
	}

	if(flag){
		struct list_head mylist = LIST_HEAD_INIT(mylist);
		numl new_nl ={prev->num,0,mylist};

		if(*s_mlr<=*len_of_mlr ){
			multil *tmp = realloc(mlr,(*len_of_mlr+1)*sizeof(multil));
			if(tmp)
				mlr=tmp;
			else
				exit(0);
			*s_mlr=*s_mlr+1;
		}

		root = calloc(1,sizeof(numl));
		root[0]= new_nl;
		INIT_LIST_HEAD(&(root->my_list));
		mlr[*len_of_mlr].item = root;
		*len_of_mlr= *len_of_mlr+1;
		*mlr_ptr = mlr;
		list_cut_position(&(root->my_list),&(((mlr+which_head)->item)->my_list),&(prev->my_list));
	}

}

int del_section(multil **mlr_ptr, multil **all, int start, int stop, int *len_of_mlr, int *all_len, int *s_mlr, int *s_all){

	int i,j,k,f, flag=0,ck=0;
	numl *pos, *first,*tmp;
	multil *mlr = *mlr_ptr;

	while(ck != *len_of_mlr){
		ck=0;
		for(i=0;i<*len_of_mlr;i++){

			pos=list_last_entry(&(((mlr+i)->item)->my_list),typeof(numl),my_list);
			first = list_first_entry(&(((mlr+i)->item)->my_list),typeof(numl),my_list);

			// perfect match
			if((first->num)>=start && (pos->num)<=stop ){
				flag = 1;
				break;
			}

			// no intersection
			else if((first->num)>stop || (pos->num)<start )
				++ck;

			//org: 3-7 del 3-6, 5-7, 4-5
			else{

				j=k=f=0;
				list_for_each_entry(tmp,&(((mlr+i)->item)->my_list),my_list){
					++k;}

				while(1){
					list_for_each_entry(tmp,&(((mlr+i)->item)->my_list),my_list){
						++j;
						if (tmp->num >=start && tmp->num<=stop){
							f=1;
							break;
						}
					}
					if(j==k && f==0)
						break;

					if(f){
						list_del(&(tmp->my_list));
						do_del_member(all,tmp->index,all_len);
						f=0;
					}
					j=0;k=0;
					list_for_each_entry(tmp,&(((mlr+i)->item)->my_list),my_list){
						++k;
					}
				}
				check_consistency(mlr_ptr, len_of_mlr, s_mlr, i);
				break;
			}
		}

		if(flag==1){
			do_del_mlr(mlr_ptr,i,len_of_mlr);
			flag = 0;
		}
	}

	return 0;
}

int add_section(multil **mlr_ptr, multil **all, int start, int stop, int *len_of_mlr, int *all_len, int *s_mlr, int *s_all){


	int i,j,flag=0;
	numl *last, *root, *first;
	multil *mlr = *mlr_ptr;

	/*Find one of the qualified list for section to insert */

	for(i=0;i<*len_of_mlr;i++){

		last=list_last_entry(&(((mlr+i)->item)->my_list),typeof(numl),my_list);
		first = list_first_entry(&(((mlr+i)->item)->my_list),typeof(numl),my_list);

		// if section is perfectly matched, do nothing
		if(start==(first->num) && stop==(last->num))
			return 0;

		else if((first->num)<=start && (last->num)>=stop)
			return 0;

		else if(start<(first->num) && stop>(last->num)){
			//ori: [4-5] insert: [3-7]
			struct list_head mylist = LIST_HEAD_INIT(mylist);
			numl new_nl ={start,0,mylist};

			root = calloc(1,sizeof(numl));
			root[0]= new_nl;
			INIT_LIST_HEAD(&(root->my_list));
			free(mlr[i].item);
			mlr[i].item = root;

			for(j=start;j<stop+1;j++)
				add_element(root,j,all,all_len,s_all);
			break;
		}

		else if (start>=(first->num) && start<=(last->num+1) && stop>(last->num)){
			//ori: [4-7] insert: [5-9] || [8-9]
			struct list_head mylist = LIST_HEAD_INIT(mylist);
			numl new_nl ={(1+last->num),0,mylist};

			root = calloc(1,sizeof(numl));
			root[0]= new_nl;
			INIT_LIST_HEAD(&(root->my_list));
			j=0;

			for(j=(1+last->num);j<(stop+1);j++)
				add_element(root,j,all,all_len,s_all);

			do_merge(TAIL,((mlr+i)->item),root);
			free(root);
			break;
		}

		else if(start < (first->num) && (stop+1) >= (first->num)) {
			//ori: [4-7], insert: [1-5]
			struct list_head mylist = LIST_HEAD_INIT(mylist);
			numl new_nl ={start,0,mylist};

			root = calloc(1,sizeof(numl));
			root[0]= new_nl;
			INIT_LIST_HEAD(&(root->my_list));
			j=0;

			for(j=start;j<(first->num);j++)
				add_element(root,j,all,all_len,s_all);
			do_merge(HEAD,((mlr+i)->item),root);
			free(root);
			break;
		}

		else
			++flag;
	}

	if(flag==*len_of_mlr){

		multil *tmp;
		struct list_head mylist = LIST_HEAD_INIT(mylist);
		numl new_nl ={start,0,mylist};

		if(*s_mlr<=*len_of_mlr ){
			tmp = realloc(mlr,(*len_of_mlr+1)*sizeof(multil));
			if(tmp)
				mlr=tmp;
			else
				exit(0);
			*s_mlr=*s_mlr+1;
		}

		root = calloc(1,sizeof(numl));
		root[0]= new_nl;
		INIT_LIST_HEAD(&(root->my_list));
		mlr[i].item = root;

		for(j=start;j<stop+1;j++)
			add_element(root,j,all,all_len,s_all);

		*len_of_mlr= *len_of_mlr+1;
		*mlr_ptr = mlr;
	}
	else{

		*mlr_ptr = mlr;
		i=0;
		while(i<*len_of_mlr){
			check_merge(mlr_ptr,all,len_of_mlr,i,all_len);
			++i;
		}
	}
	return 0;
}

void show_all_lists(multil* mlr, int len_of_mlr){

	int i;
	numl *pos;

	for(i=0;i<len_of_mlr;i++){
		printf("List %d:\t",i);
		list_for_each_entry(pos,&(((mlr+i)->item)->my_list),my_list){
							printf("%d, ",pos->num);}
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
					}
					else{
						if(flag_comma){
							base=i;
							flag_comma=0;
						}
						stop[i-base]=input[i];
					}
				}

			}
		}

	}
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

	if(_act==0 || _act==1)
		if(atoi(stop)-atoi(start)<0)
			errcode = 3;

	op[0]=_act;
	op[1]=atoi(start);
	op[2]=atoi(stop);
	return errcode;
}
