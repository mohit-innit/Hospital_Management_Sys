#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX 100

struct Patient
{
    int id, age, admitted;
    char name[50], gender[10], disease[50];
};

struct Patient patients[MAX];
int count = 0;

/* Read one line and clear any extra characters */
void readLine(char text[], int size)
{
    int i, ch;

    fgets(text, size, stdin);

    for (i = 0; text[i] != '\0' && text[i] != '\n'; i++)
        ;

    if (text[i] == '\n')
        text[i] = '\0';
    else
        while ((ch = getchar()) != '\n' && ch != EOF)
            ;
}

/* Read a whole number; reject letters and numbers that are too large */
int getNumber(char prompt[], int *number)
{
    char text[50];
    int i;
    long value;

    while (1)
    {
        printf("%s", prompt);
        readLine(text, 50);

        if (text[0] == '\0')
        {
            printf("Enter a valid number.\n");
            continue;
        }

        value = 0;
        for (i = 0; text[i] != '\0'; i++)
        {
            if (!isdigit((unsigned char)text[i]))
                break;

            value = value * 10 + (text[i] - '0');
            if (value > INT_MAX)
                break;
        }

        if (text[i] == '\0' && value <= INT_MAX)
        {
            *number = (int)value;
            return 1;
        }

        printf("Enter a valid whole number.\n");
    }
}

/* Read text containing letters and spaces only */
void getWords(char prompt[], char text[], int size)
{
    int i, valid;

    while (1)
    {
        printf("%s", prompt);
        readLine(text, size);

        valid = text[0] != '\0';
        for (i = 0; text[i] != '\0'; i++)
            if (!isalpha((unsigned char)text[i]) && text[i] != ' ')
                valid = 0;

        if (valid)
            return;

        printf("Use letters and spaces only.\n");
    }
}

int findPatient(int id)
{
    int i;

    for (i = 0; i < count; i++)
        if (patients[i].id == id)
            return i;

    return -1;
}

/* Shared prompts for name, age, gender, disease and admitted status */
void getDetails(struct Patient *p)
{
    int i;

    getWords("Name: ", p->name, 50);

    do
    {
        getNumber("Age: ", &p->age);
        if (p->age <= 0)
            printf("Age must be positive.\n");
    } while (p->age <= 0);

    while (1)
    {
        printf("Gender (M/F/Other): ");
        readLine(p->gender, 10);

        for (i = 0; p->gender[i] != '\0'; i++)
            p->gender[i] = (char)tolower((unsigned char)p->gender[i]);

        if (strcmp(p->gender, "m") == 0)
        {
            strcpy(p->gender, "M");
            break;
        }
        if (strcmp(p->gender, "f") == 0)
        {
            strcpy(p->gender, "F");
            break;
        }
        if (strcmp(p->gender, "other") == 0)
        {
            strcpy(p->gender, "Other");
            break;
        }

        printf("Enter M, F, or Other.\n");
    }

    getWords("Disease: ", p->disease, 50);

    do
    {
        getNumber("Admitted? (1=Yes, 0=No): ", &p->admitted);
        if (p->admitted != 0 && p->admitted != 1)
            printf("Enter only 0 or 1.\n");
    } while (p->admitted != 0 && p->admitted != 1);
}

void registerPatient(void)
{
    struct Patient *p;

    if (count == MAX)
    {
        printf("Hospital record is full.\n");
        return;
    }

    p = &patients[count];

    while (1)
    {
        getNumber("Patient ID: ", &p->id);

        if (p->id <= 0)
            printf("ID must be positive.\n");
        else if (findPatient(p->id) != -1)
            printf("That ID already exists.\n");
        else
            break;
    }

    getDetails(p);
    count++;
    printf("Patient registered.\n");
}

void showPatient(int i)
{
    printf("\nID: %d | Name: %s | Age: %d\n",
           patients[i].id, patients[i].name, patients[i].age);
    printf("Gender: %s | Disease: %s | Admitted: %s\n",
           patients[i].gender, patients[i].disease,
           patients[i].admitted ? "Yes" : "No");
}

void searchPatient(void)
{
    int id, position;

    getNumber("Enter patient ID: ", &id);
    position = findPatient(id);

    if (position == -1)
        printf("Patient not found.\n");
    else
        showPatient(position);
}

void updatePatient(void)
{
    int id, position;

    getNumber("Enter patient ID to update: ", &id);
    position = findPatient(id);

    if (position == -1)
        printf("Patient not found.\n");
    else
    {
        getDetails(&patients[position]);
        printf("Patient updated.\n");
    }
}

void displayPatients(void)
{
    int i;

    if (count == 0)
        printf("No patient records.\n");

    for (i = 0; i < count; i++)
        showPatient(i);
}

void countAdmitted(void)
{
    int i, total = 0;

    for (i = 0; i < count; i++)
        if (patients[i].admitted == 1)
            total++;

    printf("Admitted patients: %d\n", total);
}

int main(void)
{
    int choice;

    do
    {
        printf("\n1.Register  2.Search  3.Update\n");
        printf("4.Display   5.Count admitted  0.Exit\n");
        getNumber("Choice: ", &choice);

        switch (choice)
        {
            case 1: registerPatient(); break;
            case 2: searchPatient(); break;
            case 3: updatePatient(); break;
            case 4: displayPatients(); break;
            case 5: countAdmitted(); break;
            case 0: printf("Goodbye.\n"); break;
            default: printf("Choose from 0 to 5.\n");
        }
    } while (choice != 0);

    return 0;
}