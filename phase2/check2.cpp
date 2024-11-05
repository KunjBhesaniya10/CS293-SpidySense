#include <iostream>
#include <thread>
#include <vector>
#include <memory>
using namespace std;

void another_function(){
    std::cout << "Hello Girl!" << std::endl;
}
void my_function(std::shared_ptr<std::vector<int>> vec){
    std::cout << (*vec)[0] << std::endl;
    another_function();
}
int main(){
    std::shared_ptr<std::vector<int>> vec{new std::vector<int>{234}};
    std::thread t{my_function, vec};
    t.join();
}