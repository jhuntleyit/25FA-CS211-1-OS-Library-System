// SerifN_JonathanH_ProjectLibrary.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
/*
 * File: Library.cpp
 * Authors: Serif Nguyen & Jonathan Huntley
 * Version 0.6
 * Previous Version: 0.5
 * Date: December 15, 2025
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
 *  - 0.8 (December 15, 2025)
 *
 * - Revised the code to make the PolyMorphism and Inheritance Functions more Obvious.
 *
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
 * 
 * * - 0.5 (December 08, 2025):
 *
 *  - Improved code legbility
 * -----------------------
 * Known Issues:
 * - None at this time.
 * -----------------------
 */


#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>   // for file output
#include <sstream>
#include <limits>

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
        : id(id), title(std::move(title)), author(std::move(author)), isCheckedOut(isCheckedOut) {
    }

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
        : LibraryItem(id, std::move(title), std::move(author), isCheckedOut) {
    }

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
 * File Operations
 * ------------------------
 */
vector<Book> loadBooks(const string& filename) {
    vector<Book> books;
    ifstream infile(filename);
    string line;

    while (getline(infile, line)) {
        try {
            books.push_back(Book::deserialize(line));
        }
        catch (...) {
            cerr << "ID: N/A " << line << endl;
        }
    }
    return books;
}

void saveBook(const string& filename, const Book& book) {
    ofstream outfile(filename, ios::app);
    outfile << book.serialize() << endl;
}

void overwriteDatabase(const string& filename, const vector<Book>& books) {
    ofstream outfile(filename, ios::trunc);
    for (const auto& b : books) {
        outfile << b.serialize() << endl;
    }
}

/* ------------------------
 * Seeding Function
 * ------------------------
 */
bool fileIsEmpty(const string& filename) {
    ifstream in(filename, ios::binary);
    if (!in) return true; // file doesn�t exist
    in.seekg(0, ios::end);
    return in.tellg() == 0;
}

void seedLibrary(const string& filename) {
    if (!fileIsEmpty(filename)) return;

    vector<pair<string, string>> initial = {
        {"Harry Potter and the Sorcerer�s Stone", "JK Rowling"},
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
 * Utility Functions
 * ------------------------
 */
void listBooks(const string& filename) {
    vector<Book> books = loadBooks(filename);

    if (books.empty()) {
        cout << "\nNo books in the library yet." << endl;
        return;
    }

    cout << "\nBooks in the library:" << endl;
    for (const auto& b : books) {
        cout << "ID: " << b.getId()
            << " | Title: " << b.getTitle()
            << " | Author: " << b.getAuthor()
            << " | Status: " << (b.getCheckedOut() ? "Checked Out" : "Available")
            << endl;
    }
}

int getNextId(const string& filename) {
    vector<Book> books = loadBooks(filename);
    int maxId = 0;
    for (const auto& b : books) {
        if (b.getId() > maxId) {
            maxId = b.getId();
        }
    }
    return maxId + 1;
}

bool updateBookStatus(const string& filename, int id, bool checkOut) {
    vector<Book> books = loadBooks(filename);
    bool found = false;

    for (auto& b : books) {
        if (b.getId() == id) {
            if (checkOut) b.checkOut();
            else b.checkIn();
            found = true;
            break;
        }
    }

    if (found) {
        overwriteDatabase(filename, books);
        cout << "\nUpdated book with ID " << id << " to "
            << (checkOut ? "Checked Out" : "Available") << "." << endl;
    }
    else {
        cout << "\nError: Book with ID " << id << " not found." << endl;
    }

    return found;
}

bool deleteBookById(const string& filename, int id) {
    vector<Book> books = loadBooks(filename);
    bool found = false;

    auto it = remove_if(books.begin(), books.end(),
        [&](const Book& b) { return b.getId() == id; });

    if (it != books.end()) {
        books.erase(it, books.end());
        overwriteDatabase(filename, books);
        cout << "\nDeleted book with ID " << id << " from the library." << endl;
        found = true;
    }
    else {
        cout << "\nError: Book with ID " << id << " not found, cannot delete." << endl;
    }

    return found;
}

/* ------------------------
 * Main Menu
 * ------------------------
 */
int main() {
    const string filename = "library.csv";
    int choice;

    // Seed initial library if empty
    seedLibrary(filename);

    cout << "Welcome to the Library System!";

    while (true) {
        cout << "\n\n------------------------------" << endl;
        cout << "What would you like to do?" << endl;
        cout << "1. Add Book" << endl;
        cout << "2. List Books" << endl;
        cout << "3. Check Out Book" << endl;
        cout << "4. Check In Book" << endl;
        cout << "5. Delete Book" << endl;
        cout << "6. Exit" << endl;
        cout << "\nChoice: ";
        

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\nInvalid input. Please enter a number between 1 and 6." << endl;
            continue;
        }

        if (choice == 1) {
            string title, author;
            cout << "\nEnter title: ";
            cin.ignore();
            getline(cin, title);
            cout << "Enter author: ";
            getline(cin, author);

            int id = getNextId(filename);
            Book newBook(id, title, author);
            saveBook(filename, newBook);
            cout << "\nAdded \"" << title << "\" by " << author << " with ID " << id << "." << endl;
        }
        else if (choice == 2) {
            listBooks(filename);
        }
        else if (choice == 3) {
            listBooks(filename);
            int id;
            cout << "\nEnter the ID of the book to check out: ";
            cin >> id;
            updateBookStatus(filename, id, true);
        }
        else if (choice == 4) {
            listBooks(filename);
            int id;
            cout << "\nEnter the ID of the book to check in: ";
            cin >> id;
            updateBookStatus(filename, id, false);
        }
        else if (choice == 5) {
            listBooks(filename);
            int id;
            cout << "\nEnter the ID of the book to delete: ";
            cin >> id;
            deleteBookById(filename, id);
        }
        else if (choice == 6) {
            cout << "\nExiting program. Goodbye!" << endl;
            break;
        }
        else {
            cout << "\nInvalid choice. Please enter a number between 1 and 6." << endl;
        }
    }

    return 0;
}