#ifndef CLASSES_H
#define CLASSES_H

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

inline string priorityToString(Priority p) {
    switch (p) {
        case LOW: return "LOW";
        case MEDIUM: return "MEDIUM";
        case HIGH: return "HIGH";
    }
    return "";
}

inline string statusToString(Status s) {
    switch (s) {
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
    AbstractItem(int id, const string& title, const string& description)
        : id(id), title(title), description(description) {
        if (id < 0) {
            throw invalid_argument("Invalid id");
        }

        if (title.empty()) {
            throw invalid_argument("Invalid title");
        }

        if (description.empty()) {
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

    Date(int day, int month, int year)
        : day(day), month(month), year(year) {
        if (day < 1 || day > 31) {
            throw invalid_argument("Invalid day");
        }

        if (month < 1 || month > 12) {
            throw invalid_argument("Invalid month");
        }

        if (year < 0) {
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
        if (id < 0) {
            throw invalid_argument("Invalid user id");
        }

        if (name.empty()) {
            throw invalid_argument("Name cannot be empty");
        }

        if (email.empty()) {
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
        if (priority < LOW || priority > HIGH) {
            throw invalid_argument("Invalid priority");
        }

        if (status < NEW || status > CANCELED) {
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
    Project(int id, const string& title, const string& description)
        : AbstractItem(id, title, description) {
        tasks.reserve(1000);
    }

    void addTask(const Task& task) {
        tasks.push_back(task);
    }

    void removeTask(int index) {
        if (index >= 0 && index < tasks.size()) {
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

#endif