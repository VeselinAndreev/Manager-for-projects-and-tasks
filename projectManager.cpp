#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>
using namespace std;

enum Priority {
    LOW,
    MEDIUM,
    HIGH
};

enum Status {
    NEW,
    IN_PROGRESS,
    DONE,
    CANCELED
};

class AbstractItem {
protected:
    int id;
    string title;
    string description;
public:
    AbstractItem() {
        id = 0;
    }
    AbstractItem(int id, const string& title, const string& description) : id(id), title(title), description(description) {
        if(id < 0 || title.empty() || description.empty()) {
            throw invalid_argument("Invalid");
        }
    }
    virtual ~AbstractItem() {}
    virtual string getInfo() const = 0;
};

class Date {
public:
    int day;
    int month;
    int year;

    Date(int day, int month, int year) : day(day), month(month), year(year) {}
};

class Task;

class User {
private:
    int id;
    string name;
    string email;

public:
    User(int id, const string& name, const string& email) : id(id), name(name), email(email) {}

    vector<Task*> getTasks() {
        return {};
    }
};

class Task : public AbstractItem {
private:
    Date deadline;
    Priority priority;
    Status status;
    User* assignedUser;

public:
    Task(int id, const string& title, const string& description, const Date& deadline, Priority priority, Status status, User* assignedUser)
        : AbstractItem(id, title, description), deadline(deadline), priority(priority), status(status), assignedUser(assignedUser) {}

    void changeStatus(Status status) {
        this->status = status;
    }

    void changePriority(Priority priority) {
        this->priority = priority;
    }

    bool isOverdue() {
        return false;
    }

    string getInfo() const override {
        return title + " - " + description;
    }
};

class Project : public AbstractItem {
private:
    vector<Task> tasks;

public:
    Project(int id, const string& title, const string& description) : AbstractItem(id, title, description) {}

    void addTask(const Task& task) {
        tasks.push_back(task);
    }

    void removeTask(int index) {
        if(index >= 0 && index < tasks.size()) {
            tasks.erase(tasks.begin() + index);
        }
    }

    vector<Task> getTasks() {
        return tasks;
    }

    string getInfo() const override {
        return title + " - " + description;
    }
};

int main() {
    vector<User> users;
    vector<Project> projects;

    while (1) {
        cout << "\n===== MENU =====\n";
        cout << "1. Add User\n";
        cout << "2. Create Project\n";
        cout << "3. Add Task to Project\n";
        cout << "4. Show Projects\n";
        cout << "5. Exit\n";
        cout << "Choice: ";

        int choice;
        cin >> choice;

        if (choice == 1) {
            int id;
            string name, email;

            cout << "User ID: ";
            cin >> id;

            cin.ignore();

            cout << "Name: ";
            getline(cin, name);

            cout << "Email: ";
            getline(cin, email);

            users.push_back(User(id, name, email));

            cout << "User added successfully!\n";
        }

        else if (choice == 2) {
            int id;
            string title, description;

            cout << "Project ID: ";
            cin >> id;

            cin.ignore();

            cout << "Project title: ";
            getline(cin, title);

            cout << "Project description: ";
            getline(cin, description);

            try {
                projects.push_back(Project(id, title, description));
                cout << "Project created successfully!\n";
            }
            catch (exception& e) {
                cout << "Error: " << e.what() << endl;
            }
        }

        else if (choice == 3) {
            if (projects.empty()) {
                cout << "No projects available!\n";
                continue;
            }

            int projectIndex;

            cout << "\nProjects:\n";
            for (int i = 0; i < projects.size(); i++) {
                cout << i << ". " << projects[i].getInfo() << endl;
            }

            cout << "Choose project index: ";
            cin >> projectIndex;

            if (projectIndex < 0 || projectIndex >= projects.size()) {
                cout << "Invalid project index!\n";
                continue;
            }

            int id, day, month, year, priorityChoice, statusChoice, userIndex;
            string title, description;

            cout << "Task ID: ";
            cin >> id;

            cin.ignore();

            cout << "Task title: ";
            getline(cin, title);

            cout << "Task description: ";
            getline(cin, description);

            cout << "Deadline day month year: ";
            cin >> day >> month >> year;

            cout << "Priority (0-LOW, 1-MEDIUM, 2-HIGH): ";
            cin >> priorityChoice;

            cout << "Status (0-NEW, 1-IN_PROGRESS, 2-DONE, 3-CANCELED): ";
            cin >> statusChoice;

            User* assignedUser = nullptr;

            if (!users.empty()) {
                cout << "\nUsers:\n";

                for (int i = 0; i < users.size(); i++) {
                    cout << i << ". User\n";
                }

                cout << "Choose user index: ";
                cin >> userIndex;

                if (userIndex >= 0 && userIndex < users.size()) {
                    assignedUser = &users[userIndex];
                }
            }

            try {
                Task task(id, title, description, Date(day, month, year), (Priority)priorityChoice, (Status)statusChoice, assignedUser);

                projects[projectIndex].addTask(task);

                cout << "Task added successfully!\n";
            }
            catch (exception& e) {
                cout << "Error: " << e.what() << endl;
            }
        }

        else if (choice == 4) {
            if (projects.empty()) {
                cout << "No projects available!\n";
                continue;
            }

            for (int i = 0; i < projects.size(); i++) {
                cout << "\nProject " << i << ": ";
                cout << projects[i].getInfo() << endl;

                vector<Task> tasks = projects[i].getTasks();

                if (tasks.empty()) {
                    cout << "No tasks.\n";
                }
                else {
                    for (int j = 0; j < tasks.size(); j++) {
                        cout << "  Task " << j << ": ";
                        cout << tasks[j].getInfo() << endl;
                    }
                }
            }
        }

        else if (choice == 5) {
            cout << "Exiting...\n";
            break;
        }

        else {
            cout << "Invalid choice!\n";
        }
    }

    return 0;
}