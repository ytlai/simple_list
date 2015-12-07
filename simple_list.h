#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

enum{
	Add,
	Delete,
	List,
	Quit
};

enum{
	TAIL,
	HEAD,
	NONE
};

typedef struct area{
	int index;
	int num;
	struct list_head list;
}area;

/*item could be head or element*/
typedef struct double_linked_list{
	area *item;
}d_linkd_list;

#define NEW_D_LINKED_LIST(n, s_t, s_p,t) do{ \
		area *root; \
		root = area_new(0,n); \
		INIT_LIST_HEAD(&(root->list)); \
		j=0; \
		for(j=s_t;j<s_p;j++) \
			add_element (root,j,elements,ele_num,ele_space); \
		if (t<2) { \
			do_merge(t,((head_list+i)->item),root); \
			free(root); \
		} \
		else { \
			free(head_list[i].item); \
			head_list[i].item = root; \
		} \
		} while(0)

#define DEL_AREA(n,s_t) do {\
		j=k=f=0; \
		k = area_count((head_list+n)->item); \
		while(1){ \
			list_for_each_entry(tmp,&(((head_list+n)->item)->list),list){ \
				++j; \
				if (tmp->num >=s_t && tmp->num<=stop){ \
					f=1; \
					break; \
				} \
			} \
			if (j==k && f==0) \
				break; \
			if (f) { \
				list_del(&(tmp->list)); \
				_del_element(elements,tmp->index,ele_num); \
				f=0; \
			} \
			j=k=0; \
			k = area_count((head_list+n)->item); \
		} \
} while(0)


#define FIND_BEST_AREA() do { \
	int *matched_num = (int*) calloc (*head_num,sizeof(int)); \
	for (i=0;i<*head_num;i++) { \
		_area = ((head_list+i)->item); \
		if (!area_find_range(_area,start,stop)) \
			return 0; \
		else { \
			for (j=start;j<stop+1;j++) \
				if (!area_find(_area,j)) \
					++matched_num[i]; \
		} \
	} \
	for (i=0;i<*head_num;i++) \
		if (matched_num[i] > max_match){ \
			best_idx = i; \
			max_match = matched_num[i]; \
		} \
	free (matched_num); \
}while(0)

area *area_new (int index, int num);
int area_insert(d_linkd_list **heads, d_linkd_list **elements, int start, int stop, int *head_num, int *ele_num, int *head_space, int *ele_space);
int area_delete(d_linkd_list **heads, d_linkd_list **elements, int start, int stop, int *head_num, int *ele_num, int *head_space, int *ele_space);
int area_find(area *area, int num);
int area_find_range(area *area, int start, int stop);
int area_count(area *_area);
void area_dump(area *_area);
void show_all_areas(d_linkd_list* head_list, int head_num);
int input_handler(char input[10],int *op);


