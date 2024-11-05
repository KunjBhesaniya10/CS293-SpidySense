#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <string>

using namespace std;

// -------------------- Node for Linked List --------------------
struct Node {
    int value;
    Node* next;
    Node(int val) : value(val), next(nullptr) {}
};

// -------------------- Linked List --------------------
class LinkedList {
public:
    Node* head;
    LinkedList() : head(nullptr) {}

    void append(int value) {
        Node* newNode = new Node(value);
        if (!head) {
            head = newNode;
            return;
        }
        Node* last = head;
        while (last->next) {
            last = last->next;
        }
        last->next = newNode;
    }

    void display() {
        Node* current = head;
        while (current) {
            cout << current->value << " -> ";
            current = current->next;
        }
        cout << "nullptr" << endl;
    }
};

// -------------------- Node for Doubly Linked List --------------------
struct DoublyNode {
    int value;
    DoublyNode* prev;
    DoublyNode* next;
    DoublyNode(int val) : value(val), prev(nullptr), next(nullptr) {}
};

// -------------------- Doubly Linked List --------------------
class DoublyLinkedList {
public:
    DoublyNode* head;
    DoublyLinkedList() : head(nullptr) {}

    void append(int value) {
        DoublyNode* newNode = new DoublyNode(value);
        if (!head) {
            head = newNode;
            return;
        }
        DoublyNode* last = head;
        while (last->next) {
            last = last->next;
        }
        last->next = newNode;
        newNode->prev = last;
    }

    void display() {
        DoublyNode* current = head;
        while (current) {
            cout << current->value << " <-> ";
            current = current->next;
        }
        cout << "nullptr" << endl;
    }
};

// -------------------- Stack --------------------
class Stack {
private:
    vector<int> items;

public:
    void push(int item) {
        items.push_back(item);
    }

    int pop() {
        if (items.empty()) return -1; // Indicates empty stack
        int item = items.back();
        items.pop_back();
        return item;
    }

    int peek() {
        if (items.empty()) return -1; // Indicates empty stack
        return items.back();
    }

    bool isEmpty() {
        return items.empty();
    }
};

// -------------------- Queue --------------------
class Queue {
private:
    vector<int> items;

public:
    void enqueue(int item) {
        items.push_back(item);
    }

    int dequeue() {
        if (items.empty()) return -1; // Indicates empty queue
        int item = items.front();
        items.erase(items.begin());
        return item;
    }

    bool isEmpty() {
        return items.empty();
    }
};

// -------------------- Deque --------------------
class Deque {
private:
    vector<int> items;

public:
    void addFront(int item) {
        items.insert(items.begin(), item);
    }

    void addRear(int item) {
        items.push_back(item);
    }

    int removeFront() {
        if (items.empty()) return -1; // Indicates empty deque
        int item = items.front();
        items.erase(items.begin());
        return item;
    }

    int removeRear() {
        if (items.empty()) return -1; // Indicates empty deque
        int item = items.back();
        items.pop_back();
        return item;
    }

    bool isEmpty() {
        return items.empty();
    }
};

// -------------------- Graph --------------------
class Graph {
private:
    unordered_map<int, vector<int>> adjList;

public:
    void addEdge(int u, int v) {
        adjList[u].push_back(v);
        adjList[v].push_back(u); // For undirected graph
    }

    void display() {
        for (auto& pair : adjList) {
            cout << pair.first << " -> ";
            for (int v : pair.second) {
                cout << v << " ";
            }
            cout << endl;
        }
    }

    void bfs(int start) {
        unordered_map<int, bool> visited;
        queue<int> q;
        visited[start] = true;
        q.push(start);

        while (!q.empty()) {
            int node = q.front();
            q.pop();
            cout << node << " ";

            for (int neighbor : adjList[node]) {
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    q.push(neighbor);
                }
            }
        }
        cout << endl;
    }

    void dfsUtil(int node, unordered_map<int, bool>& visited) {
        visited[node] = true;
        cout << node << " ";

        for (int neighbor : adjList[node]) {
            if (!visited[neighbor]) {
                dfsUtil(neighbor, visited);
            }
        }
    }

    void dfs(int start) {
        unordered_map<int, bool> visited;
        dfsUtil(start, visited);
        cout << endl;
    }
};

// -------------------- Huffman Encoding --------------------
struct HuffmanNode {
    char ch;
    int freq;
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(char character, int frequency) : ch(character), freq(frequency), left(nullptr), right(nullptr) {}
};

struct Compare {
    bool operator()(HuffmanNode* l, HuffmanNode* r) {
        return l->freq > r->freq;
    }
};

void generateHuffmanTree(const string& text) {
    unordered_map<char, int> frequency;
    for (char ch : text) {
        frequency[ch]++;
    }

    priority_queue<HuffmanNode*, vector<HuffmanNode*>, Compare> pq;

    for (auto& pair : frequency) {
        pq.push(new HuffmanNode(pair.first, pair.second));
    }

    while (pq.size() > 1) {
        HuffmanNode* left = pq.top(); pq.pop();
        HuffmanNode* right = pq.top(); pq.pop();

        HuffmanNode* combined = new HuffmanNode('\0', left->freq + right->freq);
        combined->left = left;
        combined->right = right;
        pq.push(combined);
    }

    HuffmanNode* root = pq.top();

    // Further code can be added here to generate codes from the tree.
}

// -------------------- Sorting Algorithms --------------------
void merge(vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    vector<int> L(n1), R(n2);
    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

void mergeSort(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

void bucketSort(vector<int>& arr, int bucketSize) {
    if (arr.empty()) return;

    int minValue = *min_element(arr.begin(), arr.end());
    int maxValue = *max_element(arr.begin(), arr.end());
    int bucketCount = (maxValue - minValue) / bucketSize + 1;

    vector<vector<int>> buckets(bucketCount);

    for (int num : arr) {
        int bucketIndex = (num - minValue) / bucketSize;
        buckets[bucketIndex].push_back(num);
    }

    arr.clear();
    for (auto& bucket : buckets) {
        sort(bucket.begin(), bucket.end());
        arr.insert(arr.end(), bucket.begin(), bucket.end());
    }
}

// -------------------- Main Function --------------------
int main() {
    // Example usage

    cout << "Linked List:" << endl;
    LinkedList ll;
    ll.append(1);
    ll.append(2);
    ll.append(3);
    ll.display();

    cout << "Doubly Linked List:" << endl;
    DoublyLinkedList dll;
    dll.append(1);
    dll.append(2);
    dll.append(3);
    dll.display();

    cout << "Stack:" << endl;
    Stack stack;
    stack.push(1);
    stack.push(2);
    cout << stack.pop() << endl;

    cout << "Queue:" << endl;
    Queue queue;
    queue.enqueue(1);
    queue.enqueue(2);
    cout << queue.dequeue() << endl;

    cout << "Deque:" << endl;
    Deque deque;
    deque.addFront(1);
    deque.addRear(2);
    cout << deque.removeFront() << endl;

    cout << "Graph:" << endl;
    Graph graph;
    graph.addEdge(1, 2);
    graph.addEdge(1, 3);
    graph.display();
    cout << "BFS from node 1: ";
    graph.bfs(1);
    cout << "DFS from node 1: ";
    graph.dfs(1);

    cout << "Huffman Encoding for 'hello':" << endl;
    generateHuffmanTree("hello");

    cout << "Merge Sort:" << endl;
    vector<int> arr = {38, 27, 43, 3, 9, 82, 10};
    mergeSort(arr, 0, arr.size() - 1);
    for (int num : arr) cout << num << " ";
    cout << endl;

    cout << "Bucket Sort:" << endl;
    vector<int> arr2 = {5, 2, 9, 1, 5, 6};
    bucketSort(arr2, 2);
    for (int num : arr2) cout << num << " ";
    cout << endl;

    return 0;
}
