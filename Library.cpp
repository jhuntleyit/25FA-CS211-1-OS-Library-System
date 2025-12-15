/*
 * File: Library.cpp
 * Authors: Serif Nguyen & Jonathan Huntley
 * Version 0.6
 * Previous Version: 0.8
 * Date: December 08, 2025
 * 
 * Description:
 *  A simple library management system that allows users to add, list,
 *  check out, check in, and delete books from a CSV database.
 *  Includes seeding of initial books if the database is empty.
 *
 * Usage:
 *  Compile the program using a C++17 compatible compiler.
 * Run the executable and follow the on-screen menu prompts.
 * The library data is stored in "library.csv" in the same directory
 * as the executable.
 * 
 * Example Commands:
 * - To add a book, select option 1 and provide the title and author.
 * - To list all books, select option 2.
 * - To check out a book, select option 3 and provide the book ID.
 * - To check in a book, select option 4 and provide the book ID.
 * - To delete a book, select option 5 and provide the book ID.
 * - To exit the program, select option 6.
 *
 * Execution:
 * ./Library
 * -----------------------
 * Change Log:
 * 
 * - 0.5 (December 08, 2025):
 * 
 *  - Improved error handling for file operations and user inputs.
 * - Enhanced user interface with clearer prompts and messages.
 * - Code refactoring for better readability and maintainability.
 * - 0.0 (Initial version):
 *  - Basic functionality for adding, listing, checking out, checking in,
 *  and deleting books.
 * - CSV file handling for persistent storage.
 * - Simple console-based user interface.
 * - Added in a Seeding Function to populate the CSV from 1 if empty. 
 * - 0.5 (December 10, 2025):
 * - Improved code legibility 
 * - Made the PolyMorphism and Inheritance Functions more Obvious.
 * -----------------------
 * Known Issues:
 * - None at this time.
 * -----------------------
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <limits>
#include <algorithm>

using namespace std;

/* ------------------------
 * Base Class: LibraryItem
 * ------------------------
 */
class LibraryItem {
protected:
    int id;
    string title;
    string author;
    bool isCheckedOut;

public:
    LibraryItem(int id, string title, string author, bool isCheckedOut = false)
        : id(id), title(std::move(title)), author(std::move(author)), isCheckedOut(isCheckedOut) {}

    virtual ~LibraryItem() = default;

    int getId() const { return id; }
    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    bool getCheckedOut() const { return isCheckedOut; }

    void checkOut() { isCheckedOut = true; }
    void checkIn() { isCheckedOut = false; }

    virtual string serialize() const {
        ostringstream oss;
        oss << id << ", " << title << ", " << author << ", " << (isCheckedOut ? "Yes" : "No");
        return oss.str();
    }

    virtual void print() const = 0;
};

/* ------------------------
 * Subclass: Book
 * ------------------------
 */
class Book : public LibraryItem {
public:
    Book(int id, string title, string author, bool isCheckedOut = false)
        : LibraryItem(id, std::move(title), std::move(author), isCheckedOut) {}

    void print() const override {
        cout << "Book ID: " << id
             << " | Title: " << title
             << " | Author: " << author
             << " | Status: " << (isCheckedOut ? "Checked Out" : "Available")
             << endl;
    }

    static Book deserialize(const string& line) {
        istringstream iss(line);
        string token;
        vector<string> parts;

        while (getline(iss, token, ',')) {
            size_t start = token.find_first_not_of(" \t");
            size_t end = token.find_last_not_of(" \t");
            if (start == string::npos || end == string::npos) parts.push_back("");
            else parts.push_back(token.substr(start, end - start + 1));
        }

        if (parts.size() != 4) throw runtime_error("Invalid book data format.");
        int id = stoi(parts[0]);
        string title = parts[1];
        string author = parts[2];
        bool isCheckedOut = (parts[3] == "Yes");
        return Book(id, title, author, isCheckedOut);
    }
};

/* ------------------------
 * Utility: Seeding Function
 * ------------------------
 */
bool fileIsEmpty(const string& filename) {
    ifstream in(filename, ios::binary);
    if (!in) return true; // file doesn’t exist
    in.seekg(0, ios::end);
    return in.tellg() == 0;
}

void seedLibrary(const string& filename) {
    if (!fileIsEmpty(filename)) return;

    vector<pair<string,string>> initial = {
        {"Harry Potter and the Sorcerer’s Stone", "JK Rowling"},
        {"Harry Potter and the Chamber of Secrets", "JK Rowling"},
        {"Harry Potter and the Goblet of Fire", "JK Rowling"},
        {"Don Quixote", "Miguel de Cervantes"},
        {"The Hobbit", "J.R.R. Tolkien"},
        {"Wuthering Heights", "Emily Bronte"},
        {"The Lord of The Rings", "J.R.R. Tolkien"},
        {"Good Omens", "Neil Gaiman"},
        {"Coraline", "Neil Gaiman"},
        {"The Giver", "Lois Lowry"},
        {"Number the Stars", "Lois Lowry"},
        {"The Great Gatsby", "F. Scott Fitzgerald"},
        {"To Kill A Mockingbird", "Harper Lee"},
        {"The Hunger Games", "Suzanne Collins"},
        {"Catching Fire", "Suzanne Collins"},
        {"Game of Thrones", "George R. R. Martin"},
        {"The Wild Robot", "Peter Brown"},
        {"The Lightning Thief", "Rick Riordan"},
        {"The Last Olympian", "Rick Riordan"}
    };

    ofstream out(filename, ios::trunc);
    if (!out) {
        cerr << "\nError: Could not open " << filename << " for seeding.\n";
        return;
    }
    int id = 1;
    for (auto& p : initial) {
        Book b(id++, p.first, p.second, false);
        out << b.serialize() << "\n";
    }
    cout << "\nSeeded initial library with " << initial.size() << " books.\n";
}

/* ------------------------
 * Manager Class: Library
 * ------------------------
 */
class Library {
private:
    string filename;
    vector<LibraryItem*> items;

public:
    explicit Library(string file) : filename(std::move(file)) {
        seedLibrary(filename); // ensure seeding happens first
        loadBooks();
    }

    ~Library() {
        for (auto* item : items) delete item;
    }

    void loadBooks() {
        items.clear();
        ifstream infile(filename);
        string line;
        while (getline(infile, line)) {
            if (!line.empty()) {
                try {
                    items.push_back(new Book(Book::deserialize(line)));
                } catch (...) {
                    cerr << "Skipping invalid line: " << line << endl;
                }
            }
        }
    }

    void addBook(const string& title, const string& author) {
        if (title.empty() || author.empty()) {
            cout << "Error: Title and author cannot be empty.\n";
            return;
        }
        int id = getNextId();
        items.push_back(new Book(id, title, author));
        overwriteDatabase();
        cout << "\nAdded \"" << title << "\" by " << author << " with ID " << id << "." << endl;
    }

    void listItems() const {
        if (items.empty()) {
            cout << "\nNo items in the library yet." << endl;
            return;
        }
        cout << "\nItems in the library:" << endl;
        for (const auto* item : items) {
            item->print(); // Polymorphic call
        }
        cout << "Total items: " << items.size() << endl;
    }

    bool updateItemStatus(int id, bool checkOut) {
        for (auto* item : items) {
            if (item->getId() == id) {
                if (checkOut) item->checkOut();
                else item->checkIn();
                overwriteDatabase();
                cout << "\nUpdated item with ID " << id << " to "
                     << (checkOut ? "Checked Out" : "Available") << "." << endl;
                return true;
            }
        }
        cout << "\nError: Item with ID " << id << " not found." << endl;
        return false;
    }

    bool deleteItemById(int id) {
        auto it = remove_if(items.begin(), items.end(),
                            [&](LibraryItem* item) { return item->getId() == id; });
        if (it != items.end()) {
            for (auto itr = it; itr != items.end(); ++itr) delete *itr;
            items.erase(it, items.end());
            overwriteDatabase();
            cout << "\nDeleted item with ID " << id << " from the library." << endl;
            return true;
        }
        cout << "\nError: Item with ID " << id << " not found, cannot delete." << endl;
        return false;
    }

    void overwriteDatabase() {
        ofstream outfile(filename, ios::trunc);
        if (!outfile) {
            cerr << "Error: Could not open " << filename << " for writing.\n";
            return;
        }
        for (const auto* item : items) {
            outfile << item->serialize() << endl;
        }
    }

private:
    int getNextId() const {
        int maxId = 0;
        for (const auto* item : items) {
            maxId = max(maxId, item->getId());
        }
        return maxId + 1;
    }
};

/* ------------------------
 * Main Menu
 * ------------------------
 */
int main() {
    const string filename = "library.csv";
    Library lib(filename);

    cout << "Welcome to the Polymorphic Library System!";

    int choice;
    while (true) {
        cout << "\n\n------------------------------" << endl;
        cout << "What would you like to do?" << endl;
        cout << "1. Add Book" << endl;
        cout << "2. List Items" << endl;
        cout << "3. Check Out Item" << endl;
        cout << "4. Check In Item" << endl;
        cout << "5. Delete Item" << endl;
        cout << "6. Exit" << endl;
        cout << "Choice: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\nInvalid input. Please enter a number between 1 and 6." << endl;
            continue;
        }

        if (choice == 1) {
            string title, author;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\nEnter title: ";
            getline(cin, title);
            cout << "Enter author: ";
            getline(cin, author);
            lib.addBook(title, author);

        } else if (choice == 2) {
            lib.listItems();

        } else if (choice == 3) {
            lib.listItems();
            int id;
            cout << "\nEnter the ID of the item to check out: ";
            if (!(cin >> id)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid ID.\n";
                continue;
            }
            lib.updateItemStatus(id, true);

        } else if (choice == 4) {
            lib.listItems();
            int id;
            cout << "\nEnter the ID of the item to check in: ";
            if (!(cin >> id)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid ID.\n";
                continue;
            }
            lib.updateItemStatus(id, false);

        } else if (choice == 5) {
            lib.listItems();
            int id;
            cout << "\nEnter the ID of the item to delete: ";
            if (!(cin >> id)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid ID.\n";
                continue;
            }
            // optional confirmation
            char confirm;
            cout << "Are you sure you want to delete ID " << id << "? (y/n): ";
            cin >> confirm;
            if (confirm == 'y' || confirm == 'Y') {
                lib.deleteItemById(id);
            } else {
                cout << "Delete cancelled.\n";
            }

        } else if (choice == 6) {
            cout << "\nExiting program. Goodbye!" << endl;
            break;

        } else {
            cout << "\nInvalid choice. Please enter a number between 1 and 6." << endl;
        }
    }

    return 0;
}
