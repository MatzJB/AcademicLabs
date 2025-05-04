

# Matlab

To be added

## Dynamic memory allocation

To be added
 
## Using ()

To be added
 
## Avoiding details

To be added

## Fetch, read and run strange.m

To be added

## Matrix multiplication

To be added

## Using arrays for indexing

To be added
 
## Sparse matrices

Sparse matrices are efficient for large datasets with many zeros (e.g., when solving PDEs). In Matlab, the compressed column format stores nonzero elements with their row and column indices. For example, given the matrix:

1  0  0  4  0  0  8  
0  2  0  0  0  7  9  
0  3  0  5  6  0  0

The sparse representation uses three arrays:  
- `a`: Stores nonzero values.  
- `row`: Stores row indices of nonzero values.  
- `col`: Points to the start of each column.

Example:  
a   = [1 2 3 4 5 6 7 8 9]  
row = [1 2 3 1 3 3 2 1 2]  
col = [1 2 4 4 6 7 8 10]

The memory used is given by:  
`memory = 8 * nnz + 4 * (nnz + n + 1)`.  
For a 3x7 matrix with 9 nonzeros, memory used = 140 bytes. Sparse matrices reduce memory, especially when `m << n`. For instance, transposing a matrix may reduce memory usage.

Performance with Sparse Matrices:  
Matlab provides efficient sparse matrix operations, but performance can vary. For example, computing sums across sparse matrices can show significant performance differences depending on column traversal order.

Block Algorithms for Sparse Matrix Operations:  
Matrix-vector products in large sparse matrices (e.g., `A * X`) can be optimized using block algorithms, where `X` contains multiple columns. A larger block size reduces time per multiplication. To measure block size optimization, compute `Y = A * X` with `X = randn(n, bs)` and test for various block sizes (`bs`). Plot `time(bs)/time(1)` and `(time(bs)/bs)/time(1)` to determine the optimal block size.

Band Matrices:  
A band matrix stores only the diagonals and is more compact than general sparse matrices. The upper bandwidth and lower bandwidth define the number of diagonals. Performance for band matrices can be measured by varying bandwidth and comparing with a full matrix multiply.

 
## A case study

To be added
 
## An old case study 

To be added
