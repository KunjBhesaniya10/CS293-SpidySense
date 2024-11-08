#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

class Student {
private:
    int id;
    std::string name;
    int age;

public:
    Student(int id, const std::string& name, int age) : id(id), name(name), age(age) {}

    int getId() const { return id; }
    std::string getName() const { return name; }
    int getAge() const { return age; }

    void display() const {
        std::cout << "ID: " << id << ", Name: " << name << ", Age: " << age << "\n";
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

void encryptDecrypt(const std::string& filePath, char key) {
    std::ifstream inFile(filePath, std::ios::binary);
    if (!inFile) {
        std::cerr << "Error opening file for reading.\n";
        return;
    }

    std::string outFilePath = filePath + ".enc";
    std::ofstream outFile(outFilePath, std::ios::binary);
    if (!outFile) {
        std::cerr << "Error opening file for writing.\n";
        return;
    }

    char ch;
    while (inFile.get(ch)) {
        ch ^= key;
        outFile.put(ch);
    }

    inFile.close();
    outFile.close();
    std::cout << "File encrypted/decrypted and saved as " << outFilePath << "\n";
}

int main() {
    server();
    return 0;
}
