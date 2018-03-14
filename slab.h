#pragma once
#include"list.h"
#define LENGTH 1024*1024*128
#define MAX_ORDER 11
#define NULL 0
/*********************************************************/
typedef unsigned int uint_t;
typedef unsigned char uchar_t;
typedef unsigned long ulong_t;
typedef unsigned char * upchar_t;
/**************************************************/
struct free_area{
	ListHead free_list;
	ulong_t nr_free;
};
struct zone{
	struct free_area free_area[MAX_ORDER];
	ulong_t total_free;
	ulong_t total;
	uchar_t *map[MAX_ORDER];
};
struct page {
	ListHead list;
	uint_t order;
	ulong_t vm_addr;
};
/***********************************************************/
void init_free_area(struct zone* p, ulong_t size);
void init_zone(struct zone* p, ulong_t size);
uchar_t *init_memory(ulong_t size);
void * kmalloc(struct zone* p, ulong_t size);
void set_bit(struct zone *p, ulong_t page_index, uint_t order);
void reset_bit(struct zone *p, ulong_t page_index, uint_t order);
uint_t get_bit(struct zone *p, ulong_t page_index, uint_t order);
void kfree(struct zone* p, ulong_t vm_addr, uint_t order);
bool has_buddy(struct zone * p, ulong_t buddy, uint_t order);
ulong_t find_buddy(ulong_t vm_addr, uint_t order);
struct page* find_page(struct zone * p, ulong_t buddy, uint_t order);

