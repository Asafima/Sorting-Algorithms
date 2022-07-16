#ifndef __COMPARISON_SORT_H__
#define __COMPARISON_SORT_H__

#include <stddef.h> /* size_t */

typedef int (*cmp_func)(const void *, const void *);

void SelectionSort(int *array, size_t size);
void BubbleSort(int *array, size_t size);
void InsertionSort(int *array, size_t size);
int CountingSort(int *arr, size_t size);
int RadixSort(int *arr, size_t size);
int MergeSort(int *arr, size_t size);
long BinarySearchIterative(int *arr, size_t size, int find);
long BinarySearchRecursion(int *arr, size_t size, int find);
void QuickSort(void *base, size_t num_elements, size_t element_size, cmp_func cmp);
void HeapSort(int *arr, size_t size);

#endif /* __COMPARISON_SORT_H__ */
