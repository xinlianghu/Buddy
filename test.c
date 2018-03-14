#include"slab.h"
#include<stdio.h>
ulong_t size = (1 << 12) * 16;
upchar_t memory = NULL;
void main()
{
	memory = init_memory(size);
	struct zone zone;
	init_zone(&zone,size);
	init_free_area(&zone, size);
	struct page * pos = NULL;
	list_for_each_entry(&(zone.free_area[10].free_list), pos, struct page, list)
	{
		printf("%x,%d,%d\n", pos,pos->order, pos->vm_addr);
	}
	ulong_t addr = kmalloc(&zone, (1 << 6) - 16);
	for (int i = 0; i < 11; i++)
	{
		printf("***************%d********************\n", i);
		list_for_each_entry(&(zone.free_area[i].free_list), pos, struct page, list)
		{
			printf("%x,%d,%d\t\n", pos, pos->order, pos->vm_addr);
		}
		printf("***********************************\n");
	}
	ulong_t addr1 = kmalloc(&zone, (1 << 5) - 2);
	printf("%d:%d\n",addr,addr1);
	for (int i = 0;i < 11; i++)
	{
		printf("***************%d********************\n",i);
		list_for_each_entry(&(zone.free_area[i].free_list), pos, struct page, list)
		{
			printf("%x,%d,%d\t\n", pos, pos->order, pos->vm_addr);
		}
		printf("***********************************\n");
	}
	printf("\n***************free********************\n");
	kfree(&zone,addr,6);
	kfree(&zone, addr1, 5);
	printf("\n***************free********************\n" );
	for (int i = 0; i < 11; i++)
	{
		printf("***************%d********************\n", i);
		list_for_each_entry(&(zone.free_area[i].free_list), pos, struct page, list)
		{
			printf("%x,%d,%d\t\n", pos, pos->order, pos->vm_addr);
		}
		printf("***********************************\n");
	}
	__asm{ mov eax,10 };
}