#include <iostream>
#include <thread>

int main() {
    int n = std::thread::hardware_concurrency();
    std::cout << n << std::endl;
}
