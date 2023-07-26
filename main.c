#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_COURSES  20
#define MAX_STUDENTS  30
#define NUM_LEVELS  12
#define NAME_LEN  15
#define PHONE_LEN  11 // Includes null terminator '\0'
#define NUM_OF_CLASSES  10
#define LINE_LEN  200

struct course {
    char name[NAME_LEN];
    int grade;
    int available;
};

struct student {
    char first_name[NAME_LEN];
    char last_name[NAME_LEN];
    char phone_number[PHONE_LEN];
    int level;
    int class_number;
    struct course* courses; // Dynamic allocation for courses
};

struct school {
    struct student* DB[NUM_LEVELS][NUM_OF_CLASSES];
};

int main() {
    FILE *file = fopen("/home/saja/projects/students_with_class.txt", "r");
    if (file == NULL) {
        printf("Error opening the file.\n");
        return 1;
    }

    struct school mySchool;

    // Initialize the database to NULL
    for (int i = 0; i < NUM_LEVELS; i++) {
        for (int j = 0; j < NUM_OF_CLASSES; j++) {
            mySchool.DB[i][j] = NULL;
        }
    }

    // Read the file line by line
    char line[LINE_LEN];
    int num_students = 0;

    while (fgets(line, sizeof(line), file) != NULL) {
        // Create a new student instance dynamically
        struct student* new_student = malloc(sizeof(struct student));

        // Extract information from the line
        char first_name[NAME_LEN];
        char last_name[NAME_LEN];
        char phone_number[PHONE_LEN];
        int level, class_number;
        int grades[NUM_COURSES] = {};

        sscanf(line, "%s %s %s %d %d %d %d %d %d %d %d %d %d %d %d",
                    first_name, last_name, phone_number, &level, &class_number,
                    &grades[0], &grades[1], &grades[2], &grades[3], &grades[4], &grades[5], &grades[6], &grades[7], &grades[8], &grades[9]);


        // Store information in the student struct
        strncpy(new_student->first_name, first_name, NAME_LEN - 1);
        new_student->first_name[NAME_LEN - 1] = '\0';
        strncpy(new_student->last_name, last_name, NAME_LEN - 1);
        new_student->last_name[NAME_LEN - 1] = '\0';
        strncpy(new_student->phone_number, phone_number, PHONE_LEN - 1);
        new_student->phone_number[PHONE_LEN - 1] = '\0';
        new_student->level = level;
        new_student->class_number = class_number;

        // Allocate memory for courses
        new_student->courses = malloc(NUM_COURSES * sizeof(struct course));

        // Store the grades in the student struct
        for (int i = 0; i < NUM_COURSES - 10; i++) {
        new_student->courses[i].grade = grades[i];
        new_student->courses[i].available = 1;
        }

        // Store the new student in the school database
        mySchool.DB[level - 1][class_number - 1] = new_student;

        num_students++;
    }

    // Close the file
    fclose(file);

    // free the dynamically allocated memory for each student
    for (int i = 0; i < NUM_LEVELS; i++) {
        for (int j = 0; j < NUM_OF_CLASSES; j++) {
            if (mySchool.DB[i][j] != NULL) {
                free(mySchool.DB[i][j]->courses);
                free(mySchool.DB[i][j]);
            }
        }
    }

    return 0;
}
