// Anthony Bilic 20514128

#include <stdio.h>

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
int handle_input(char** input_p);
void free_mem(char** input_p);

int main(int argc, const char * argv[])
{
	int pt[PT_SIZE];
	pte_struct pte[PT_SIZE];
	int main_mem[MAIN_MEM_SIZE];
	int disk_mem[DISK_MEM_SIZE];
	initalize_mem(pt, pte, main_mem, disk_mem);

	// This will go in while loop later, once we're done testing
	/*
	char* input = "read 4";
	char* input_p[MAX_INPUTS];
	handle_input(input_p, input);
	free_mem(input_p);
	*/	

	/*
	FILE *file;
 	char *filename = "t_simple.txt";
	file = fopen(filename,"r");
	
	fclose(file);	
	*/

	// ==============================================================	
	/* Can substitute manual input with input from test files for now
	int running = 1;
	while(running)
	{
		running = quit();
	}
	*/
	
	return 0;
}

void free_mem(char** input_p)
{
	int i;
	for(i=0; i < MAX_INPUTS;i++)
		free(input_p[i]);
}

void parse_input(char** input_p, char* input)
{
	/*char* temp = strtok(input, " \n");
	while (temp != NULL)
	{
		input_p[numbersIndex] = (int*) malloc(sizeof(int));
		*numbers[numbersIndex] = (int) strtol(tempStr, NULL, 10);
		temp = strtok(NULL, " \n");
		numbersIndex++;
	}
	*/
}

int handle_input(char** input_p)
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
