# External Sort with Replacement Selection (C)

An implementation of External Sorting using the **Replacement Selection** algorithm with a Min-Heap data structure in C. Designed to process large datasets (e.g., 9 GB CSV file).

## Project Specifications

* **Sorting Column:** `price` (`double` numeric type with repeated values).
* **Algorithm:** Replacement Selection using Min-Heap.
* **Allocated RAM (`HEAP_SIZE`):** ~412 MB (`HEAP_SIZE = 200,000` records).
* **Generated Partitions:** 170 files.
* **Average Partition Size:** ~54 MB per CSV file.

## How to Build and Run

```bash
# Compile with C99
gcc external_sort.c -o external_sort

# Run the program
./external_sort
