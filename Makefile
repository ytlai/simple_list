
all:
	gcc -g -c -Werror -fpic simple_list.c
	#gcc -c -Werror -fpic fake_lm_sensor.c
	gcc -shared -g -o liball.so simple_list.o
	gcc -L /home/lai/c_practice/simple_list -Wl,-rpath=/home/lai/c_practice/simple_list -Werror -g -o main main.c -l all

clean:
	rm -rf ./*.o	
