/*Originial provied by mascent

textbased implimentation of minesweeper
*/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#define KNRM "\x1B[0m"
#define KRED "\x1B[31m"
#define KGRN "\x1B[32m"
#define KYEL "\x1B[33m"
#define KBLU "\x1B[34m"
#define KMAG "\x1B[35m"
#define KCYN "\x1B[36m"
#define KWHT "\x1B[37m"

#define fields_x 8
#define fields_y 8

char color[7][9] = {"\x1B[31m", "\x1B[32m", "\x1B[33m", "\x1B[34m", "\x1B[35m", "\x1B[36m", "\x1B[37m"};

char zuordnung[16] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p'};
int field_value[fields_y][fields_x];
int sieg = 0;
int lose_win = 0;
int flag = 0;
int bomb_n;


int initial_field(void){
	//printf("Debugger: wurde aufgerufen!\n");
	for (int i = 0; i < fields_y; ++i)
	{
		for (int j = 0; j < fields_x; ++j)
		{
			field_value[i][j] = -11;
			//printf("%d\t%d\t%d\n", i, j, field_value[i][j]);
		}
	}

	return 0;
}

int generate_mines(int n){
	srand(time(NULL));
	int x,y;
	int i = 0;

	do
	{
		x = (rand() % fields_x);
		y = (rand() % fields_y);

		if (field_value[y][x] != -10)
		{
			field_value[y][x] = -10;
			//printf("True(%d): %d\t%d\n", i, y, x);
			for (int k = y-1; k <= y+1; ++k)
			{
				if (k > -1 && k < fields_y)
				{
					for (int l = x - 1; l <= x+1; ++l)
					{
						if (l > -1 && l < fields_x && field_value[k][l] != -10)
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
	} while (i < n);
	return 0;
}

int print_field(void){
	printf("\n");
	for (int i = -1; i < fields_y; ++i)
	{
		for (int j = -1; j < fields_x; ++j)
		{
				if (i < 0 && j > -1) {
					printf(KRED "%d\t" KNRM, j);				//Ausgabe der Achsenbeschriftung
				} else if (j < 0 ) {
					printf(KRED "%c\t" KNRM, zuordnung[i]);		//Ausgabe der Achsenbeschriftung
				}
				else if (field_value[i][j] < 0 && field_value[i][j] > -12){
					printf("[]\t");
				}
				else if (field_value[i][j] == 10){
					printf(KMAG "X\t" KNRM);
				}else if (field_value[i][j] == 11)
				{
					printf(".\t" KNRM);
				}
				else if (field_value[i][j] < -11) {
					printf("[F]\t");
				}
				else {
					printf("%s%d\t"KNRM, color[field_value[i][j]], field_value[i][j]);
				}
		}
		printf("\n");
	}
	printf("\n");
	for (int i = 3; i < fields_x; ++i)
	{
		printf("\t");
	}
	printf("Minen verbleibend | %d |\n", bomb_n - flag);
}

int open_field(int x, int y){
	int help;
	//printf("%d\t%d\t%d\n", x, y, field_value[y][x]);
	if (field_value[y][x] < 0 && field_value[y][x] > -12) { 
		help = field_value[y][x]; 
		field_value[y][x] = -help;
		//printf("%d\t%d\t%d\n", x, y, field_value[y][x]);
		print_field();
		if (field_value[y][x] == 10){
			lose_win = 2;
		}
		if (field_value[y][x] == 11)
		{
			for (int k = y-1; k < y+2; ++k)
				{ 
					for (int l = x - 1; l < x+2; ++l)
						{
							if (l > -1 && l < fields_x && k > -1 && k < fields_y)
							{
								open_field(l ,k);
							}
						}
					
			}
		}
	}
	if (field_value[y][x] < -11)
	{
		//print_field();
		printf("Entferne zunächst die Flagge um dieses Feld aufzudecken!\n");
	}
	return 0;
}

int open_mines (void){
	for (int i = 0; i < fields_y; ++i)
	{
		for (int j = 0; j < fields_x; ++j)
		{
			if (field_value[i][j] == -10)
			{
				open_field(j, i);
			}
		}
	}
}

int set_flag (int x, int y){
	int help;
	if (field_value[y][x] < 0 && field_value[y][x] > -12)
	{
		help = field_value[y][x];
		field_value[y][x] = help - 12;
		flag ++;
		if (field_value[y][x] == -22){
			sieg ++;
		}
	} else if (field_value[y][x] < -11)
	{
		help = field_value[y][x];
		field_value[y][x] = help + 12;
		flag --;
		if (field_value[y][x] == -10)
		{
			sieg  --;
		}
	} else 
	{
		printf("Du kannst hier keine Flagge setzen\n");
	}
	print_field();
}

int game_loop(void){
	int mem_y, mem_x;
	char first,second,third,fourth;
	char input[5];
	char quit[5]; quit[0] = 'q';

	while (input[0] != quit[0])
	{
		if (sieg == bomb_n && flag == sieg){
			lose_win = 1;
		}
		if (lose_win == 0) 
		{
			printf(">");
			scanf("%5s", &input[0]);
			first = input[0]; second = input[1]; third = input[2]; fourth = input[3];
			for (int i = 0; i < fields_y; ++i)
			{
				if (first == zuordnung[i])
				{
					mem_y = i;
				}
			}

			mem_x = second - '0';
			/*if (mem_y == 4 && mem_x == 2)
			{
				lose_win = 1;
			} else */

			if (mem_x > -1 && mem_x < fields_x){
				if (third == 's'){
					open_field(mem_x,mem_y);
				} else if (third == 'f')
				{
					set_flag(mem_x, mem_y);
				}
				//printf("%d\t%d\n", mem_y, mem_x);
			} else if (input[0] == quit[0])
			{
				printf("Danke fürs spielen einen schönen Tag noch!\n");
			} else {
				open_mines();
				printf("Die Eingabe war fehlerhaft!\n");
			}

			//printf("%c %c %c %c\n", first, second, third, fourth);
			//printf("%s\n", input);
		} else if (lose_win == 1)
		{
			printf("Du hast gewonnen!\nGood job!\n");
			input[0] = 'q';
		} else {
			printf("Du hast leider verloren\n");
			input[0] = 'q';
		}
	}

	return 0;
}

int main(void){	
	bomb_n = 10;//Anzahl der Bomben

	initial_field();
	generate_mines(bomb_n);
	print_field();
	game_loop();

	//open_field(5, 5); // vorläufiger dummy test

	return 0;
}
