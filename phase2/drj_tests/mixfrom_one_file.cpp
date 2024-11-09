#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

class Account {
protected:
    int accountNumber;
    std::string accountHolder;
    double balance;

public:
    Account(int accountNum, const std::string& holder, double initialBalance = 0.0)
        : accountNumber(accountNum), accountHolder(holder), balance(initialBalance) {}

    virtual void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            std::cout << "Deposited: $" << amount << "\n";
        } else {
            std::cout << "Invalid deposit amount.\n";
        }
    }

    virtual void withdraw(double amount) {
        if (amount > 0 && amount <= balance) {
            balance -= amount;
            std::cout << "Withdrawn: $" << amount << "\n";
        } else {
            std::cout << "Invalid withdrawal amount.\n";
        }
    }

    virtual void displayAccountDetails() const {
        std::cout << "Account Number: " << accountNumber << "\n"
                  << "Account Holder: " << accountHolder << "\n"
                  << "Balance: $" << std::fixed << std::setprecision(2) << balance << "\n";
    }

    int getAccountNumber() const {
        return accountNumber;
    }

    virtual ~Account() = default;
};

class SavingsAccount : public Account {
private:
    double interestRate;

public:
    SavingsAccount(int accountNum, const std::string& holder, double initialBalance, double rate)
        : Account(accountNum, holder, initialBalance), interestRate(rate) {}

    void applyInterest() {
        double interest = balance * interestRate / 100;
        balance += interest;
        std::cout << "Interest applied: $" << interest << "\n";
    }

    void displayAccountDetails() const override {
        std::cout << "Savings Account:\n";
        Account::displayAccountDetails();
        std::cout << "Interest Rate: " << interestRate << "%\n";
    }
};