#include <iostream>
#include <fstream>
#include <conio.h>
#include <windows.h>
#include <unistd.h>
#include <vector>
using namespace std;

struct Node {
    char data;
    Node* next;
    Node* prev;
    Node* above;
    Node* below;
};

class TextEditor {
private:
    Node* head;
    Node* tail;
    Node* cursor;

public:
    TextEditor() {
        head = NULL;
        tail = NULL;
        cursor = NULL;
    }
    void addNode(char data) {
        if (data >= 32 && data <= 125) {
            Node* node = new Node;
            node->data = data;
            node->next = NULL;

            if (head == NULL) {
                node->prev = NULL;
                head = node;
                tail = node;
                cursor = node;
            }
            else {
                if (cursor == NULL) {
                    cursor = node;
                }
                node->prev = tail;
                tail->next = node;
                tail = node;
            }
        }
    }
    void deleteNode() {
        if (tail != NULL) {
            Node* temp = tail;
            tail = tail->prev;

            if (cursor == temp) {
                cursor = cursor->prev;
            }
            if (tail != NULL) {
                tail->next = NULL;
            }
            else {
                head = NULL;
            }
            delete temp;
        }
    }
    void deleteAtCursor() {
        if (cursor != NULL) {
            if (cursor->next != NULL) {
                Node* temp = cursor->next;
                if (temp->data == '\n') {
                    cursor->next = temp->next;
                    if (temp->next != NULL) {
                        temp->next->prev = cursor;
                    }
                    delete temp;
                }
                else {
                    cursor->next = temp->next;
                    if (temp->next != NULL) {
                        temp->next->prev = cursor;
                    }
                    delete temp;
                }
            }
        }
    }
    void newLine() {
        Node* node = new Node;
        node->data = '\n';
        node->next = NULL;

        if (head == NULL) {
            node->prev = NULL;
            head = node;
            tail = node;
            cursor = node;
        }
        else {
            if (cursor == NULL) {
                cursor = node;
            }
            tail->next = node;
            node->prev = tail;
            tail = node;
        }
    }
    void display() {
        Node* curr = head;

        while (curr != NULL) {
            if (curr == cursor) {
                cout << '|';
            }
            if (curr->data == '\n' || (curr->data >= 32 && curr->data <= 125)) {
                cout << curr->data;
            }
            curr = curr->next;
        }
    }
    void insertAtCursor(const string &str) {
        for (int i = 0; i < str.size(); ++i) {
            char ch = str[i];
            Node* node = new Node;
            node->data = ch;

            if (cursor == NULL) {
                node->prev = tail;
                tail->next = node;
                tail = node;
            } else {
                node->prev = cursor;
                node->next = cursor->next;
                if (cursor->next != NULL) {
                    cursor->next->prev = node;
                }
                cursor->next = node;
                if (cursor == tail) {
                    tail = node;
                }
            }
            cursor = node;
        }
    }
    void moveUp() {
        if (cursor != NULL) {
            if (cursor->prev != NULL) {
                if (cursor->prev->data == '\n') { 
                    while (cursor != NULL && cursor->prev != NULL && cursor->data != '\n') {
                        cursor = cursor->prev;
                    }
                    if (cursor->prev != NULL) {
                        cursor = cursor->prev;
                    }
                }
                else {
                    while (cursor != NULL && cursor->prev != NULL && cursor->data != '\n') {
                        cursor = cursor->prev;
                    }
                }
            }
        }
    }
    void moveDown() {
        if (cursor != NULL) {
            if (cursor->next != NULL) {
                if (cursor->next->data == '\n') {
                    cursor = cursor->next;
                }
                else {
                    while (cursor != NULL && cursor->next != NULL && cursor->data != '\n') {
                        cursor = cursor->next;
                    }
                    if (cursor->next != NULL) {
                        cursor = cursor->next;
                    }
                }
            }
        }
    }
    void moveLeft() {
        if (cursor != NULL && cursor->prev != NULL) {
            cursor = cursor->prev;
        }
    }
    void moveRight() {
        if (cursor != NULL && cursor->next != NULL) {
            cursor = cursor->next;
        }
    }
    void save() {
        ofstream file;
        file.open("save.txt");
        Node* curr = head;

        while (curr != NULL) {
            if (curr->data == '\n' || (curr->data >= 32 && curr->data <= 125)) {
                file << curr->data;
            }
            curr = curr->next;
        }
        file.close();
    }
    void load() {
        ifstream file;
        file.open("save.txt");

        if (file.is_open()) {
            char ch;

            while (file.get(ch)) {
                addNode(ch);
            }
            file.close();
        }
        else {
            cout << "Unable to load file." << endl;
            sleep(2);
        }
    }
    void grammarCheck(const vector<string> &dict) {
        Node *curr = head;
        string str = "";

        for (int i = 0; i < 22; i++) {
            cout << endl;
        }
        cout << "=> Spell Checker Console <=" << endl;
        cout << endl;

        while (curr != NULL) {
            if (curr->data == ' ' || curr->data == '\n' || curr->next == NULL) {
                if (!str.empty()) {
                    bool alphaCheck = true;
                    for (int i = 0; i < str.size(); ++i) {
                        if (!isalpha(str[i])) {
                            alphaCheck = false;
                            break;
                        }
                    }
                    if (alphaCheck) {
                        bool found = false;
                        for (int i = 0; i < dict.size(); ++i) {
                            if (str == dict[i]) {
                                found = true;
                                break;
                            }
                        }
                        if (!found) {
                            cout << "=> Unidentified Word: " << str << endl;
                            cout << "\n=> Autocorrection <= \n" << endl;
                            // Letter Substitution
                            for (int i = 0; i < str.size(); ++i) {
                                for (char c = 'a'; c <= 'z'; ++c) {
                                    string temp = str;
                                    temp[i] = c;
                                    bool check = false;

                                    for (int j = 0; j < dict.size(); ++j) {
                                        if (temp == dict[j]) {
                                            check = true;
                                            break;
                                        }
                                    }
                                    if (check) {
                                        cout << str << " -> " << temp << ", ";
                                    }
                                }
                            }
                            // Letter Omission
                            for (int i = 0; i < str.size(); ++i) {
                                string temp = str.substr(0, i) + str.substr(i + 1);
                                bool check = false;

                                for (int j = 0; j < dict.size(); ++j) {
                                    if (temp == dict[j]) {
                                        check = true;
                                        break;
                                    }
                                }
                                if (check) {
                                    cout << str << " -> " << temp << ", ";
                                }
                            }
                            // Letter Insertion
                            for (int i = 0; i <= str.size(); ++i) {
                                for (char c = 'a'; c <= 'z'; ++c) {
                                    string temp = str.substr(0, i) + c + str.substr(i);
                                    bool check = false;

                                    for (int j = 0; j < dict.size(); ++j) {
                                        if (temp == dict[j]) {
                                            check = true;
                                            break;
                                        }
                                    }
                                    if (check) {
                                        cout << str << " -> " << temp << ", ";
                                    }
                                }
                            }
                            // Letter Reversal
                            for (int i = 0; i < str.size() - 1; ++i) {
                                string temp = str;
                                swap(temp[i], temp[i + 1]);
                                bool check = false;

                                for (int j = 0; j < dict.size(); ++j) {
                                    if (temp == dict[j]) {
                                        check = true;
                                        break;
                                    }
                                }
                                if (check) {
                                    cout << str << " -> " << temp << ", ";
                                }
                            }
                        }
                        str = "";
                    }
                }
            }
            else {
                str += curr->data;
            }
            curr = curr->next;
        }
    }
    ~TextEditor() {
        Node* current = head;
        Node* next = NULL;
        while (current != NULL) {
            next = current->next;
            delete current;
            current = next;
        }
    }
};

int main() {
    TextEditor text;
    vector<string> dictionary;
    ifstream dictionaryFile("dictionary.txt");
    char input;

    if (dictionaryFile.is_open()) {
        string word;
        while (getline(dictionaryFile, word)) {
            dictionary.push_back(word);
        }
        dictionaryFile.close();
    }
    system("cls");

    while (true) {
        system("cls");
        cout << "=> Text Editor <=\n" << endl;
        text.display();

        text.grammarCheck(dictionary);
        input = _getch();

        if (input == 27) {                  // Esc Key for Exit
            break;
        } 
        else if (input == 8) {              // Backspace for Deletion
            text.deleteNode();
        }
        else if (input == 4) {              // Ctrl + D for deleting character at cursor
            text.deleteAtCursor();
        } 
        else if (input == 13) {             // Enter Key for New Line
            text.newLine();
        } 
        else if (input == 19) {             // Ctrl + S for saving the file
            text.save();
        } 
        else if (input == 12) {             // Ctrl + L for loading the fileh
            text.load();
        } 
        else if (input == 72) {             // Arrow Up for moving cursor up
            text.moveUp();
        } 
        else if (input == 80) {             // Arrow Down for moving cursor down
            text.moveDown();
        } 
        else if (input == 75) {             // Arrow Left for moving cursor left
            text.moveLeft();
        } 
        else if (input == 77) {             // Arrow Right for moving cursor right
            text.moveRight();
        }
        else if (input == 9) {              // Tab for inserting text at cursor
            string textToInsert;
            cout << "\n\n\n\n\n=> Enter text: ";
            cin >> textToInsert;
            text.insertAtCursor(textToInsert);
        }
        else {                              // Base-case for Character Insertion
            text.addNode(input);
        }
    }
    return 0;
}