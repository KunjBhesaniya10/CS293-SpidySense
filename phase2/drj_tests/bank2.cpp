#include <iostream>
#include <thread>
#include <mutex>

class BankAccount {
private:
    double balance;
    std::mutex mtx;

public:
    BankAccount(double initialBalance) : balance(initialBalance) {}

    void deposit(double amount) {
        std::lock_guard<std::mutex> lock(mtx);
        balance += amount;
        std::cout << "Deposited: " << amount << ", Balance: " << balance << "\n";
    }

    void withdraw(double amount) {
        std::lock_guard<std::mutex> lock(mtx);
        if (balance >= amount) {
            balance -= amount;
            std::cout << "Withdrawn: " << amount << ", Balance: " << balance << "\n";
        } else {
            std::cout << "Insufficient funds.\n";
        }
    }
};

void depositThread(BankAccount &account, double amount) {
    account.deposit(amount);
}

void withdrawThread(BankAccount &account, double amount) {
    account.withdraw(amount);
}

int main() {
    BankAccount account(100.0);

    std::thread t1(depositThread, std::ref(account), 50.0);
    std::thread t2(withdrawThread, std::ref(account), 30.0);

    t1.join();
    t2.join();

    return 0;
}