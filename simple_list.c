#include "simple_list.h"

int add_element(area *root, int num, d_linkd_list **elements, int *ele_num, int *all_space){

	d_linkd_list *_elements = *elements;
	d_linkd_list *tmp=NULL;

	if (*ele_num <=0) {
		_elements = (d_linkd_list*) calloc (1+*ele_num,sizeof(d_linkd_list));
		*all_space=*all_space+1;
	}
	else {
		if (*all_space<=*ele_num) {
			tmp =  realloc(_elements, (1+*ele_num)*sizeof(d_linkd_list));
			if (tmp)
				_elements = tmp;
			else {
				printf ("failed to allocate new space for element\n");
				exit (0);
			}
			*all_space = *all_space+1;
		}
	}
	_elements[*ele_num].item = area_new (*ele_num,num);
	INIT_LIST_HEAD(&((_elements[*ele_num].item)->list));
	list_add_tail(&((_elements[*ele_num].item)->list), &(root->list));
	*elements = _elements;
	*ele_num = *ele_num+1;
	return 0;
}

int add_head(d_linkd_list **heads, int *head_num, int *head_space){

	d_linkd_list *tmp;
	d_linkd_list *head_list = *heads;

	if ( *head_space <= *head_num ){
		tmp = (d_linkd_list*) realloc (head_list,(*head_num+1)*sizeof(d_linkd_list));
		if (tmp)
			head_list=tmp;
		else {
			printf ("failed to allocate new space for new area\n");
			exit (0);
		}
		*head_space = *head_space + 1;
	}
	head_list[*head_num].item = area_new (*head_num, 0);
	INIT_LIST_HEAD(&((head_list[*head_num].item)->list));

	*head_num= *head_num+1;
	*heads = head_list;
	return 0;
}

int _del_head(d_linkd_list **heads,int begin,int *head_num){

	d_linkd_list *head_list =*heads;
	int i;
	free(head_list[begin].item);
	for (i=begin;i<(*head_num-1);i++)
		head_list[i].item = head_list[i+1].item;
	*head_num = *head_num-1;
	return 0;
}

int _del_element(d_linkd_list **elements,int begin, int *ele_num){

	d_linkd_list *_elements =*elements;
	int j;
	free (_elements[begin].item);
	for (j=begin;j<(*ele_num-1);j++) {
		_elements[j] = _elements[j+1];
		_elements[j].item->index = j;
	}
	*ele_num = *ele_num-1;
	return 0;
}

int do_merge(int type, area *old, area *new){

	switch(type){
		case TAIL:
			list_splice_tail(&(new->list),&(old->list));
			break;
		case HEAD:
			list_splice(&(new->list),&(old->list));
			break;
		default:
			break;
	}
	return 0;
}

int check_merge(d_linkd_list **heads, d_linkd_list **elements, int *head_num, int begin, int *ele_num){

	int i, j, k, f, start, stop, flag = 0;
	area  *last, *tmp, *first;
	d_linkd_list *head_list = *heads;

	first= list_first_entry(&(((head_list+begin)->item)->list),typeof(area),list);
	last = list_last_entry(&(((head_list+begin)->item)->list),typeof(area),list);
	start = first->num;
	stop = last->num;

	for (i=0;i<*head_num;i++) {
		if (i==begin)
			continue;

		last=list_last_entry(&(((head_list+i)->item)->list),typeof(area),list);
		first = list_first_entry(&(((head_list+i)->item)->list),typeof(area),list);

		if (start==(first->num) && stop==(last->num)) {
			flag=1;
			break;
		}
		else if (start<(first->num) && stop>(last->num)) {
			flag=2;
			break;
		}
		else if (start>=(first->num) && start<=(last->num+1) && stop>(last->num) ){
			DEL_AREA(i,start);
			do_merge(TAIL,((head_list+i)->item),(head_list+begin)->item); \
			flag =1;
			break;
		}
		else if (start < (first->num) && (stop+1) >=(first->num)) {
			DEL_AREA(begin,(first->num));
			do_merge(HEAD,((head_list+i)->item),(head_list+begin)->item); \
			flag =1;
			break;
		}
	}
	switch(flag){
		case 1:
			_del_head(heads,begin,head_num);
			break;
		case 2:
			_del_head(heads,i,head_num);
			break;
		default:
			break;
	}

	return 0;
}

int check_area_consistency(d_linkd_list **heads, int *head_num, int *head_space, int which_head){

	d_linkd_list *head_list = *heads;
	area *pos, *prev;
	int flag = 0;
	pos = list_first_entry(&(((head_list+which_head)->item)->list),typeof(area),list);
	prev = pos;

	list_for_each_entry (pos,&(((head_list+which_head)->item)->list),list) {
		if (prev->num==pos->num || (prev->num)+1==(pos->num))
			prev = pos;
		else {
			flag=1;
			break;
		}
	}
	if (flag) {
		add_head(heads, head_num, head_space);
		list_cut_position(&((head_list[*head_num-1].item)->list),&(((head_list+which_head)->item)->list),&(prev->list));
	}
	return 0;
}

int area_count(area *_area){

	int k=0;
	area *tmp;
	list_for_each_entry (tmp,&(_area->list),list) {
		++k;
	}
	return k;
}
int area_delete(d_linkd_list **heads, d_linkd_list **elements, int start, int stop, int *head_num, int *ele_num, int *head_space, int *ele_space){

	int i,j,k,f,flag=0,ck=0;
	area *last, *first,*tmp;
	d_linkd_list *head_list = *heads;

	while (ck != *head_num) {

		ck=0;
		for (i=0;i<*head_num;i++) {

			last = list_last_entry(&(((head_list+i)->item)->list),typeof(area),list);
			first = list_first_entry(&(((head_list+i)->item)->list),typeof(area),list);
			if ( (first->num)>=start && (last->num)<=stop ){
				flag = 1;
				break;
			}
			else if ( (first->num)>stop || (last->num)<start )
				++ck;
			//i: 3-7 del 3-6, 5-7, 4-5
			else {
				DEL_AREA(i,start);
				check_area_consistency(heads, head_num, head_space, i);
				break;
			}
		}
		if(flag==1){
			_del_head(heads,i,head_num);
			flag = 0;
		}
	}
	return 0;
}

area *area_new (int index, int num){

	struct list_head newlist = LIST_HEAD_INIT(newlist);
	area new_a = { index, num, newlist };
	area *root = (area*) calloc (1,sizeof(area));
	root[0] = new_a;
	return root;
}
int area_find(area *_area, int num){

	area *last = list_last_entry(&(_area->list),typeof(area),list);
	area *first = list_first_entry(&(_area->list),typeof(area),list);

	if ( num >= first->num && num <= last->num)
		return 0;
	return 1;
}

int area_find_range(area *_area, int start, int stop){

	int i;
	for (i=start;i<stop+1;i++)
		if (area_find(_area,i))
			return 1;
	return 0;
}

int area_insert(d_linkd_list **heads, d_linkd_list **elements, int start, int stop, int *head_num, int *ele_num, int *head_space, int *ele_space){

	int i,j;
	area *last, *first, *_area;
	d_linkd_list *head_list = *heads;
	int best_idx = 0, max_match = 0;

	FIND_BEST_AREA();

	if (max_match == 0){
		add_head(heads, head_num, head_space);
		for (j=start;j<stop+1;j++)
			add_element((*heads)[*head_num-1].item, j, elements, ele_num, ele_space);
	}
	else {
		i = best_idx;
		_area = ((head_list+best_idx)->item);
		last = list_last_entry(&(_area->list),typeof(area),list);
		first = list_first_entry(&(_area->list),typeof(area),list);

		if (start<(first->num) && stop>(last->num))
			NEW_D_LINKED_LIST (0,start,(stop+1),NONE);

		else if (start>=(first->num) && start<=(last->num+1) && stop>(last->num))
			NEW_D_LINKED_LIST ((1+last->num),(1+last->num),(stop+1),TAIL);

		else if (start < (first->num) && (stop+1) >= (first->num))
			NEW_D_LINKED_LIST (start,start,(first->num),HEAD);

		*heads = head_list;
	}

	i=0;
	while (i<*head_num) {
		check_merge (heads,elements,head_num,i,ele_num);
		++i;
	}
	return 0;
}

void area_dump(area *_area){

	area *pos;
	list_for_each_entry(pos,&(_area->list),list){
		printf("%d, ",pos->num);
	}
}
void show_all_areas(d_linkd_list* head_list, int head_num){

	int i;
	for (i=0;i<head_num;i++){
		printf("List %d:\t",i);
		area_dump ((head_list+i)->item);
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

	if(!strcasecmp(act,"Add")) _act=0;
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
