#pragma once
#define INVALID 0x0
#define bool char
#define true 1
#define false 0
#define size_t unsigned long
/******************************************/
#define list_for_each(head,pos) (for(pos = head->next;pos!=head;pos=pos->next))
#define offset_of(type,member) (size_t)&(((type *)0)->member)
#define container_of(ptr,type,member) (type *)((char *)(ptr)- offset_of(type,member))
#define list_entry(ptr,type,member) container_of(ptr,type,member)
#define list_for_each_entry(head,pos,type,member) for(pos = list_entry((head)->next,type,member);\
&((pos)->member)!=(head);\
pos = list_entry((pos)->member.next,type,member))
typedef struct _list
{
	struct _list * pre;
	struct _list * next;
} ListHead, *PListHead;

void init_list_head(PList);
void list_add(PListHead pre, PListHead entry);
void list_delete(PListHead current);
void move_to_head(PListHead head, PListHead current);
void move_to_tail(PListHead head, PListHead current);
bool isEmpty(PListHead head);
bool list_is_last(PListHead head, PListHead entry);
void list_add_to_head(PListHead head, PListHead entry);
void list_add_to_tail(PListHead head, PListHead entry);

