#include <inc/lib.h>

// malloc()
//	This function use NEXT FIT strategy to allocate space in heap
//  with the given size and return void pointer to the start of the allocated space

//	To do this, we need to switch to the kernel, allocate the required space
//	in Page File then switch back to the user again.
//
//	We can use sys_allocateMem(uint32 virtual_address, uint32 size); which
//		switches to the kernel mode, calls allocateMem(struct Env* e, uint32 virtual_address, uint32 size) in
//		"memory_manager.c", then switch back to the user mode here
//	the allocateMem function is empty, make sure to implement it.

//==================================================================================//
//============================ REQUIRED FUNCTIONS ==================================//
//==================================================================================//
uint32 checker[(USER_HEAP_MAX - USER_HEAP_START) / PAGE_SIZE] = { 0 };
struct information {
	uint32 size2;
	uint32 va;
};
struct information arrx[(USER_HEAP_MAX - USER_HEAP_START) / PAGE_SIZE];
int allocated = 0;
uint32 last_allocated = USER_HEAP_START;
void* malloc(uint32 size) {

	size = ROUNDUP(size, PAGE_SIZE);
	arrx[allocated].size2 = size;
	int size1 = 0;
	int rounds = 0;
	int ptr = 0;
	int alloc;
	uint32 z = 0;
	uint32 b = 0;
	//TODO: [PROJECT 2022 - [9] User Heap malloc()] [User Side]
	// Write your code here, remove the panic and write your code
	//panic("malloc() is not implemented yet...!!");
	//if(sys_isUHeapPlacementStrategyNEXTFIT()){
	//uint32 maximumsize = 131072;

	for (uint32 i = last_allocated; i <= USER_HEAP_MAX; i += PAGE_SIZE)
	{

		if (i == USER_HEAP_MAX)
		{
			i = USER_HEAP_START;
		}
		if(i == last_allocated)
		{
			rounds += 1;
		}
		if (rounds == 2)
		{
			ptr = 0;
			break;
		}
		if (checker[(i - USER_HEAP_START) / PAGE_SIZE] == 0) {
			if (size1 == 0) {
				arrx[allocated].va = i;
			}
			size1 += PAGE_SIZE;
		} else {
			size1 = 0;
		}
		if (size1 == size) {
			ptr = 1;
			break;
		}

	}
	if (ptr != 0)
	{
		for (uint32 i = arrx[allocated].va; i < (size + arrx[allocated].va);i += PAGE_SIZE)
				{
					checker[(i - USER_HEAP_START) / PAGE_SIZE] = 1;
				}
		sys_allocateMem((uint32) arrx[allocated].va, size);
	} else
	{
		arrx[allocated].va = 0;
		return NULL;
	}
	last_allocated = arrx[allocated].va + size;
	alloc = allocated;
	allocated++;
	return (void *) arrx[alloc].va;
	// Steps:
	//	1) Implement NEXT FIT strategy to search the heap for suitable space
	//		to the required allocation size (space should be on 4 KB BOUNDARY)
	//	2) if no suitable space found, return NULL
	//	 Else,
	//	3) Call sys_allocateMem to invoke the Kernel for allocation
	// 	4) Return pointer containing the virtual address of allocated space,
	//

	//This function should find the space of the required range
	// *** ON 4KB BOUNDARY ******** //

	//Use sys_isUHeapPlacementStrategyNEXTFIT() and
	//sys_isUHeapPlacementStrategyBESTFIT() for the bonus
	//to check the current strategy

	//return NULL;

	// Steps:
	//	1) Implement NEXT FIT strategy to search the heap for suitable space
	//		to the required allocation size (space should be on 4 KB BOUNDARY)
	//	2) if no suitable space found, return NULL
	//	 Else,
	//	3) Call sys_allocateMem to invoke the Kernel for allocation
	// 	4) Return pointer containing the virtual address of allocated space,
	//

	//This function should find the space of the required range
	// ******** ON 4KB BOUNDARY ******************* //

	//Use sys_isUHeapPlacementStrategyNEXTFIT() and
	//sys_isUHeapPlacementStrategyBESTFIT() for the bonus
	//to check the current strategy

	//return NULL;
}

void* smalloc(char *sharedVarName, uint32 size, uint8 isWritable) {
	panic("smalloc() is not required ..!!");
	return NULL;
}

void* sget(int32 ownerEnvID, char *sharedVarName) {
	panic("sget() is not required ..!!");
	return 0;
}

// free():
//	This function frees the allocation of the given virtual_address
//	To do this, we need to switch to the kernel, free the pages AND "EMPTY" PAGE TABLES
//	from page file and main memory then switch back to the user again.
//
//	We can use sys_freeMem(uint32 virtual_address, uint32 size); which
//		switches to the kernel mode, calls freeMem(struct Env* e, uint32 virtual_address, uint32 size) in
//		"memory_manager.c", then switch back to the user mode here
//	the freeMem function is empty, make sure to implement it.

void free(void* virtual_address) {
	//TODO: [PROJECT 2022 - [11] User Heap free()] [User Side]
	// Write your code here, remove the panic and write your code
	//panic("free() is not implemented yet...!!");
	int index;
	for (int i = 0; i < allocated; i++) {
		if (arrx[i].va == (uint32) virtual_address) {
			index = i;
		}
	}
	for (uint32 i = arrx[index].va;
			i < (uint32) virtual_address + arrx[index].size2; i += PAGE_SIZE)
			{
		checker[(i - USER_HEAP_START) / PAGE_SIZE] = 0;
	}
	arrx[index].va = 0;
	arrx[index].size2 = ROUNDUP(arrx[index].size2, PAGE_SIZE);
	sys_freeMem((uint32) virtual_address, arrx[index].size2);

	//you shold get the size of the given allocation using its address
	//you need to call sys_freeMem()
	//refer to the project presentation and documentation for details
}

void sfree(void* virtual_address) {
	panic("sfree() is not requried ..!!");
}

//===============
// [2] realloc():
//===============

//	Attempts to resize the allocated space at "virtual_address" to "new_size" bytes,
//	possibly moving it in the heap.
//	If successful, returns the new virtual_address, in which case the old virtual_address must no longer be accessed.
//	On failure, returns a null pointer, and the old virtual_address remains valid.

//	A call with virtual_address = null is equivalent to malloc().
//	A call with new_size = zero is equivalent to free().

//  Hint: you may need to use the sys_moveMem(uint32 src_virtual_address, uint32 dst_virtual_address, uint32 size)
//		which switches to the kernel mode, calls moveMem(struct Env* e, uint32 src_virtual_address, uint32 dst_virtual_address, uint32 size)
//		in "memory_manager.c", then switch back to the user mode here
//	the moveMem function is empty, make sure to implement it.

void *realloc(void *virtual_address, uint32 new_size) {
	//TODO: [PROJECT 2022 - BONUS3] User Heap Realloc [User Side]
	// Write your code here, remove the panic and write your code
	panic("realloc() is not implemented yet...!!");

	return NULL;
}
