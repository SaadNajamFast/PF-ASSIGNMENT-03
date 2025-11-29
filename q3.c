#include <stdio.h>
#include <string.h>

struct Employee
{
    int id;
    char name[50];
    char designation[50];
    float salary;
};

void displayEmployees(struct Employee e[], int n)
{
    int i;

    printf("\nID\tName\t\tDesignation\tSalary\n");
    printf("-----------------------------------------------\n");

    for (i = 0; i < n; i++)
        printf("%d\t%s\t\t%s\t\t%.2f\n", e[i].id, e[i].name, e[i].designation, e[i].salary);
}

void findHighestSalary(struct Employee e[], int n)
{
    int i, index = 0;

    for (i = 1; i < n; i++)
        if (e[i].salary > e[index].salary)
            index = i;

    printf("\nHighest Salary Employee:\n");
    printf("ID: %d\n", e[index].id);
    printf("Name: %s\n", e[index].name);
    printf("Designation: %s\n", e[index].designation);
    printf("Salary: %.2f\n", e[index].salary);
}

void searchEmployee(struct Employee e[], int n)
{
    int choice, id, i;
    char name[50];

    printf("\nSearch by:\n1. ID\n2. Name\n");
    scanf("%d", &choice);

    if (choice == 1)
    {
        scanf("%d", &id);

        for (i = 0; i < n; i++)
            if (e[i].id == id)
            {
                printf("\nID: %d\nName: %s\nDesignation: %s\nSalary: %.2f\n",
                       e[i].id, e[i].name, e[i].designation, e[i].salary);
                return;
            }

        printf("Employee not found.\n");
    }

    else if (choice == 2)
    {
        scanf("%s", name);

        for (i = 0; i < n; i++)
            if (strcmp(e[i].name, name) == 0)
            {
                printf("\nID: %d\nName: %s\nDesignation: %s\nSalary: %.2f\n",
                       e[i].id, e[i].name, e[i].designation, e[i].salary);
                return;
            }

        printf("Employee not found.\n");
    }
}

int main()
{
    int n, i;

    printf("Enter number of employees: ");
    scanf("%d", &n);

    struct Employee e[n];

    for (i = 0; i < n; i++)
    {
        printf("\nEmployee %d\n", i + 1);

        printf("ID: ");
        scanf("%d", &e[i].id);

        printf("Name: ");
        scanf("%s", e[i].name);

        printf("Designation: ");
        scanf("%s", e[i].designation);

        printf("Salary: ");
        scanf("%f", &e[i].salary);
    }

    displayEmployees(e, n);
    findHighestSalary(e, n);
    searchEmployee(e, n);

    return 0;
}