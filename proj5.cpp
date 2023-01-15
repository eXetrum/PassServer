#include <iostream>
#include <string>
#include <utility>
#include <cstdlib>
#include "passserver.h"

using namespace std;
using namespace cop4530;

void Menu();

int main()
{
  int capacity;
  cout << "Enter preferred hash table capacity: ";
  cin >> capacity;
  PassServer server(capacity);
  string choice;

  do {
    Menu();
    cin >> choice;
    if(choice == "l") {
        string filename;
        cout << "Enter password file name to load from: ";
        cin >> filename;
        if(server.load(filename.c_str()) == false) {
            cout << "Error: Cannot open file " << filename << endl;
        }
        
    } else if (choice == "a") {
        string username;
        string password;
        cout << "Enter username: ";
        cin >> username;
        cout << "Enter password: ";
        cin >> password;
        std::pair<string, string> p(username, password);
        if(!server.addUser(p)) {
            cout << "*****Error: User already exists. Could not add user." << endl;
        } else {
            cout << "User " << username << " added." << endl;
        }
    } else if (choice == "r") {
        string username;
        cout << "Enter username    : ";
        cin >> username;
        if(!server.removeUser(username)) {
            cout << "*****Error: User not found.  Could not delete user" << endl;
        } else {
            cout << "User " << username << " deleted." << endl;
        }
    } else if (choice == "c") {
        string username;
        string password;
        string newpassword;
        cout << "Enter username    : ";
        cin >> username;
        cout << "Enter password    : ";
        cin >> password;
        cout << endl;
        cout << "Enter new password: ";
        cin >> newpassword;
        std::pair<string, string> p(username, password);
        if(!server.changePassword(p, newpassword)) {
            cout << "*****Error: Could not change user password" << endl;
        } else {
            cout << "Password changed for user " << username;
        }        
    } else if (choice == "f") {
        string username;
        cout << "Enter username    :";
        cin >> username;
        if(server.find(username)) {
            cout << "User '" << username << "' found." << endl;
        } else {
            cout << "User '" << username << "' not found." << endl;
        }
    } else if (choice == "d") {
        server.dump();
    } else if (choice == "s") {
        cout << "Size of hashtable: " << server.size() << endl;
    } else if (choice == "w") {
        string filename;
        cout << "Enter password file name to write to: ";
        cin >> filename;
        if(!server.write_to_file(filename.c_str())) {
            cout << "Error: Cannot open file to write." << endl;
        }
    } else if (choice == "x") {
    } else {
        cout << "*****Error: Invalid entry.  Try again." << endl;
    }
    
  } while(choice != "x");
}

void Menu()
{
  string input;
  cout << "\n\n";
  cout << "l - Load From File" << endl;
  cout << "a - Add User" << endl;
  cout << "r - Remove User" << endl;
  cout << "c - Change User Password" << endl;
  cout << "f - Find User" << endl;
  cout << "d - Dump HashTable" << endl;
  cout << "s - HashTable Size" << endl;
  cout << "w - Write to Password File" << endl;
  cout << "x - Exit program" << endl;
  cout << "\nEnter choice : ";
}
