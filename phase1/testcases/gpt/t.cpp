#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <ctime>
#include <sstream>
using namespace std;

// TransactionLog Class
class TransactionLog {
private:
    string timestamp;
    string transactionType;
    double transactionAmount;
    double postBalance;

public:
    TransactionLog(string type, double amount, double balance) 
        : transactionType(type), transactionAmount(amount), postBalance(balance) {
        time_t now = time(0);
        timestamp = ctime(&now);  // Save transaction time
    }

    void displayLog() const {
        cout << setw(20) << timestamp << setw(15) << transactionType << setw(15) 
             << transactionAmount << setw(20) << postBalance << endl;
    }
};

// BankAccount Class
class BankAccount {
protected:
    int accNumber;
    string holderName;
    double accBalance;
    vector<TransactionLog> logs;

public:
    BankAccount(int accNum, string name, double initBalance) 
        : accNumber(accNum), holderName(name), accBalance(initBalance) {}

    virtual void addFunds(double amount) {
        accBalance += amount;
        logs.emplace_back("Deposit", amount, accBalance);
        cout << "Deposited $" << amount << ". New Balance: $" << accBalance << endl;
    }

    virtual bool removeFunds(double amount) {
        if (amount > accBalance) {
            cout << "Insufficient balance. Withdrawal declined." << endl;
            return false;
        }
        accBalance -= amount;
        logs.emplace_back("Withdraw", amount, accBalance);
        cout << "Withdrew $" << amount << ". New Balance: $" << accBalance << endl;
        return true;
    }

    void showStatement() const {
        cout << "Account Statement for " << holderName << " (Account Number: " 
             << accNumber << ")" << endl;
        cout << setw(20) << "Timestamp" << setw(15) << "Type" << setw(15) 
             << "Amount" << setw(20) << "Balance After" << endl;
        cout << string(70, '-') << endl;
        for (const auto& log : logs) {
            log.displayLog();
        }
    }

    double getBalance() const { return accBalance; }
};

// SavingsAccount Class
class SavingsAccount : public BankAccount {
private:
    double interestPercent;

public:
    SavingsAccount(int accNum, string name, double initBalance, double rate)
        : BankAccount(accNum, name, initBalance), interestPercent(rate) {}

    void calculateInterest() {
        double interest = accBalance * (interestPercent / 100);
        addFunds(interest);
        cout << "Interest of $" << interest << " calculated at rate " 
             << interestPercent << "%." << endl;
    }
};

// CurrentAccount Class
class CurrentAccount : public BankAccount {
private:
    double overdraftLimit;

public:
    CurrentAccount(int accNum, string name, double initBalance, double limit)
        : BankAccount(accNum, name, initBalance), overdraftLimit(limit) {}

    bool removeFunds(double amount) override {
        if (amount > accBalance + overdraftLimit) {
            cout << "Overdraft limit exceeded. Withdrawal failed." << endl;
            return false;
        }
        accBalance -= amount;
        logs.emplace_back("Withdraw", amount, accBalance);
        cout << "Withdrew $" << amount << ". New Balance: $" << accBalance << endl;
        return true;
    }
};

// Client Class
class Client {
private:
    string clientName;
    vector<BankAccount*> clientAccounts;

public:
    Client(string name) : clientName(name) {}

    void addNewAccount(BankAccount* account) {
        clientAccounts.push_back(account);
        cout << "New account added for " << clientName << endl;
    }

    void listAccounts() const {
        cout << "Accounts for " << clientName << ":" << endl;
        for (size_t i = 0; i < clientAccounts.size(); ++i) {
            cout << i + 1 << ". Account Number: " << clientAccounts[i]->getBalance() 
                 << " | Balance: $" << clientAccounts[i]->getBalance() << endl;
        }
    }

    void showAllStatements() const {
        cout << "Statements for " << clientName << ":" << endl;
        for (const auto& account : clientAccounts) {
            account->showStatement();
        }
    }
};

// BankApplication Class
class BankApplication {
private:
    vector<Client> clients;
    int accountNumSeed;

public:
    BankApplication() : accountNumSeed(1000) {}

    Client* registerClient(string name) {
        clients.emplace_back(name);
        cout << "Client " << name << " registered." << endl;
        return &clients.back();
    }

    BankAccount* createNewAccount(Client& client, string accType, double initialBalance, double extraParam = 0.0) {
        BankAccount* newAccount;
        int accountNum = ++accountNumSeed;

        if (accType == "Savings") {
            newAccount = new SavingsAccount(accountNum, client.clientName, initialBalance, extraParam);
        } else if (accType == "Current") {
            newAccount = new CurrentAccount(accountNum, client.clientName, initialBalance, extraParam);
        } else {
            cout << "Invalid account type." << endl;
            return nullptr;
        }

        client.addNewAccount(newAccount);
        cout << accType << " account created with Account Number: " << accountNum << endl;
        return newAccount;
    }
};

// Main Function
int main() {
    BankApplication bankApp;

    // Register Client
    Client* client = bankApp.registerClient("Alice");

    // Create Savings Account
    BankAccount* savingsAcc = bankApp.createNewAccount(*client, "Savings", 1000.0, 2.0);
    savingsAcc->addFunds(500);
    savingsAcc->removeFunds(200);
    static_cast<SavingsAccount*>(savingsAcc)->calculateInterest();
    savingsAcc->showStatement();

    // Create Current Account
    BankAccount* currentAcc = bankApp.createNewAccount(*client, "Current", 500.0, 300.0);
    currentAcc->removeFunds(600);
    currentAcc->addFunds(300);
    currentAcc->showStatement();

    // Display all statements
    client->showAllStatements();

    return 0;
}
