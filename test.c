#include"buddy.h"
#include<stdio.h>
ulong_t size = (1 << 12) * 16 * 1024;
upchar_t memory = NULL;
static struct page pages_map[16*1024] = {0};
void show(struct zone *zone)
{
	struct page * pos = NULL;
	printf("\n***********************************\n");
	for (int i = 0;i < 11; i++)
	{
		printf("order=[%d]\n",i);
		list_for_each_entry(&(zone->free_area[i].free_list), pos, struct page, list)
		{
			printf("%x, ", PAGE_INDEX(pos));
		}
		printf("\n");
	}
	printf("\n***********************************\n");
}
int main(int argc,char *argv[])
{
	pages = pages_map;
	memory = init_memory(size);
	struct zone zone;
	init_zone(&zone,size >> 12);
	init_free_area(&zone, size >> 12);
	struct page * pos = NULL;
	show(&zone);
	ulong_t addr = kmalloc(&zone, (1 << 6) - 16);
	show(&zone);
	ulong_t addr1 = kmalloc(&zone, (1 << 5) - 2);
	ulong_t addr4 = kmalloc(&zone, (1 << 5) -1);
	ulong_t addr5 = kmalloc(&zone, (1 << 5) -1);
	ulong_t addr6 = kmalloc(&zone, (1 << 10) -1);
	ulong_t addr7 = kmalloc(&zone, (1 << 9));
	ulong_t addr8 = kmalloc(&zone, 5);
	printf("%d:%d\n",addr,addr1);
	show(&zone);
	printf("\n***************free********************\n");
	kfree(&zone,addr,6);
	ulong_t addr3 = kmalloc(&zone,1);
	kfree(&zone, addr1, 5);
	printf("\n***************free********************\n" );
	show(&zone);
	kfree(&zone, addr8, 3);
	kfree(&zone, addr3, 0);
	kfree(&zone, addr5, 5);
	kfree(&zone, addr6, 10);
	kfree(&zone, addr7, 9);
	show(&zone);
	kfree(&zone, addr4, 5);
	show(&zone);
}
