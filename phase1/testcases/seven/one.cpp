#include <iostream>
#include <vector>
#include <stack>
#include <unordered_map>
#include <set>

// Graph class to represent an undirected graph using an adjacency list
class Graph {
private:
    // Adjacency list where each node points to a set of neighboring nodes
    std::unordered_map<int, std::set<int>> adjacencyList;
    int totalVertices; // Total number of vertices in the graph

    // Private function for DFS traversal from a given starting node
    void depthFirstSearchUtil(int currentNode, std::unordered_map<int, bool>& visitedNodes) {
        std::stack<int> dfsStack; // Stack to hold nodes during traversal
        dfsStack.push(currentNode); // Initialize stack with the starting node

        // Process all nodes reachable from the starting node
        while (!dfsStack.empty()) {
            int topNode = dfsStack.top();
            dfsStack.pop();

            // If the node has not been visited, mark it as visited
            if (!visitedNodes[topNode]) {
                std::cout << "Visiting node: " << topNode << std::endl;
                visitedNodes[topNode] = true;
            }

            // Get all unvisited neighbors of the current node and push them onto the stack
            for (const int& neighbor : adjacencyList[topNode]) {
                if (!visitedNodes[neighbor]) {
                    dfsStack.push(neighbor);
                }
            }
        }
    }

public:
    // Constructor to initialize the graph with a specified number of vertices
    Graph(int vertices) : totalVertices(vertices) {}

    // Function to add an undirected edge between two nodes
    void addEdge(int node1, int node2) {
        adjacencyList[node1].insert(node2);
        adjacencyList[node2].insert(node1);
        std::cout << "Edge added between node " << node1 << " and node " << node2 << std::endl;
    }

    // Function to print the entire adjacency list representation of the graph
    void printGraph() {
        std::cout << "Graph adjacency list:" << std::endl;
        for (const auto& pair : adjacencyList) {
            std::cout << "Node " << pair.first << ": ";
            for (const int& neighbor : pair.second) {
                std::cout << neighbor << " ";
            }
            std::cout << std::endl;
        }
    }

    // Function to detect and print all connected components in the graph
    void findConnectedComponents() {
        std::unordered_map<int, bool> visitedNodes; // Track visited nodes
        for (int i = 0; i < totalVertices; i++) {
            visitedNodes[i] = false; // Initialize all nodes as unvisited
        }

        int componentCount = 0; // Counter for connected components
        for (int i = 0; i < totalVertices; i++) {
            if (!visitedNodes[i]) {
                componentCount++;
                std::cout << "Connected Component " << componentCount << ":" << std::endl;
                depthFirstSearchUtil(i, visitedNodes);
                std::cout << "End of Component " << componentCount << "\n" << std::endl;
            }
        }
        std::cout << "Total connected components found: " << componentCount << std::endl;
    }
};

int main() {
    int numVertices = 8; // Specify the number of vertices
    Graph graph(numVertices); // Create a graph with the specified vertices

    // Add edges between nodes to form undirected connections
    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(1, 3);
    graph.addEdge(2, 4);
    graph.addEdge(5, 6);
    graph.addEdge(6, 7);

    // Print the adjacency list of the graph
    graph.printGraph();

    // Find and print all connected components in the graph
    std::cout << "\nDetecting connected components:" << std::endl;
    graph.findConnectedComponents();

    return 0;
}
