#include<iostream>
#include<string>
using namespace std;

const int MAX_USERS = 100;
const int MAX_TASKS = 100;

bool containsSpace(const string& s) {
    for (int i = 0; i < s.length(); ++i) {
        if (s[i] == ' ') return true;
    }
    return false;
}

class Task {
private:
    string title;
    string description;

public:
    Task() {}
    Task(string t, string d) {
        title = t;
        description = d;
    }

    void display() {
        cout << "Title: " << title << "\nDescription: " << description << "\n";
    }
};

class User {
protected:
    string username;
    string password;
    Task* tasks[MAX_TASKS];
    int taskCount;

public:
    User() {
        taskCount = 0;
    }

    User(string u, string p) {
        username = u;
        password = p;
        taskCount = 0;
    }

    string getUsername() const {
        return username;
    }

    bool validate(string u, string p) {
        return (username == u && password == p);
    }

    virtual void addTask() {
        if (taskCount >= MAX_TASKS) {
            cout << "Task limit reached.\n";
            return;
        }

        string title, desc;
        cin.ignore();
        cout << "Enter task title: ";
        getline(cin, title);
        cout << "Enter task description: ";
        getline(cin, desc);

        tasks[taskCount++] = new Task(title, desc);
        cout << "Task added!\n";
    }

    virtual void viewTasks() {
        if (taskCount == 0) {
            cout << "No tasks available.\n";
            return;
        }

        for (int i = 0; i < taskCount; ++i) {
            cout << i + 1 << ". ";
            tasks[i]->display();
            cout << "------------------\n";
        }
    }

    virtual void deleteTask() {
        if (taskCount == 0) {
            cout << "No tasks to delete.\n";
            return;
        }

        viewTasks();
        int index;
        cout << "Enter task number to delete: ";
        cin >> index;

        if (cin.fail() || index < 1 || index > taskCount) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid task number.\n";
            return;
        }

        delete tasks[index - 1];
        for (int i = index - 1; i < taskCount - 1; ++i) {
            tasks[i] = tasks[i + 1];
        }
        taskCount--;
        cout << "Task deleted.\n";
    }

    virtual ~User() {
        for (int i = 0; i < taskCount; ++i) {
            delete tasks[i];
        }
    }
};

class Admin : public User {
public:
    Admin(string u, string p) : User(u, p) {}
};

User* findUser(User* users[], int userCount, string username) {
    for (int i = 0; i < userCount; ++i) {
        if (users[i]->getUsername() == username) {
            return users[i];
        }
    }
    return NULL;
}

int main() {
    User* users[MAX_USERS];
    int userCount = 0;
    users[userCount++] = new Admin("admin", "1234"); 

    User* currentUser = NULL;
    int option;

    while (true) {
        cout << "\n==== Task Management System ====\n";
        cout << "1. Register\n2. Login\n3. Exit\nChoose option: ";
        cin >> option;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        cin.ignore(); 
        if (option == 1) {
            string uname, pwd;
            cout << "Enter new username (no spaces): ";
            getline(cin, uname);

            if (containsSpace(uname)) {
                cout << "Username cannot contain spaces.\n";
                continue;
            }

            if (findUser(users, userCount, uname) != NULL) {
                cout << "Username already exists. Try another.\n";
                continue;
            }

            cout << "Enter new password (no spaces): ";
            getline(cin, pwd);

            if (containsSpace(pwd)) {
                cout << "Password cannot contain spaces.\n";
                continue;
            }

            if (userCount < MAX_USERS) {
                users[userCount++] = new User(uname, pwd);
                cout << "Registration successful.\n";
            } else {
                cout << "User limit reached.\n";
            }

        } else if (option == 2) {
            string uname, pwd;
            cout << "Enter username (no spaces): ";
            getline(cin, uname);

            if (containsSpace(uname)) {
                cout << "Username cannot contain spaces.\n";
                continue;
            }

            cout << "Enter password (no spaces): ";
            getline(cin, pwd);

            if (containsSpace(pwd)) {
                cout << "Password cannot contain spaces.\n";
                continue;
            }

            User* user = findUser(users, userCount, uname);
            if (user != NULL && user->validate(uname, pwd)) {
                currentUser = user;
            } else {
                cout << "Invalid credentials.\n";
                continue;
            }

            int choice;
            do {
                cout << "\n--- Menu for " << currentUser->getUsername() << " ---\n";
                cout << "1. Add Task\n2. View Tasks\n3. Delete Task\n4. Logout\nEnter choice: ";
                cin >> choice;

                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "Invalid input. Please enter a number.\n";
                    continue;
                }

                switch (choice) {
                    case 1:
                        currentUser->addTask();
                        break;
                    case 2:
                        currentUser->viewTasks();
                        break;
                    case 3:
                        currentUser->deleteTask();
                        break;
                    case 4:
                        cout << "Logging out...\n";
                        break;
                    default:
                        cout << "Invalid choice.\n";
                }
            } while (choice != 4);

        } else if (option == 3) {
            cout << "Exiting system...\n";
            break;
        } else {
            cout << "Invalid option.\n";
        }
    }

   
    for (int i = 0; i < userCount; ++i) {
        delete users[i];
    }

    return 0;
}

