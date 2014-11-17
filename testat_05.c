/*Originial provied by mascent

textbased implimentation of minesweeper not finished yet
*/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#define fields 9

int field_value[fields][fields];

int intial_field (void){

	for (int i = 0; i < fields; ++i)
	{
		for (int j = 0; j < fields; ++j)
		{
			field_value[i][j] = -11;
		}
	}

}

int generate_mines(int n){
	srand(time(NULL));
	int x,y;
	int i = 0;

	do
	{
		x = (rand() % fields);
		y = (rand() % fields);

		if (field_value[y][x] != -10)
		{
			field_value[y][x] = -10;
			//printf("True(%d): %d\t%d\n", i, y, x);
			for (int k = y-1; k <= y+1; ++k)
			{
				if (k > -1 && k < fields)
				{
					for (int l = x - 1; l <= x+1; ++l)
					{
						if (l > -1 && l < fields && field_value[k][l] != -10)
						{
							if (field_value[k][l] > -10){
								field_value[k][l] -= 1;
							} else{
								field_value[k][l] = -1;
							}
						}
						//printf("%d\t%d\t%d\n", k, l, field_value[k][l]);
					}
				}
			}
			i++;
		} else {
			//printf("False(%d): %d\t%d\n", i, y, x);
		}
	} while (i < fields);
	return 0;
}

int print_field(void){
	printf("\n");
	for (int i = 0; i < fields; ++i)
	{
		for (int j = 0; j < fields; ++j)
		{
			if (field_value[i][j] < 0){
				printf("[]\t");
			}
			else if (field_value[i][j] == 10){
				printf("[X]\t");
			} else if (field_value[i][j] == 11)
			{
				printf("[.]\t");
			} else {
				printf("[%d]\t", field_value[i][j]);
			}
		}
		printf("\n");
	}
}

int open_field(int x, int y){
	int help;
	//printf("%d\t%d\t%d\n", x, y, field_value[y][x]);
	if (field_value[y][x] < 0) { 
		help = field_value[y][x]; 
		field_value[y][x] = -help;
		printf("%d\t%d\t%d\n", x, y, field_value[y][x]);
		print_field();
		if (field_value[y][x] == 11)
		{
			for (int k = y-1; k < y+2; ++k)
				{ 
					for (int l = x - 1; l < x+2; ++l)
						{
							if (l > -1 && l < fields && k > -1 && k < fields)
							{
								open_field(l ,k);
							}
						}
					
			}
		}
	}
	return 0;
}


int main(void){
	int n = 8, mem_y, mem_x;
	char first,second,third,fourth;
	char zuordnung[16] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p'};
	char input[5];
	char quit[5]; quit[0] = 'q';


	intial_field();
	generate_mines(n);
	print_field();
	//open_field(5, 5);

	while (input[0] != quit[0])
	{
		printf(">");
		scanf("%5s", &input[0]);
		first = input[0]; second = input[1]; third = input[2]; fourth = input[3];
		for (int i = 0; i < fields; ++i)
		{
			if (first == zuordnung[i])
			{
				mem_y = i;
			}
		}

		mem_x = second - '0';

		if (mem_x > -1){
			open_field(mem_x,mem_y);
			printf("%d\t%d\n", mem_y, mem_x);
		}

		//printf("%c %c %c %c\n", first, second, third, fourth);
		//printf("%s\n", input);
	}


	return 0;
}
