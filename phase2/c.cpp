#include <iostream>
#include <thread>



int main(){
    // create threads more than 16.
    std::thread t1([](){std::cout << "Hello from thread 1" << std::endl;});
    std::thread t2([](){std::cout << "Hello from thread 2" << std::endl;});
    std::thread t3([](){std::cout << "Hello from thread 3" << std::endl;});
    std::thread t4([](){std::cout << "Hello from thread 4" << std::endl;});
    std::thread t5([](){std::cout << "Hello from thread 5" << std::endl;});
    std::thread t6([](){std::cout << "Hello from thread 6" << std::endl;});
    std::thread t7([](){std::cout << "Hello from thread 7" << std::endl;});
    std::thread t8([](){std::cout << "Hello from thread 8" << std::endl;});
    std::thread t9([](){std::cout << "Hello from thread 9" << std::endl;});
    std::thread t10([](){std::cout << "Hello from thread 10" << std::endl;});
    std::thread t11([](){std::cout << "Hello from thread 11" << std::endl;});
    std::thread t12([](){std::cout << "Hello from thread 12" << std::endl;});
    std::thread t13([](){std::cout << "Hello from thread 13" << std::endl;});
    std::thread t14([](){std::cout << "Hello from thread 14" << std::endl;});
    std::thread t15([](){std::cout << "Hello from thread 15" << std::endl;});
    std::thread t16([](){std::cout << "Hello from thread 16" << std::endl;});
    std::thread t17([](){std::cout << "Hello from thread 17" << std::endl;});
    std::thread t18([](){std::cout << "Hello from thread 18" << std::endl;});
    std::thread t19([](){std::cout << "Hello from thread 19" << std::endl;});
    std::thread t20([](){std::cout << "Hello from thread 20" << std::endl;});

    // t1.join();
    // t2.join();
    // t3.join();
    // t4.join();
    // t5.join();
    // t6.join();
    // t7.join();
    // t8.join();
    // t9.join();
    // t10.join();
    // t11.join();
    // t12.join();
    // t13.join();
    // t14.join();
    // t15.join();
    // t16.join();
    // t17.join();
    // t18.join();
    // t19.join();
    // t20.join();

    return 0;
}
