#include "simple_list.h"
int main(){

	char input[15] = {0};
	int errcode,quit = 0;
	int operation[3] = {0};
	int len_of_mlr =1, len_all_member=0, _s_all =0, _s_mlr=1;
	int *len_mlr = &len_of_mlr;
	int *all_len = &len_all_member;
	int *s_all = &_s_all;
	int *s_mlr = &_s_mlr;
	multil **mlr_ptr = calloc(1,sizeof(multil));
	multil **all = calloc(1,sizeof(multil));
	init(mlr_ptr,all,all_len,s_all);

	printf("Usage:\tInput/Delete/List section.\n");
	printf("Input:\tinput section, ex:Input 0,7\n");
	printf("Delete:\tdelete section, ex:Delete 3,5\n");
	printf("List:\t list all sections\n");
	printf("Exit enter quit\n");

	while(!quit){

		printf("\n#: ");
		fgets(input,14,stdin);
		errcode = input_handler(input,operation);
		if(!errcode){
			switch(operation[0]){

			case Input:
				add_section(mlr_ptr,all,operation[1],operation[2],len_mlr,all_len,s_mlr,s_all);
				show_all_lists(*mlr_ptr,*len_mlr);
				break;
			case Delete:
				del_section(mlr_ptr,all,operation[1],operation[2],len_mlr,all_len,s_mlr,s_all);
				show_all_lists(*mlr_ptr,*len_mlr);
				break;
			case List:
				show_all_lists(*mlr_ptr,*len_mlr);
				break;
			case Quit:
				quit=1;
				break;
			default:
				break;

			}
		}

		else if(errcode==1)
			printf("Please enter Input/Delete/List or quit\n");
		else if(errcode==2)
			printf("Please add section afetr Input/Delete action\n");
		else
			printf("Please input incremental section\n");

	}
	return 0;
}
