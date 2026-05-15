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

string priorityToString(Priority p) {
    switch(p) {
        case LOW: return "LOW";
        case MEDIUM: return "MEDIUM";
        case HIGH: return "HIGH";
    }
    return "";
}

string statusToString(Status s) {
    switch(s) {
        case NEW: return "NEW";
        case IN_PROGRESS: return "IN_PROGRESS";
        case DONE: return "DONE";
        case CANCELED: return "CANCELED";
    }
    return "";
}

class AbstractItem {
protected:
    int id;
    string title;
    string description;
public:
    AbstractItem(int id, const string& title, const string& description) : id(id), title(title), description(description) {
        if(id < 0) {
            throw invalid_argument("Invalid id");
        }

        if(title.empty()) {
            throw invalid_argument("Invalid title");
        }

        if(description.empty()) {
            throw invalid_argument("Invalid description");
        }
    }

    virtual ~AbstractItem() {}
    virtual void getInfo() const = 0;

    string getTitle() const {
        return title;
    }

    string getDescription() const {
        return description;
    }

    int getId() const {
        return id;
    }

    void updateTitle(const string& newTitle) {
        if (newTitle.empty()) {
            throw invalid_argument("Title cannot be empty");
        }
        title = newTitle;
    }

    void updateDescription(const string& newDescription) {
        if (newDescription.empty()) {
            throw invalid_argument("Description cannot be empty");
        }
        description = newDescription;
    }
};

class Date {
public:
    int day;
    int month;
    int year;

    Date(int day, int month, int year) : day(day), month(month), year(year) {
        if(day < 1 || day > 31) {
            throw invalid_argument("Invalid day");
        }

        if(month < 1 || month > 12) {
            throw invalid_argument("Invalid month");
        }

        if(year < 0) {
            throw invalid_argument("Invalid year");
        }
    }

    void getInfo() const {
        cout << day << "/" << month << "/" << year;
    }
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
        : id(id), name(name), email(email) {
            if(id < 0) {
                throw invalid_argument("Invalid user id");
            }

            if(name.empty()) {
                throw invalid_argument("Name cannot be empty");
            }

            if(email.empty()) {
                throw invalid_argument("Email cannot be empty");
            }
        }

    int getId() const {
        return id;
    }

    string getName() const {
        return name;
    }

    string getEmail() const {
        return email;
    }

    void updateName(const string& newName) {
        if (newName.empty()) {
            throw invalid_argument("Name cannot be empty");
        }
        name = newName;
    }

    void updateEmail(const string& newEmail) {
        if (newEmail.empty()) {
            throw invalid_argument("Email cannot be empty");
        }
        email = newEmail;
    }

    void addTask(Task* task) {
        assignedTasks.push_back(task);
    }

    void removeTask(Task* task) {
        for (int i = 0; i < assignedTasks.size(); i++) {
            if (assignedTasks[i] == task) {
                assignedTasks.erase(assignedTasks.begin() + i);
                i--;
            }
        }
    }

    const vector<Task*>& getTasks() const {
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
        : AbstractItem(id, title, description), deadline(deadline), priority(priority), status(status) {
            if(priority < LOW || priority > HIGH) {
                throw invalid_argument("Invalid priority");
            }

            if(status < NEW || status > CANCELED) {
                throw invalid_argument("Invalid status");
            }
        }

    void changeStatus(Status status) {
        this->status = status;
    }

    void changePriority(Priority priority) {
        this->priority = priority;
    }

    void updateDeadline(const Date& newDeadline) {
        deadline = newDeadline;
    }

    Priority getPriority() const {
        return priority;
    }

    Status getStatus() const {
        return status;
    }

    const Date& getDeadline() const {
        return deadline;
    }

    void assignUser(User* user) {
        assignedUsers.push_back(user);
    }

    void removeUser(User* user) {
        for (int i = 0; i < assignedUsers.size(); i++) {
            if (assignedUsers[i] == user) {
                assignedUsers.erase(assignedUsers.begin() + i);
                i--;
            }
        }
    }

    bool isOverdue() {
        return false;
    }

    void getInfo() const override {
        cout << "id = " << id << ", " << title << " - " << description << ", ";
        deadline.getInfo();
        cout << ", " << priorityToString(priority) << ", " << statusToString(status) << endl;
    }
};


class Project : public AbstractItem {
private:
    vector<Task> tasks;

public:
    Project(int id, const string& title, const string& description) : AbstractItem(id, title, description) {
        tasks.reserve(1000);
    }

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

    bool taskIdExists(int id) const {
        for (const auto& task : tasks) {
            if (task.getId() == id) {
                return true;
            }
        }

        return false;
    }

    void getInfo() const override {
        cout << "id = " << id << ", " << title << " - " << description << endl;
    }
};

bool userIdExists(const vector<User>& users, int id) {
    for (const auto& user : users) {
        if (user.getId() == id) {
            return true;
        }
    }
    return false;
}

bool projectIdExists(const vector<Project>& projects, int id) {
    for (const auto& project : projects) {
        if (project.getId() == id) {
            return true;
        }
    }
    return false;
}

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
        cout << "6. Update User\n";
        cout << "7. Delete User\n";
        cout << "8. Update Project\n";
        cout << "9. Delete Project\n";
        cout << "10. Update Task\n";
        cout << "11. Delete Task\n";
        cout << "12. Exit\n";
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

            if (userIdExists(users, id)) {
                cout << "Error: User with this id already exists!\n";
                continue;
            }

            cout << "Name: ";
            getline(cin, name);

            cout << "Email: ";
            getline(cin, email);

            try {
                users.push_back(User(id, name, email));
                cout << "User added successfully!\n";
            }
            catch(exception& e) {
                cout << "Error: " << e.what() << endl;
            }
        }

        else if (choice == 2) {
            int id;
            string title, description;

            cout << "Project ID: ";
            cin >> id;

            cin.ignore();

            if (projectIdExists(projects, id)) {
                cout << "Project with this id already exists!\n";
                continue;
            }

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

            if (projects[projectIndex].taskIdExists(id)) {
                cout << "Task with this id already exists in this project!\n";
                continue;
            }

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
                projects[i].getInfo();

                vector<Task>& tasks = projects[i].getTasks();

                if (tasks.empty()) {
                    cout << "No tasks.\n";
                }
                else {
                    for (int j = 0; j < tasks.size(); j++) {
                        cout << "  Task " << j << ": ";
                        tasks[j].getInfo();
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
            if (users.empty()) {
                cout << "No users available!\n";
                continue;
            }

            cout << "\nUsers:\n";
            for (int i = 0; i < users.size(); i++) {
                cout << i << ". " << users[i].getName() << " - " << users[i].getEmail() << endl;
            }

            int userIndex;
            cout << "Choose user index: ";
            cin >> userIndex;

            if (userIndex < 0 || userIndex >= users.size()) {
                cout << "Invalid user index!\n";
                continue;
            }

            string name, email;
            cin.ignore();

            cout << "New name: ";
            getline(cin, name);

            cout << "New email: ";
            getline(cin, email);

            try {
                users[userIndex].updateName(name);
                users[userIndex].updateEmail(email);
                cout << "User updated successfully!\n";
            }
            catch (exception& e) {
                cout << "Error: " << e.what() << endl;
            }
        }

        else if (choice == 7) {
            if (users.empty()) {
                cout << "No users available!\n";
                continue;
            }

            cout << "\nUsers:\n";
            for (int i = 0; i < users.size(); i++) {
                cout << i << ". " << users[i].getName() << endl;
            }

            int userIndex;
            cout << "Choose user index to delete: ";
            cin >> userIndex;

            if (userIndex < 0 || userIndex >= users.size()) {
                cout << "Invalid user index!\n";
                continue;
            }

            for(auto* task : users[userIndex].getTasks()) {
                task->removeUser(&users[userIndex]);
            }

            users.erase(users.begin() + userIndex);
            cout << "User deleted successfully!\n";
        }

        else if (choice == 8) {
            if (projects.empty()) {
                cout << "No projects available!\n";
                continue;
            }

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

            string title, description;
            cin.ignore();

            cout << "New project title: ";
            getline(cin, title);

            cout << "New project description: ";
            getline(cin, description);

            try {
                projects[projectIndex].updateTitle(title);
                projects[projectIndex].updateDescription(description);
                cout << "Project updated successfully!\n";
            }
            catch (exception& e) {
                cout << "Error: " << e.what() << endl;
            }
        }

        else if (choice == 9) {
            if (projects.empty()) {
                cout << "No projects available!\n";
                continue;
            }

            cout << "\nProjects:\n";
            for (int i = 0; i < projects.size(); i++) {
                cout << i << ". " << projects[i].getTitle() << endl;
            }

            int projectIndex;
            cout << "Choose project index to delete: ";
            cin >> projectIndex;

            if (projectIndex < 0 || projectIndex >= projects.size()) {
                cout << "Invalid project index!\n";
                continue;
            }

            projects.erase(projects.begin() + projectIndex);
            cout << "Project deleted successfully!\n";
        }

        else if (choice == 10) {
            if (projects.empty()) {
                cout << "No projects available!\n";
                continue;
            }

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

            vector<Task>& tasks = projects[projectIndex].getTasks();
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

            int day, month, year, priorityChoice, statusChoice;
            string title, description;
            cin.ignore();

            cout << "New task title: ";
            getline(cin, title);

            cout << "New task description: ";
            getline(cin, description);

            cout << "New deadline day month year: ";
            cin >> day >> month >> year;

            cout << "New priority (0-LOW, 1-MEDIUM, 2-HIGH): ";
            cin >> priorityChoice;

            cout << "New status (0-NEW, 1-IN_PROGRESS, 2-DONE, 3-CANCELED): ";
            cin >> statusChoice;

            try {
                tasks[taskIndex].updateTitle(title);
                tasks[taskIndex].updateDescription(description);
                tasks[taskIndex].updateDeadline(Date(day, month, year));
                tasks[taskIndex].changePriority((Priority)priorityChoice);
                tasks[taskIndex].changeStatus((Status)statusChoice);
                cout << "Task updated successfully!\n";
            }
            catch (exception& e) {
                cout << "Error: " << e.what() << endl;
            }
        }

        else if (choice == 11) {
            if (projects.empty()) {
                cout << "No projects available!\n";
                continue;
            }

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

            vector<Task>& tasks = projects[projectIndex].getTasks();
            if (tasks.empty()) {
                cout << "No tasks in this project!\n";
                continue;
            }

            cout << "\nTasks:\n";
            for (int i = 0; i < tasks.size(); i++) {
                cout << i << ". " << tasks[i].getTitle() << endl;
            }

            int taskIndex;
            cout << "Choose task index to delete: ";
            cin >> taskIndex;

            if (taskIndex < 0 || taskIndex >= tasks.size()) {
                cout << "Invalid task index!\n";
                continue;
            }

            Task* taskToDelete = &tasks[taskIndex];
            for (int i = 0; i < users.size(); i++) {
                users[i].removeTask(taskToDelete);
            }

            projects[projectIndex].removeTask(taskIndex);
            cout << "Task deleted successfully!\n";
        }

        else if (choice == 12) {
            cout << "Exiting...\n";
            break;
        }

        else {
            cout << "Invalid choice!\n";
        }
    }

    return 0;
}