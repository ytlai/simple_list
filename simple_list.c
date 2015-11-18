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

	int j;
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
	multil *tmp;

	free(mlr[begin].item);

	for(i=begin;i<(*len_of_mlr-1);i++)
		mlr[i].item = mlr[i+1].item;

	*len_of_mlr=*len_of_mlr-1;

	return 0;
}

int do_del_member(multil **all,int begin, int *all_len){

	multil *mlr_member =*all;
	multil *tmp;

	int j,i;


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


	printf("begin = %d len %d\n",begin,*len_of_mlr);

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
			printf("merge:1  start %d stop %d, mlr start %d stop %d\n", start, stop,(first->num),pos->num);
					flag=2;
					break;
				}

		else if (start>=(first->num) && start<=(pos->num+1)){
				//list i: [4-7] list begin: [5-9] || [8-9]
			if(stop>(pos->num)){

				printf("merge:2  start %d stop %d, mlr start %d stop %d\n", start, stop,(first->num),pos->num);
					j=k=f=0;

					list_for_each_entry(tmp,&(((mlr+i)->item)->my_list),my_list){
						++k;
					}

					while(1){
						list_for_each_entry(tmp,&(((mlr+i)->item)->my_list),my_list)
						{
							++j;
							//printf("tmp->num  %d ,(mlr+i)->item->num % d \n",tmp->num,(mlr+i)->item->num);
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


					//move_list(mlr_ptr,added_len,(mlr+begin)->item);
					do_merge(TAIL,((mlr+i)->item),(mlr+begin)->item);
					flag =1;
					break;
				}

		}

		else if(start < (first->num)) {
					//list i: [4-7],[9-13] list begin: [1-5] || [1-3] [0-12]

				if ((stop+1) >=(first->num)){
					printf("merge:3  start %d stop %d, mlr start %d stop %d\n", start, stop,(first->num),pos->num);

					j=k=f=0;

					list_for_each_entry(tmp,&(((mlr+begin)->item)->my_list),my_list){
						++k;
					}
					while(1){
						list_for_each_entry(tmp,&(((mlr+begin)->item)->my_list),my_list)
						{
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
							printf("index to delete %d \n",tmp->index);
							do_del_member(all,tmp->index,all_len);
							f=0;
						}
						j=0;k=0;
						list_for_each_entry(tmp,&(((mlr+begin)->item)->my_list),my_list){
												++k;
						}


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

	printf("In merge\n");
	printf("len of mlr %d\n",*len_of_mlr);

	for(i=0;i<*len_of_mlr;i++){
		printf("%d \n",(*mlr_ptr +i)->item->num);
	}
	printf("len of all %d\n",*all_len);
	for(i=0;i<*all_len;i++){
			printf("%d \n",(*all +i)->item->num);
		}

}


int add_section(multil **mlr_ptr, multil **all, int start, int stop, int *len_of_mlr, int *all_len, int *s_mlr, int *s_all){


	int i,j,flag=0;
	numl *pos, *root, *first;
	multil *mlr = *mlr_ptr;



	/*Find one of the qualified list for section to insert */

	for(i=0;i<*len_of_mlr;i++){

		pos=list_last_entry(&(((mlr+i)->item)->my_list),typeof(numl),my_list);
		first = list_first_entry(&(((mlr+i)->item)->my_list),typeof(numl),my_list);
		printf("pos->num %d\n",pos->num);
		printf("mlr first num %d\n",first->num);

		// if section is perfectly matched, do nothing
		if(start==(first->num) && stop==(pos->num))
			return 0;
		else if((first->num)<=start && (pos->num)>=stop){
			printf("hello\n");
			return 0;
		}

		else if(start<(first->num) && stop>(pos->num)){
			//ori: [4-5] insert: [3-7]
			printf("add:1  start %d stop %d, mlr start %d stop %d\n", start, stop,(first->num),pos->num);
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

		else if (start>=(first->num) && start<=(pos->num+1)){
				//ori: [4-7] insert: [5-9] || [8-9]

				if(stop>(pos->num)){
					printf("add:2  start %d stop %d, mlr start %d stop %d\n", start, stop,(first->num),pos->num);



					struct list_head mylist = LIST_HEAD_INIT(mylist);
					numl new_nl ={(1+pos->num),0,mylist};


					root = calloc(1,sizeof(numl));

					root[0]= new_nl;
					INIT_LIST_HEAD(&(root->my_list));


					j=0;

					for(j=(1+pos->num);j<(stop+1);j++)
						add_element(root,j,all,all_len,s_all);

					do_merge(TAIL,((mlr+i)->item),root);
					free(root);

				}
				//show_all_lists(mlr,*len_of_mlr);

				break;
		}

		else if(start < (first->num)) {
			//ori: [4-7], insert: [1-5]
			if ((stop+1) >= (first->num)){
				printf("add:3  start %d stop %d, mlr start %d stop %d\n", start, stop,(first->num),pos->num);

				struct list_head mylist = LIST_HEAD_INIT(mylist);

				numl new_nl ={start,0,mylist};


				root = calloc(1,sizeof(numl));
				root[0]= new_nl;
				INIT_LIST_HEAD(&(root->my_list));



				j=0;

				for(j=start;j<(first->num);j++)
					add_element(root,j,all,all_len,s_all);

				do_merge(HEAD,((mlr+i)->item),root);

				}
				free(root);
				break;
		}


		else
			++flag;


	}



	if(flag==*len_of_mlr){

		printf("hello\n");

		//printf("hello %d %d\n",*len_of_mlr,i);
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
		//printf("addr root %p addr new_nl %p \n",root,&new_nl);
		INIT_LIST_HEAD(&(root->my_list));
		mlr[i].item = root;


		for(j=start;j<stop+1;j++)
			add_element(root,j,all,all_len,s_all);


		*len_of_mlr= *len_of_mlr+1;

		*mlr_ptr = mlr;

		printf("In add_section\n");
		printf("len of mlr %d\n",*len_of_mlr);

			for(i=0;i<*len_of_mlr;i++){
				printf("%d \n",(*mlr_ptr +i)->item->num);
			}
			printf("len of all %d\n",*all_len);
			for(i=0;i<*all_len;i++){
					printf("%d \n",(*all +i)->item->num);
				}

	}

	else{

	//Todo: check merge;

	*mlr_ptr = mlr;

	printf("Before merge\n");
	printf("len of mlr %d\n",*len_of_mlr);

		for(i=0;i<*len_of_mlr;i++){
			printf("%d \n",(*mlr_ptr +i)->item->num);
		}
		printf("len of all %d\n",*all_len);
		for(i=0;i<*all_len;i++){
				printf("%d \n",(*all +i)->item->num);
			}

	i=0;
	//printf("len of mlr %d \n",*len_of_mlr );
	while(i<*len_of_mlr){
		check_merge(mlr_ptr,all,len_of_mlr,i,all_len);
	++i;}



	}




	return 0;
}

void show_all_lists(multil* mlr, int len_of_mlr){

	int i;
	numl *pos;
	//printf("\n\n%d\n",len_of_mlr);



	for(i=0;i<len_of_mlr;i++){
		printf("List %d:\t",i);



		list_for_each_entry(pos,&(((mlr+i)->item)->my_list),my_list){
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

	if(_act==0 || _act==1)
		if(atoi(stop)-atoi(start)<0)
			errcode = 3;

	op[0]=_act;
	op[1]=atoi(start);
	op[2]=atoi(stop);
	return errcode;


}
