# Kokkos Stabilized Unstable Sort
When sorting elements in an array, an unstable sorting algorithm may not always preserve the order of equal elements, resulting in different orderings each time the algorithm is run on the same array. To stabilize the sorting algorithm, a permutation array can be used.

A permutation array is an auxiliary array that stores the original indices of the elements in the array to be sorted. For example, if the array to be sorted is [3, 1, 5, 2, 4], the permutation array would be [0, 1, 2, 3, 4]. The permutation array can be used to determine the original order of elements that are considered equal by the comparator function.

To stabilize the sorting algorithm, the comparator function must be modified to compare the indices in the permutation array when two elements are considered equal. This ensures that the order of equal elements is preserved based on their original positions in the array.