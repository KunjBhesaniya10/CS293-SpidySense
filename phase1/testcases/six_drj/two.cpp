#include <iostream>
#include <vector>
#include <queue>
#include <climits>

struct Edge {
    int destination;
    int weight;
};

struct CompareDistance {
    bool operator()(std::pair<int, int> a, std::pair<int, int> b) {
        return a.second > b.second;
    }
};

void relax(int currentNode, int edgeWeight, int nextNode, std::vector<int>& distances, std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, CompareDistance>& minHeap) {
    if (distances[currentNode] + edgeWeight < distances[nextNode]) {
        distances[nextNode] = distances[currentNode] + edgeWeight;
        minHeap.push({nextNode, distances[nextNode]});
    }
}

void dijkstraWithCustomComparator(int source, int vertices, const std::vector<std::vector<Edge>>& adjacencyList) {
    std::vector<int> distances(vertices, INT_MAX);
    distances[source] = 0;

    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, CompareDistance> minHeap;
    minHeap.push({source, 0});

    while (!minHeap.empty()) {
        int currentNode = minHeap.top().first;
        int currentDist = minHeap.top().second;
        minHeap.pop();

        if (currentDist > distances[currentNode]) continue;

        for (const Edge& edge : adjacencyList[currentNode]) {
            relax(currentNode, edge.weight, edge.destination, distances, minHeap);
        }
    }

    for (int i = 0; i < vertices; i++) {
        std::cout << "Shortest path from source to vertex " << i << " is " << distances[i] << "\n";
    }
}
