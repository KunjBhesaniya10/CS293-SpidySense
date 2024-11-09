#include <iostream>
#include <vector>
#include <string>
using namespace std;

class Book {
public:
    string title;
    string author;
    int year;
    string genre;

    Book(string t, string a, int y, string g) : title(t), author(a), year(y), genre(g) {}

    void display() const {
        cout << title << " by " << author << " (" << year << ") - " << genre << endl;
    }
};

class Library {
private:
    vector<Book> books;

public:
    void addBook(const string& title, const string& author, int year, const string& genre) {
        Book newBook(title, author, year, genre);
        books.push_back(newBook);
        cout << "Book '" << title << "' added to the library.\n";
    }

    void removeBook(const string& title) {
        for (auto it = books.begin(); it != books.end(); ++it) {
            if (it->title == title) {
                books.erase(it);
                cout << "Book '" << title << "' removed from the library.\n";
                return;
            }
        }
        cout << "Book '" << title << "' not found in the library.\n";
    }

    void searchBook(const string& title) const {
        for (const auto& book : books) {
            if (book.title == title) {
                cout << "Book found: ";
                book.display();
                return;
            }
        }
        cout << "Book '" << title << "' not found.\n";
    }

    void listBooks() const {
        if (books.empty()) {
            cout << "No books in the library.\n";
            return;
        }
        cout << "Books in the library:\n";
        for (const auto& book : books) {
            book.display();
        }
    }
};

int main() {
    Library lib;
    int choice;
    string title, author, genre;
    int year;

    do {
        cout << "\nLibrary Management System\n";
        cout << "1. Add Book\n2. Remove Book\n3. Search Book\n4. List Books\n5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1:
            cout << "Enter title: ";
            getline(cin, title);
            cout << "Enter author: ";
            getline(cin, author);
            cout << "Enter year: ";
            cin >> year;
            cin.ignore();
            cout << "Enter genre: ";
            getline(cin, genre);
            lib.addBook(title, author, year, genre);
            break;
        case 2:
            cout << "Enter title to remove: ";
            getline(cin, title);
            lib.removeBook(title);
            break;
        case 3:
            cout << "Enter title to search: ";
            getline(cin, title);
            lib.searchBook(title);
            break;
        case 4:
            lib.listBooks();
            break;
        case 5:
            cout << "Exiting Library System.\n";
            break;
        default:
            cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 5);

    return 0;
}
