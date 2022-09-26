#include "usart.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define container_of(ptr, TYPE, member)   \
								(TYPE *)((char *)ptr - (unsigned int)&(((TYPE *)0)->member))

typedef struct Item {
	int val;
	void * pvOwner;
	struct Item *pxNext;
	struct Item *pxPre;
}ITEM, *PITEM;
typedef struct list {
		char *name;
		volatile uint32_t uxNumberOfItems;
		struct Item head;
}LIST, *PLIST;
typedef struct person {
		char *name;
		int age;
		struct Item iItem;
}PERSON, *PPERSON;


/* 链表初始化 */
void ListInit(PLIST plist, char *name)
{
	plist->name = name;
	plist->uxNumberOfItems = 0U;
	plist->head.pxNext = NULL;
	plist->head.pxPre = NULL;
}

/* 添加 Item 到链表  */
void AddItemToList(PLIST pList, PITEM new_item)
{
	PITEM pLastItem = &(pList->head);
	
	while(pLastItem->pxNext)
	{
		pLastItem = pLastItem->pxNext;
	}
	
	pLastItem->pxNext = new_item;
	new_item->pxNext = NULL;
	new_item->pvOwner = (container_of(new_item, PERSON, iItem))->name;
	(pList->uxNumberOfItems)++;
}
/* 在  item  后添加  new_item */
void AddItemAfter(PLIST pList, PITEM item, PITEM new_item)
{
	new_item->pxNext = item->pxNext;
	item->pxNext = new_item;
	(pList->uxNumberOfItems)++;
}

/* 删除 val 的所有节点 */
void DeleteItemFromList(PLIST pList, int val)
{
	PITEM pre = &pList->head;
	
	while(pre->pxNext != NULL)
	{
		if(pre->pxNext->val == val)
		{
			printf("Dele person name %s \r\n", (char *)pre->pxNext->pvOwner);
			pre->pxNext = pre->pxNext->pxNext;
			(pList->uxNumberOfItems)--;
		}
		else
			pre = pre->pxNext;
	}
}
/* 删除 Item */
void DelItemFromList(struct list *pList, PITEM Item)
{
	PITEM pre = &pList->head;
	
	while (pre != NULL && pre->pxNext != Item)
	{
		pre = pre->pxNext;
	}
	
	if (pre == NULL)
		return;
	else
		pre->pxNext = Item->pxNext;
	(pList->uxNumberOfItems)--;
}


/* 反转链表 */
void ReversalList(PLIST pList)
{
	PITEM pre = NULL;
	PITEM cur = pList->head.pxNext;
	PITEM temp = NULL;
	
	if(cur != NULL)
	{
		temp = cur->pxNext;
		while(temp != NULL)
		{
			cur->pxNext = pre;
			pre = cur;
			cur = temp;
			temp = temp->pxNext;
		}
		cur->pxNext = pre;
		pList->head.pxNext = cur;

	}
}

/* 打印中间项 */
void printfListCentre(PLIST pList)
{
	PITEM printfItem = pList->head.pxNext; 
	PPERSON pDex;
	int i;
	
	for(i = 1; i < (pList->uxNumberOfItems)/2 + 1; i++)
	{
		printfItem = printfItem->pxNext;
	}
	pDex = container_of(printfItem, PERSON, iItem);
	printf("pList  %s, age = %d, val = %d\r\n", (char *)printfItem->pvOwner, pDex->age, printfItem->val);

}

/* 打印倒数 k 项 */
void printfListK(PLIST pList, int k)
{
	PITEM printfItem = pList->head.pxNext; 
	PPERSON pDex;
	int i;
	if(k <= pList->uxNumberOfItems && k > 0)
	{
		for(i = 1; i < (pList->uxNumberOfItems) - k + 1; i++)
		{
			printfItem = printfItem->pxNext;
		}
		pDex = container_of(printfItem, PERSON, iItem);
		printf("pList  %s, age = %d, val = %d\r\n", (char *)printfItem->pvOwner, pDex->age, printfItem->val);

	}
	else
		return;
}


//排序
int SortList(PLIST pList)
{
	PITEM pre = &(pList->head);
	PITEM head = pre->pxNext;
	PITEM next;
	PITEM head_font = pre;
	PITEM next_font = NULL;
	PITEM temp;
	
	if(pre == NULL)
		return -1;
	
	while(head)
	{
		
		next_font = head;
		next = head->pxNext;
		while(next)
		{
			if(head->val > next->val)
			{
				DelItemFromList(pList, head);
				DelItemFromList(pList, next);
				
				AddItemAfter(pList, head_font, next);
				
				if(head == next_font)
					AddItemAfter(pList, next, head);
				else
					AddItemAfter(pList, next_font, head);
				
				temp = head;
				head = next;
				next = temp;				
			}
			next_font = next;
			next = next->pxNext;
		}
		head_font = head;
		head = head->pxNext;
	}
	
	return 0;
}


/* 删除排序链表的相同项 */
void DelSListSame(PLIST pList)
{
	PITEM p = &pList->head;
	PITEM cur;
	PITEM pre;
	
	if(p->pxNext != NULL && p->pxNext->pxNext != NULL)
	{
		cur = p->pxNext;
		pre = p->pxNext->pxNext;	
	}
	
	while(pre != NULL)
	{
		if(cur->val == pre->val)
		{
			while(pre && cur->val == pre->val)
			{
				pre = pre->pxNext;
				(pList->uxNumberOfItems)--;
			}
			p->pxNext = pre;
			if(pre != NULL)
				pre = pre->pxNext;
			cur = p->pxNext;
			(pList->uxNumberOfItems)--;
		}
		else
		{
			p = p->pxNext;
			cur = cur->pxNext;
			pre = pre->pxNext;
		}
		
		
	}

}




/* 合并两个有序链表 */
void MergeList(PLIST paList, PLIST pbList)
{
	PITEM p = &paList->head;
	PITEM pa;
	PITEM pb;
	if(paList->head.pxNext != NULL && pbList->head.pxNext != NULL)
	{
		pa = paList->head.pxNext;
		pb = pbList->head.pxNext;
	}
	while(pa != NULL && pb!= NULL)
	{
		if(pa->val <= pb->val)
		{
			p->pxNext = pa;
			pa = pa->pxNext;
		}
		else
		{
			p->pxNext = pb;
			pb = pb->pxNext;
		}
		p = p->pxNext;
	}
	if(pa == NULL)
		p->pxNext = pb;
	if(pb == NULL)
		p->pxNext = pa;
	
	paList->uxNumberOfItems += pbList->uxNumberOfItems;
}


/* 打印链表的所有项 */
void printfListAll(PLIST pList)
{
	
	PITEM pLastItem = pList->head.pxNext; 
	PPERSON pDex;
	int i = 0;
	while(pLastItem != NULL)
	{
		pDex = container_of(pLastItem, PERSON, iItem);
		printf("pList %d  %s, age = %d, val = %d\r\n", i++, (char *)pLastItem->pvOwner, pDex->age, pLastItem->val);
		pLastItem = pLastItem->pxNext;
	}
	printf("List has  %d  iItem\r\n", pList->uxNumberOfItems);
}

/* 链表分割 */
void printfListSeg(PLIST pList, int m)
{
	ITEM big;
	PITEM pbig = &big;
	ITEM smaller;
	PITEM psmaller = &smaller;
	PITEM p = pList->head.pxNext;
	PITEM next;
	
	big.pxNext = NULL;
	smaller.pxNext = NULL;
	
	while(p)
	{
		next = p->pxNext;
		if(p->val < m)
		{
			while(psmaller->pxNext)
			{
				psmaller = psmaller->pxNext;
			}
			psmaller->pxNext = p;
			p->pxNext = NULL;
			
		}
		if(p->val >= m)
		{
			while(pbig->pxNext)
			{
				pbig = pbig->pxNext;
			}
			pbig->pxNext = p;
			p->pxNext = NULL;
		}
		p = next;
	
	}
	psmaller->pxNext->pxNext = big.pxNext;
	
	pList->head.pxNext = smaller.pxNext;
	
	printfListAll(pList);
}

int main(int argc, char **arg)
{
		LIST a_list;
		LIST b_list;
		int i = 0;
		PERSON p[] = {
									{"p1", 20, {6, NULL, NULL, NULL}},
									{"p2", 12, {2, NULL, NULL, NULL}},
									{"p3", 39, {6, NULL, NULL, NULL}},
									{"p4", 56, {1, NULL, NULL, NULL}},				
									{"p5", 34, {0, NULL, NULL, NULL}},
									{"p6", 43, {5, NULL, NULL, NULL}},		
									{"p7", 12, {7, NULL, NULL, NULL}},
									{"p8", 10, {1,NULL, NULL, NULL}},		
									{"P9", 67, {1, NULL, NULL, NULL}},
									{"pa", 23, {0, NULL, NULL, NULL}}
		};
		
		PERSON pb[] = {
									{"pb", 20, {4, NULL, NULL, NULL}},
									{"pc", 12, {2, NULL, NULL, NULL}},
									{"pd", 39, {1, NULL, NULL, NULL}},
									{"pe", 56, {7, NULL, NULL, NULL}}				
		};
		
		
		
		volatile int DeleteItemInVal = 0;
    volatile int printfK = 1;
		volatile int List_seg = 6;
    HAL_Init();
    
    MX_USART1_UART_Init();
    
		/* 初始化链表 a_list */
		ListInit(&a_list, "A_Class");
		ListInit(&b_list, "B_Class");
		
		/* 添加元素至链表 */
		for(i = 0; i < sizeof(p)/sizeof(p[0]); i++)
		{
			AddItemToList(&a_list, &p[i].iItem);
		}
		for(i = 0; i < sizeof(pb)/sizeof(pb[0]); i++)
		{
			AddItemToList(&b_list, &pb[i].iItem);
		}
		
		printf("----------------------------------------------\r\n");
		/* 打印链表 */
		printf("All person : \r\n");
		printfListAll(&a_list);
		printf("----------------------------------------------\r\n");
		
		/* 删除 val 的所有节点 */
		printf("Dele person val = %d \r\n", DeleteItemInVal);
		DeleteItemFromList(&a_list, DeleteItemInVal);
		printfListAll(&a_list);
		printf("----------------------------------------------\r\n");

		/* 反转链表 */
		ReversalList(&a_list);
		printf("ReversalList\r\n");
		printfListAll(&a_list);
		printf("----------------------------------------------\r\n");		
		
				/* 打印链表的中间节点 */
		printf("printfListCentre\r\n");		
		printfListCentre(&a_list);
		printf("----------------------------------------------\r\n");		
		
			 /* 打印链表的倒数第K个节点 */
		printf("printfK   %d\r\n", printfK);		
		printfListK(&a_list, printfK);
		printf("----------------------------------------------\r\n");		
		
				/* 链表分割 */
		printf("List Segmentation %d\r\n", List_seg);		
		printfListSeg(&a_list, List_seg);
		printf("----------------------------------------------\r\n");	
		
			/* 链表排序 */
		printf("SortList a_list\r\n");		
		SortList(&a_list);
		printfListAll(&a_list);
		printf("----------------------------------------------\r\n");	
		
				/* 链表排序删除同val */
		printf("DelSListSame\r\n");		
		DelSListSame(&a_list);
		printfListAll(&a_list);
		printf("----------------------------------------------\r\n");	
		
				/* 合并两个有序链表 */
		printf("MergeList\r\n");	
		SortList(&b_list);
		printf("SortList b_list\r\n");	
		printfListAll(&b_list);
		MergeList(&a_list, &b_list);
		printfListAll(&a_list);
		printf("----------------------------------------------\r\n");	
		
		
		
		
		
    
    while(1)
    {
    }
}

void Error_Handler(void)
{
    printf("Error\r\n");
    while(1)
    {
    }
}
