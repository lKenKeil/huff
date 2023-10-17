#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 힙을 사용하여 정수 정렬하기
void heapify(int arr[], int n, int i) {
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;

    if (l < n && arr[l] > arr[largest])
        largest = l;

    if (r < n && arr[r] > arr[largest])
        largest = r;

    if (largest != i) {
        int temp = arr[i];
        arr[i] = arr[largest];
        arr[largest] = temp;

        // 힙 변화 과정 출력
        printf("Heap 변화: ");
        for (int j = 0; j < n; j++) {
            printf("%d ", arr[j]);
        }
        printf("\n");

        heapify(arr, n, largest);
    }
}

// 힙 정렬 함수
void heapSort(int arr[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    for (int i = n - 1; i > 0; i--) {
        int temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;

        // 힙 변화 과정 출력
        printf("Heap 변화: ");
        for (int j = 0; j < n; j++) {
            printf("%d ", arr[j]);
        }
        printf("\n");

        heapify(arr, i, 0);
    }
}

// Huffman 코드 생성을 위한 구조체
struct MinHeapNode {
    char data;
    unsigned freq;
    struct MinHeapNode* left, * right;
};

// 힙 구조체
struct MinHeap {
    unsigned size;
    unsigned capacity;
    struct MinHeapNode** array;
};

// 새 노드 생성
struct MinHeapNode* newNode(char data, unsigned freq) {
    struct MinHeapNode* temp = (struct MinHeapNode*)malloc(sizeof(struct MinHeapNode));
    temp->left = temp->right = NULL;
    temp->data = data;
    temp->freq = freq;
    return temp;
}

// 새 힙 생성
struct MinHeap* createMinHeap(unsigned capacity) {
    struct MinHeap* minHeap = (struct MinHeap*)malloc(sizeof(struct MinHeap));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (struct MinHeapNode**)malloc(minHeap->capacity * sizeof(struct MinHeapNode*));
    return minHeap;
}

// 두 노드 교환
void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b) {
    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

// 최소 힙의 최소 요소 찾기
void minHeapify(struct MinHeap* minHeap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq)
        smallest = left;

    if (right < minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq)
        smallest = right;

    if (smallest != idx) {
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

// 힙 크기가 1인지 확인
int isSizeOne(struct MinHeap* minHeap) {
    return (minHeap->size == 1);
}

// 최소 노드 추출
struct MinHeapNode* extractMin(struct MinHeap* minHeap) {
    struct MinHeapNode* temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];

    --minHeap->size;
    minHeapify(minHeap, 0);

    return temp;
}

// 최소 힙에 노드 삽입
void insertMinHeap(struct MinHeap* minHeap, struct MinHeapNode* minHeapNode) {
    ++minHeap->size;
    int i = minHeap->size - 1;

    while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq) {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }

    minHeap->array[i] = minHeapNode;
}

// 힙 구축
void buildMinHeap(struct MinHeap* minHeap) {
    int n = minHeap->size - 1;
    int i;

    for (i = (n - 1) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
}

// 리프 노드 확인
int isLeaf(struct MinHeapNode* root) {
    return !(root->left) && !(root->right);
}

// 새 힙 생성 및 구축
struct MinHeap* createAndBuildMinHeap(char data[], int freq[], int size) {
    struct MinHeap* minHeap = createMinHeap(size);

    for (int i = 0; i < size; ++i)
        minHeap->array[i] = newNode(data[i], freq[i]);

    minHeap->size = size;
    buildMinHeap(minHeap);

    return minHeap;
}

// Huffman 트리 구축
struct MinHeapNode* buildHuffmanTree(char data[], int freq[], int size) {
    struct MinHeapNode* left, * right, * top;

    // 힙 생성
    struct MinHeap* minHeap = createAndBuildMinHeap(data, freq, size);

    // 노드가 1개 이상 남아있을 때까지 반복하여 두 노드 추출하고 새 노드 삽입하여 트리 구축
    while (!isSizeOne(minHeap)) {
        left = extractMin(minHeap);
        right = extractMin(minHeap);

        top = newNode('$', left->freq + right->freq);
        top->left = left;
        top->right = right;
        insertMinHeap(minHeap, top);
    }

    // 완성된 트리의 루트가 Huffman 트리가 됨
    return extractMin(minHeap);
}

// Huffman 코드 출력
void printCodes(struct MinHeapNode* root, int arr[], int top) {
    if (root->left) {
        arr[top] = 0;
        printCodes(root->left, arr, top + 1);
    }

    if (root->right) {
        arr[top] = 1;
        printCodes(root->right, arr, top + 1);
    }

    if (isLeaf(root)) {
        printf("%c: ", root->data);
        for (int i = 0; i < top; i++) {
            printf("%d", arr[i]);
        }
        printf("\n");
    }
}

int main() {
    int arr[] = { 34, 12, 76, 59, 32, 55, 88, 26, 16, 79, 34, 85, 29, 78, 41, 56, 86 };
    int n = sizeof(arr) / sizeof(arr[0]);

    // 주어진 배열 출력
    printf("주어진 배열: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    // 힙 정렬 수행
    heapSort(arr, n);

    // 정렬된 배열 출력
    printf("힙 정렬 후 배열: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    // Huffman 코드 생성을 위한 문자와 빈도 배열
    char data[] = { 'a', 'e', 'i', 'o', 'u', 's', 't' };
    int freq[] = { 10, 15, 12, 3, 4, 13, 1 };
    int size = sizeof(data) / sizeof(data[0]);

    struct MinHeapNode* root = buildHuffmanTree(data, freq, size);

    int codes[100], top = 0;
    printf("주어진 문자 및 빈도에 대한 Huffman 코드:\n");
    printCodes(root, codes, top);

    return 0;
}
