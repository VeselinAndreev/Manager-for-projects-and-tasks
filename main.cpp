#include "classes.h"
#include "functions.h"

vector<User> users;
vector<Project> projects;

int main() {
    while (true) {
        printMenu();

        int choice;
        if (!(cin >> choice)) {
            cout << "Input error\n";
            break;
        }

        switch (choice) {
            case 1:
                addUser();
                break;
            case 2:
                createProject();
                break;
            case 3:
                addTaskToProject();
                break;
            case 4:
                showUsers();
                break;
            case 5:
                showProjects();
                break;
            case 6:
                assignTaskToUser();
                break;
            case 7:
                updateUser();
                break;
            case 8:
                deleteUser();
                break;
            case 9:
                updateProject();
                break;
            case 10:
                deleteProject();
                break;
            case 11:
                updateTask();
                break;
            case 12:
                deleteTask();
                break;
            case 13:
                sortTasksByPriority();
                break;
            case 14:
                sortTasksByStatus();
                break;
            case 15:
                showDeadlineWarnings();
                break;
            case 16:
                cout << "Exiting...\n";
                return 0;
        }
    }

    return 0;
}