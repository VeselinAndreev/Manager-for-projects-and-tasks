#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "classes.h"
#include <algorithm>

extern vector<User> users;
extern vector<Project> projects;

bool userIdExists(int id) {
    for (const auto& user : users) {
        if (user.getId() == id) {
            return true;
        }
    }
    return false;
}

bool projectIdExists(int id) {
    for (const auto& project : projects) {
        if (project.getId() == id) {
            return true;
        }
    }
    return false;
}

void printMenu() {
    cout << "\n===== MENU =====\n";
    cout << "1. Add User\n";
    cout << "2. Create Project\n";
    cout << "3. Add Task to Project\n";
    cout << "4. Show Users\n";
    cout << "5. Show Projects\n";
    cout << "6. Assign task to user\n";
    cout << "7. Update User\n";
    cout << "8. Delete User\n";
    cout << "9. Update Project\n";
    cout << "10. Delete Project\n";
    cout << "11. Update Task\n";
    cout << "12. Delete Task\n";
    cout << "13. Sort tasks by priority\n";
    cout << "14. Sort tasks by status\n";
    cout << "15. Exit\n";
    cout << "Choice: ";
}

void printUsers() {
    cout << "\nUsers:\n";
    for (int i = 0; i < users.size(); i++) {
        cout << i << ". " << users[i].getName() << endl;
    }
}

void printUsersWithEmail() {
    cout << "\nUsers:\n";
    for (int i = 0; i < users.size(); i++) {
        cout << i << ". " << users[i].getName() << " - " << users[i].getEmail() << endl;
    }
}

void printProjects() {
    cout << "\nProjects:\n";
    for (int i = 0; i < projects.size(); i++) {
        cout << i << ". " << projects[i].getTitle() << endl;
    }
}

void printTasks(vector<Task>& tasks) {
    cout << "\nTasks:\n";
    for (int i = 0; i < tasks.size(); i++) {
        cout << i << ". " << tasks[i].getTitle() << endl;
    }
}

void addUser() {
    int id;
    string name, email;

    cout << "User ID: ";
    cin >> id;
    cin.ignore();

    if (userIdExists(id)) {
        cout << "Error: User with this id already exists!\n";
        return;
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

void createProject() {
    int id;
    string title, description;

    cout << "Project ID: ";
    cin >> id;
    cin.ignore();

    if (projectIdExists(id)) {
        cout << "Project with this id already exists!\n";
        return;
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

void addTaskToProject() {
    if (projects.empty()) {
        cout << "No projects available!\n";
        return;
    }

    printProjects();

    int projectIndex;
    cout << "Choose project index: ";
    cin >> projectIndex;

    if (projectIndex < 0 || projectIndex >= projects.size()) {
        cout << "Invalid project index!\n";
        return;
    }

    int id, day, month, year, priorityChoice, statusChoice;
    string title, description;

    cout << "Task ID: ";
    cin >> id;
    cin.ignore();

    if (projects[projectIndex].taskIdExists(id)) {
        cout << "Task with this id already exists in this project!\n";
        return;
    }

    cout << "Task title: ";
    getline(cin, title);

    cout << "Task description: ";
    getline(cin, description);

    cout << "Deadline day month year: ";
    cin >> day >> month >> year;

    cout << "Priority (0-LOW, 1-MEDIUM, 2-HIGH): ";
    cin >> priorityChoice;

    try {
        Task task(id, title, description, Date(day, month, year), (Priority)priorityChoice, NEW);
        projects[projectIndex].addTask(task);
        cout << "Task added successfully!\n";
    }
    catch (exception& e) {
        cout << "Error: " << e.what() << endl;
    }
}

void showUsers() {
    if(users.empty()) {
        cout << "No users available!\n";
        return;
    }

    for(int i = 0; i < users.size(); i++) {
        cout << "\nUser " << i << ": ";
        cout << users[i] << endl;

        const vector<Task*>& tasks = users[i].getTasks();

        if (tasks.empty()) {
            cout << "  No tasks assigned.\n";
        }
        else {
            for (int j = 0; j < tasks.size(); j++) {
                cout << "  Task " << j << ": ";     
                cout << *tasks[j] << endl;
            }
        }
    }
}

void showProjects() {
    if (projects.empty()) {
        cout << "No projects available!\n";
        return;
    }

    for (int i = 0; i < projects.size(); i++) {
        cout << "\nProject " << i << ": ";
        cout << projects[i] << endl;

        vector<Task>& tasks = projects[i].getTasks();

        if (tasks.empty()) {
            cout << "  No tasks.\n";
        }
        else {
            for (int j = 0; j < tasks.size(); j++) {
                cout << "  Task " << j << ": ";
                cout << tasks[j] << endl;
            }
        }
    }
}

    void assignTaskToUser() {
        if (users.empty() || projects.empty()) {
            cout << "No users or projects available!\n";
            return;
        }

        printUsers();

        int userIndex;
        cout << "Choose user index: ";
        cin >> userIndex;

        if (userIndex < 0 || userIndex >= users.size()) {
            cout << "Invalid user index!\n";
            return;
        }

        User& chosenUser = users[userIndex];

        printProjects();

        int projectIndex;
        cout << "Choose project index: ";
        cin >> projectIndex;

        if (projectIndex < 0 || projectIndex >= projects.size()) {
            cout << "Invalid project index!\n";
            return;
        }

        Project& chosenProject = projects[projectIndex];
        vector<Task>& tasks = chosenProject.getTasks();

        if (tasks.empty()) {
            cout << "No tasks in this project!\n";
            return;
        }

        printTasks(tasks);

        int taskIndex;
        cout << "Choose task index: ";
        cin >> taskIndex;

        if (taskIndex < 0 || taskIndex >= tasks.size()) {
            cout << "Invalid task index!\n";
            return;
        }

        Task& chosenTask = tasks[taskIndex];

        if (!chosenTask.assignUser(&chosenUser)) {
            cout << "This user is already assigned to this task!\n";
            return;
        }

        if (!chosenUser.addTask(&chosenTask)) {
            cout << "This task is already assigned to this user!\n";
            return;
        }

        cout << "Task assigned successfully!\n";
    }

void updateUser() {
    if (users.empty()) {
        cout << "No users available!\n";
        return;
    }

    printUsersWithEmail();

    int userIndex;
    cout << "Choose user index: ";
    cin >> userIndex;

    if (userIndex < 0 || userIndex >= users.size()) {
        cout << "Invalid user index!\n";
        return;
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

void deleteUser() {
    if (users.empty()) {
        cout << "No users available!\n";
        return;
    }

    printUsers();

    int userIndex;
    cout << "Choose user index to delete: ";
    cin >> userIndex;

    if (userIndex < 0 || userIndex >= users.size()) {
        cout << "Invalid user index!\n";
        return;
    }

    for(auto* task : users[userIndex].getTasks()) {
        task->removeUser(&users[userIndex]);
    }

    users.erase(users.begin() + userIndex);
    cout << "User deleted successfully!\n";
}

void updateProject() {
    if (projects.empty()) {
        cout << "No projects available!\n";
        return;
    }

    printProjects();

    int projectIndex;
    cout << "Choose project index: ";
    cin >> projectIndex;

    if (projectIndex < 0 || projectIndex >= projects.size()) {
        cout << "Invalid project index!\n";
        return;
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

void deleteProject() {
    if (projects.empty()) {
        cout << "No projects available!\n";
        return;
    }

    printProjects();

    int projectIndex;
    cout << "Choose project index to delete: ";
    cin >> projectIndex;

    if (projectIndex < 0 || projectIndex >= projects.size()) {
        cout << "Invalid project index!\n";
        return;
    }

    vector<Task>& tasks = projects[projectIndex].getTasks();

    for (int i = 0; i < tasks.size(); i++) {
        Task* taskToDelete = &tasks[i];
        for (int j = 0; j < users.size(); j++) {
            users[j].removeTask(taskToDelete);
        }
    }

    projects.erase(projects.begin() + projectIndex);
    cout << "Project deleted successfully!\n";
}

void updateTask() {
    if (projects.empty()) {
        cout << "No projects available!\n";
        return;
    }

    printProjects();

    int projectIndex;
    cout << "Choose project index: ";
    cin >> projectIndex;

    if (projectIndex < 0 || projectIndex >= projects.size()) {
        cout << "Invalid project index!\n";
        return;
    }

    vector<Task>& tasks = projects[projectIndex].getTasks();
    if (tasks.empty()) {
        cout << "No tasks in this project!\n";
        return;
    }

    printTasks(tasks);

    int taskIndex;
    cout << "Choose task index: ";
    cin >> taskIndex;

    if (taskIndex < 0 || taskIndex >= tasks.size()) {
        cout << "Invalid task index!\n";
        return;
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

void deleteTask() {
    if (projects.empty()) {
        cout << "No projects available!\n";
        return;
    }

    printProjects();

    int projectIndex;
    cout << "Choose project index: ";
    cin >> projectIndex;

    if (projectIndex < 0 || projectIndex >= projects.size()) {
        cout << "Invalid project index!\n";
        return;
    }

    vector<Task>& tasks = projects[projectIndex].getTasks();
    if (tasks.empty()) {
        cout << "No tasks in this project!\n";
        return;
    }

    printTasks(tasks);

    int taskIndex;
    cout << "Choose task index to delete: ";
    cin >> taskIndex;

    if (taskIndex < 0 || taskIndex >= tasks.size()) {
        cout << "Invalid task index!\n";
        return;
    }

    Task* taskToDelete = &tasks[taskIndex];
    for (int i = 0; i < users.size(); i++) {
        users[i].removeTask(taskToDelete);
    }

    projects[projectIndex].removeTask(taskIndex);
    cout << "Task deleted successfully!\n";
}

bool comparePriorityLowToHigh(const Task& a, const Task& b) {
    return a.getPriority() < b.getPriority();
}

bool comparePriorityHighToLow(const Task& a, const Task& b) {
    return a.getPriority() > b.getPriority();
}

bool compareStatusNewToCanceled(const Task& a, const Task& b) {
    return a.getStatus() < b.getStatus();
}

bool compareStatusCanceledToNew(const Task& a, const Task& b) {
    return a.getStatus() > b.getStatus();
}   

void sortTasksByPriority() {
    if (projects.empty()) {
        cout << "No projects available!\n";
        return;
    }

    printProjects();

    int projectIndex;
    cout << "Choose project index: ";
    cin >> projectIndex;

    if (projectIndex < 0 || projectIndex >= projects.size()) {
        cout << "Invalid project index!\n";
        return;
    }

    vector<Task> tasks = projects[projectIndex].getTasks();

    if (tasks.empty()) {
        cout << "No tasks in this project!\n";
        return;
    }

    int choice;
    cout << "1. LOW to HIGH\n";
    cout << "2. HIGH to LOW\n";
    cout << "Choice: ";
    cin >> choice;

    if (choice == 1) {
        sort(tasks.begin(), tasks.end(), comparePriorityLowToHigh);
    }
    else if (choice == 2) {
        sort(tasks.begin(), tasks.end(), comparePriorityHighToLow);
    }
    else {
        cout << "Invalid choice!\n";
        return;
    }

    cout << "\nSorted tasks:\n";

    for (int i = 0; i < tasks.size(); i++) {
        cout << "Task " << i << ": ";
        cout << tasks[i] << endl;
    }
}

void sortTasksByStatus() {
    if (projects.empty()) {
        cout << "No projects available!\n";
        return;
    }

    printProjects();

    int projectIndex;
    cout << "Choose project index: ";
    cin >> projectIndex;

    if (projectIndex < 0 || projectIndex >= projects.size()) {
        cout << "Invalid project index!\n";
        return;
    }

    vector<Task> tasks = projects[projectIndex].getTasks();

    if (tasks.empty()) {
        cout << "No tasks in this project!\n";
        return;
    }

    int choice;
    cout << "1. NEW to CANCELED\n";
    cout << "2. CANCELED to NEW\n";
    cout << "Choice: ";
    cin >> choice;

    if (choice == 1) {
        sort(tasks.begin(), tasks.end(), compareStatusNewToCanceled);
    }
    else if (choice == 2) {
        sort(tasks.begin(), tasks.end(), compareStatusCanceledToNew);
    }
    else {
        cout << "Invalid choice!\n";
        return;
    }

    cout << "\nSorted tasks:\n";

    for (int i = 0; i < tasks.size(); i++) {
        cout << "Task " << i << ": ";
        cout << tasks[i] << endl;
    }
}

#endif