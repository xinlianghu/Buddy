#include"list.h"

void init_list_head(PListHead listHead)
{
	listHead->pre = listHead;
	listHead->next = listHead;
}

void list_add(PListHead pre, PListHead entry)
{
	entry->pre = pre;
	entry->next = pre->next;
	entry->next->pre = entry;
	pre->next = entry;
}

void list_delete(PListHead current)
{
	current->pre->next = current->next;
	current->next->pre = current->pre;
	current->pre = INVALID;
	current->next = INVALID;
}
void move_to_head(PListHead head, PListHead current)
{
	list_delete(current);
	list_add(head, current);
}
void move_to_tail(PListHead head, PListHead current)
{
	list_delete(current);
	list_add(head->pre, current);
}
bool isEmpty(PListHead head)
{
	return head->next == head;
}
bool list_is_last(PListHead head, PListHead entry)
{
	return entry->next == head;
}
void list_add_to_head(PListHead head, PListHead entry)
{
	list_add(head, entry);
}
void list_add_to_tail(PListHead head, PListHead entry)
{
	list_add(head->pre, entry);
}
