// Anthony Bilic 20514128
// Minh Vu 28602763

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PT_SIZE 8
#define MAIN_MEM_SIZE 8
#define DISK_MEM_SIZE 16
#define MAX_INPUTS 3
#define MAIN_PAGE_COUNT 4

typedef struct {
	int entry;
	int valid; //1 if in main_mem, 0 is on disk_mem
	int dirty; //1 if newer data in main_mem than disk_mem
	int page_number;
} pte_struct;

void initalize_mem();
void fill_array(int* array, int array_size, int n);

int read(int va);
int write(int va, int n);
int showmain(int page_num);
int showdisk(int page_num);
int showptable();
int quit();

void handle_pf(int va);
int va_to_pa(int va);

int handle_input(int** input_p);
void prep_input_p(int** input_p);
void parse_input(char* input, int** input_p);

void free_mem(int** input_p);

pte_struct pte[PT_SIZE];
int main_mem[MAIN_MEM_SIZE]; 
int disk_mem[DISK_MEM_SIZE];
int page_use_order[MAIN_PAGE_COUNT]; //New global var, keeps track of use order

int main(int argc, const char * argv[])
{
	initalize_mem();

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
	if(*input_p[0] == 0)
		return read(*input_p[1]);
	if(*input_p[0] == 1)
		return write(*input_p[1], *input_p[2]);
	if(*input_p[0] == 2)
		return showmain(*input_p[1]);
	if(*input_p[0] == 3)
		return showdisk(*input_p[1]);
	if(*input_p[0] == 4)
		return showptable();
	return quit();
} 

int read(int va) //virtual address are converted to physical addresses | phys addresses are indexes in main_mem array | disk addresses are indexes in disk array
{
	
	int pa = va_to_pa(va);
	printf("Virtual address: %i\n", va);
	printf("Physical address: %i\n", va);
	/*
	int virtualPageNum = va >> 1;
	int offset = va % 2;
	int pageNum;
	if (var%2==0) {
		pageNum = var/2;
	}
	else {
		pageNum = (var-1)/2;
	}
	int physAddr = pageNum*2 + offset;
	
	if (pte[pageNum].valid == 1) { //page in main memory
		return main_mem[physAddr]; //read the data
	}
	else {//page in disk memory
	 //page fault
	 //handle_pf();
	}
	
	
	
	//If true: read the data
	//If false: page fault
	//	Then read the data
	*/
	return 1;
}

// In progress (feel free to work on it)
int write(int va, int n)
{
	/*
	int main_page = find_free_page();
	if(main_page == -1);
		handle_pf(va);
	*/
	return 1;
}

// Returns -1 if no free memory, otherwise returns free  main_mem page (Not tested, feel free to test)
int find_free_page()
{
	int page_nums[4] = {-1,-1,-1,-1};
	int index = 0;
	int va;
	for(va=0; i<PT_SIZE; i++)
	{
		if(pte[i].valid  == 1)
		{
			page_nums[index] = pte[va.page_number];
			index++;
		}	
	}

	if(index < 3)
	{	
		int sum = 6; // hardcoded: 3+2+1 = 6
		int i;
		for(i=0; i<index; i++)
			index -= page_nums[i];
		return sum;
	}		
	return -1;
}

int va_to_pa(int va)
{
	int vpage = va/2;
	pte_struct vpageEntry = pte[vpage];
	if (vpageEntry.valid == 0) {
		handle_pf(va);
	}
	int ppage = vpageEntry.page;
	return (ppage*2 + (va % 2));
}

// In progress (Feel free to work on it)
int handle_pf(int va)
{
	/*
	//Find first available page //if data = -1?
	int main_page = find_free_page();
	if(main_page != -1)
		return main_page; // not handled yet

	printf("%i", find_victim());
	
	//If none available
	//	Find victim page
	//	copy to disk if dirty
	//	copy disk page to victim page	
	//
	//	After found page, copy disk_mem page to main_mem page
	//	Then update page table (set Valid = 1)
	*/
}

// Completed but not functional as va_use_order is not being updated currently
int find_victim()
{
	int lowest_page = 0;
	int i;
	for(i=1; i<MAIN_PAGE_COUNT;i++)
		if(va_use_order[i] > va_use_order[lowest_page])
			lowest_page = i;
	return lowest_page;
}

int showmain(int page_num)
{
	printf("showmain %i\n", page_num);
	/*
	int index1 = page_num*2;
	int index2 = page_num*2 +1;
	printf("Address   Contents\n");
	printf("%d   %d\n", index1, main_mem[index1]); //main_mem undeclared?
	printf("%d   %d\n", index2, main_mem[index2]); //main_mem undeclared?
	*/
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

// ===================== FINISHED ====================

int quit()
{
	printf("quit\n");
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

void initalize_mem()
{
	fill_array(main_mem, MAIN_MEM_SIZE, 0);
	fill_array(disk_mem, DISK_MEM_SIZE, -1);

	int i;
	for(i = 0; i < PT_SIZE; i++)
	{
		pte[i].entry = i;
		pte[i].valid = 0;
		pte[i].dirty = 0;
		pte[i].page_number = i;
	}

	for(i=0; i<MAIN_SLOT_SIZE; i++)
		page_use_order = -1;
}

void fill_array(int* array, int array_size, int n)
{
	int i;
	for(i = 0; i < array_size; i++)
		array[i] = n;
}
