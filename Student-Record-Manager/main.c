// This is a student record manager program.
// Through which we can add/update/delete a student info.
// For that it uses struct handling.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_LENGTH 35
#define MAX_ROLLNUMBER 99999999
#define MIN_ROLLNUMBER 1
#define MAX_GRADE 12
#define MIN_GRADE 1
#define MAX_MARKS 100.00
#define MIN_MARKS 0.00
#define MAX_CHOICE 7

typedef struct
{
   char *name;
   int rollnumber;
   int grade;
   float marks;
} Student;

typedef struct
{
   Student *student;
   int count;
} Student_Manager;

void toUpper(char *name);

int input_int(const char *prompt, int min, int max);
float input_float(const char *prompt, float min, float max);

void input_buffer();
int find_student_rollNumber(Student_Manager *manager, int roll);
int find_student_name(Student_Manager *manager, char *name);
void delete_one(Student *student, int index, int newCount);
void free_student_names(Student *student, int index, int last);
int get_confirmation();
int is_yesorno_response(char *response);
int is_no_response(char *response);
void print_student_info(Student student);

int menu();
void new_student(Student_Manager *manager);
void view_info_all(Student_Manager *manager);
void view_info_one(Student_Manager *manager);
void update_info(Student_Manager *manager);
void delete(Student_Manager *manager);
void delete_all(Student_Manager *manager);

int main(void)
{
   printf("                ***STUDENT RECORD MANAGER***\n\n\n");

   Student_Manager manager;
   manager.student = NULL;
   manager.count = 0;

   int choice = menu();

   while (choice != 7)
   {
      switch (choice)
      {
      case 1:
         new_student(&manager);
         break;
      case 2:
         view_info_all(&manager);
         break;
      case 3:
         view_info_one(&manager);
         break;
      case 4:
         update_info(&manager);
         break;
      case 5:
         delete(&manager);
         break;
      case 6:
         delete_all(&manager);
         break;
      }
      printf("\n\n");
      choice = menu();
   }

   printf("Thank you for using our platform.\n");

   for (int i = 0; i < manager.count; i++)
   {
      free(manager.student[i].name);
      manager.student[i].name = NULL;
   }

   free(manager.student);
   manager.student = NULL;

   return 0;
}

void toUpper(char *name)
{
   for (int i = 0; name[i] != '\0'; i++)
   {
      name[i] = toupper((unsigned char)name[i]);
   }
}

int input_int(const char *prompt, int min, int max)
{
   int input;

   while (1)
   {
      printf("%s", prompt);

      if (!scanf("%d", &input))
      {
         printf("Invalid input. Please enter a number.\n");
         input_buffer();
      }
      else if (input < min || input > max)
      {
         printf("Out of range. Please enter between %d to %d.\n", min, max);
      }
      else
      {
         input_buffer();
         return input;
      }
      input_buffer();
   }
}

float input_float(const char *prompt, float min, float max)
{
   float input;

   while (1)
   {
      printf("%s", prompt);

      if (!scanf("%f", &input))
      {
         printf("Invalid input. Please enter a number.\n");
         input_buffer();
      }
      else if (input < min || input > max)
      {
         printf("Out of range. Please enter between %.2f to %.2f.\n", min, max);
      }
      else
      {
         input_buffer();
         return input;
      }
   }
}

void input_buffer()
{
   int c;

   while ((c = getchar()) != '\n' && c != EOF)
      ;
}

int find_student_rollNumber(Student_Manager *manager, int roll)
{
   for (int i = 0; i < manager->count; i++)
   {
      if (manager->student[i].rollnumber == roll)
         return i;
   }

   return -1;
}

int find_student_name(Student_Manager *manager, char *name)
{
   for (int i = 0; i < manager->count; i++)
   {
      if (!strcmp(manager->student[i].name, name))
      {
         return i;
      }
   }

   return -1;
}

void delete_one(Student *student, int index, int newCount)
{
   for (int i = index; i < (newCount); i++)
   {
      student[i].name = student[i + 1].name;
      student[i].rollnumber = student[i + 1].rollnumber;
      student[i].grade = student[i + 1].grade;
      student[i].marks = student[i + 1].marks;
   }
}

void free_student_names(Student *student, int index, int last)
{
   for (int i = index; i < last; i++)
   {
      free(student[i].name);
      student[i].name = NULL;
   }
}

int get_confirmation()
{
   char response[10];

   do
   {
      fgets(response, sizeof(response), stdin);
      response[strcspn(response, "\n")] = '\0';
      toUpper(response);
      if (!is_yesorno_response(response))
      {
         printf("Invalid input. Please answer with yes (y) or no (n).\n");
      }
   } while (!is_yesorno_response(response));

   if (is_no_response(response))
      return 0;
   return 1;
}

int is_yesorno_response(char *response)
{
   return (
       !strcmp(response, "Y") ||
       !strcmp(response, "YES") ||
       !strcmp(response, "N") ||
       !strcmp(response, "NO"));
}

int is_no_response(char *response)
{
   return (!strcmp(response, "N") || !strcmp(response, "NO"));
}

void print_student_info(Student student)
{
   printf("\nStudent name: %s\n", student.name ? student.name : "NULL");
   printf("Student roll number: %08d\n", student.rollnumber);
   printf("Student grade: %02d\n", student.grade);
   printf("Student marks: %02.2f\n", student.marks);
   printf("\n");
}

int menu()
{
   printf("What would you like to do?\n");
   printf("1.Add new student's data.\n");
   printf("2.View the data of all the students.\n");
   printf("3.View the data of a specific students.\n");
   printf("4.Update the data related to a student.\n");
   printf("5.Delete the data of a specific student.\n");
   printf("6.Delete all the data stored related to students.\n");
   printf("7.Exit the program.\n");

   return input_int("Enter your choice: ", 1, MAX_CHOICE);
}

void new_student(Student_Manager *manager)
{
   int numberOfStudents = input_int("Number of student to be filled: ", 0, INT_MAX);

   if (numberOfStudents == 0)
   {
      printf("No entry of student data done.\n");
      return;
   }

   printf("Do you want to add %d new student data?\n", numberOfStudents);
   printf("Answer in yes(y) or no(n).\n");

   if (!get_confirmation())
      return;

   Student *temp = realloc(manager->student, (manager->count + numberOfStudents) * sizeof(Student));

   if (temp == NULL)
   {
      printf("Memory allocation failed.\n");
      return;
   }

   int new_count = manager->count + numberOfStudents;

   for (int i = manager->count; i < new_count; i++)
   {
      printf("Enter the student name: ");

      char name[MAX_LENGTH];
      fgets(name, sizeof(name), stdin);
      name[strcspn(name, "\n")] = '\0';

      temp[i].name = malloc(strlen(name) + 1);

      if (temp[i].name == NULL)
      {
         printf("Failed to allocate memory for the name of the student.\n");

         free_student_names(temp, manager->count, i);
         free(temp);
         temp = NULL;
         return;
      }

      strcpy(temp[i].name, name);
      toUpper(temp[i].name);

      temp[i].rollnumber = input_int("Enter the roll number: ", MIN_ROLLNUMBER, MAX_ROLLNUMBER);

      if (find_student_rollNumber(manager, temp[i].rollnumber) != -1)
      {
         printf("Roll number already exists\n");

         free(temp[i].name);
         temp[i].name = NULL;
         continue;
      }

      temp[i].grade = input_int("Enter the grade: ", MIN_GRADE, MAX_GRADE);

      temp[i].marks = input_float("Enter the marks: ", MIN_MARKS, MAX_MARKS);

      printf("\n");
   }

   printf("Successfully added the data of %d student(s).\n", numberOfStudents);

   manager->student = temp;
   manager->count = new_count;
}

void view_info_all(Student_Manager *manager)
{
   if (manager->count == 0)
   {
      printf("There is no data stored of student.\n");
      return;
   }

   for (int i = 0; i < manager->count; i++)
   {
      print_student_info(manager->student[i]);
   }

   printf("Total number of student: %d\n", manager->count);
}

void view_info_one(Student_Manager *manager)
{
   if (manager->count == 0)
   {
      printf("There is no data stored of student.\n");
      return;
   }

   printf("How do you want to search the student?\n");
   printf("1. By roll number.\n");
   printf("2. By name.\n");

   int choice = input_int("Enter your choice: ", 1, 2);

   int index;

   if (choice == 1)
   {
      int rollNumber = input_int("Enter the roll number: ", MIN_ROLLNUMBER, MAX_ROLLNUMBER);

      index = find_student_rollNumber(manager, rollNumber);
   }
   else if (choice == 2)
   {
      char name[MAX_LENGTH];
      fgets(name, sizeof(name), stdin);
      name[strcspn(name, "\n")] = '\0';
      toUpper(name);

      index = find_student_name(manager, name);
   }

   if (index == -1)
   {
      printf("No student data found.\n");
      return;
   }

   print_student_info(manager->student[index]);
}

void update_info(Student_Manager *manager)
{
   if (manager->count == 0)
   {
      printf("There is no available data to update it.\n");
      return;
   }

   int numberOfStudentToUpdate = input_int("How many students data do you want to update: ", 0, INT_MAX);

   if (numberOfStudentToUpdate == 0)
   {
      printf("No student data will be updated.\n");
      return;
   }
   else if (numberOfStudentToUpdate > manager->count)
   {
      printf("More than the number of students data stored.\n");
      printf("Total number students data stored: %d\n", manager->count);
      return;
   }

   printf("Do you want to update %d number of students data?\n", numberOfStudentToUpdate);
   printf("Answer in yes(y) or no(n).\n");

   if (!get_confirmation())
      return;

   for (int i = 0; i < numberOfStudentToUpdate; i++)
   {
      printf("How do you want to search the student?\n");
      printf("1. By roll number.\n");
      printf("2. By name.\n");

      int choice = input_int("Enter your choice: ", 1, 2);

      int index;

      if (choice == 1)
      {
         int rollNumber = input_int("Enter the roll number: ", MIN_ROLLNUMBER, MAX_ROLLNUMBER);

         index = find_student_rollNumber(manager, rollNumber);
      }
      else if (choice == 2)
      {
         char name[MAX_LENGTH];
         fgets(name, sizeof(name), stdin);
         name[strcspn(name, "\n")] = '\0';
         toUpper(name);

         index = find_student_name(manager, name);
      }

      if (index == -1)
      {
         printf("No such student data found.\n");
         return;
      }

      printf("Student data to be updated.\n");
      print_student_info(manager->student[index]);

      manager->student[index].grade = input_int("Enter the new grade of the student: ", MIN_GRADE, MAX_GRADE);

      manager->student[index].marks = input_float("Enter the new marks of the student: ", MIN_MARKS, MAX_MARKS);

      printf("Do you want to update the name too?\n");
      printf("Answer in yes(y) or no(n).\n");

      if (!get_confirmation())
         return;

      char name[MAX_LENGTH];
      char *temp;
      printf("Enter the updated name of the student: ");

      fgets(name, sizeof(name), stdin);
      name[strcspn(name, "\n")] = '\0';
      toUpper(name);

      temp = realloc(manager->student[index].name, strlen(name) + 1);

      if (temp == NULL)
      {
         printf("Failed to alllocated memory for new name.\n");
         return;
      }

      manager->student[index].name = temp;
      strcpy(manager->student[index].name, name);

      printf("Updated data of the student.\n");
      print_student_info(manager->student[index]);
   }

   printf("Student data updated successfully.\n");
}

void delete(Student_Manager *manager)
{
   if (manager->count == 0)
   {
      printf("There is no data related to any student stored to delete it.\n");
      return;
   }

   int numberOfStudentToDelete = input_int("How many students data do you want to delete: ", 0, INT_MAX);

   if (numberOfStudentToDelete == 0)
   {
      printf("No student data will be deleted.\n");
      return;
   }
   else if (numberOfStudentToDelete > manager->count)
   {
      printf("More than the number of students data stored.\n");
      printf("Total number students data stored: %d\n", manager->count);
      return;
   }

   printf("Do you want to delete %d number of students data?\n", numberOfStudentToDelete);
   printf("Answer in yes(y) or no(n).\n");

   if (!get_confirmation())
      return;

   for (int i = 0; i < numberOfStudentToDelete; i++)
   {
      printf("How do you want to search the student?\n");
      printf("1. By roll number.\n");
      printf("2. By name.\n");

      int choice = input_int("Enter your choice: ", 1, 2);

      int index;
      int rollNumber;
      char name[MAX_LENGTH];

      if (choice == 1)
      {
         rollNumber = input_int("Enter the roll number: ", MIN_ROLLNUMBER, MAX_ROLLNUMBER);

         index = find_student_rollNumber(manager, rollNumber);
      }
      else if (choice == 2)
      {
         fgets(name, sizeof(name), stdin);
         name[strcspn(name, "\n")] = '\0';
         toUpper(name);

         index = find_student_name(manager, name);
      }

      if (index == -1)
      {
         printf("No such student data found.\n");
         return;
      }

      printf("Student data to be deleted.\n");
      print_student_info(manager->student[index]);

      printf("Do you want to delete this student data?\n");
      printf("Answer in yes(y) or no(n).\n");

      if (!get_confirmation())
         continue;

      int newCount = manager->count - 1;

      free(manager->student[index].name);
      delete_one(manager->student, index, newCount - 1);

      Student *temp = realloc(manager->student, newCount * sizeof(Student));

      if (temp == NULL)
      {
         printf("Memory expansion failed. Existing data remains unchanged.\n");
         return;
      }

      manager->student = temp;
      manager->count = newCount;

      if (choice == 1)
      {
         printf("Successfully deleted the data of the student whose roll number is: %d\n", rollNumber);
      }
      else
      {
         printf("Successfully deleted the data of the student whose name is: %s\n", name);
      }
   }

   printf("Total number of student: %d\n", manager->count);
}

void delete_all(Student_Manager *manager)
{
   if (manager->count == 0)
   {
      printf("There is no data stored related any student to delete it.\n");
      return;
   }

   printf("Do you want to delete all the data related to the students permanently.\n");

   if (!get_confirmation())
      return;

   free_student_names(manager->student, 0, manager->count);

   free(manager->student);
   manager->student = NULL;
   manager->count = 0;
}