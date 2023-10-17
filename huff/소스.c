#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ���� ����Ͽ� ���� �����ϱ�
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

        // �� ��ȭ ���� ���
        printf("Heap ��ȭ: ");
        for (int j = 0; j < n; j++) {
            printf("%d ", arr[j]);
        }
        printf("\n");

        heapify(arr, n, largest);
    }
}

// �� ���� �Լ�
void heapSort(int arr[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    for (int i = n - 1; i > 0; i--) {
        int temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;

        // �� ��ȭ ���� ���
        printf("Heap ��ȭ: ");
        for (int j = 0; j < n; j++) {
            printf("%d ", arr[j]);
        }
        printf("\n");

        heapify(arr, i, 0);
    }
}

// Huffman �ڵ� ������ ���� ����ü
struct MinHeapNode {
    char data;
    unsigned freq;
    struct MinHeapNode* left, * right;
};

// �� ����ü
struct MinHeap {
    unsigned size;
    unsigned capacity;
    struct MinHeapNode** array;
};

// �� ��� ����
struct MinHeapNode* newNode(char data, unsigned freq) {
    struct MinHeapNode* temp = (struct MinHeapNode*)malloc(sizeof(struct MinHeapNode));
    temp->left = temp->right = NULL;
    temp->data = data;
    temp->freq = freq;
    return temp;
}

// �� �� ����
struct MinHeap* createMinHeap(unsigned capacity) {
    struct MinHeap* minHeap = (struct MinHeap*)malloc(sizeof(struct MinHeap));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (struct MinHeapNode**)malloc(minHeap->capacity * sizeof(struct MinHeapNode*));
    return minHeap;
}

// �� ��� ��ȯ
void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b) {
    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

// �ּ� ���� �ּ� ��� ã��
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

// �� ũ�Ⱑ 1���� Ȯ��
int isSizeOne(struct MinHeap* minHeap) {
    return (minHeap->size == 1);
}

// �ּ� ��� ����
struct MinHeapNode* extractMin(struct MinHeap* minHeap) {
    struct MinHeapNode* temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];

    --minHeap->size;
    minHeapify(minHeap, 0);

    return temp;
}

// �ּ� ���� ��� ����
void insertMinHeap(struct MinHeap* minHeap, struct MinHeapNode* minHeapNode) {
    ++minHeap->size;
    int i = minHeap->size - 1;

    while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq) {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }

    minHeap->array[i] = minHeapNode;
}

// �� ����
void buildMinHeap(struct MinHeap* minHeap) {
    int n = minHeap->size - 1;
    int i;

    for (i = (n - 1) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
}

// ���� ��� Ȯ��
int isLeaf(struct MinHeapNode* root) {
    return !(root->left) && !(root->right);
}

// �� �� ���� �� ����
struct MinHeap* createAndBuildMinHeap(char data[], int freq[], int size) {
    struct MinHeap* minHeap = createMinHeap(size);

    for (int i = 0; i < size; ++i)
        minHeap->array[i] = newNode(data[i], freq[i]);

    minHeap->size = size;
    buildMinHeap(minHeap);

    return minHeap;
}

// Huffman Ʈ�� ����
struct MinHeapNode* buildHuffmanTree(char data[], int freq[], int size) {
    struct MinHeapNode* left, * right, * top;

    // �� ����
    struct MinHeap* minHeap = createAndBuildMinHeap(data, freq, size);

    // ��尡 1�� �̻� �������� ������ �ݺ��Ͽ� �� ��� �����ϰ� �� ��� �����Ͽ� Ʈ�� ����
    while (!isSizeOne(minHeap)) {
        left = extractMin(minHeap);
        right = extractMin(minHeap);

        top = newNode('$', left->freq + right->freq);
        top->left = left;
        top->right = right;
        insertMinHeap(minHeap, top);
    }

    // �ϼ��� Ʈ���� ��Ʈ�� Huffman Ʈ���� ��
    return extractMin(minHeap);
}

// Huffman �ڵ� ���
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

    // �־��� �迭 ���
    printf("�־��� �迭: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    // �� ���� ����
    heapSort(arr, n);

    // ���ĵ� �迭 ���
    printf("�� ���� �� �迭: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    // Huffman �ڵ� ������ ���� ���ڿ� �� �迭
    char data[] = { 'a', 'e', 'i', 'o', 'u', 's', 't' };
    int freq[] = { 10, 15, 12, 3, 4, 13, 1 };
    int size = sizeof(data) / sizeof(data[0]);

    struct MinHeapNode* root = buildHuffmanTree(data, freq, size);

    int codes[100], top = 0;
    printf("�־��� ���� �� �󵵿� ���� Huffman �ڵ�:\n");
    printCodes(root, codes, top);

    return 0;
}
