#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define NUM_COURSES  20
#define MAX_STUDENTS  30
#define NUM_LEVELS  12
#define NAME_LEN  15
#define PHONE_LEN  11
#define NUM_OF_CLASSES  10
#define LINE_LEN  200


// Simple XOR-based encryption/decryption function
void encrypt_decrypt(char *data, int data_len) {
   const char key = 'K'; // Secret key
   for (int i = 0; i < data_len; i++) {
       data[i] ^= key;
   }
}


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
   struct course* courses;
   struct student* next; // Pointer to the next student in the class
};


struct school {
   struct student* DB[NUM_LEVELS][NUM_OF_CLASSES];
};


// Function prototypes
void display_students(const struct school *school);
void add_student(struct school *school);
void delete_student(struct school *school);
void update_student_grade(const struct school *school);
void search_student(const struct school *school);
void top_students_by_course(const struct school *school);
void potential_dropouts(const struct school *school);
void calculate_average_by_class(const struct school *school);
void export_database(const struct school *school);
void display_student_courses(const struct course *courses);


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
           mySchool.DB[i][j] = NULL; // Set the head pointer of each class to NULL
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


       // Decrypt the data read from the file
       //encrypt_decrypt(first_name, NAME_LEN);
       //encrypt_decrypt(last_name, NAME_LEN);
       //encrypt_decrypt(phone_number, PHONE_LEN);


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
       for (int i = 0; i < NUM_COURSES; i++) {
           new_student->courses[i].grade = grades[i];
           new_student->courses[i].available = 1;
       }


       // Insert the new student into the linked list of the corresponding class
       new_student->next = mySchool.DB[level - 1][class_number - 1];
       mySchool.DB[level - 1][class_number - 1] = new_student;


       num_students++;
   }


   // Close the file
   fclose(file);


   int choice;
   do {
       printf("\nMenu:\n");
       printf("1. Display all students\n");
       printf("2. Add a new student\n");
       printf("3. Delete a student\n");
       printf("4. Update student grade\n");
       printf("5. Search student\n");
       printf("6. Top students by course\n");
       printf("7. Potential dropouts\n");
       printf("8. Calculate average by class\n");
       printf("9. Export database\n");
       printf("0. Exit\n");
       printf("Enter your choice: ");
       scanf("%d", &choice);


       switch (choice) {
           case 1:
               display_students(&mySchool);
               break;
           case 2:
               add_student(&mySchool);
               break;
           case 3:
               delete_student(&mySchool);
               break;
           case 4:
               update_student_grade(&mySchool);
               break;
           case 5:
               search_student(&mySchool);
               break;
           case 6:
               top_students_by_course(&mySchool);
               break;
           case 7:
               potential_dropouts(&mySchool);
               break;
           case 8:
               calculate_average_by_class(&mySchool);
               break;
           case 9:
               export_database(&mySchool);
               break;
           case 0:
               break;
           default:
               printf("Invalid choice. Please try again.\n");
       }
   } while (choice != 0);


   // Free the dynamically allocated memory for each student
for (int i = 0; i < NUM_LEVELS; i++) {
   for (int j = 0; j < NUM_OF_CLASSES; j++) {
       struct student *current_student = mySchool.DB[i][j];
       while (current_student != NULL) {
           struct student *next_student = current_student->next;
           free(current_student->courses);
           free(current_student);
           current_student = next_student;
       }
   }
}
   return 0;
}




// Function to display all students in the school
void display_students(const struct school *school) {
   for (int level = 0; level < NUM_LEVELS; level++) {
       for (int class_number = 0; class_number < NUM_OF_CLASSES; class_number++) {
           struct student *current_student = school->DB[level][class_number];
           while (current_student != NULL) {
               printf("Name: %s %s\n", current_student->first_name, current_student->last_name);
               printf("Phone: %s\n", current_student->phone_number);
               printf("Level: %d\n", current_student->level);
               printf("Class: %d\n", current_student->class_number);
               printf("Courses:\n");
               display_student_courses(current_student->courses);
               printf("----------------------------------------\n");


               current_student = current_student->next;
           }
       }
   }
}


// Helper function to display a student's courses
void display_student_courses(const struct course *courses) {
   for (int i = 0; i < NUM_COURSES; i++) {
       if (courses[i].available) {
           printf("%s: %d\n", courses[i].name, courses[i].grade);
       }
   }
}




// Function to add a new student to the school
void add_student(struct school *school) {
   char first_name[NAME_LEN];
   char last_name[NAME_LEN];
   char phone_number[PHONE_LEN];
   int level, class_number;


   // Prompt the user for the new student's information
   printf("Enter first name: ");
   scanf("%s", first_name);
   printf("Enter last name: ");
   scanf("%s", last_name);
   printf("Enter phone number: ");
   scanf("%s", phone_number);
   printf("Enter level: ");
   scanf("%d", &level);
   printf("Enter class number: ");
   scanf("%d", &class_number);


   // Create a new student instance dynamically
   struct student* new_student = malloc(sizeof(struct student));


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


   // Initialize grades and availability for courses
   for (int i = 0; i < NUM_COURSES; i++) {
       new_student->courses[i].grade = 0;
       new_student->courses[i].available = 1;
   }


   // Insert the new student into the linked list of the corresponding class
   new_student->next = school->DB[level - 1][class_number - 1];
   school->DB[level - 1][class_number - 1] = new_student;


   printf("Student added successfully.\n");
}






// Function to delete a student based on their phone number
void delete_student(struct school *school) {
   char phone_number[PHONE_LEN];


   // Prompt the user for the phone number of the student to delete
   printf("Enter the phone number of the student to delete: ");
   scanf("%s", phone_number);


   // Traverse the database to find the student with the specified phone number
   for (int level = 0; level < NUM_LEVELS; level++) {
       for (int class_number = 0; class_number < NUM_OF_CLASSES; class_number++) {
           struct student **current_student_ptr = &school->DB[level][class_number];
           while (*current_student_ptr != NULL) {
               if (strcmp((*current_student_ptr)->phone_number, phone_number) == 0) {
                   // Found the student with the specified phone number
                   struct student *temp = *current_student_ptr;
                   *current_student_ptr = (*current_student_ptr)->next; // Remove the student from the linked list
                   free(temp->courses);
                   free(temp);
                   printf("Student with phone number %s deleted successfully.\n", phone_number);
                   return;
               }
               current_student_ptr = &(*current_student_ptr)->next;
           }
       }
   }


   // If the student is not found, display an error message
   printf("Student with phone number %s not found.\n", phone_number);
}




// Function to update student grade based on their phone number
void update_student_grade(const struct school *school) {
   char phone_number[PHONE_LEN];


   // Prompt the user for the phone number of the student to update
   printf("Enter the phone number of the student to update: ");
   scanf("%s", phone_number);


   // Traverse the database to find the student with the specified phone number
   for (int level = 0; level < NUM_LEVELS; level++) {
       for (int class_number = 0; class_number < NUM_OF_CLASSES; class_number++) {
           struct student *current_student = school->DB[level][class_number];
           while (current_student != NULL) {
               if (strcmp(current_student->phone_number, phone_number) == 0) {
                   // Found the student with the specified phone number
                   printf("Student found:\n");
                   printf("Name: %s %s\n", current_student->first_name, current_student->last_name);
                   printf("Phone: %s\n", current_student->phone_number);
                   printf("Level: %d\n", current_student->level);
                   printf("Class: %d\n", current_student->class_number);
                   printf("Courses:\n");
                   display_student_courses(current_student->courses);


                   // Prompt the user to update the grades for the courses
                   for (int i = 0; i < NUM_COURSES; i++) {
                       if (current_student->courses[i].available) {
                           printf("Enter new grade for %s: ", current_student->courses[i].name);
                           scanf("%d", &current_student->courses[i].grade);
                       }
                   }


                   printf("Student grade updated successfully.\n");
                   return;
               }
               current_student = current_student->next;
           }
       }
   }


   // If the student is not found, display an error message
   printf("Student with phone number %s not found.\n", phone_number);
}




// Function to search for a student based on their phone number
void search_student(const struct school *school) {
   char phone_number[PHONE_LEN];


   // Prompt the user for the phone number of the student to search for
   printf("Enter the phone number of the student to search for: ");
   scanf("%s", phone_number);


   // Traverse the database to find the student with the specified phone number
   for (int level = 0; level < NUM_LEVELS; level++) {
       for (int class_number = 0; class_number < NUM_OF_CLASSES; class_number++) {
           struct student *current_student = school->DB[level][class_number];
           while (current_student != NULL) {
               if (strcmp(current_student->phone_number, phone_number) == 0) {
                   // Found the student with the specified phone number
                   printf("Student found:\n");
                   printf("Name: %s %s\n", current_student->first_name, current_student->last_name);
                   printf("Phone: %s\n", current_student->phone_number);
                   printf("Level: %d\n", current_student->level);
                   printf("Class: %d\n", current_student->class_number);
                   printf("Courses:\n");
                   display_student_courses(current_student->courses);
                   return;
               }
               current_student = current_student->next;
           }
       }
   }


   // If the student is not found, display an error message
   printf("Student with phone number %s not found.\n", phone_number);
}




// Function to find and display the top-performing students by course
void top_students_by_course(const struct school *school) {
   // Array to keep track of top-performing students for each course
   struct student *top_students[NUM_COURSES] = {NULL};


   // Traverse the database to find the top-performing students for each course
   for (int level = 0; level < NUM_LEVELS; level++) {
       for (int class_number = 0; class_number < NUM_OF_CLASSES; class_number++) {
           struct student *current_student = school->DB[level][class_number];
           while (current_student != NULL) {
               // Check each course for the current student
               for (int i = 0; i < NUM_COURSES; i++) {
                   // Only consider available courses
                   if (current_student->courses[i].available) {
                       // If the current student has a higher grade than the current top student for the course, update the top student
                       if (top_students[i] == NULL || current_student->courses[i].grade > top_students[i]->courses[i].grade) {
                           top_students[i] = current_student;
                       }
                   }
               }
               current_student = current_student->next;
           }
       }
   }


   // Display the information of top-performing students for each course
   for (int i = 0; i < NUM_COURSES; i++) {
       if (top_students[i] != NULL) {
           printf("Top student for course %s:\n", top_students[i]->courses[i].name);
           printf("Name: %s %s\n", top_students[i]->first_name, top_students[i]->last_name);
           printf("Phone: %s\n", top_students[i]->phone_number);
           printf("Level: %d\n", top_students[i]->level);
           printf("Class: %d\n", top_students[i]->class_number);
           printf("Grade: %d\n", top_students[i]->courses[i].grade);
           printf("----------------------------------------\n");
       } else {
           printf("No top student found for course %s.\n", top_students[i]->courses[i].name);
       }
   }
}




void potential_dropouts(const struct school *school) {
   int num_failing_courses_threshold = 3; // Adjust this threshold as needed


   printf("Potential Dropouts:\n");


   // Traverse the database to find potential dropouts
   for (int level = 0; level < NUM_LEVELS; level++) {
       for (int class_number = 0; class_number < NUM_OF_CLASSES; class_number++) {
           struct student *current_student = school->DB[level][class_number];
           while (current_student != NULL) {
               int num_failing_courses = 0;
              
               // Check each course for the current student
               for (int i = 0; i < NUM_COURSES; i++) {
                   // Only consider available courses
                   if (current_student->courses[i].available) {
                       // If the student has a failing grade (0-49) in the course, increment the count
                       if (current_student->courses[i].grade >= 0 && current_student->courses[i].grade < 50) {
                           num_failing_courses++;
                       }
                   }
               }


               // If the number of failing courses exceeds the threshold, consider the student a potential dropout
               if (num_failing_courses >= num_failing_courses_threshold) {
                   printf("Name: %s %s\n", current_student->first_name, current_student->last_name);
                   printf("Phone: %s\n", current_student->phone_number);
                   printf("Level: %d\n", current_student->level);
                   printf("Class: %d\n", current_student->class_number);
                   printf("Number of Failing Courses: %d\n", num_failing_courses);
                   printf("----------------------------------------\n");
               }


               current_student = current_student->next;
           }
       }
   }
}




// Function to calculate the average grade for each class in each level
void calculate_average_by_class(const struct school *school) {
   printf("Average Grade by Class:\n");


   // Traverse the database to calculate the average grade for each class
   for (int level = 0; level < NUM_LEVELS; level++) {
       for (int class_number = 0; class_number < NUM_OF_CLASSES; class_number++) {
           struct student *current_student = school->DB[level][class_number];
           int total_grades = 0;
           int num_students = 0;


           while (current_student != NULL) {
               // Calculate the total grade for the current student
               int student_total_grade = 0;
               for (int i = 0; i < NUM_COURSES; i++) {
                   if (current_student->courses[i].available) {
                       student_total_grade += current_student->courses[i].grade;
                   }
               }


               // Increment the total grades and number of students
               total_grades += student_total_grade;
               num_students++;


               current_student = current_student->next;
           }


           // Calculate the average grade for the class (rounded to the nearest integer)
           int class_average = (num_students > 0) ? (total_grades / num_students) : 0;


           // Display the result
           printf("Level %d, Class %d: Average Grade = %d\n", level + 1, class_number + 1, class_average);
       }
   }
}




void export_database(const struct school *school) {


}