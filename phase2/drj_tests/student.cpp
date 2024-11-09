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

class StudentManager {
private:
    std::vector<Student> students;

public:
    void addStudent(int id, const std::string& name, int age) {
        students.emplace_back(id, name, age);
    }

    void removeStudent(int id) {
        auto it = std::remove_if(students.begin(), students.end(),
                                 [id](const Student& s) { return s.getId() == id; });
        if (it != students.end()) {
            students.erase(it, students.end());
            std::cout << "Student with ID " << id << " removed.\n";
        } else {
            std::cout << "Student not found.\n";
        }
    }

    void listStudents() const {
        for (const auto& student : students) {
            student.display();
        }
    }
};

int main() {
    StudentManager manager;
    manager.addStudent(1, "Alice", 20);
    manager.addStudent(2, "Bob", 22);
    manager.listStudents();
    manager.removeStudent(1);
    manager.listStudents();
    return 0;
}
