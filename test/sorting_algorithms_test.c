#include <time.h>	/* time, clock */
#include <stdlib.h> /* rand */
#include <stdio.h>	/* printf */
#include "sorting_algorithms.h"

#define TRUE (1)
#define FALSE (0)
#define FAIL (-1)
#define ARR_SIZE (3000)
#define MS (1000)

typedef void (*func)(int *, size_t);

static void TestQuadSorts();
static void AssignRand(int *array, size_t size);
static int IsSorted(int *arr, size_t size);
static double ClockMeasure(func function, int *arr, size_t size);
static double CalcAverage(double num, int times);
static double CheckAverageTime(func function, int *array, size_t size, int times);
static void TestQuick();
static int IntCompare(const void *num1, const void *num2);
static void TestCountingSort();
static void TestRadixSort();
static void TestBinarySearch();
static void TestMergeSort();
static void TestQuickSort();
static void TestHeapSort();

#ifndef NDEBUG
void PrintArray(int *arr, size_t size);
#endif /* NDEBUG */

int main(void)
{
	TestQuadSorts();
	TestCountingSort();
	TestRadixSort();
	TestBinarySearch();
	TestMergeSort();
	TestQuick();
	TestQuickSort();
	TestHeapSort();
	return 0;
}

static void TestQuadSorts()
{
	int array[ARR_SIZE] = {0};
	size_t size = sizeof(array) / sizeof(int);
	func bubble_sort = BubbleSort;
	func insertion_sort = InsertionSort;
	func selection_sort = SelectionSort;
	double average = 0;

	srand(time(NULL));

	AssignRand(array, size);
	BubbleSort(array, size);

	printf("\033[1;31m");
	if (IsSorted(array, size))
	{
		printf("BubbleSort Working\n");
	}

	AssignRand(array, size);
	SelectionSort(array, size);
	if (IsSorted(array, size))
	{
		printf("SelectionSort Working\n");
	}

	AssignRand(array, size);
	InsertionSort(array, size);
	if (IsSorted(array, size))
	{
		printf("InsertionSort Working\n");
	}

	printf("\033[1;35m");
	average = CheckAverageTime(bubble_sort, array, size, 100);
	printf("BubbleSort time: %fms\n", average);

	printf("\033[1;34m");
	average = CheckAverageTime(selection_sort, array, size, 100);
	printf("SelectionSort time: %fms\n", average);

	printf("\033[1;33m");
	average = CheckAverageTime(insertion_sort, array, size, 100);
	printf("InsertionSort time: %fms\n", average);
}

static void TestQuick()
{
	int i = 0;
	int arr[ARR_SIZE] = {0};
	clock_t start_t, end_t, delta_t = 0;
	double sum = 0, average = 0;

	while (i++ < 100)
	{
		AssignRand(arr, ARR_SIZE);
		start_t = clock();
		qsort(arr, ARR_SIZE, sizeof(int), IntCompare);
		end_t = clock();
		delta_t = (double)(end_t - start_t) / (CLOCKS_PER_SEC / MS);
		sum += delta_t;
	}

	average = CalcAverage(sum, 100);
	printf("QuickSort time: %fms\n", average);
}

static void TestCountingSort()
{
	int i = 0;
	int arr[ARR_SIZE] = {0};
	clock_t start_t, end_t, delta_t = 0;
	double sum = 0, average = 0;

	while (i++ < 100)
	{
		AssignRand(arr, ARR_SIZE);
		start_t = clock();
		CountingSort(arr, ARR_SIZE);
		end_t = clock();
		delta_t = (double)(end_t - start_t) / (CLOCKS_PER_SEC / MS);
		sum += delta_t;
	}
	printf("\033[1;38m");
	if (IsSorted(arr, ARR_SIZE))
	{
		printf("CountingSort Working\n");
	}
	average = CalcAverage(sum, 100);
	printf("CountingSort time: %fms\n", average);
}

static void TestRadixSort()
{
	int i = 0;
	int arr[ARR_SIZE] = {0};
	clock_t start_t, end_t, delta_t = 0;
	double sum = 0, average = 0;

	while (i++ < 100)
	{
		AssignRand(arr, ARR_SIZE);
		start_t = clock();
		RadixSort(arr, ARR_SIZE);
		end_t = clock();
		delta_t = (double)(end_t - start_t) / (CLOCKS_PER_SEC / MS);
		sum += delta_t;
	}
	printf("\033[1;32m");
	if (IsSorted(arr, ARR_SIZE))
	{
		printf("RadixSort Working\n");
	}
	average = CalcAverage(sum, 100);
	printf("RadixSort time: %fms\n", average);
}

static double CheckAverageTime(func function, int *array, size_t size, int times)
{
	double average = 0, sum = 0;
	size_t i;

	for (i = 0; i < 100; ++i)
	{
		AssignRand(array, size);
		sum += ClockMeasure(function, array, size);
	}
	average = CalcAverage(sum, times);
	return (average);
}

static double CalcAverage(double num, int times)
{
	return ((double)num / times);
}

void AssignRand(int *array, size_t size)
{
	size_t i;

	for (i = 0; i < size; ++i)
	{
		array[i] = rand() % 1000;
	}
}

static int IsSorted(int *arr, size_t size)
{
	size_t i = 0;
	int is_sorted = TRUE;

	for (i = 0; i < size - 1; ++i)
	{
		if (arr[i] > arr[i + 1])
		{
			is_sorted = FALSE;
			break;
		}
	}

	return (is_sorted);
}

static double ClockMeasure(func function, int *arr, size_t size)
{
	clock_t start_t, end_t;
	double delta_t;

	start_t = clock();
	function(arr, size);
	end_t = clock();

	delta_t = (double)(end_t - start_t) / (CLOCKS_PER_SEC / MS);
	return (delta_t);
}

static int IntCompare(const void *num1, const void *num2)
{
	return (*(int *)num1 > *(int *)num2);
}

static void TestBinarySearch()
{
	int arr[100] = {0};
	size_t size = sizeof(arr) / sizeof(arr[0]);
	int to_find = 0;
	int not_found = 0;
	size_t i = 0;

	for (; i < 100; ++i)
	{
		arr[i] = i;
	}

	to_find = arr[30];
	not_found = 7222222;

	printf("\033[1;31m");
	if (30 == BinarySearchIterative(arr, size, to_find) &&
		FAIL == BinarySearchIterative(arr, size, not_found))
	{
		printf("BinarySearchIterative Working\n");
	}
	else
	{
		printf("BinarySearchIterative Not Working!");
	}

	if (30 == BinarySearchRecursion(arr, size, to_find) &&
		FAIL == BinarySearchRecursion(arr, size, not_found))
	{
		printf("BinarySearchRecursion Working\n");
	}
	else
	{
		printf("BinarySearchRecursion Not Working!");
	}
}

static void TestMergeSort()
{
	int i = 0;
	int arr[ARR_SIZE] = {0};
	clock_t start_t, end_t, delta_t = 0;
	double sum = 0, average = 0;

	while (i++ < 100)
	{
		AssignRand(arr, ARR_SIZE);
		start_t = clock();
		MergeSort(arr, ARR_SIZE);
		end_t = clock();
		delta_t = (double)(end_t - start_t) / (CLOCKS_PER_SEC / MS);
		sum += delta_t;
	}

	average = CalcAverage(sum, 100);
	printf("MergeSort time: %fms\n", average);
}

static void TestQuickSort()
{
	int i = 0;
	int arr[ARR_SIZE] = {0};
	clock_t start_t, end_t, delta_t = 0;
	double sum = 0, average = 0;

	while (i++ < 100)
	{
		AssignRand(arr, ARR_SIZE);
		start_t = clock();
		QuickSort(arr, ARR_SIZE, sizeof(int), IntCompare);
		end_t = clock();
		delta_t = (double)(end_t - start_t) / (CLOCKS_PER_SEC / MS);
		sum += delta_t;
	}

	average = CalcAverage(sum, 100);
	printf("\033[1;36m");
	printf("My QuickSort time: %fms\n", average);
}

static void TestHeapSort()
{
	int i = 0;
	int arr[ARR_SIZE] = {0};
	clock_t start_t, end_t, delta_t = 0;
	double sum = 0, average = 0;

	while (i++ < 100)
	{
		AssignRand(arr, ARR_SIZE);
		start_t = clock();
		HeapSort(arr, ARR_SIZE);
		end_t = clock();
		delta_t = (double)(end_t - start_t) / (CLOCKS_PER_SEC / MS);
		sum += delta_t;
	}

	average = CalcAverage(sum, 100);
	printf("\033[1;37m");
	printf("My HeapSort time: %fms\n", average);
}

#ifndef NDEBUG
void PrintArray(int *arr, size_t size)
{
	size_t i = 0;

	for (; i < size; ++i)
	{
		printf("%d ", arr[i]);
	}
	printf("\n");
}
#endif /* NDEBUG */