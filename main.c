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
        //printf("%s", line);
        // Create a new student instance dynamically
        struct student* new_student = malloc(sizeof(struct student));

        // Extract information from the line
        char first_name[NAME_LEN];
        char last_name[NAME_LEN];
        char phone_number[PHONE_LEN];
        int level, class_number;

        int tt = sscanf(line, "%s %s %s %d %d", first_name, last_name, phone_number, &level, &class_number);
        printf("tt %d", tt);

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

        // Read grades of the courses for the student
        for (int i = 0; i < NUM_COURSES; i++) {
            int grade;
            //sscanf(line, "%d", &grade);
            int temp = sscanf(line, "%d", &grade);
            printf("the temp %d\n",temp);
            if (temp == 0) {
         //       printf("Error reading grades for student %s %s\n", first_name, last_name);
                break;
            }
            //printf("%d",grade);
            new_student->courses[i].grade = grade;
            new_student->courses[i].available = 1;
        }

        // Store the new student in the school database
        mySchool.DB[level - 1][class_number - 1] = new_student;

        num_students++;
    }

    // Close the file
    fclose(file);

    printf("%d\n",num_students);
    // print the db
    for (int i = 0; i < NUM_LEVELS; i++) {
        for (int j = 0; j < NUM_OF_CLASSES; j++) {
            struct student* current_student = mySchool.DB[i][j];
            if (current_student != NULL) {
                printf("Level: %d, Class: %d\n", current_student->level, current_student->class_number);
                printf("Name: %s %s\n", current_student->first_name, current_student->last_name);
                printf("Phone Number: %s\n", current_student->phone_number);
                printf("Courses:\n");
                for (int k = 0; k < NUM_COURSES; k++) {
                    printf("%s - Grade: %d\n", current_student->courses[k].name, current_student->courses[k].grade);
                }
                printf("------------------------\n");
            }
        }
    }



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
