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
	int valid;
	int dirty;
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

int find_victim();
int find_free_page();
int handle_pf(int va);
int va_to_pa(int va);

int handle_input(int** input_p);
void prep_input_p(int** input_p);
void parse_input(char* input, int** input_p);
int convert_string(char* temp);
void update_order(int main_page);

void free_mem(int** input_p);

pte_struct pte[PT_SIZE];
int main_mem[MAIN_MEM_SIZE]; 
int disk_mem[DISK_MEM_SIZE];
int use_order[MAIN_PAGE_COUNT];

void print_all();

// 4 different kinds of addresses
// va = disk_addr = disk_mem[i]
// disk_page = disk_mem[i/2]
// main_page = main_mem[i/2]
// pa = main_addr = main_mem[i]

int main(int argc, const char * argv[])
{
	initalize_mem();
/*
	// Testing input from file
	FILE* file;
	char* filename = "test.txt";
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
	
	print_all();
	*/
	// ==============================================================	
	///* Unfinished manual input, to be used later when done testing	   
	   char input[50];
	   int running = 1;
	   while(running)
	   {
	   printf("$ ");
	   int* input_p[MAX_INPUTS];
	   prep_input_p(input_p);
	   fgets(input, 20, stdin);
	   parse_input(input, input_p);
	   running = handle_input(input_p);
	   free_mem(input_p);
	   printf("\n");
	   }

	 //*/	

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

int read(int va)
{
	int main_page;
	if (pte[va/2].valid == 1)
		main_page = pte[va/2].page_number;
	else
		main_page = handle_pf(va);

	int main_addr = main_page*2;
	if(va%2==1)
		main_addr++;	

	printf("Address[%i] is %i\n", va, main_mem[main_addr]); 
	update_order(main_page);
	return 1;
}

int write(int va, int n)
{
	printf("Write %i %i\n", va, n);
	// ^TO BE DELETED
	int free_page;
	if(pte[va/2].valid == 1)
		free_page = pte[va/2].page_number;
	else
		free_page = find_free_page();

	if(free_page == -1)
		free_page = handle_pf(va);
	
	int free_addr = free_page*2;
	if(va%2==1)
	{
		if(pte[va/2].valid != 1)
			main_mem[free_addr] = disk_mem[va/2];
		main_mem[free_addr+1] = n;
	}
	else
	{
		if(pte[va/2].valid != 1)
			main_mem[free_addr+1] = disk_mem[va/2+1];
		main_mem[free_addr] = n;
	}
	disk_mem[va] = n;
	
	pte[va/2].valid = 1;
	pte[va/2].dirty = 1;
	pte[va/2].page_number = free_page;

	update_order(va/2);
	return 1;
}

// Returns -1 if no free memory, otherwise returns free main_mem page (Not tested)
int find_free_page()
{
	int i;
	int largest_page = -1;
	for(i=0; i<PT_SIZE; i++)
		if(pte[i].valid == 1 && pte[i].page_number > largest_page)
			largest_page = pte[i].page_number;

	for(i=0; i<MAIN_MEM_SIZE; i=i+2)
		if(main_mem[i] == -1 && main_mem[i+1] == -1 && i>largest_page*2)
			return i/2;
	return -1;
}

// Returns freed page number in main_mem
int handle_pf(int va)
{
	printf("HANDLE_PF\n");
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
	}

	disk_page = va/2;
	int va_data[2] = {disk_mem[disk_page*2], disk_mem[disk_page*2 + 1]};
	
	main_mem[main_page*2] = va_data[0];
	main_mem[main_page*2 + 1] = va_data[1];

	pte[disk_page].valid = 1;
	pte[disk_page].page_number = main_page;

	return main_page;
}

void update_order(int main_page)
{
	use_order[main_page] = 0;
	int i;	
	for(i=0; i<MAIN_PAGE_COUNT; i++)
		if(i != main_page)
			use_order[i] = use_order[i] + 1;
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

	printf("Error: find_victim\n");
	return -1;
}

int showmain(int page_num)
{
	int i = page_num*2;
	printf("Address  Contents\n");
	printf("%d       %d\n", i, main_mem[i]);
	printf("%d       %d\n", i+1, main_mem[i+1]);
	return 1;
}

int showdisk(int page_num)
{
	int i = page_num*2;
	printf("Address  Contents\n");
	printf("%d       %d\n", i, disk_mem[i]);
	printf("%d       %d\n", i+1, disk_mem[i+1]);
	return 1;
}

int showptable()
{
	printf("VPageNum Valid Dirty PN\n");

	int i;
	for(i=0; i<PT_SIZE; i++)
	{
		printf("%i        ", pte[i].entry);
		printf("%i     ", pte[i].valid);
		printf("%i     ", pte[i].dirty);
		printf("%i\n", pte[i].page_number);	
	}
	return 1;
}

int quit()
{
	printf("Quitting\n");
	return 0;
}

// ======================= FINISHED ========================
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
	fill_array(main_mem, MAIN_MEM_SIZE, -1);
	fill_array(disk_mem, DISK_MEM_SIZE, -1);

	int i;
	for(i = 0; i < PT_SIZE; i++)
	{
		pte[i].entry = i;
		pte[i].valid = 0;
		pte[i].dirty = 0;
		pte[i].page_number = i;
	}

	for(i=0; i<MAIN_PAGE_COUNT; i++)
		use_order[i] = -1;
}

void fill_array(int* array, int array_size, int n)
{
	int i;
	for(i = 0; i < array_size; i++)
		array[i] = n;
}

void print_all()
{
	int i;
	printf("DISK_MEM\n");
	for(i=0; i<DISK_MEM_SIZE; i++)
		printf("%i ", disk_mem[i]);
	printf("\n");
	
	printf("MAIN_MEM\n");
	for(i=0; i<MAIN_MEM_SIZE; i++)
		printf("%i ", main_mem[i]);
	printf("\n");

	showptable();
}
