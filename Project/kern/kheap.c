#include <inc/memlayout.h>
#include <kern/kheap.h>
#include <kern/memory_manager.h>

//2022: NOTE: All kernel heap allocations are multiples of PAGE_SIZE (4KB)

struct kheap_data
{
	uint32 virt_address1;
	uint32 virt_address2;
	int size;
};
struct kheap_data Arr[1024];
uint32 index = 0;
uint32 kernel_start = KERNEL_HEAP_START;
uint32 kernel_max =KERNEL_HEAP_MAX;
void* kmalloc(unsigned int size)
{
	//TODO: [PROJECT 2022 - [1] Kernel Heap] kmalloc()
	// Write your code here, remove the panic and write your code
	//kpanic_into_prompt("kmalloc() is not implemented yet...!!");


	//NOTE: Allocation using NEXTFIT strategy
	//NOTE: All kernel heap allocations are multiples of PAGE_SIZE (4KB)
	//refer to the project presentation and documentation for details

	    uint32 temp_size = ROUNDUP(size, PAGE_SIZE);
		Arr[index].size = temp_size;
		uint32 requested_size = temp_size;
		uint32 empty_count= 0;
		struct Frame_Info *frame_pointer1;
		int counter = 0;
		uint32 *pointer2;
		uint32 x = kernel_start;
		struct Frame_Info *frame;
		while (x <= kernel_max)
		{
			if (empty_count == requested_size)
			{
				uint32 y = Arr[index].virt_address1;
				while(y <= Arr[index].virt_address2)
				{
					int alloc = allocate_frame(&frame_pointer1);
					map_frame(ptr_page_directory, frame_pointer1, (void*)y, PERM_PRESENT | PERM_WRITEABLE);
					y += PAGE_SIZE;
				}
			}
			if (x == kernel_max)
			{
				x = KERNEL_HEAP_START;
			}
			if (x == kernel_start)
			{
				counter += 1;
			}
			if (counter == 2)
			{
				if (empty_count != requested_size)
				{
					return NULL;
				}
			}
			frame= get_frame_info(ptr_page_directory, (void *)x, &pointer2);
			if (empty_count  == requested_size)
			{
				break;
			}
			if (x == KERNEL_HEAP_MAX)
			{
				x = KERNEL_HEAP_START;
			}
			if (frame != NULL)
			{
				empty_count  = 0;
			}
			else
			{
				if (empty_count  == 0)
				{
					Arr[index].virt_address1 = x;
				}
				Arr[index].virt_address2 = x;
				empty_count  += PAGE_SIZE;
				temp_size -= PAGE_SIZE;
			}
			x+= PAGE_SIZE;
		}
		kernel_start = Arr[index].virt_address2 + PAGE_SIZE;
		int var = index;
		index++;
		return (void *)(Arr[var].virt_address1);
	//TODO: [PROJECT 2022 - BONUS1] Implement a Kernel allocation strategy
	// Instead of the Next allocation/deallocation, implement
	// BEST FIT strategy
	// use "isKHeapPlacementStrategyBESTFIT() ..."
	// and "isKHeapPlacementStrategyNEXTFIT() ..."
	//functions to check the current strategy
	//change this "return" according to your answer

}

void kfree(void* virtual_address)
{
	//TODO: [PROJECT 2022 - [2] Kernel Heap] kfree()
	// Write your code here, remove the panic and write your code
	//panic("kfree() is not implemented yet...!!");
	uint32 address1;
			uint32 address2;
		    uint32 temp = 0;
			int a=0;
			while(a<index)
			{
				uint32 b=Arr[a].virt_address1;
				while ( b<=Arr[a].virt_address2)
				{
					if ((uint32)virtual_address == b)
					{
						temp = 1;
						address2 = Arr[a].virt_address2;
						address1 = (uint32)virtual_address;
						uint32 c=address1;
						while(c<=address2)
						{
							unmap_frame(ptr_page_directory, (void *)c);
							uint32 *pagetable=NULL;
							get_page_table(ptr_page_directory,(void *)c,&pagetable);
							pagetable[PTX(c)]=0;
							c+=PAGE_SIZE;
						}
						break;
					}

					b+=PAGE_SIZE;
				}
				a++;
			}


	//you need to get the size of the given allocation using its address
	//refer to the project presentation and documentation for details

}

unsigned int kheap_virtual_address(unsigned int physical_address)
{
	//TODO: [PROJECT 2022 - [3] Kernel Heap] kheap_virtual_address()
	// Write your code here, remove the panic and write your code
	//panic("kheap_virtual_address() is not implemented yet...!!");
	    struct Frame_Info *raneen = to_frame_info(physical_address);
		uint32 xx = to_frame_number(raneen);
		int m = 0;
		while(m < index)
		{
			uint32 n = Arr[m].virt_address1;
			while ( n <= Arr[m].virt_address2)
			{
				{
					uint32 *menna = NULL;
					get_page_table(ptr_page_directory, (void *)n, &menna);
					uint32 shift = menna[PTX(n)] >> 12;
					uint32 var= menna[PTX(n)] & PERM_PRESENT;
					if (var != 0 && shift == xx)
					   {
							return n;
					   }
				}
				n += PAGE_SIZE;
			}
			m++;
		}
	//return the virtual address corresponding to given physical_address
	//refer to the project presentation and documentation for details

	//change this "return" according to your answer
     return 0;
}

unsigned int kheap_physical_address(unsigned int virtual_address)
{
	//TODO: [PROJECT 2022 - [4] Kernel Heap] kheap_physical_address()
	// Write your code here, remove the panic and write your code
	//panic("kheap_physical_address() is not implemented yet...!!");
	    uint32 physicaladdress=0;
		struct Frame_Info *frame=NULL;
		uint32 *pointer1=NULL;
		frame=get_frame_info(ptr_page_directory,(void *)virtual_address,&pointer1);
		if(frame!=NULL){
			physicaladdress=to_physical_address(frame);
		}
	    return physicaladdress;

	//return the physical address corresponding to given virtual_address
	//refer to the project presentation and documentation for details

	//change this "return" according to your answer
	//return 0;
}

