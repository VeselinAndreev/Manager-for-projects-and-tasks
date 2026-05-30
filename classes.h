#ifndef CLASSES_H
#define CLASSES_H

#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>
#include <ctime>

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

inline tm createDeadline(int day, int month, int year) {
    if (year < 0) {
        throw invalid_argument("Invalid year");
    }

    if (month < 1 || month > 12) {
        throw invalid_argument("Invalid month");
    }

    int daysInMonth[] = {
        31, 28, 31, 30,
        31, 30, 31, 31,
        30, 31, 30, 31
    };

    if ((year % 400 == 0) || (year % 4 == 0 && year % 100 != 0)) {
        daysInMonth[1] = 29;
    }

    if (day < 1 || day > daysInMonth[month - 1]) {
        throw invalid_argument("Invalid date");
    }

    tm deadline = {};
    deadline.tm_mday = day;
    deadline.tm_mon = month - 1;
    deadline.tm_year = year - 1900;

    return deadline;
}

inline ostream& operator<<(ostream& out, const tm& date) {
    out << date.tm_mday << "/" << (date.tm_mon + 1) << "/" << (date.tm_year + 1900);
    return out;
}

inline bool operator<(const tm& a, const tm& b) {
    if (a.tm_year != b.tm_year)
        return a.tm_year < b.tm_year;

    if (a.tm_mon != b.tm_mon)
        return a.tm_mon < b.tm_mon;

    return a.tm_mday < b.tm_mday;
}

inline int daysBetweenDates(const tm& from, const tm& to) {
    tm fromCopy = from;
    tm toCopy = to;
    //copies, because there is more data in tm and tm& from and tm& to can change
    fromCopy.tm_hour = 0;
    fromCopy.tm_min = 0;
    fromCopy.tm_sec = 0;

    toCopy.tm_hour = 0;
    toCopy.tm_min = 0;
    toCopy.tm_sec = 0;

    time_t fromTime = mktime(&fromCopy);
    time_t toTime = mktime(&toCopy);

    return (toTime - fromTime) / (60 * 60 * 24);
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

    bool addTask(Task* task) {
        for (auto* assignedTask : assignedTasks) {
            if (assignedTask == task) {
                return false;
            }
        }

        assignedTasks.push_back(task);
        return true;
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

    friend ostream& operator<<(ostream& out, const User& user) {
        out << "id = " << user.id << ", " << user.name << " - " << user.email;
        return out;
    }
};

class Task : public AbstractItem {
private:
    tm deadline;
    Priority priority;
    Status status;
    vector<User*> assignedUsers;

public:
    Task(int id, const string& title, const string& description, const tm& deadline, Priority priority, Status status)
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

    void updateDeadline(const tm& newDeadline) {
        deadline = newDeadline;
    }

    Priority getPriority() const {
        return priority;
    }

    Status getStatus() const {
        return status;
    }

    const tm& getDeadline() const {
        return deadline;
    }

    bool assignUser(User* user) {
        for (auto* assignedUser : assignedUsers) {
            if (assignedUser == user) {
                return false;
            }
        }

        assignedUsers.push_back(user);
        return true;
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
        time_t now = time(nullptr);
        //time from january 1 1970 is seconds
        tm* today = localtime(&now);
        //localtime turns time_t seconds into tm object and returns adress to it

        tm currentDate = {};
        currentDate.tm_mday = today->tm_mday;
        currentDate.tm_mon = today->tm_mon;
        //from 0
        currentDate.tm_year = today->tm_year;
        //from 1900
        return deadline < currentDate;
    }

    int daysUntilDeadline() const {
        time_t now = time(nullptr);
        tm* today = localtime(&now);

        tm currentDate = {};
        currentDate.tm_mday = today->tm_mday;
        currentDate.tm_mon = today->tm_mon;
        currentDate.tm_year = today->tm_year;

        return daysBetweenDates(currentDate, deadline);
    }

    friend ostream& operator<<(ostream& out, const Task& task) {
        out << "id = " << task.id << ", " << task.title << " - " << task.description << ", ";
        out << task.deadline;
        out << ", " << priorityToString(task.priority) << ", " << statusToString(task.status);
        return out;
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


    friend ostream& operator<<(ostream& out, const Project& project) {
        out << "id = " << project.id << ", " << project.title << " - " << project.description;
        return out;
    }
};

#endif