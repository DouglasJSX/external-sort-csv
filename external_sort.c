#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>

#define MAX_LINE 2048
#define HEAP_SIZE 200000 

typedef struct {
    double price;
    char full_line[MAX_LINE];
    int frozen;
} HeapNode;

HeapNode *heap = NULL;
int current_heap_size = 0;

void swap(HeapNode *a, HeapNode *b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void min_heapify(int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < current_heap_size && heap[left].price < heap[smallest].price)
        smallest = left;
    if (right < current_heap_size && heap[right].price < heap[smallest].price)
        smallest = right;

    if (smallest != idx) {
        swap(&heap[idx], &heap[smallest]);
        min_heapify(smallest);
    }
}

void build_min_heap() {
    for (int i = (current_heap_size / 2) - 1; i >= 0; i--) {
        min_heapify(i);
    }
}

double extract_price_fast(const char *line) {
    int comma_count = 0;
    const char *p = line;
    
    while (*p) {
        if (*p == ',') {
            comma_count++;
            if (comma_count == 6) {
                return strtod(p + 1, NULL);
            }
        }
        p++;
    }
    return 0.0;
}

int main() {
    heap = (HeapNode *) malloc(HEAP_SIZE * sizeof(HeapNode));
    if (heap == NULL) {
        printf("Erro ao alocar memoria.\n");
        return 1;
    }

    FILE *in = fopen("data.csv", "r");
    if (!in) {
        perror("Erro ao abrir arquivo");
        free(heap);
        return 1;
    }

    char line_buffer[MAX_LINE];
    if (!fgets(line_buffer, sizeof(line_buffer), in)) {
        fclose(in);
        free(heap);
        return 1;
    }

    int total_records = 0;
    while (total_records < HEAP_SIZE && fgets(line_buffer, sizeof(line_buffer), in)) {
        heap[total_records].price = extract_price_fast(line_buffer);
        strcpy(heap[total_records].full_line, line_buffer);
        heap[total_records].frozen = 0;
        total_records++;
    }

    current_heap_size = total_records;
    build_min_heap();

    int partition_count = 0;
    int has_more_input = 1;

    while (total_records > 0) {
        partition_count++;
        char part_filename[64];
        sprintf(part_filename, "particao_%d.csv", partition_count);
        FILE *out = fopen(part_filename, "w");

        if (!out) break;

        for (int i = 0; i < total_records; i++) {
            heap[i].frozen = 0;
        }

        current_heap_size = total_records;
        build_min_heap();

        double last_written_price = -DBL_MAX;

        while (current_heap_size > 0) {
            HeapNode min_node = heap[0];

            fputs(min_node.full_line, out);
            last_written_price = min_node.price;

            if (has_more_input && fgets(line_buffer, sizeof(line_buffer), in)) {
                double new_price = extract_price_fast(line_buffer);

                if (new_price < last_written_price) {
                    heap[0].price = new_price;
                    strcpy(heap[0].full_line, line_buffer);
                    heap[0].frozen = 1;

                    swap(&heap[0], &heap[current_heap_size - 1]);
                    current_heap_size--;
                } else {
                    heap[0].price = new_price;
                    strcpy(heap[0].full_line, line_buffer);
                    heap[0].frozen = 0;
                }
            } else {
                has_more_input = 0;
                swap(&heap[0], &heap[current_heap_size - 1]);
                swap(&heap[current_heap_size - 1], &heap[total_records - 1]);
                current_heap_size--;
                total_records--;
            }

            if (current_heap_size > 0) {
                min_heapify(0);
            }
        }

        fclose(out);
    }

    fclose(in);
    free(heap);
    printf("Concluido. Total de particoes geradas: %d\n", partition_count);
    return 0;
}