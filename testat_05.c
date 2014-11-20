/*	Originial provied by mascent
	comments and fixes by hanspeter

textbased implementation of minesweeper
*/

/*
Changes 2014-11-19 by hanspeter:
- commented all the procedures
- fixed code formatting
- renamed some functions and variables for readabilty
- normalized magic numbers
- reordered cell colors
- minor code simplifications

Changes 2014-11-20 by hanspeter:
- string capturing now works
- TO DO: dynamically adapt input validation to field size

/*
Routine:
- Generate a field of given size, set all cells to -11 (empty)
- Field should not be bigger than 9 cells

- Place n mines in randomly chosen cells, mines have value -10
- Update surrounding cells by decrementing their value starting from -1
- A cell has 8 neighboring mines max.

- Flags have values < -11
- Covered cells have negative values

- When a cell is revealed, its value is turned positive

- The input function takes characters and transforms them to coordinates
- After evaluation of the change and the new game status, the view is updated 
*/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* colors */
// color macros
#define KNRM "\x1B[0m"
#define KRED "\x1B[31m"
#define KGRN "\x1B[32m"
#define KYEL "\x1B[33m"
#define KBLU "\x1B[34m"
#define KMAG "\x1B[35m"
#define KCYN "\x1B[36m"
#define KWHT "\x1B[37m"

// color list
const char color[7][9] = {KBLU, KCYN, KGRN, KYEL, KMAG, KRED, KWHT};

/* field settings */
// field labels
#define L_COVR "[ ]\t"
#define L_EMPT " .\t"
#define L_MINE " *\t"
#define L_FLAG "[F]\t"
#define L_EXPL " X\t"

// field size
#define field_x 11
#define field_y 11

// number of mines
#define mines 1

// alphabet
const char alpha[] = "abcdefghijklmnopqrstuvwxyz";

int flags = mines;
int score = 0;
int win = 0;

/* model functions */
// prepare field
int field_value[field_y][field_x];



void clean_stdin(void)	//cleaning the pipes
{    
  while ( getchar() != '\n' );
}

int init_field(void){
	//printf("Debugger: wurde aufgerufen!\n");
	for (int i = 0; i < field_y; ++i)
	{
		for (int j = 0; j < field_x; ++j)
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
		x = (rand() % field_x);
		y = (rand() % field_y);

		if (field_value[y][x] != -10)
		{
			field_value[y][x] = -10;
			//printf("True(%d): %d\t%d\n", i, y, x);
			for (int k = y-1; k <= y+1; ++k)
			{
				if (k > -1 && k < field_y)
				{
					for (int l = x - 1; l <= x+1; ++l)
					{
						if (l > -1 && l < field_x && field_value[k][l] != -10)
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
	for (int i = -1; i < field_y; ++i)
	{
		for (int j = -1; j < field_x; ++j)
		{
				if (i < 0 && j > -1) {
					printf(KRED " %d\t" KNRM, j);				//Ausgabe der Achsenbeschriftung
				} else if (j < 0 ) {
					printf(KRED "%c\t" KNRM, alpha[i]);		//Ausgabe der Achsenbeschriftung
				}
				else if (field_value[i][j] < 0 && field_value[i][j] > -12){
					printf("[ ]\t");								//Ausgabe umgekehrtes Feld
				}
				else if (field_value[i][j] == 10){
					printf(KMAG " X\t" KNRM);					//Ausgabe Bombe
				}else if (field_value[i][j] == 11)
				{
					printf(" . \t" KNRM);							//Ausgabe leere Feld
				}
				else if (field_value[i][j] < -11) {
					printf("[F]\t");							//Ausgabe Feld mit flagsge
				}
				else {
					printf(" %s%d\t"KNRM, color[field_value[i][j]], field_value[i][j]);	//Ausgabe des Feldes mit Wert
				}
		}
		printf("\n");
	}
	printf("\n");
	for (int i = 3; i < field_x; ++i)
	{
		printf("\t");
	}
	printf("Minen verbleibend | %d |\n", flags);
}

int reveal_cell(int x, int y){
	int help;
	//printf("%d\t%d\t%d\n", x, y, field_value[y][x]);
	if (field_value[y][x] < 0 && field_value[y][x] > -12) { 
		help = field_value[y][x]; 
		field_value[y][x] = -help;
		//printf("%d\t%d\t%d\n", x, y, field_value[y][x]);
		//print_field();
		if (field_value[y][x] == 10){
			win = 2;
		}
		if (field_value[y][x] == 11)
		{
			for (int k = y-1; k < y+2; ++k)
				{ 
					for (int l = x - 1; l < x+2; ++l)
						{
							if (l > -1 && l < field_x && k > -1 && k < field_y)
							{
								reveal_cell(l ,k);
							}
						}
					
			}
		}
	}
	if (field_value[y][x] < -11)
	{
		//print_field();
		printf("Entferne zunächst die flagsge um dieses Feld aufzudecken!\n");
	}
	return 0;
}

int reveal_mines (void){
	for (int i = 0; i < field_y; ++i)
	{
		for (int j = 0; j < field_x; ++j)
		{
			if (field_value[i][j] == -10)
			{
				reveal_cell(j, i);
			}
		}
	}
}

int set_flags (int x, int y){
	int help;
	if (field_value[y][x] < 0 && field_value[y][x] > -12)
	{
		help = field_value[y][x];
		field_value[y][x] = help - 12;
		flags --;
		if (field_value[y][x] == -22){
			score ++;
		}
	} else if (field_value[y][x] < -11)
	{
		help = field_value[y][x];
		field_value[y][x] = help + 12;
		flags ++;
		if (field_value[y][x] == -10)
		{
			score  --;
		}
	} else 
	{
		printf("Du kannst hier keine Flage setzen\n");
	}
	//print_field();
}

int input_func(void){
	char input_char;
	char input[5];
	int i = 0;
	int mem_y = -1;
	int mem_x_1 = -1;	//damit werden zehn mal so viele Integer möglich
	int mem_x_2 = -1;
	int mem_com = 0;
 
	//printf("%c,%c\n", getchar(), getchar());
//do{	
	printf("> ");
	i = 0;
	do{												//erstellen des Strings
		input_char = getchar();
		//printf("%c\n", input_char);
		
		if (i < 5)
		{
			input[i] = input_char;
			//printf("%c\n", input[i]);
		}
		i++;
	} while (input_char != '\n' and i < 5);
	if (i > 5)
	{
		clean_stdin();
	}else if (i == 5){
		printf("Bitte drücke Enter\n");
		clean_stdin();
	}


	if (input[0] == '\n' or input[0] == 0){							//Fehler wenn die erste Stelle eine Leerausgabe ist
		printf("Bitte mache eine gültige Eingabe\n");
		return (1);
	}else if (input[0] == ':' && input[1] == 'q')	//Befehlseingabe(bisher nur q impimentiert)
	{
		printf("Danke fürs Spielen einen schönen Tag noch\n");
		return (-1);
	}else{											//normale Eingabe des Befehls
		mem_y = input[0] - 'a';									//Wert zuweisung y
		/*for (int i = 0; i < strlen(alpha); ++i)
		{
			if (input[0] == alpha[i]){
				mem_y = alpha[i];
			}
		}*/
		if (mem_y > -1 && mem_y < 26 && mem_y < field_y)
		{							//Eingabe von y war korrekt
			mem_x_1 = input[1] - '0';							//wert zu weisung von x1

			if (mem_x_1 > -1 && mem_x_1 < 10)						//Eingabe von x1 war korrekt
			{
			  	mem_x_2 = input[2] - '0';						//check ob x2 existiert - wenn ja wert zu weisung
				if (mem_x_2 > -1 && mem_x_2 < 10 && (mem_x_1 * 10 + mem_x_2) < field_x)
				{					//wenn x2 existiert kombination von beiden x1 als 10 x2 als einer
				 	mem_x_2 = 10 * mem_x_1 + mem_x_2;
				 	if (input[3] == 's')
				 	{			//check der aktion
				 		reveal_cell(mem_x_2, mem_y);
				 		return(0);
				 	}else if (input[3] == 'f')
				 	{
				 		set_flags(mem_x_2, mem_y);
				 		return(0);
				 	}else
				 	{
				 		printf("Die Eingabe ist fehlerhaft\n");	
				 		return (1);
				 	}
				}else
				{							//x2 existiert nicht wird der einfachheit halber aber aus x1 defniert, damit wir nur einen wert übergeben müssen - check der aktion
				 	mem_x_2 = mem_x_1;
				 	//printf("%d %d %c\n", mem_x_2, mem_y, input[2]);
				 	if (input[2] == 's')
				 	{			//check der aktion
				 		reveal_cell(mem_x_2, mem_y);
				 		return(0);
				 	}else if (input[2] == 'f')
				 	{
				 		set_flags(mem_x_2, mem_y);
				 		return(0);
				 	}else
				 	{
				 		printf("Die Eingabe ist fehlerhaft\n");	
				 		return (1);
				 	}
				}
			}else
			{								//x1 wurde nicht korrekt eingegeben
				printf("Die Eingabe ist fehlerhaft!\n");
				return (1);
			}
		}else 
		{									//y wurde nicht korrekt eingegeben
			printf("Die Eingabe ist fehlerhaft!\n");
			return (1);
		}

	}
	//printf("%s\n", test2);
//} while (1);
}

int game_loop(void){
	int status;
	while (1){
		status = input_func();
		if (status == 0)
		{
			if (flags == 0 && score == mines) 
			{
				win = 1;
				print_field();
				printf("\n\nGEWONNEN!\n");
				exit(0);
			}else if (win == -1){
				reveal_mines();
				print_field();
				printf("\n\nVERLOREN!\n");
				exit(0);
			}
			print_field();
		}
		if (status == 1){
			//error message printed by input_func()
			continue;
		}
		if (status == -1)
		{
			printf("Spiel beendet\n");
			exit(0);
		}
	}
	
}

int main(void){	
	//flags = mines = 10;//Anzahl der Bomben

	init_field();
	generate_mines(mines);
	print_field();
	game_loop();

	//reveal_cell(5, 5); // vorläufiger dummy test

	return 0;
}
