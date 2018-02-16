// Anthony Bilic 20514128

#include <stdio.h>

#define PT_SIZE 8
#define MAIN_MEM_SIZE 8
#define DISK_MEM_SIZE 16
#define PAGE_SIZE 2

typedef struct pte_struct {
	int entry;
	int valid;
	int dirty;
	int page;
} pte_struct;

void initalize_mem(int* pt, pte_struct* pte, int* main_mem, int* disk_mem);
void fill_array(int* array, int array_size, int n);

void read(int va);
void write(int va, int n);
void showmain(int page_num);
void showdisk(int page_num);
void showptable();
int quit();

void handle_pf();

int main(int argc, const char * argv[])
{
	int pt[PT_SIZE];
	pte_struct pte[PT_SIZE];
	int main_mem[MAIN_MEM_SIZE];
	int disk_mem[DISK_MEM_SIZE];
	initalize_mem(pt, pte, main_mem, disk_mem);

	int running = 1;
	while(running)
	{
		running = quit();
	}
}

void read(int va)
{

}

void write(int va, int n)
{
	handle_pf(); // handles page fault
}

void handle_pf()
{

}

void showmain(int page_num)
{

}

void showdisk(int page_num)
{

}

void showptable()
{

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
