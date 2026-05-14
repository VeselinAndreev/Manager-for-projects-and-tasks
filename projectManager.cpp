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
    virtual string getTitle() const = 0;
    virtual string getDescription() const = 0;
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
    vector<Task*> assignedTasks;

public:
    User(int id, const string& name, const string& email)
        : id(id), name(name), email(email) {}

    string getName() const {
        return name;
    }

    void addTask(Task* task) {
        assignedTasks.push_back(task);
    }

    vector<Task*> getTasks() const {
        return assignedTasks;
    }
};

class Task : public AbstractItem {
private:
    Date deadline;
    Priority priority;
    Status status;
    vector<User*> assignedUsers;

public:
    Task(int id, const string& title, const string& description, const Date& deadline, Priority priority, Status status)
        : AbstractItem(id, title, description), deadline(deadline), priority(priority), status(status) {}

    void changeStatus(Status status) {
        this->status = status;
    }

    void changePriority(Priority priority) {
        this->priority = priority;
    }

    void assignUser(User* user) {
        assignedUsers.push_back(user);
    }

    bool isOverdue() {
        return false;
    }

    string getTitle() const override {
        return title;
    }

    string getDescription() const override {
        return description;
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

    vector<Task>& getTasks() {
        return tasks;
    }

    string getTitle() const override {
        return title;
    }

    string getDescription() const override {
        return description;
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
        cout << "5. Assign task to user\n";
        cout << "6. Exit\n";
        cout << "Choice: ";

        int choice;

        if (!(cin >> choice)) {
            cout << "Input error\n";
            break;
        }

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
                cout << i << ". " << projects[i].getTitle() << endl;
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

            try {
                Task task(id, title, description, Date(day, month, year), (Priority)priorityChoice, (Status)statusChoice);

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
                cout << projects[i].getTitle() << " - " << projects[i].getDescription() << endl;

                vector<Task>& tasks = projects[i].getTasks();

                if (tasks.empty()) {
                    cout << "No tasks.\n";
                }
                else {
                    for (int j = 0; j < tasks.size(); j++) {
                        cout << "  Task " << j << ": ";
                        cout << tasks[j].getTitle() << " - " << tasks[j].getDescription() << endl;
                    }
                }
            }
        }

        else if (choice == 5) {

        if (users.empty() || projects.empty()) {
            cout << "No users or projects available!\n";
            continue;
        }

        cout << "\nUsers:\n";

        for (int i = 0; i < users.size(); i++) {
            cout << i << ". " << users[i].getName() << endl;
        }

        int userIndex;

        cout << "Choose user index: ";
        cin >> userIndex;

        if (userIndex < 0 || userIndex >= users.size()) {
            cout << "Invalid user index!\n";
            continue;
        }

        User& chosenUser = users[userIndex];

        cout << "\nProjects:\n";

        for (int i = 0; i < projects.size(); i++) {
            cout << i << ". " << projects[i].getTitle() << endl;
        }

        int projectIndex;

        cout << "Choose project index: ";
        cin >> projectIndex;

        if (projectIndex < 0 || projectIndex >= projects.size()) {
            cout << "Invalid project index!\n";
            continue;
        }

        Project& chosenProject = projects[projectIndex];

        vector<Task>& tasks = chosenProject.getTasks();

        if (tasks.empty()) {
            cout << "No tasks in this project!\n";
            continue;
        }

        cout << "\nTasks:\n";

        for (int i = 0; i < tasks.size(); i++) {
            cout << i << ". " << tasks[i].getTitle() << endl;
        }

        int taskIndex;

        cout << "Choose task index: ";
        cin >> taskIndex;

        if (taskIndex < 0 || taskIndex >= tasks.size()) {
            cout << "Invalid task index!\n";
            continue;
        }

        Task& chosenTask = tasks[taskIndex];

        chosenTask.assignUser(&chosenUser);

        chosenUser.addTask(&chosenTask);

        cout << "Task assigned successfully!\n";
    }

        else if (choice == 6) {
            cout << "Exiting...\n";
            break;
        }

        else {
            cout << "Invalid choice!\n";
        }
    }

    return 0;
}