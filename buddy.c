#include"buddy.h"
#include <stdio.h>
#include <malloc.h>
#include<string.h>
uchar_t *init_memory(ulong_t size)
{
	uchar_t *p = NULL;
	if ((p = (uchar_t *)malloc(size)) == NULL)
	{
		printf("�����ڴ�ʧ�ܣ�");
		return NULL;
	}
	else {
		return p;
	}
}

void init_zone(struct zone* p,ulong_t size)
{
	for (int i = 0; i < MAX_ORDER; i++)
	{
		ulong_t len = size >> (i + 1 + 3);
		p->map[i] = (uchar_t *)malloc(len);
		memset(p->map[i], 0, len);
	}

	p->total_free = size;
	p->total = size;
}
//����ʼ�������order��ʼ��
void init_free_area(struct zone* p, ulong_t size)
{
	for (int i = 0; i<MAX_ORDER;i++)
	{
		init_list_head(&(p->free_area[i].free_list));
		p->free_area[i].nr_free = 0;
	}
	uint_t current_order = 0;
	for (uint_t i = 1; i <= size && current_order< MAX_ORDER-1; i = i << 1, current_order++);
	uint_t num = size >> current_order;
	struct free_area *current =&(p->free_area[current_order]);
	current->nr_free = num;
	for (int i = 0; i < num; i++)
	{
		struct page* page = (struct page*)malloc(sizeof(struct page));
		page->order = current_order;
		page->vm_addr = i * (1<< current_order);
		list_add_to_tail(&(current->free_list), &(page->list));
	}
}
void * kmalloc(struct zone* p, ulong_t size)
{
	if (p->total_free < size)
		return NULL;
	uint_t order = 0;
	//�ҵ���С��order
	for ( uint_t i= 1; i < size && order < MAX_ORDER-1; i = i << 1,order++);
	printf("%d,%d\n", size,order);
	uint_t current_order = 0;
	//�ҵ���С�Ŀ��õ�order
	for (current_order = order; current_order < MAX_ORDER; current_order++)
	{
		if (p->free_area[current_order].nr_free)
			break;
	}
	printf("%d\n", current_order);
	//�����ڴ������ͷ
	PListHead head = NULL;
	struct page *page = NULL;
	ulong_t base = 0,index=0;
	struct page * tmp = NULL;

		head  = &(p->free_area[current_order].free_list);
		page = list_entry(head->next, struct page, list);
		//�Ӹ�������ɾ���ýڵ�
		list_delete(head->next);
		//������Ӧ�Ļ��λ
		set_bit(p, page->vm_addr>>(current_order+1), current_order);
		//������Ӧ��������õĽڵ���
		p->free_area[current_order].nr_free--;
		struct page * pos = NULL;
		printf("********************\n");

		printf("%x,%x\,%d,%dn", page->list.pre, page->list.next,page->order,page->vm_addr);
		//ֱ�����ѵ�����С�ڵ�Ϊֹ
		while (current_order != order)
		{
		printf("***********current_order:%d*********\n",current_order);
		//���ѽڵ�
		base = page->vm_addr;
		page->order = current_order - 1;
		index = base + (1 << (current_order - 1));
		tmp = (struct page *)malloc(sizeof(struct page));
		//ԭ�ڵ�����һ���ڵ�ĵ�ַ
		tmp->vm_addr = index;
		tmp->order = current_order - 1;
		current_order = current_order - 1;
		//��ȡ��ǰorder��Ӧ������ͷ
		head = &(p->free_area[current_order].free_list);
		//�������ڵ��е�С��һ�����뵽��������
		list_add_to_head(head, &(page->list));
		//������Ӧorder�еĻ��λ
		set_bit(p, page->vm_addr>>(current_order+1), current_order);
		p->free_area[current_order].nr_free++;
		//����һ���ڵ�����Ϊ��ǰ�ڵ�
		page = tmp;
		printf("base:%d\tindex:%d\n", base, index);
		}
		//���ܿ��ÿռ������Ӧ��ֵ
		p->total_free -= size;
		printf("********************\n");
	return tmp->vm_addr;
}
void set_bit(struct zone *p, ulong_t index, uint_t order)
{
	//����Ӧorder��Ӧ�Ļ��λ�������indexΪ����λ����
	p->map[order][index>>3] = (p->map[order][index >> 3] ^ (1 << (index % 8)));
	printf("set_bit:index:%d,order:%d,index:%d,bit:%d\n", index, order, index >> 3, index % 8);
}
//��ȡ��Ӧ��bitλ
uint_t get_bit(struct zone *p, ulong_t index, uint_t order)
{
	 return (p->map[order][index >> 3] & (1 << (index % 8)))?1:0;
	
}
void kfree(struct zone* p, ulong_t vm_addr,uint_t order)
{
	ulong_t buddy = 0;
	uint_t current_order = order;
	struct page * page = NULL;
	struct page * buddy_page = NULL;
	page = (struct page *)malloc(sizeof(struct page));
	page->order = current_order;
	page->vm_addr = vm_addr & ((~1L) << (current_order - 1));
	//���һ���ĵ�ַ
	buddy = find_buddy(page->vm_addr, current_order);
	PListHead head = NULL;
	p->total_free += 1 << page->order;
	printf("free current_order:%d,vm_addr:%d,buddy:%d\n", current_order, vm_addr, buddy);
	//���л������ʱ
	while(has_buddy(p, buddy>>(current_order+1), current_order)&& current_order < MAX_ORDER-1)
	{
		set_bit(p, buddy>>(current_order+1), current_order);
	//	page = find_page(p, vm_addr, order);
		//���һ����Ӧ�Ľڵ�
		buddy_page = find_page(p, buddy, current_order);
		//��������ժ���ýڵ�
		list_delete(&(buddy_page->list));
		p->free_area[current_order].nr_free--;
		//���ͷŽڵ���ýڵ�ϲ�
		current_order++;
		page->order = current_order;
		//�½ڵ��ַ
		page->vm_addr = page->vm_addr & ((~1L) << (current_order-1));
		//�����½ڵ�Ļ����ַ
		buddy = find_buddy(page->vm_addr, current_order);
		printf("free current_order:%d,vm_addr:%d,buddy:%d\n", current_order, vm_addr,buddy);
	}
	//��δ�ҵ����л��飬��ֱ�Ӳ��뵽��������
	head = &(p->free_area[current_order].free_list);
	list_add(head, &(page->list));
	p->free_area[current_order].nr_free++;
	set_bit(p, (page->vm_addr) >> (current_order + 1), current_order);
	printf("free current_order:%d,vm_addr:%d,buddy:%d\n", current_order, vm_addr, buddy);
}
//�ж��Ƿ���ڿ��еĻ���
bool has_buddy(struct zone * p, ulong_t buddy, uint_t order)
{
	return get_bit(p, buddy, order);
}
//�õ�����ĵ�ַ
ulong_t find_buddy(ulong_t vm_addr, uint_t order)
{
	ulong_t buddy = vm_addr ^ (1 << order );
	return buddy;
}
//���������ҵ��õ�ַ����Ӧ�Ľڵ�
struct page* find_page(struct zone * p,ulong_t buddy,uint_t order)
{
	struct page * page = NULL;
	list_for_each_entry(&(p->free_area[order].free_list), page, struct page, list)
	{
		if (page->vm_addr == buddy)
			return page;
	}
	return NULL;
}
