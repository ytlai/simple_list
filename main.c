#include "simple_list.h"



int main(){

	printf("Usage:\tInput/Delete/List section.\n");
	printf("Input:\tinput section, ex:Input 0,7\n");
	printf("Delete:\tdelete section, ex:Delete 3,5\n");
	printf("List:\t list all sections\n");
	printf("Exit enter quit\n");



	struct list_head nlist=LIST_HEAD_INIT(nlist);;

	numl nl = {0,nlist};

	int len_of_mlr =1;
	int *len_mlr = &len_of_mlr;
	multil _mlr ={&nl};
	multil *mlr=calloc(len_of_mlr,sizeof(multil));
	multil **mlr_ptr;
	char input[15] = {0};
	int errcode,quit = 0;
	int operation[3] = {0};
	mlr[0]=_mlr;
	mlr_ptr = &mlr;
	numl *pos;
	struct list_head *head;
	int i;
	INIT_LIST_HEAD(&((mlr->head)->my_list));


	i=0;

	while(!quit){

		printf("\n#: ");
		fgets(input,14,stdin);
		errcode = input_handler(input,operation);
		if(!errcode){
			switch(operation[0]){

			case Input:
				add_section(mlr_ptr,operation[1],operation[2],len_mlr);
				show_all_lists(*mlr_ptr,*len_mlr);
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
