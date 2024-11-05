#include <iostream>
#include <thread>
using namespace std;

void another_function(){
    std::cout << "Hello Girl!" << std::endl;
}
void my_function(){
    std::cout << "Hello World!" << std::endl;
    another_function();
}
int main(){
    std::thread{my_function}.detach(); 
    for (int i = 0; i < 2; i++) {
        std::cout << "Main Thread: " << i << std::endl;
    }
}