#include <iostream>
#include <fstream>
#include <string>

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
    std::string filePath = "sample.txt";
    char key = 'K';
    encryptDecrypt(filePath, key);
    return 0;
}
