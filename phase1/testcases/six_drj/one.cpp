#include <iostream>
#include <vector>
#include <queue>
#include <climits>

typedef std::pair<int, int> Edge; // first = distance, second = node

void dijkstra(int source, int vertices, const std::vector<std::vector<Edge>>& graph) {
    std::vector<int> distance(vertices, INT_MAX);
    distance[source] = 0;
    std::priority_queue<Edge, std::vector<Edge>, std::greater<Edge>> pq;
    pq.push({0, source});

    while (!pq.empty()) {
        int dist = pq.top().first;
        int node = pq.top().second;
        pq.pop();

        if (dist > distance[node]) continue;

        for (const auto& neighbor : graph[node]) {
            int edgeDist = neighbor.first;
            int neighborNode = neighbor.second;
            if (distance[node] + edgeDist < distance[neighborNode]) {
                distance[neighborNode] = distance[node] + edgeDist;
                pq.push({distance[neighborNode], neighborNode});
            }
        }
    }

    for (int i = 0; i < vertices; i++) {
        std::cout << "Distance from source to vertex " << i << " is " << distance[i] << "\n";
    }
}
