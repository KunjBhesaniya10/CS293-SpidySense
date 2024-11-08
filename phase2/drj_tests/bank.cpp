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

class CheckingAccount : public Account {
private:
    double overdraftLimit;

public:
    CheckingAccount(int accountNum, const std::string& holder, double initialBalance, double overdraft)
        : Account(accountNum, holder, initialBalance), overdraftLimit(overdraft) {}

    void withdraw(double amount) override {
        if (amount > 0 && amount <= balance + overdraftLimit) {
            balance -= amount;
            std::cout << "Withdrawn: $" << amount << "\n";
        } else {
            std::cout << "Withdrawal exceeds overdraft limit.\n";
        }
    }

    void displayAccountDetails() const override {
        std::cout << "Checking Account:\n";
        Account::displayAccountDetails();
        std::cout << "Overdraft Limit: $" << overdraftLimit << "\n";
    }
};

class Bank {
private:
    std::vector<Account*> accounts;
    int nextAccountNumber = 1001;

public:
    ~Bank() {
        for (Account* account : accounts) {
            delete account;
        }
    }

    Account* findAccount(int accountNumber) {
        for (Account* account : accounts) {
            if (account->getAccountNumber() == accountNumber) {
                return account;
            }
        }
        return nullptr;
    }

    void createSavingsAccount(const std::string& holder, double initialBalance, double interestRate) {
        Account* newAccount = new SavingsAccount(nextAccountNumber++, holder, initialBalance, interestRate);
        accounts.push_back(newAccount);
        std::cout << "Savings account created. Account number: " << newAccount->getAccountNumber() << "\n";
    }

    void createCheckingAccount(const std::string& holder, double initialBalance, double overdraftLimit) {
        Account* newAccount = new CheckingAccount(nextAccountNumber++, holder, initialBalance, overdraftLimit);
        accounts.push_back(newAccount);
        std::cout << "Checking account created. Account number: " << newAccount->getAccountNumber() << "\n";
    }

    void deposit(int accountNumber, double amount) {
        Account* account = findAccount(accountNumber);
        if (account) {
            account->deposit(amount);
        } else {
            std::cout << "Account not found.\n";
        }
    }

    void withdraw(int accountNumber, double amount) {
        Account* account = findAccount(accountNumber);
        if (account) {
            account->withdraw(amount);
        } else {
            std::cout << "Account not found.\n";
        }
    }

    void displayAccountDetails(int accountNumber) const {
        Account* account = findAccount(accountNumber);
        if (account) {
            account->displayAccountDetails();
        } else {
            std::cout << "Account not found.\n";
        }
    }

    void applyInterestToSavingsAccounts() {
        for (Account* account : accounts) {
            SavingsAccount* savings = dynamic_cast<SavingsAccount*>(account);
            if (savings) {
                savings->applyInterest();
            }
        }
    }
};

void displayMenu() {
    std::cout << "\n=== Bank System Menu ===\n";
    std::cout << "1. Create Savings Account\n";
    std::cout << "2. Create Checking Account\n";
    std::cout << "3. Deposit Money\n";
    std::cout << "4. Withdraw Money\n";
    std::cout << "5. Display Account Details\n";
    std::cout << "6. Apply Interest to Savings Accounts\n";
    std::cout << "7. Exit\n";
    std::cout << "Choose an option: ";
}

int main() {
    Bank bank;
    int choice;

    while (true) {
        displayMenu();
        std::cin >> choice;

        if (choice == 1) {
            std::string name;
            double balance, rate;
            std::cout << "Enter account holder's name: ";
            std::cin.ignore();
            std::getline(std::cin, name);
            std::cout << "Enter initial balance: ";
            std::cin >> balance;
            std::cout << "Enter interest rate (%): ";
            std::cin >> rate;
            bank.createSavingsAccount(name, balance, rate);
        }
        else if (choice == 2) {
            std::string name;
            double balance, overdraft;
            std::cout << "Enter account holder's name: ";
            std::cin.ignore();
            std::getline(std::cin, name);
            std::cout << "Enter initial balance: ";
            std::cin >> balance;
            std::cout << "Enter overdraft limit: ";
            std::cin >> overdraft;
            bank.createCheckingAccount(name, balance, overdraft);
        }
        else if (choice == 3) {
            int accountNumber;
            double amount;
            std::cout << "Enter account number: ";
            std::cin >> accountNumber;
            std::cout << "Enter deposit amount: ";
            std::cin >> amount;
            bank.deposit(accountNumber, amount);
        }
        else if (choice == 4) {
            int accountNumber;
            double amount;
            std::cout << "Enter account number: ";
            std::cin >> accountNumber;
            std::cout << "Enter withdrawal amount: ";
            std::cin >> amount;
            bank.withdraw(accountNumber, amount);
        }
        else if (choice == 5) {
            int accountNumber;
            std::cout << "Enter account number: ";
            std::cin >> accountNumber;
            bank.displayAccountDetails(accountNumber);
        }
        else if (choice == 6) {
            bank.applyInterestToSavingsAccounts();
        }
        else if (choice == 7) {
            std::cout << "Exiting the program...\n";
            break;
        }
        else {
            std::cout << "Invalid choice. Try again.\n";
        }
    }

    return 0;
}
