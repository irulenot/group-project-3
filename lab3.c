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
	int entry; //page in disk_mem (not index!)
	int valid; //1 if in main_mem, 0 is on disk_mem
	int dirty; //1 if newer data in main_mem than disk_mem
	int page_number; //page in main_mem (not index!)
} pte_struct;

void initalize_mem();
void fill_array(int* array, int array_size, int n);

int read(int va);
int write(int va, int n);
int showmain(int page_num);
int showdisk(int page_num);
int showptable();
int quit();

int find_victim();
int find_free_page();
int handle_pf(int va);
int va_to_pa(int va);

int handle_input(int** input_p);
void prep_input_p(int** input_p);
void parse_input(char* input, int** input_p);
int convert_string(char* temp);

void free_mem(int** input_p);

pte_struct pte[PT_SIZE];
int main_mem[MAIN_MEM_SIZE]; 
int disk_mem[DISK_MEM_SIZE];
int use_order[MAIN_PAGE_COUNT]; //New global var, keeps track of use order

// 4 different kinds of addresses
// va = disk_addr = disk_mem[i/2 + 1(if odd number)]
// disk_page = disk_mem[i]
// main_page = main_mem[i]
// pa = main_addr = main_mem[i/2 + 1(if odd number)]

int main(int argc, const char * argv[])
{
	initalize_mem();

	// Testing input from file
	FILE *file;
	char *filename = "t_simple.txt";
	file = fopen(filename,"r");

	// TESTING
	
	// TESTING

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

// I didn't touch anything here
int read(int va) //virtual address are converted to physical addresses | phys addresses are indexes in main_mem array | disk addresses are indexes in disk array
{
	printf("read %i\n", va);
	/*
	int pa = va_to_pa(va);
	printf("Virtual address: %d\n", va);
	printf("Physical address: %d\n", pa);

	int virtualPageNum = va >> 1; //used to index the page table
	
	if (pte[virtualPageNum].valid == 1) { //page in main memory
		printf("Address: \n"); // print----------------------------
		printf("Content: %d\n", main_mem[pa]);
		return main_mem[pa]; //read the data
	}
	else {//page in disk memory
	 //page fault
	 //handle_pf();
	}
	*/

	//If true: read the data
	//If false: page fault
	//	Then read the data

	return 1;
}

// Moves va page to main_mem and writes n to it (IN PROGRESS)
int write(int va, int n)
{
	printf("write %i %i", va, n);
	int free_page = find_free_page();
	if(free_page != -1)
		free_page = handle_pf(va);
	int free_addr = free_page*2;
	
	/* IN PROGRESS
	if(va%2=1)
	{
		main_mem[free_page*2] = disk_mem[va];
		main_mem[free_page*2+1] = disk_mem[va+1];
	}
	else
	{
		main_mem[free_page*2] = disk_mem[va
	}
	pte[va].valid = 1;
	pte[va].dirty = 1;
	pte[va].page_num = va/2;
	*/

	return 1;
}

// Returns -1 if no free memory, otherwise returns free main_mem page (Not tested)
int find_free_page()
{
	int page_nums[4] = {-1,-1,-1,-1};
	int index = 0;
	int i;
	for(i=0; i<PT_SIZE; i++)
	{
		if(pte[i].valid  == 1)
		{
			page_nums[index] = pte[i].page_number;
			index++;
		}	
	}

	if(index < 3)
	{	
		int sum = 6; // factorial of PT_SIZE-1
		for(i=0; i<index; i++)
			index -= page_nums[i];
		return sum;
	}		
	return -1;
}

// Don't think we actually need this, however if you're going to use it in your code go for it
int va_to_pa(int va) //only valid values for pa are 0-7
{
	/*
	int vpage = va/2;
	pte_struct vpageEntry = pte[vpage];
	vpageEntry.valid = 1;//DELETE LATER FOR DEBUGGING PURPOSES---------------------
	if (vpageEntry.valid == 0) {
		//handle_pf(vpageEntry);
		return -1; //temporary
	}
	
	int ppage = vpageEntry.page;
	int pa = (ppage*2 + (va % 2));
	*/
	return 1; // pa
}

// Returns freed page number in main_mem (Not tested)
int handle_pf(int va)
{
	int victim_pte = -1;
	int disk_page = -1;
	int main_page = -1;

	int free_page = find_free_page();
	if(free_page != -1)
	{
		main_page = free_page;
	}
	else
	{
		victim_pte = find_victim();
		main_page = pte[victim_pte].page_number;
		disk_page = pte[victim_pte].entry;

		disk_mem[disk_page*2] = main_mem[main_page];
		disk_mem[disk_page*2+1] = main_mem[main_page+1];
		pte[victim_pte].valid = 0;
		pte[victim_pte].dirty = 0;
		pte[victim_pte].page_number = -1;
	}

	disk_page = va/2;
	int va_data[2] = {disk_mem[disk_page*2], disk_mem[disk_page*2 + 1]};
	main_mem[main_page*2] = va_data[0];
	main_mem[main_page*2 + 1] = va_data[1];

	pte[disk_page].valid = 1;
	pte[disk_page].page_number = main_page;

	return main_page;
}

// Returns index of pte of to be freed (refer to that pte's page_num for the main_mem page) (Not tested)
int find_victim()
{
	int lowest_page = 0;
	int i;
	for(i=1; i<MAIN_PAGE_COUNT; i++)
		if(use_order[i] > use_order[lowest_page])
			lowest_page = i;

	for(i=0; i<PT_SIZE; i++)
		if(pte[i].page_number == lowest_page)
			return i; 

	printf("Error: find_victim (Not necessarily function itself)\n");
	return -1;
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

	for(i=0; i<MAIN_MEM_SIZE; i++)
		use_order[i] = -1;
}

void fill_array(int* array, int array_size, int n)
{
	int i;
	for(i = 0; i < array_size; i++)
		array[i] = n;
}
