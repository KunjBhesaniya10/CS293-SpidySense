#include <iostream>
#include <unordered_map>
#include <string>
using namespace std;

class Account {
public:
    int accountNumber;
    string owner;
    double balance;

    Account(int accNum, const string& accOwner, double initialBalance) 
        : accountNumber(accNum), owner(accOwner), balance(initialBalance) {}

    void deposit(double amount) {
        balance += amount;
        cout << "Deposited $" << amount << ". New balance: $" << balance << endl;
    }

    bool withdraw(double amount) {
        if (amount > balance) {
            cout << "Insufficient balance. Current balance: $" << balance << endl;
            return false;
        }
        balance -= amount;
        cout << "Withdrew $" << amount << ". New balance: $" << balance << endl;
        return true;
    }

    void displayBalance() const {
        cout << "Account #" << accountNumber << " (" << owner << "): Balance $" << balance << endl;
    }
};

class Bank {
private:
    unordered_map<int, Account> accounts;
    int nextAccountNumber = 1001;

public:
    int createAccount(const string& owner, double initialDeposit) {
        Account newAccount(nextAccountNumber, owner, initialDeposit);
        accounts[nextAccountNumber] = newAccount;
        cout << "Account created. Account Number: " << nextAccountNumber << endl;
        return nextAccountNumber++;
    }

    void deposit(int accountNumber, double amount) {
        if (accounts.find(accountNumber) != accounts.end()) {
            accounts[accountNumber].deposit(amount);
        } else {
            cout << "Account not found.\n";
        }
    }

    void withdraw(int accountNumber, double amount) {
        if (accounts.find(accountNumber) != accounts.end()) {
            accounts[accountNumber].withdraw(amount);
        } else {
            cout << "Account not found.\n";
        }
    }

    void displayBalance(int accountNumber) const {
        if (accounts.find(accountNumber) != accounts.end()) {
            accounts.at(accountNumber).displayBalance();
        } else {
            cout << "Account not found.\n";
        }
    }
};

int main() {
    Bank bank;
    int choice, accountNumber;
    string owner;
    double amount;

    do {
        cout << "\nBanking System\n";
        cout << "1. Create Account\n2. Deposit\n3. Withdraw\n4. Check Balance\n5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "Enter owner's name: ";
            cin.ignore();
            getline(cin, owner);
            cout << "Enter initial deposit: ";
            cin >> amount;
            accountNumber = bank.createAccount(owner, amount);
            break;
        case 2:
            cout << "Enter account number: ";
            cin >> accountNumber;
            cout << "Enter deposit amount: ";
            cin >> amount;
            bank.deposit(accountNumber, amount);
            break;
        case 3:
            cout << "Enter account number: ";
            cin >> accountNumber;
            cout << "Enter withdrawal amount: ";
            cin >> amount;
            bank.withdraw(accountNumber, amount);
            break;
        case 4:
            cout << "Enter account number: ";
            cin >> accountNumber;
            bank.displayBalance(accountNumber);
            break;
        case 5:
            cout << "Exiting Banking System.\n";
            break;
        default:
            cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 5);

    return 0;
}
