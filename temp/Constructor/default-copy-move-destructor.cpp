#include <iostream>
#include <string>

class Person {
public:
    std::string name;
    int age;

    // 1. Overloaded Constructor (Normal Constructor)
    // Used for creating new Person objects with initial values
    Person(const std::string _name, int _age) : name(_name), age(_age) {
        std::cout << "  [NORMAL CTOR] Person created: " << name << ", " << age << std::endl;
    }
    // 2. Copy Constructor
    Person(const Person& other) : name(other.name), age(other.age) {
        std::cout << "  [COPY CTOR] Person copied: " << other.name << ", " << other.age
                  << " (new object's name: " << this->name << ")" << std::endl;
    }
    // 3. Move Constructor
    Person(const Person&& move) noexcept : name(move.name), age(move.age) {
        std::cout << "  [MOVE CTOR] Person Move: " << move.name << ", " << move.age
                  << " (new object's name: " << this->name << ")" << std::endl;
    }
    void greet() const {
        std::cout << "Hello, my name is " << name << " and I am " << age << " years old." << std::endl;
    }
    // Destructor
    ~Person() {
        if (name.empty() && age) { // Check if name, age is not nullptr (i.e., not moved-from)
            std::cout << "Person Destructor. Deallocating " << name << age << std::endl;
        } else {
            std::cout << "Person Destructor. (No data to deallocate, was moved-from or empty) " << std::endl;
        }
    }
};

// Function that returns a Person object by value
// This is a common scenario where the Move Constructor might be invoked
Person createPersonForMoving(const std::string& name, int age) {
    std::cout << "\n--- Inside createPersonForMoving function ---" << std::endl;
    // This creates a temporary Person object (an rvalue)
    return Person(name, age);
} // The temporary Person object would typically be moved from here

int main() {
    std::cout << "===== START OF MAIN =====" << std::endl;

    // Scenario 1: Direct Initialization - Normal Constructor called
    Person person1("Alice", 30);
    // Copy person1 to person2
    Person person2 = person1;
    Person Persontemp = createPersonForMoving("dump",25);
    std::cout << "--- Back in main after createPersonForMoving ---" << std::endl;
    Person personmove = std::move(Persontemp);

    std::cout << "\n===== END OF MAIN (Objects will be destructed) =====" << std::endl;
    return 0;
}
