// CS 421 HW3: Multithreaded Sorting Application
// Guan Ce Looa

#include <iostream>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

typedef struct
{
	int head;
	int tail;
	int *sublist;
} parameters;

const int SIZE = 10;
int list[SIZE] = { 7, 12, 19, 3, 18, 4, 2, 6, 15, 8 };
int sortedList[SIZE] = { 0 };
int sublistHead[5] = { 0 };
int sublistTail[5] = { 0 };

void sort(void *p);
void merge();

int main()
{
	cout << "Unsorted list: ";
	for (int i = 0; i < SIZE; i++)
	{
		cout << list[i] << " ";
	}
	cout << endl;

	// Create thread for first half of list
	parameters *dataHead = (parameters *)malloc(sizeof(parameters));
	dataHead->head = 0;
	dataHead->tail = (SIZE / 2) - 1;
	dataHead->sublist = sublistHead;
	thread sortHead(sort, dataHead);

	// Create thread for second half of list
	parameters *dataTail = (parameters *)malloc(sizeof(parameters));
	dataTail->head = (SIZE / 2);
	dataTail->tail = SIZE - 1;
	dataTail->sublist = sublistTail;
	thread sortTail(sort, dataTail);

	// Wait on threads to complete tasks
	sortHead.join();
	sortTail.join();

	// Create thread to merge two halves into one
	thread mergesort(merge);

	// Wait on thread to complete task
	mergesort.join();

	// Print array
	cout << "Sorted list: ";
	for (int i = 0; i < 10; i++)
	{
		cout << sortedList[i] << " ";
	}
	cout << endl;

	cin.get();
	return 0;
}

void sort(void *p)
{
	parameters *data = (parameters *)p;
	// Copy values from unsorted list to sublist
	// Sublist pointer points to either sublistHead (first half) or sublistTail (second half) depending on parameter passed when creating the thread
	for (int i = data->head; i < data->tail + 1; i++)
	{
		data->sublist[i % 5] = list[i];
	}
	// Simple insertion sort on sublists
	for (int i = 0; i < SIZE / 2; i++)
	{
		int t = data->sublist[i];
		int j = i;
		while (j > 0 && data->sublist[j - 1] > t)
		{
			data->sublist[j] = data->sublist[j - 1];
			j = j - 1;
		}
		data->sublist[j] = t;
	}
}

void merge()
{
	int indexHead = 0;
	int indexTail = 0;
	int index = 0;
	while (indexHead < SIZE / 2 && indexTail < SIZE / 2)
	{
		if (sublistHead[indexHead] < sublistTail[indexTail])
		{
			sortedList[index++] = sublistHead[indexHead++];
		}
		else
		{
			sortedList[index++] = sublistTail[indexTail++];
		}
	}
	while (indexHead < SIZE / 2)
	{
		sortedList[index++] = sublistHead[indexHead++];
	}
	while (indexTail < SIZE / 2)
	{
		sortedList[index++] = sublistTail[indexTail++];
	}
}