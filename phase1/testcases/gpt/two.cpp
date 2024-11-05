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
#include <iostream>
#include <vector>
#include <array>
#include <cmath>
using namespace std;
#define eps 1.0e-10
// Time Complexity: O(log(n) * q + n), Space Complexity: O(n + q)
// Here, int n = no. of vertices in polygon; int q = no. of queries. (1 per query)

bool isZero(double x) {
    return (x < eps && x > -eps);
} 

bool isEqual(const double& x, const double& y) {
    return (x - y <= x * eps && y - x <= x * eps);
}
// Self-explanatory (can't equate floating points!)

double areaTriangle(double x1, double y1, double x2, double y2, double x3, double y3) {
    double sum = 0;
    sum += x1 * (y2 - y3);
    sum += x2 * (y3 - y1);
    sum += x3 * (y1 - y2);
    return sum; 
    // Not really the area (actually +-2 times),
     // but kind of checks the way we rotate to get x1 x2 x3 clockwise or anticlockwise.
}

// This function checks whether the vertex (x, y) is 
// inside the triangle pt0, pt1, pt2 or on or outside. 
// Also what to do next if it is on or outside?
int inTriangle(vector<array<double, 2>> vertices, double x, double y, int m) {
    // m varies from 1 to n - 2 strictly inclusive.
    double x1 = vertices[m][0], x2 = vertices[m+1][0];
    double y1 = vertices[m][1], y2 = vertices[m+1][1];
    double x0 = vertices[0][0], y0 = vertices[0][1];
    int n = vertices.size();

    if (isEqual(x, x0) && isEqual(y, y0)) 
        return 0;
    // Nomenclature:
    // point 0 [pt0] is the first vertex (x0 y0) of polygon from array,  or 0th position. 
    // point 1 [pt1] is vertex at mth position, 
    // point 2 [pt2] at (m+1)th position.

    double area = areaTriangle (x1, y1, x2, y2, x0, y0);
    double pointArea0 = area*areaTriangle (x1, y1, x2, y2, x, y);
    double pointArea1 = area*areaTriangle (x2, y2, x0, y0, x, y);
    double pointArea2 = area*areaTriangle (x0, y0, x1, y1, x, y);
    // The whole point of these: pointArea0 > 0 means point (x, y) is on 
    // SAME side to line (pt1, pt2) as pt0.
    // < 0 means OPPOSITE side, same points.
    // == 0 means point (x, y),
    // point 1, point 2 are COLLINEAR.
    // Come up with similar definitions for pointArea1 and pointArea2 yourself. 
    // These kindof identify whether given point is 
    // inside, on or outside triangle(pt0, pt1, pt2)
    // Nomenclature (for returning) 0 ==> CONFIRMED point on the polygon. 
    // 1 ==> CONFIRMED point outside the polygon. -1 ==> CONFIRMED point inside polygon.
    // 2/3 ==> INCONCLUSIVE cases.

    if (pointArea0 < 0) 
        return 1; 
    // Think of a drawing part of the polygon and look at this triangle. 
    // Case: (x, y) lies beyond the side opposite to pt0 and 
    // CANNOT lie on or inside polygon.

    else if (isZero(pointArea0)) { 
        // So (x, y) is collinear to pt1 and pt2. Then it is on or outside.
        double RATIO;
        (isEqual(x1, x2)) ? RATIO = (y - y1)/(y2 - y1) : RATIO = (x - x1)/(x2 - x1); 
        // Ratio similar to m1 / (m1 + m2) for point on line, 
        // at which it divides line segment between pt1 and pt2
        
        if (RATIO >= 0 && RATIO <= 1) { 
            // Is (x, y) in the line SEGMENT? 
            // (Endpoints are accepted. Those are the vertices)
            return 0; 
            // If so, then point is on polygon.
        } 
        else return 1; 
        // So point is outside the line segment and polygon. 
        // Case: square (+-1, +-1), point (10000, 1)
    } 

    // OK. After this we know pointArea1 > 0 necessarily. Now what?
    else if (isZero(pointArea2) && m == 1 && pointArea1 >= 0) 
        return 0; 
    // So it lies on segment (pt0, pt1) and it is the first edge; 
    // it can be treated as 'on the polygon'.
    else if (isZero(pointArea1) && m == n - 2 && pointArea2 >= 0) 
        return 0;
    // Same rule, just for segment (pt0, pt2) and now the last edge;
    // NOTE: pointArea1 >= 0 (firstcase), 
    // pointArea2 >= 0 (secondcase) 
    // and pointArea0 > 0 (both cases) 
    // This (three rules) ensures that point lies on line SEGMENT not just on line.

    else if (pointArea1 >= 0 && pointArea2 >= 0) {
        return -1; 
        // Just a point inside the triangle. (or On it and not on an edge).
    }
    else if (pointArea2 < 0) { 
        // Think of the meaning of this.
        if (m == 1) {
            return 1; 
            // Certainly outside. 
            // Think of multiple triangle and rotation/angles about pt0.
        } 
        else {
            if (pointArea1 < 0) 
                return 1; 
            // Extend BOTH line segments from pt0 backward.
            // A point inside polygon CANNOT lie on the opposite side.
            else return 2;
            // Could be outside, middle or inside. Inconclusive.
            // Go check the left triangles for if inside or on, 
            // it is surely on those. 
            // (back to main algorithm).
        }
    }
    else if (pointArea1 < 0) {
        if (m == n - 2) {
            return 1;
        }
        else {
            if (pointArea2 < 0) return 1; 
            // If you think about it a bit, it is an exhaustive case check.
            else return 3; 
            // Very similar to the last block, 
            // just asked to check triangles on the right (bakc to main algorithm).
        }
    } 
    return 1;
    // The function won't really go to this case. Just to not give warning...
}

// The main function. 
// Check whether the point is inside or 'on polygon' traingle wise
// (join each line segment to the point pt0 to get n - 2 triangles).
int posPoint(vector<array<double, 2>> vertices, double x, double y) { 
    // NOTE: vertices is array of length 2n not n. 
    // x and y coordinates for each point.
    int n = vertices.size();
    int left = 1;
    int right = n - 1;
    int mid = (left + right) / 2; 
    // Like binary search.

    while (right > left) { 
        // This will execute inTriangle only once in case of triangle.
        // There m = 1 = n-2 ofc.
        int midTriangle = inTriangle (vertices, x, y, mid);

        if (midTriangle == 0) 
            return 0;
        else if (midTriangle == 1) 
            return 1;
        else if (midTriangle == -1) 
            return -1; 
        // Certain cases. Exit fn.

        else if (midTriangle == 2) {
            right = mid;
            mid = (left + right)/2;
        } 
        else if (midTriangle == 3) {
            left = mid;
            mid = (left + right)/2;
        } 
        // Compare with binary search of integers how we check triangles.
    } 
    return 1;
}

int main() {
    cout << endl << "Enter number of vertices of convex polygon: ";
    int n; cin >> n; cout << endl;
    vector<array<double, 2>> points(n);
    for (int i = 0; i < n; i++) {
        cout << 
            "Enter x and y coordinates [space separated floating point numbers] of vertex "
            << i+1 << ": ";
        // YOU HAVE TO input meaningful coordinates that form a convex polygon 
        // IN THE SAME ORDER of atleast three vertices.
        // Otherwise expect random outputs.
        cin >> points[i][0] >> points[i][1];
    }
    cout << endl;

    while (true) {
        double x, y;
        cout << "Enter x and y coordinates of point to check: "; 
        cin >> x >> y;
        int posn = posPoint(points, x, y);

        if (posn == -1) {
            cout << "Point (" << x << ", " << y << ") lies inside the polygon." << endl;
        } 
        else if (posn == 0) {
            cout << "Point (" << x << ", " << y << ") lies on the polygon." << endl;
        } 
        else if (posn == 1) {
            cout << "Point (" << x << ", " << y << ") lies outside the polygon." << endl;
        } 
        int chk = 0;
        cout << "Enter 1 to check another vertex, 0 to exit: "; cin >> chk;
        if (chk) {
            cout << endl;
            continue;
        } 
        else {
            cout << endl << "Thank you!" << endl << endl;
            break;
        }
    }
}