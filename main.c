#include "simple_list.h"



int main(){

	printf("Usage:\tInput/Delete/List section.\n");
	printf("Input:\tinput section, ex:Input 0,7\n");
	printf("Delete:\tdelete section, ex:Delete 3,5\n");
	printf("List:\t list all sections");
	printf("Exit enter quit");

	struct list_head mlist = LIST_HEAD_INIT(mlist);
	numl nl = {0,mlist};
	numl *nl_root = &nl;
	int len_of_mlr =1;
	numl *mlr=calloc(len_of_mlr,sizeof(numl));
	char input[15] = {0};
	int quit = 0;
	int operation[3] = {0};
	mlr=nl_root;

	while(!quit){

		fgets(input,14,stdin);
		errcode = input_handler(input,operation);
		if(!errcode){
			switch(operation[0]){

			case Input:
				add_section();
				break;
			case Delete:
				break;
			case List:
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
		else
			printf("Please add section afetr Input/Delete action\n");

	}
	return 0;
}
