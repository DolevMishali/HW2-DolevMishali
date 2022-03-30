#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Worker
{
	long int id;
	char* name;
	long int salary;
	
	int select;
	union
	{
		char hebYear[6];
		long int year;
	}Year;

}Worker;

typedef struct WorkerList
{
	Worker* data;
	struct WorkerList* next;
} WorkerList;

void initList(WorkerList* list)
{
	list->next = NULL;
}

WorkerList* addWorker(WorkerList* head, Worker* w)
{
	WorkerList* tmphead=NULL;
	tmphead = (WorkerList*)calloc(10, sizeof(WorkerList));// initialize all data to 0
	initList(tmphead);
	tmphead->data = w;

	if (!head->data)
	{
		head->data = w;
		return head;
	}

	if(head->data->salary < w->salary)// put the worker first in the list.
	{
		tmphead->next = head;
		return tmphead;
	}

	if(head->data->salary >= w->salary && head->next ==NULL)
	{
		head->next = tmphead;
		return head;
	}

	WorkerList* curr=head,*ptr=NULL;
	for(;curr->next!=NULL;curr=curr->next)
	{
		if( (curr->data->salary > w->salary) && (curr->next->data->salary < w->salary) )
		{
			ptr = curr->next;
			curr->next = tmphead;
			tmphead->next = ptr;
			return head;
		}
	}
	curr->next = tmphead;
	return head;
}

WorkerList* createWorker(WorkerList* list)
{
	Worker* newWorker = (Worker*)calloc(1, sizeof(Worker));// initialize all data to 0
	newWorker->name = (char*)calloc(4, sizeof(char));
	printf("Enter the info about the Worker - id:\n");
	scanf_s("%d",&newWorker->id);

	fseek(stdin, 0, SEEK_END);

	printf("Name:\n");
	fseek(stdin,0,SEEK_END);
	gets(newWorker->name);

	printf("Salary:\n");
	scanf_s("%d", &newWorker->salary);

	printf("Enter the type of the year - Hebrew year(press 1) foreign year(press 0) and then write the year:\n");
	scanf_s("%d", &newWorker->select);
	switch (newWorker->select)
	{
	case 1://Hebrew
		{
		fseek(stdin, 0, SEEK_END);
		gets(newWorker->Year.hebYear);
			break;
		}
	case 0://Foreign
		{
			scanf_s("%d", &newWorker->Year.year);
			break;
		}
	}
	return addWorker(list, newWorker);
}

void PrintWorker(Worker* w)
{
	printf("\n\nThe info about the Worker:\n");
	printf("ID:\t\t%d\n", w->id);
	printf("Name:\t\t%s\n", w->name);
	printf("Salary:\t\t%d\n", w->salary);

	if (w->select == 1)
		printf("Year:\t\t%s\n\n\n", w->Year.hebYear);
	else
		printf("Year:\t\t%d\n\n\n", w->Year.year);
}

void helprint(WorkerList* list)//run on the list and send the nodes to the printworker func.
{
	if (list == NULL)
		printf("There is no list to print, back to menu.\n\n");
	WorkerList* tmp = list;
	while (tmp)
	{
		PrintWorker(tmp->data);
		tmp = tmp->next;
	}
}

//int index(WorkerList * head, long unsigned id)//original function.
//{
//	WorkerList* tmp = head;
//	int index = 0;
//	while (tmp)
//	{
//		index++;
//		if (tmp->data->id == id)
//		return index;
//		tmp = tmp->next;
//	}
//	return index = -1;
//}

int index(WorkerList* head, long unsigned id)//recursive function.
{
		WorkerList* tmp = head;
	static int idx = 0;
	if (tmp)
	{
		idx++;
		if (tmp->data->id == id)
			return idx;
		return index(tmp->next , id);
	}
	return idx = -1;
}

WorkerList* deleteWorstWorker(WorkerList * head)
{
	if (head == NULL)
	{
		printf("There is no workers to delete, back to the menu.");
		return head;
	}

	WorkerList* tmp = head;
	Worker* freeOne = (Worker*)calloc(1, sizeof(Worker));// initialize all data to 0
	freeOne->name = (char*)calloc(4, sizeof(char));

	while(tmp)
	{
		if (tmp->next->next == NULL)
		{
			freeOne = tmp->next;
			break;
		}

		tmp = tmp->next;
	}
	free(freeOne->name);
	free(freeOne);
	tmp->next = NULL;
	return head;

} //Run on the list and find the low salary, then delete the Worker

void update_Worker(WorkerList* head, float percent)
{
	WorkerList* tmp=head;
	percent = percent / 100 + 1;
	while (tmp)
	{
		tmp->data->salary = (tmp->data->salary * percent);
		tmp = tmp->next;
	}
}

WorkerList* reverse(WorkerList* head)
{
	if (head->next == NULL)
	{
		printf("No reverse required, the list here is a 1 Worker list.\n\n");
		return head;
	}
	else
	{
		WorkerList* prev = head;
		WorkerList* curr = head->next;
		head = head->next;
		prev->next = NULL; // Make first Worker as last Worker.
		while (head != NULL)
		{
			head = head->next;
			curr->next = prev;
			prev = curr;
			curr = head;
		}
		head = prev; //convert the last Worker as head
		printf("Successfully reversed list.\n\n");
		return head;
	}
}

void freeWorkers(WorkerList* head)
{
	if (head == NULL)
	{
		printf("There is no Workers to free.\n\n");
		return;
	}
	while (head != NULL)
	{
		WorkerList* tmp = head;
		head = head->next;
		free(tmp->data->name);
		free(tmp);
	}
}

void showMenu(WorkerList* list)
{
	int select=0;

	while (select != 9)
	{
		printf("Please enter your choice:\n\n(1) - add new worker.\t(2) - the index of the worker.\n\n(3) - Delete the worst worker from the list.\t(4) - To update the salary of the workers.\n\n(5) - To reverse the list.\t(6) - free the list.\n\n(8) - To print the list.\n\n(9) - exit the program.\n");
		scanf_s("%d", &select);
		switch (select)
		{
		case 1:
		{
			list = createWorker(list);
			break;
		}
		case 2:
		{
			int id,loc = 0;
			printf("Enter the ID of the worker:\n");
			scanf_s("%d", &id);
			loc = index(list,id);
			printf("location of the worker in the list: %d \n",loc);
			break;
		}
		case 3:
		{
			list = deleteWorstWorker(list);
			break;
		}
		case 4:
		{
			float percent;
			printf("Enter the number of percents:\n");
			scanf_s("%f", &percent);
			while(percent < 0)
			{
				printf("Wrong number - try again.\n");
				printf("Enter the number of percents:\n");
				scanf_s("%f", &percent);
			}
			update_Worker(list,percent);
			break;
		}
		case 5:
		{
			list = reverse(list);
			break;
		}
		case 6:
		{
			freeWorkers(list);
			printf("successfully operation.\n\n");
			list = NULL;
			break;
		}
		case 8:
		{
			helprint(list);
		}
		case 9:
			break;
		}
	}
}

void main()
{
	WorkerList* list;
	list = (WorkerList*)calloc(1, sizeof(WorkerList));// initialize all data to 0
	showMenu(list);
}