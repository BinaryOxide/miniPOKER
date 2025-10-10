/*
    ============================================================
                        miniPOKER
    ============================================================
    Description:
    A lightweight terminal-based tool to visually display the
    file structure of any directory in a tree-like format.

    Features:
    - Recursive directory traversal
    - Color-coded output for folders, files, extensions, and tree
    - Sanitized user input for robust path handling
    - Cross-checked folder/file detection on Windows

    Developer Notes:
    Developed by Maruf Hasan (Indie Developer)
    Source: BinaryOxide
    Version: 1.0
    Platform: Windows (Console Application)
    ============================================================
*/

#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include <algorithm>
#include <conio.h>

/* ============================================================
   ?? Console Color Definitions
   ============================================================
   These codes are used to differentiate between types of text:
   - DEFAULT   : Regular file names
   - FOLDER    : Folder names
   - EXTENSION : File extensions
   - TREE      : Tree branch symbols
   - PROMPT    : User input prompts
*/
enum ConsoleColor {
    DEFAULT = 7, // Light grey
    FOLDER = 1, // Blue
    EXTENSION = 4, // Red
    TREE = 2, // Yellow
    PROMPT = 4  // red
};

/* ============================================================
   ?? clear_screen()
   ------------------------------------------------------------
   Clears the entire console window for a fresh display.
   Uses Windows API for performance and proper buffer handling.
*/
void clear_screen() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD count;
    DWORD cellCount;
    COORD homeCoords = { 0, 0 };

    if (hConsole == INVALID_HANDLE_VALUE) return;
    if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) return;

    cellCount = csbi.dwSize.X * csbi.dwSize.Y;
    FillConsoleOutputCharacter(hConsole, ' ', cellCount, homeCoords, &count);
    FillConsoleOutputAttribute(hConsole, csbi.wAttributes, cellCount, homeCoords, &count);
    SetConsoleCursorPosition(hConsole, homeCoords);
}

/* ============================================================
   ?? is_directory()
   ------------------------------------------------------------
   Checks if a given path points to a directory.
   Uses Windows API GetFileAttributesA.
*/
bool is_directory(const std::string& path) {
    DWORD attrib = GetFileAttributesA(path.c_str());
    return (attrib != INVALID_FILE_ATTRIBUTES) && (attrib & FILE_ATTRIBUTE_DIRECTORY);
}

/* ============================================================
   ?? get_directory_entries()
   ------------------------------------------------------------
   Retrieves all entries (files and folders) inside a directory.
   Ignores "." and ".." special entries.
   Returns a sorted vector of names.
*/
std::vector<std::string> get_directory_entries(const std::string& path) {
    std::vector<std::string> entries;
    std::string search_path = path + "\\*";

    WIN32_FIND_DATAA find_data;
    HANDLE hFind = FindFirstFileA(search_path.c_str(), &find_data);
    if (hFind == INVALID_HANDLE_VALUE) return entries;

    do {
        std::string name = find_data.cFileName;
        if (name != "." && name != "..") entries.push_back(name);
    } while (FindNextFileA(hFind, &find_data) != 0);

    FindClose(hFind);
    std::sort(entries.begin(), entries.end());
    return entries;
}

/* ============================================================
   ?? print_tree()
   ------------------------------------------------------------
   Recursively prints the directory tree with colors:
   - Folders appear in blue
   - Files have default name color and red extension
   - Tree branches in yellow
*/
void print_tree(const std::string& path, const std::string& prefix = "", bool isLastFolder = true) {
    if (!is_directory(path)) {
        std::cout << "Invalid Path" << std::endl;
        return;
    }

    std::vector<std::string> items = get_directory_entries(path);
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    for (size_t i = 0; i < items.size(); ++i) {
        const std::string& entry_name = items[i];
        bool isLast = (i == items.size() - 1);
        std::string full_path = path + "\\" + entry_name;

        std::string branchSymbol = (isLast ? "#-->" : "|-->");

        if (is_directory(full_path)) {
            // Folder: display in square brackets
            SetConsoleTextAttribute(hConsole, TREE);
            std::cout << prefix << branchSymbol;
            SetConsoleTextAttribute(hConsole, FOLDER);
            std::cout << "[" << entry_name << "]" << std::endl;
            SetConsoleTextAttribute(hConsole, DEFAULT);

            // Compute new prefix for nested items
            std::string newPrefix = prefix + (isLast ? "     " : "|    ");

            // Recursive call for subdirectory
            print_tree(full_path, newPrefix, isLast);
        }
        else {
            // File
            SetConsoleTextAttribute(hConsole, TREE);
            std::cout << prefix << branchSymbol;
            SetConsoleTextAttribute(hConsole, DEFAULT);

            size_t dot_pos = entry_name.find_last_of('.');
            if (dot_pos != std::string::npos) {
                std::string name_only = entry_name.substr(0, dot_pos);
                std::string ext = entry_name.substr(dot_pos);

                std::cout << name_only;
                SetConsoleTextAttribute(hConsole, EXTENSION);
                std::cout << ext << std::endl;
                SetConsoleTextAttribute(hConsole, DEFAULT);
            }
            else {
                std::cout << entry_name << std::endl;
            }
        }
    }
}


/* ============================================================
   ?? sanitize_path()
   ------------------------------------------------------------
   Removes leading/trailing whitespace and optional quotes.
   Ensures robust handling of user input paths.
*/
std::string sanitize_path(std::string input) {
    input.erase(0, input.find_first_not_of(" \t\n\r"));
    input.erase(input.find_last_not_of(" \t\n\r") + 1);
    if (!input.empty() && (input.front() == '"' || input.front() == '\'')) input.erase(0, 1);
    if (!input.empty() && (input.back() == '"' || input.back() == '\'')) input.pop_back();
    return input;
}

/* ============================================================
   ?? main()
   ------------------------------------------------------------
   Program entry point:
   - Clears screen
   - Prompts user for directory path
   - Sanitizes input
   - Prints root folder
   - Calls recursive tree printer
   - Displays footer and waits for key press
*/
int main() {
    SetConsoleOutputCP(CP_UTF8);  // Enable UTF-8 output for symbols
    clear_screen();

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    std::string dir;

    // Prompt user for input (green color)
    SetConsoleTextAttribute(hConsole, PROMPT);
    std::cout << "Enter Path: ";
    SetConsoleTextAttribute(hConsole, DEFAULT);
    std::getline(std::cin, dir);

    dir = sanitize_path(dir);

    // Extract folder name from full path
    std::size_t last_slash = dir.find_last_of("/\\");
    std::string dir_name = (last_slash != std::string::npos) ? dir.substr(last_slash + 1) : dir;

    // Print root folder
    SetConsoleTextAttribute(hConsole, FOLDER);
    std::cout << "\n" << dir_name << "\\" << std::endl;
    SetConsoleTextAttribute(hConsole, DEFAULT);

    // Recursive tree printing
    print_tree(dir);

    // Footer
    SetConsoleTextAttribute(hConsole, TREE);
    //std::cout << "\n<==============================>\n";
    SetConsoleTextAttribute(hConsole, DEFAULT);
   // std::cout << "         miniPOKER\n";
    SetConsoleTextAttribute(hConsole, TREE);
   // std::cout << "<==============================>" << std::endl;
    SetConsoleTextAttribute(hConsole, DEFAULT);

    // Wait for key press before exit
    //std::cout << "\nPress any key to exit..." << std::endl;
    _getch();  // Keep window open for user

    return 0;
}

/* ============================================================
   DEVELOPMENT NOTES
   ============================================================
   Project: miniPOKER
   Source: BinaryOxide
   Developer: Maruf Hasan (Indie Developer)
   Date: 2025
   Language: C++17 (Windows Console)
   Dependencies:
       - Windows API: Console manipulation and file system functions
       - Conio.h for _getch()
   Description:
       miniPOKER is a lightweight terminal-based directory
       tree viewer inspired by popular tools like 'tree'.
       The primary goal is fast, color-coded visualization
       with minimal dependencies.

   Key Features & Decisions:
       1. Color-coded output improves readability.
       2. Recursive traversal handles nested folders.
       3. Input sanitation avoids common user mistakes.
       4. UTF-8 console code page ensures proper symbols.
       5. Clean exit with key press avoids abrupt closures.

   Future Enhancements:
       - Support for Linux/macOS console with ANSI codes.
       - Add file sizes and modified dates.
       - Optional JSON/CSV export of directory structure.
       - Dynamic width adjustment for long filenames.
       - Integration as a terminal command ('minipoker').
*/

