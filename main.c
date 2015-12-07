#include "simple_list.h"

int main (void) {

	char input[15] = {0};
	int errcode,quit = 0;
	int operation[3] = {0};
	int _head_num = 0, _ele_num = 0, _ele_space = 0, _head_space=0;
	int *head_num = &_head_num;
	int *ele_num = &_ele_num;
	int *head_space = &_head_space;
	int *ele_space = &_ele_space;
	d_linkd_list **heads = calloc(1,sizeof(d_linkd_list));
	d_linkd_list **elements = calloc(1,sizeof(d_linkd_list));


	printf("Usage:\tAdd/Delete/List section.\n");
	printf("Add:\tadd section, ex:Add 0,7\n");
	printf("Delete:\tdelete section, ex:Delete 3,5\n");
	printf("List:\t list all sections\n");
	printf("Exit enter quit\n");

	//initialize
	area_insert(heads,elements,0,0,head_num,ele_num,head_space,ele_space);

	while(!quit){

		printf("\n#: ");
		fgets(input,14,stdin);
		errcode = input_handler(input,operation);
		if(!errcode){
			switch(operation[0]){
			case Add:
				area_insert (heads,elements,operation[1],operation[2],head_num,ele_num,head_space,ele_space);
				show_all_areas (*heads,*head_num);
				break;
			case Delete:
				area_delete (heads,elements,operation[1],operation[2],head_num,ele_num,head_space,ele_space);
				show_all_areas (*heads,*head_num);
				break;
			case List:
				show_all_areas (*heads,*head_num);
				break;
			case Quit:
				quit=1;
				break;
			default:
				break;
			}
		}
		else if( errcode==1)
			printf("Please enter Add/Delete/List or quit\n");
		else if (errcode==2)
			printf("Please add section after Add/Delete action\n");
		else
			printf("Please input incremental section\n");
	}
	return 0;
}
