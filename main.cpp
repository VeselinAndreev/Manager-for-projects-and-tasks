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
                showProjects();
                break;
            case 5:
                assignTaskToUser();
                break;
            case 6:
                updateUser();
                break;
            case 7:
                deleteUser();
                break;
            case 8:
                updateProject();
                break;
            case 9:
                deleteProject();
                break;
            case 10:
                updateTask();
                break;
            case 11:
                deleteTask();
                break;
            case 12:
                cout << "Exiting...\n";
                return 0;
            default:
                cout << "Invalid choice!\n";
        }
    }

    return 0;
}