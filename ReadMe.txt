/*
 * File: Library.cpp
 * Authors: Serif Nguyen & Jonathan Huntley
 * Version 0.6
 * Previous Version: 0.5
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
 * -----------------------
 * Known Issues:
 * - None at this time.
 * -----------------------
 */
