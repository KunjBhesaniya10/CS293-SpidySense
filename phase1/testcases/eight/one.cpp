#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <map>
#include <algorithm>
#include <utility>
#define M_PI 3.14
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
class MathOperations {
public:
    // Basic arithmetic operations
    int add(int a, int b) { return a + b; }
    int subtract(int a, int b) { return a - b; }
    int multiply(int a, int b) { return a * b; }
    double divide(double a, double b) { return (b != 0) ? (a / b) : 0; }

    // Advanced operations
    int factorial(int n) {
        int result = 1;
        for (int i = 1; i <= n; ++i) result *= i;
        return result;
    }

    double power(double base, int exponent) {
        double result = 1;
        for (int i = 0; i < exponent; ++i) result *= base;
        return result;
    }
};

class Statistics {
private:
    std::vector<double> data;
public:
    void addData(double value) { data.push_back(value); }

    double mean() {
        if (data.empty()) return 0;
        double sum = 0;
        for (double val : data) sum += val;
        return sum / data.size();
    }

    double variance() {
        double avg = mean();
        double sum = 0;
        for (double val : data) {
            sum += (val - avg) * (val - avg);
        }
        return sum / data.size();
    }

    double standardDeviation() { return sqrt(variance()); }
};

class Shape {
public:
    virtual double area() const = 0;
    virtual double perimeter() const = 0;
    virtual void displayProperties() const = 0;
};

class Circle : public Shape {
private:
    double radius;
public:
    Circle(double r) : radius(r) {}

    double area() const override { return M_PI * radius * radius; }
    double perimeter() const override { return 2 * M_PI * radius; }
    void displayProperties() const override {
        std::cout << "Circle - Radius: " << radius << ", Area: " << area()
                  << ", Perimeter: " << perimeter() << std::endl;
    }
};

class Rectangle : public Shape {
private:
    double length, width;
public:
    Rectangle(double l, double w) : length(l), width(w) {}

    double area() const override { return length * width; }
    double perimeter() const override { return 2 * (length + width); }
    void displayProperties() const override {
        std::cout << "Rectangle - Length: " << length << ", Width: " << width
                  << ", Area: " << area() << ", Perimeter: " << perimeter() << std::endl;
    }
};

class Geometry {
public:
    double calculateTotalArea(const std::vector<Shape*>& shapes) {
        double totalArea = 0;
        for (const Shape* shape : shapes) {
            totalArea += shape->area();
        }
        return totalArea;
    }

    double calculateTotalPerimeter(const std::vector<Shape*>& shapes) {
        double totalPerimeter = 0;
        for (const Shape* shape : shapes) {
            totalPerimeter += shape->perimeter();
        }
        return totalPerimeter;
    }
};

class TextProcessor {
public:
    std::string reverseString(const std::string& text) {
        std::string reversed = text;
        std::reverse(reversed.begin(), reversed.end());
        return reversed;
    }

    int countWords(const std::string& text) {
        int wordCount = 0;
        bool inWord = false;
        for (char c : text) {
            if (isspace(c)) {
                if (inWord) {
                    wordCount++;
                    inWord = false;
                }
            } else {
                inWord = true;
            }
        }
        return inWord ? wordCount + 1 : wordCount;
    }
};

// Main function to test various classes and functions
int main() {
    // Math operations
    MathOperations mathOps;
    int sum = mathOps.add(5, 7);
    int factorial = mathOps.factorial(5);
    double power = mathOps.power(2, 8);

    std::cout << "Sum: " << sum << "\nFactorial: " << factorial << "\nPower: " << power << std::endl;

    // Statistics calculations
    Statistics stats;
    stats.addData(10.5);
    stats.addData(23.7);
    stats.addData(5.4);
    stats.addData(18.9);
    stats.addData(11.2);

    std::cout << "Mean: " << stats.mean() << "\nVariance: " << stats.variance() 
              << "\nStandard Deviation: " << stats.standardDeviation() << std::endl;

    // Shape properties
    Circle circle(5.0);
    Rectangle rectangle(10.0, 4.0);

    std::vector<Shape*> shapes = { &circle, &rectangle };
    Geometry geometry;

    std::cout << "Total Area: " << geometry.calculateTotalArea(shapes) << "\nTotal Perimeter: " 
              << geometry.calculateTotalPerimeter(shapes) << std::endl;

    // Text processing
    TextProcessor textProcessor;
    std::string sampleText = "This is a simple sentence with multiple words.";
    std::string reversedText = textProcessor.reverseString(sampleText);
    int wordCount = textProcessor.countWords(sampleText);

    std::cout << "Original Text: " << sampleText << "\nReversed Text: " << reversedText 
              << "\nWord Count: " << wordCount << std::endl;

    return 0;
}



