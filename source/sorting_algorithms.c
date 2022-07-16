#include <assert.h> /* assert */
#include <stdlib.h> /* malloc */
#include <string.h> /* memcpy */

#ifndef NDEBUG
#include <stdio.h>
#endif /* NDEBUG */

#include "sorting_algorithms.h" /* Declerations */

/*============================ MACROS & ENUMS ===========================*/

#define FAIL (-1)
#define SUCCESS (0)
#define NUM_OF_DIGITS (10)
#define GET_CHILD(x, y) (2 * x + y)

typedef enum children
{
	LEFT = 1,
	RIGHT = 2
} children_t;

/*============================ STATIC HELPERS ===========================*/
static void Swap(int *a, int *b);
static int FindMin(int *arr, size_t size);
static int FindMax(int *arr, size_t size);

/*============================ LINEAR SORTING ===========================*/
static int *CreateArray(size_t size);
static void CountEvents(int *arr, size_t size, int *event_array, int min);
static void SumArrayIndexes(int *event_array, size_t size);
static void RotateOnceRight(int *arr, size_t size);
static void FillArray(int *ret, int *arr, int *event_array, size_t size, int min);
static void CopyArray(int *dest, int *src, size_t size);
static int DigitCountingSort(int *arr, size_t size, int digit);
/*========================== DIVIDE & CONQUER ===========================*/
static long BinarySearchRec(int *arr, long start, long mid, long end, int find);
static int MergeSortRec(int *arr, long start, long end);
static int Merge(int *arr, long start, long mid, long end);
static void CopyDataToArray(int *dest, int *src, long size, long offset);
static void CopyRemainingData(int *dest, long dest_idx, int *src, long src_size, long runner);
static void QuickSortRec(void *base, size_t element_size, int start, int end, cmp_func cmp);
static int Partition(void *base, size_t element_size, int start, int end, cmp_func cmp);
static void *GetElement(void *base, size_t element_size, int place);
static void SwapElements(size_t element_size, void *a, void *b);

/*============================ HEAP SORT =================================*/
static void BuildMaxHeapFromArray(int *arr, long size);
void SwapInts(int *a, int *b);
static void HeapifyDown(int *arr, long parent, long size);
static long GetBiggerChild(int *arr, long parent, long size);
static void SortArray(int *arr, long last);

void BubbleSort(int *array, size_t size)
{
	size_t i = 0, j = 0;

	assert(NULL != array);

	for (i = 0; i < size - 1; ++i)
	{
		for (j = 0; j < size - i - 1; ++j)
		{
			if (array[j] > array[j + 1])
			{
				Swap(&array[j], &array[j + 1]);
			}
		}
	}
}

void SelectionSort(int *array, size_t size)
{
	size_t i = 0, j = 0;
	int min_idx = 0;

	for (i = 0; i < size - 1; ++i)
	{
		min_idx = i;
		for (j = i; j < size; ++j)
		{
			if (array[j] < array[min_idx])
			{
				min_idx = j;
			}
		}
		Swap(&array[i], &array[min_idx]);
	}
}

void InsertionSort(int *array, size_t size)
{
	size_t i = 0;
	long j = 0;
	int checked = 0;

	for (i = 1; i < size; ++i)
	{
		checked = array[i];
		j = i - 1;
		while (j >= 0 && checked < array[j])
		{
			array[j + 1] = array[j];
			--j;
		}
		array[j + 1] = checked;
	}
}

int CountingSort(int *arr, size_t size)
{
	int min = arr[0], max = arr[0];
	int *event_array = NULL, *ret = NULL;
	size_t event_arr_size = 0;

	min = FindMin(arr, size);
	max = FindMax(arr, size);
	event_arr_size = max - min + 1;

	event_array = CreateArray(max - min + 1);
	if (NULL == event_array)
	{
		return (FAIL);
	}
	CountEvents(arr, size, event_array, min);
	SumArrayIndexes(event_array, event_arr_size);
	RotateOnceRight(event_array, event_arr_size);
	ret = CreateArray(size);
	if (NULL == ret)
	{
		return (FAIL);
	}
	FillArray(ret, arr, event_array, size, min);
	CopyArray(arr, ret, size);

	free(event_array);
	free(ret);

	return (SUCCESS);
}

int RadixSort(int *arr, size_t size)
{
	int max = arr[0];
	int digit = 1, status = SUCCESS;

	max = FindMax(arr, size);

	for (digit = 1; (max / digit) > 0 && (SUCCESS == status); digit *= 10)
	{
		status = DigitCountingSort(arr, size, digit);
	}
	return 0;
}

static int DigitCountingSort(int *arr, size_t size, int digit)
{
	size_t i = 0;
	int bucket[NUM_OF_DIGITS] = {0};
	int *output = NULL;

	assert(NULL != arr);

	for (i = 0; i < size; ++i)
	{
		++bucket[(arr[i] / digit) % 10];
	}

	SumArrayIndexes(bucket, NUM_OF_DIGITS);

	RotateOnceRight(bucket, NUM_OF_DIGITS);

	output = (int *)calloc(size, sizeof(int));
	if (NULL == output)
	{
		return (FAIL);
	}

	for (i = 0; i < size; ++i)
	{
		output[bucket[(arr[i] / digit) % 10]] = arr[i];
		++bucket[(arr[i] / digit) % 10];
	}

	CopyArray(arr, output, size);
	free(output);

	return (SUCCESS);
}

static int FindMin(int *arr, size_t size)
{
	int min = arr[0];
	size_t i = 0;
	for (i = 0; i < size; ++i)
	{
		if (arr[i] < min)
		{
			min = arr[i];
		}
	}
	return (min);
}

static int FindMax(int *arr, size_t size)
{
	int max = arr[0];
	size_t i = 0;
	for (i = 0; i < size; ++i)
	{
		if (arr[i] > max)
		{
			max = arr[i];
		}
	}
	return (max);
}

static int *CreateArray(size_t size)
{
	int *ret = (int *)calloc(size, sizeof(int));
	if (NULL == ret)
	{
		return (NULL);
	}
	return (ret);
}

static void CountEvents(int *arr, size_t size, int *event_array, int min)
{
	size_t i = 0;
	for (i = 0; i < size; ++i)
	{
		++event_array[arr[i] - min];
	}
}

static void SumArrayIndexes(int *event_array, size_t size)
{
	size_t i;
	for (i = 1; i < size; ++i)
	{
		event_array[i] += event_array[i - 1];
	}
}

static void RotateOnceRight(int *arr, size_t size)
{
	size_t i = size - 1;
	for (; i > 0; --i)
	{
		arr[i] = arr[i - 1];
	}
	arr[0] = 0;
}

static void FillArray(int *ret, int *arr, int *event_array, size_t size, int min)
{
	size_t i = 0;
	for (; i < size; ++i)
	{
		ret[event_array[arr[i] - min]] = arr[i];
		++event_array[arr[i] - min];
	}
}

static void CopyArray(int *dest, int *src, size_t size)
{
	size_t i = 0;
	for (; i < size; ++i)
	{
		dest[i] = src[i];
	}
}

static void Swap(int *a, int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

long BinarySearchIterative(int *arr, size_t size, int find)
{
	long start = 0, end = size - 1, mid = start;
	assert(NULL != arr);
	while (arr[mid] != find && start <= end)
	{
		mid = (start + end) / 2;
		if (find < arr[mid])
		{
			end = mid - 1;
		}
		else if (find > arr[mid])
		{
			start = mid + 1;
		}
	}
	return ((arr[mid] == find) ? mid : FAIL);
}

long BinarySearchRecursion(int *arr, size_t size, int find)
{
	long start = 0, end = size - 1, mid = (start + end) / 2;
	assert(NULL != arr);
	return (BinarySearchRec(arr, start, mid, end, find));
}

static long BinarySearchRec(int *arr, long start, long mid, long end, int find)
{
	if (find == arr[mid] || start == end)
	{
		return ((find == arr[mid]) ? mid : FAIL);
	}
	else if (find > arr[mid])
	{
		start = mid + 1;
		mid = BinarySearchRec(arr, start, (start + end) / 2, end, find);
	}
	else
	{
		end = mid - 1;
		mid = BinarySearchRec(arr, start, (start + end) / 2, end, find);
	}

	return (mid);
}

int MergeSort(int *arr, size_t size)
{
	return (MergeSortRec(arr, 0, size - 1));
}

static int MergeSortRec(int *arr, long start, long end)
{
	if (start < end)
	{
		long mid = (start + end) / 2;
		if (FAIL == MergeSortRec(arr, start, mid))
		{
			return (FAIL);
		}
		if (FAIL == MergeSortRec(arr, mid + 1, end))
		{
			return (FAIL);
		}
		if (FAIL == Merge(arr, start, mid, end))
		{
			return (FAIL);
		}
	}
	return (SUCCESS);
}

static int Merge(int *arr, long start, long mid, long end)
{
	long left_size = mid - start + 1;
	long right_size = end - mid;
	long left_runner = 0, right_runner = 0, merged = start;
	int *left_arr = NULL, *right_arr = NULL;

	left_arr = (int *)malloc(sizeof(arr[0]) * left_size);
	if (NULL == left_arr)
	{
		return (FAIL);
	}
	right_arr = (int *)malloc(sizeof(arr[0]) * right_size);
	if (NULL == right_arr)
	{
		return (FAIL);
	}
	CopyDataToArray(left_arr, arr, left_size, start);
	CopyDataToArray(right_arr, arr, right_size, mid + 1);

	while (left_runner < left_size && right_runner < right_size)
	{
		if (left_arr[left_runner] <= right_arr[right_runner])
		{
			arr[merged] = left_arr[left_runner];
			++left_runner;
		}
		else
		{
			arr[merged] = right_arr[right_runner];
			++right_runner;
		}
		++merged;
	}

	CopyRemainingData(arr, merged, left_arr, left_size, left_runner);
	CopyRemainingData(arr, merged, right_arr, right_size, right_runner);
	free(left_arr);
	free(right_arr);
	return (SUCCESS);
}

static void CopyRemainingData(int *dest, long dest_idx, int *src, long src_size, long runner)
{
	while (runner < src_size)
	{
		dest[dest_idx] = src[runner];
		++runner;
		++dest_idx;
	}
}
static void CopyDataToArray(int *dest, int *src, long size, long offset)
{
	long i = 0;
	for (; i < size; ++i)
	{
		dest[i] = src[offset + i];
	}
}

void QuickSort(void *base, size_t num_elements, size_t element_size, cmp_func cmp)
{
	QuickSortRec(base, element_size, 0, num_elements - 1, cmp);
}

static void QuickSortRec(void *base, size_t element_size, int start, int end, cmp_func cmp)
{
	if (start < end)
	{
		int partition_idx = Partition(base, element_size, start, end, cmp);
		QuickSortRec(base, element_size, start, partition_idx - 1, cmp);
		QuickSortRec(base, element_size, partition_idx + 1, end, cmp);
	}
}

static int Partition(void *base, size_t element_size, int start, int end, cmp_func cmp)
{
	int i = start, j = start;
	for (; j < end; ++j)
	{
		if (0 < cmp(GetElement(base, element_size, end), GetElement(base, element_size, j)))
		{
			SwapElements(element_size, ((char *)base + (i * element_size)), ((char *)base + (j * element_size)));
			++i;
		}
	}
	SwapElements(element_size, ((char *)base + ((i)*element_size)), ((char *)base + (end * element_size)));

	return (i);
}

static void *GetElement(void *base, size_t element_size, int place)
{
	return ((char *)base + (element_size * place));
}

static void SwapElements(size_t element_size, void *a, void *b)
{
	unsigned char *tmp1 = a, *tmp2 = b, tmp;
	size_t i = 0;
	for (i = 0; i != element_size; ++i)
	{
		tmp = tmp1[i];
		tmp1[i] = tmp2[i];
		tmp2[i] = tmp;
	}
}

void HeapSort(int *arr, size_t size)
{
	long last_position = size - 1;
	BuildMaxHeapFromArray(arr, size);
	SortArray(arr, last_position);
}

static void SortArray(int *arr, long last)
{
	for (; last > 0; --last)
	{
		SwapInts(&arr[0], &arr[last]);
		HeapifyDown(arr, 0, last);
	}
}

static void BuildMaxHeapFromArray(int *arr, long size)
{
	long last_parent = (size / 2) - 1;
	for (; last_parent >= 0; --last_parent)
	{
		HeapifyDown(arr, last_parent, size);
	}
}

void SwapInts(int *a, int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

static void HeapifyDown(int *arr, long parent, long size)
{
	long larger = GetBiggerChild(arr, parent, size);

	if (larger != parent)
	{
		SwapInts(&arr[parent], &arr[larger]);
		HeapifyDown(arr, larger, size);
	}
}

static long GetBiggerChild(int *arr, long parent, long size)
{
	long left_child = 0, right_child = 0, larger = parent;

	left_child = GET_CHILD(parent, LEFT);
	right_child = GET_CHILD(parent, RIGHT);

	if (left_child < size && arr[left_child] > arr[parent])
	{
		larger = left_child;
	}
	if (right_child < size && arr[right_child] > arr[larger])
	{
		larger = right_child;
	}

	return (larger);
}
