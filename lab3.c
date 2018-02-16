// Anthony Bilic 20514128

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PT_SIZE 8
#define MAIN_MEM_SIZE 8
#define DISK_MEM_SIZE 16
#define PAGE_SIZE 2
#define MAX_INPUTS 3

typedef struct pte_struct {
	int entry;
	int valid; //1 if in main_mem, 0 is on disk_mem
	int dirty;
	int page;
} pte_struct;

void initalize_mem(int* pt, pte_struct* pte, int* main_mem, int* disk_mem);
void fill_array(int* array, int array_size, int n);

int read(int va);
int write(int va, int n);
int showmain(int page_num);
int showdisk(int page_num);
int showptable();
int quit();

void handle_pf();
void parse_input(char* input, int** input_p);
int handle_input(int** input_p);

void prep_input_p(int** input_p);
void free_mem(int** input_p);

int main(int argc, const char * argv[])
{
	int pt[PT_SIZE];
	pte_struct pte[PT_SIZE];
	int main_mem[MAIN_MEM_SIZE];
	int disk_mem[DISK_MEM_SIZE];
	initalize_mem(pt, pte, main_mem, disk_mem);

	// Testing input from file
	FILE *file;
	char *filename = "t_simple.txt";
	file = fopen(filename,"r");

	char input[50];
	while(fgets(input, 50, file) != NULL)
	{
		int* input_p[MAX_INPUTS];
		prep_input_p(input_p);

		parse_input(input, input_p);
		handle_input(input_p);
		free_mem(input_p);
	}
	fclose(file);	

// ==============================================================	
/* Unfinished manual input, to be used later when done testing	   
	
	int running = 1;
	while(running)
	{
		printf("$ ");
		int* input_p[MAX_INPUTS];
		prep_input_p(input_p);

		parse_input(input, input_p);
		running = handle_input(input_p);
		free_mem(input_p);
		printf("\n");
	}

*/	

	return 0;
}

int handle_input(int** input_p)
{


	//if(input_p[0] == "read")
} 

int read(int va)
{
	printf("read %i\n", va);
	//Check if va in main_mem
	//If true: read the data
	//If false: page fault
	//	Then read the data
	return 1;
}

int write(int va, int n)
{
	printf("write %i %i\n", va, n);
	handle_pf(); // handles page fault
	return 1;
}

void handle_pf()
{
	//Find first available page
	//If none available
	//	Find victim page
	//	copy to disk if dirty
	//	copy disk page to victim page	
	//
	//	After found page, copy disk_mem page to main_mem page
	//	Then update page table (set Valid = 1)
}

int showmain(int page_num)
{
	printf("showdomain %i\n", page_num);
	return 1;
}

int showdisk(int page_num)
{
	printf("showdisk %i\n", page_num);
	return 1;
}

int showptable()
{
	printf("showptable\n");
	return 1;
}

int quit()
{
	return 0;
}

void prep_input_p(int** input_p)
{
	int i;
	for(i=0; i<MAX_INPUTS; i++)
	{
		input_p[i] = (int*) malloc(sizeof(int));
		*input_p[i] = -1;
	}
}

void free_mem(int** input_p)
{
	int i;
	for(i=0; i < MAX_INPUTS;i++)
		free(input_p[i]);
}

void parse_input(char* input, int** input_p)
{
	int i = 0;
	char str[50];
	strcpy(str, input);
	char* temp = strtok(str, " \n");
	while (temp != NULL)
	{
		*input_p[i] = (int) convert_string(temp);
		temp = strtok(NULL, " \n");
		i++;
	}
}

int convert_string(char* temp)
{
	if(!strcmp(temp, "read"))
		return 0;
	if(!strcmp(temp, "write"))
		return 1;
	if(!strcmp(temp, "showmain"))
		return 2;
	if(!strcmp(temp, "showdisk"))
		return 3;
	if(!strcmp(temp, "showptable"))
		return 4;
	if(!strcmp(temp, "quit"))
		return 5;
	return (int) atol(temp);
}

void initalize_mem(int* pt, pte_struct* pte, int* main_mem, int* disk_mem)
{
	fill_array(pt, PT_SIZE, 0);
	fill_array(main_mem, MAIN_MEM_SIZE, 0);
	fill_array(disk_mem, DISK_MEM_SIZE, -1);

	int i;
	for(i = 0; i < PT_SIZE; i++)
	{
		pte[i].entry = i;
		pte[i].valid = 0;
		pte[i].dirty = 0;
		pte[i].page = i;
	}
}

void fill_array(int* array, int array_size, int n)
{
	int i;
	for(i = 0; i < array_size; i++)
		array[i] = n;
}
