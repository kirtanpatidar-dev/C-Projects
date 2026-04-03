/* This is a program for storing contact information.
   It allows the user to add, view and delete contacts  from a file.
   It uses struct to store contact information and file handling to save the data.
   It stores the contact information in a file named "contact.dat" in the same directory as the program.
   Contact information it stored in binary format for efficient storage and retrieval.
   Both name and phone number are unique for each contact and the program will not allow duplicate entries for these fields.
   In future i will make it so phone or name will not be unique and 
   there can be multiple contacts with same name or phone number but for now it is unique for each contact.
   I have divided the code into different layers with diffrent use.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define FILE_NAME "contact.dat"
#define MAX_LENGTH 50
#define MAX_LENGTH_PHONE 15
#define MAX_LENGTH_CONFIRMATION 10
#define MAX_FUNCTION 7

typedef struct
{
    char name[MAX_LENGTH];
    char phone[MAX_LENGTH_PHONE];
    char email[MAX_LENGTH];
} Contact;

typedef struct {
    Contact *list;
    int count;
} ContactManager;

typedef enum {
    SUCCESS,
    FILE_ERROR,
    MEMORY_ERROR,
} Status;

void input_buffer();
void to_upper(char *s);

void input_string(const char *prompt, char *s, int length);
int input_int(const char *prompt, int min, int max);

int get_confirmation();
int is_yesorno_response(char *response);
int is_no_response(char *response);

void print_contact(const Contact *c);
void input_contact(const ContactManager *mgr, Contact *c);
void input_contact_for_editing(const ContactManager *mgr, Contact *c, const char *old_name, const char *old_phone);

int is_phone_valid(const char *phone);
int is_email_valid(const char *email);

void name_format(char *name);
void phone_format(char *phone);
void email_format(char *email);

int realloc_array(Contact **list, const int count);
int read_from_file(ContactManager *mgr);
int write_to_file(const ContactManager *mgr);

int find_by_name(const ContactManager *mgr, const char *name);
int find_by_phone(const ContactManager *mgr, const char *phone);

void view_by_name(const ContactManager *mgr, const char *name);
void view_by_phone(const ContactManager *mgr, const char *phone);

void edit_by_name(ContactManager *mgr, const char *name);
void edit_by_phone(ContactManager *mgr, const char *phone);

void delete_by_name(ContactManager *mgr, const char *name);
void delete_by_phone(ContactManager *mgr, const char *phone);

void edit_index(ContactManager *mgr, int index);
void delete_index(ContactManager *mgr, int index);

int menu();
void add_contact(ContactManager *mgr);
void view_contacts(ContactManager *mgr);
void edit_contact(ContactManager *mgr);
void delete_contact(ContactManager *mgr);
void see_all_contacts(ContactManager *mgr);
void delete_all_contacts(ContactManager *mgr);

int main(void)
{
    printf("                    ***CONTACT RECORD MANAGER***\n\n\n");

    int choice = menu();

    ContactManager cmgr = {NULL, 0};
    Status s = read_from_file(&cmgr);
    if(s == FILE_ERROR || s == MEMORY_ERROR){
        printf("Error loading contact.\n");
        return 0;
    }

    while (choice != MAX_FUNCTION)
    {
        switch (choice)
        {
        case 1:
            add_contact(&cmgr);
            break;
        case 2:
            view_contacts(&cmgr);
            break;
        case 3:
            edit_contact(&cmgr);
            break;
        case 4:
            delete_contact(&cmgr);
            break;
        case 5:
            see_all_contacts(&cmgr);
            break;
        case 6:
            delete_all_contacts(&cmgr);
            break;
        }

        printf("\n\n");
        choice = menu();
    }

    s = write_to_file(&cmgr);
    if(s == FILE_ERROR || s == MEMORY_ERROR){
        printf("Error loading contact.\n");
    }

    printf("Thank you for using our platform.\n");
    free(cmgr.list);

    return 0;
}

void input_buffer()
{
    int c;

    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

void to_upper(char *s)
{
    for (int i = 0; s[i]; i++)
    {
        s[i] = toupper(s[i]);
    }
}

void input_string(const char *prompt, char *s, int length)
{
    printf("%s", prompt);
    fgets(s, length, stdin);
    s[strcspn(s, "\n")] = '\0';
}

int input_int(const char *prompt, int min, int max)
{
    int num;

    while (1)
    {
        printf("%s", prompt);

        if (!scanf("%d", &num))
        {
            printf("Invalid input. Enter a whole number.\n");
            input_buffer();
        }
        else if (num < min || num > max)
        {
            printf("Out of range. Enter between %d to %d.\n", min, max);
        }
        else
        {
            input_buffer();
            return num;
        }
    }
}

int get_confirmation()
{
    char response[MAX_LENGTH_CONFIRMATION];

    do
    {
        fgets(response, sizeof(response), stdin);
        response[strcspn(response, "\n")] = '\0';
        to_upper(response);

        if (!is_yesorno_response(response))
        {
            printf("Invalid input. Please enter yes(y) or no(n): ");
        }
    } while (!is_yesorno_response(response));

    if (is_no_response(response))
    {
        return 0;
    }

    return 1;
}

int is_yesorno_response(char *response)
{
    if (!strcmp(response, "YES") || !strcmp(response, "Y") || !strcmp(response, "NO") || !strcmp(response, "N"))
    {
        return 1;
    }
    return 0;
}

int is_no_response(char *response)
{
    if (!strcmp(response, "NO") || !strcmp(response, "N"))
    {
        return 1;
    }
    return 0;
}

void print_contact(const Contact *c)
{
    printf("Name: %s\n", c->name);
    printf("Phone: %s\n", c->phone);
    printf("Email: %s\n", c->email);
    printf("\n");
}

void input_contact(const ContactManager *mgr, Contact *c)
{
    while (1)
    {
        input_string("Enter the name: ", c->name, MAX_LENGTH);
        name_format(c->name);

        if (find_by_name(mgr, c->name) != -1)
        {
            printf("Name already used for another contact.\n");
        }
        else
        {
            break;
        }
    }

    while (1)
    {
        input_string("Enter the phone number: ", c->phone, MAX_LENGTH_PHONE);
        phone_format(c->phone);

        if (!is_phone_valid(c->phone))
        {
            printf("Invalid input. Please enter correct number.\n");
        }
        else if (find_by_phone(mgr, c->phone) != -1)
        {
            printf("Phone number already used for another contact.\n");
        }
        else
        {
            break;
        }
    }

    while (1)
    {
        input_string("Enter the email(Optinal / Press enter to skip): ", c->email, MAX_LENGTH);
        email_format(c->email);

        if (!is_email_valid(c->email))
        {
            printf("Invalid input. Please enter correct email.\n");
        }
        else
        {
            break;
        }
    }
}

void input_contact_for_editing(const ContactManager *mgr, Contact *c, const char *old_name, const char *old_phone)
{
    while (1)
    {
        input_string("Enter the name: ", c->name, MAX_LENGTH);
        name_format(c->name);

        if (strcmp(c->name, old_name) != 0 && find_by_name(mgr, c->name) != -1)
        {
            printf("Name already used for another contact.\n");
        }
        else
        {
            break;
        }
    }

    while (1)
    {
        input_string("Enter the phone number: ", c->phone, MAX_LENGTH_PHONE);
        phone_format(c->phone);

        if (!is_phone_valid(c->phone))
        {
            printf("Invalid input. Please enter correct number.\n");
        }
        else if (strcmp(c->phone, old_phone) != 0 && find_by_phone(mgr, c->phone) != -1)
        {
            printf("Phone number already used for another contact.\n");
        }
        else
        {
            break;
        }
    }

    while (1)
    {
        input_string("Enter the email(Optinal / Press enter to skip): ", c->email, MAX_LENGTH);
        email_format(c->email);

        if (!is_email_valid(c->email))
        {
            printf("Invalid input. Please enter correct email.\n");
        }
        else
        {
            break;
        }
    }
}

int is_phone_valid(const char *phone)
{
    // Ckecking the length and if it starts with +91.
    if (strlen(phone) != 13 || strncmp(phone, "+91", 3) != 0)
    {
        return 0;
    }

    for (int i = 3; phone[i]; i++)
    {
        if (!isdigit(phone[i]))
        {
            return 0;
        }
    }

    return 1;
}

int is_email_valid(const char *email)
{
    if (strlen(email) == 0)
    {
        return 1;
    }

    if (!strchr(email, '@') || !strchr(email, '.'))
    {
        return 0;
    }

    return 1;
}

void name_format(char *name)
{
    for (int i = 0; name[i]; i++)
    {
        if (name[i] != ' ' && (i == 0 || name[i - 1] == ' '))
        {
            name[i] = toupper(name[i]);
        }
        else
        {
            name[i] = tolower(name[i]);
        }
    }
}

void phone_format(char *phone)
{
    char temp[MAX_LENGTH_PHONE];
    int j = 0;

    for (int i = 0; phone[i]; i++)
    {
        if (isdigit(phone[i]))
        {
            temp[j++] = phone[i];
        }
    }
    temp[j] = '\0';

    // Checking the length and if it is in the format of +91XXXXXXXXXX.
    if (strlen(temp) == 12 && !strncmp(temp, "91", 2))
    {
        snprintf(phone, MAX_LENGTH_PHONE, "+%s", temp);
    }
    // Checking the length and if it is in the format of 0XXXXXXXXXX.
    else if (strlen(temp) == 11 && !strncmp(temp, "0", 1))
    {
        snprintf(phone, MAX_LENGTH_PHONE, "+91%s", temp + 1);
    }
    // Checking the length and if it is in the format of XXXXXXXXXX.
    else if (strlen(temp) == 10)
    {
        snprintf(phone, MAX_LENGTH_PHONE, "+91%s", temp);
    }
    // If the phone number is not in the valid format, it will be set to an empty string.
    else
    {
        phone[0] = '\0';
    }
}

void email_format(char *email)
{
    for (int i = 0; email[i]; i++)
    {
        email[i] = tolower(email[i]);
    }
}

int realloc_array(Contact **list, const int count)
{
    if(count == 0){
        free(*list);
        *list = NULL;
        return SUCCESS;
    }
    Contact *arr = realloc(*list, count * sizeof(Contact));
    if(arr == NULL){
        return MEMORY_ERROR;
    }

    *list = arr;
    return SUCCESS;
}

int read_from_file(ContactManager *mgr)
{
    FILE *fp = fopen(FILE_NAME, "rb");
    if(fp == NULL){
        mgr->count = 0;
        mgr->list = NULL;
        return SUCCESS;
    }

    if(fread(&mgr->count, sizeof(int), 1, fp) != 1){
        fclose(fp);
        mgr->count = 0;
        mgr->list = NULL;
        return FILE_ERROR;
    }

    if(mgr->count <= 0){
        fclose(fp);
        mgr->count = 0;
        mgr->list = NULL;
        return SUCCESS;
    }

    mgr->list = malloc(mgr->count * sizeof(Contact));
    if(mgr->list == NULL){
        fclose(fp);
        mgr->count = 0;
        return MEMORY_ERROR;
    }

    if(fread(mgr->list, sizeof(Contact), mgr->count, fp) != mgr->count){
        fclose(fp);
        mgr->count = 0;
        mgr->list = NULL;
        return FILE_ERROR;
    }
    
    fclose(fp);
    return SUCCESS;
}

int write_to_file(const ContactManager *mgr)
{
    FILE *fp = fopen(FILE_NAME, "wb");
    if(fp == NULL){
        return FILE_ERROR;
    }

    if(fwrite(&mgr->count, sizeof(int), 1, fp) != 1){
        fclose(fp);
        return FILE_ERROR;
    }

    if(fwrite(mgr->list, sizeof(Contact), mgr->count, fp) != mgr->count){
        fclose(fp);
        return FILE_ERROR;
    }

    fclose(fp);
    return SUCCESS;
}

int find_by_name(const ContactManager *mgr, const char *name)
{
    for (int i = 0; i < mgr->count; i++)
    {
        if (!strcmp(mgr->list[i].name, name))
        {
            return i;
        }
    }

    return -1;
}

int find_by_phone(const ContactManager *mgr, const char *phone)
{
    for (int i = 0; i < mgr->count; i++)
    {
        if (!strcmp(mgr->list[i].phone, phone))
        {
            return i;
        }
    }

    return -1;
}

void view_by_name(const ContactManager *mgr, const char *name)
{
    int found = find_by_name(mgr, name);

    if (found == -1)
    {
        printf("Contact not found.\n");
        return;
    }
    print_contact(&mgr->list[found]);
}

void view_by_phone(const ContactManager *mgr, const char *phone)
{
    int found = find_by_phone(mgr, phone);

    if (found == -1)
    {
        printf("Contact not found.\n");
        return;
    }
    print_contact(&mgr->list[found]);
}

void edit_by_name(ContactManager *mgr, const char *name)
{
    printf("Contact to be edited.\n");
    int found = find_by_name(mgr, name);
    if (found == -1)
    {
        printf("Contact not found.\n");
        return;
    }
    
    edit_index(mgr, found);
}

void edit_by_phone(ContactManager *mgr, const char *phone)
{
    printf("Contact to be edited.\n");
    int found = find_by_phone(mgr, phone);
    if (found == -1)
    {
        printf("Contact not found.\n");
        return;
    }
    
    edit_index(mgr, found);
}

void delete_by_name(ContactManager *mgr, const char *name)
{
    int found = find_by_name(mgr, name);
    if (found == -1)
    {
        printf("Contact not found.\n");
        return;
    }
    
    delete_index(mgr, found);
}

void delete_by_phone(ContactManager *mgr, const char *phone)
{
    int found = find_by_phone(mgr, phone);
    if (found == -1)
    {
        printf("Contact not found.\n");
        return;
    }
    
    delete_index(mgr, found);
}

void edit_index(ContactManager *mgr, int index)
{
    print_contact(&mgr->list[index]);

    Contact contactEdited = mgr->list[index];
    printf("Enter the edited contact data.\n");
    input_contact_for_editing(mgr, &contactEdited, mgr->list[index].name, mgr->list[index].phone);

    printf("Edited contact data.\n");
    print_contact(&contactEdited);
    printf("Do you want to save this edited contact data?\n");
    printf("Answer in yes(y) or no(n): ");
    if (!get_confirmation())
    {
        printf("Edited contact data will not be saved and previous data will be kept saved as it is upon your choice.\n");
        return;
    }

    mgr->list[index] = contactEdited;
    printf("Edited contact successfully.\n");
}

void delete_index(ContactManager *mgr, int index)
{
    print_contact(&mgr->list[index]);

    printf("Do you want to delete this contact permanently?\n");
    printf("Answer in yes(y) or no(n): ");
    if (!get_confirmation())
    {
        printf("Contact data will be NOT deleted as upon your choice.\n");
        return;
    }

    for(int i = index; i < mgr->count - 1; i++){
        mgr->list[i] = mgr->list[i + 1];
    }

    realloc_array(&mgr->list, mgr->count - 1);
    mgr->count--;
    printf("Contact successfully deleted.\n");
}

int menu()
{
    printf("\nWhat would you like to do?\n");
    printf("1. Add new contact.\n");
    printf("2. View contact.\n");
    printf("3. Update contact.\n");
    printf("4. Delete contact.\n");
    printf("5. View all contact.\n");
    printf("6. Delete all contact.\n");
    printf("7. Exit the program.\n");

    return input_int("Enter your choice: ", 1, MAX_FUNCTION);
}

void add_contact(ContactManager *mgr)
{
    Contact c;
    input_contact(mgr, &c);

    printf("Contact to be saved is: \n");
    print_contact(&c);
    printf("Do you want to save this contact?\n");
    printf("Answer in yes(y) or no(n): ");
    if (!get_confirmation())
    {
        printf("Contact data will not be saved as per your choice.\n");
        return;
    }

    Status s = realloc_array(&mgr->list, mgr->count + 1);
    if(s == MEMORY_ERROR){
        printf("Error saving contact.\n");
        return;
    }

    mgr->list[mgr->count] = c;
    mgr->count++;
}

void view_contacts(ContactManager *mgr)
{
    if(mgr->count == 0){
        printf("No contact data stored yet.\n");
        return;
    }

    printf("How would you like to view the contact?\n");
    printf("1. With name.\n");
    printf("2. With phone number.\n");

    int choice = input_int("Enter your choice: ", 1, 2);

    switch (choice)
    {
    case 1:
        char name[MAX_LENGTH];
        input_string("Enter the name: ", name, MAX_LENGTH);
        name_format(name);
        view_by_name(mgr, name);
        break;
    case 2:
        char phone[MAX_LENGTH_PHONE];
        input_string("Enter the phone number: ", phone, MAX_LENGTH_PHONE);
        phone_format(phone);
        view_by_phone(mgr, phone);
        break;
    }
}

void edit_contact(ContactManager *mgr)
{
    if(mgr->count == 0){
        printf("No contact data stored yet.\n");
        return;
    }

    printf("How would you like to edit the contact?\n");
    printf("1. With name.\n");
    printf("2. With phone number.\n");

    int choice = input_int("Enter your choice: ", 1, 2);

    switch (choice)
    {
    case 1:
        char name[MAX_LENGTH];
        input_string("Enter the name: ", name, MAX_LENGTH);
        name_format(name);
        edit_by_name(mgr, name);
        break;

    case 2:
        char phone[MAX_LENGTH_PHONE];
        input_string("Enter the phone number: ", phone, MAX_LENGTH_PHONE);
        phone_format(phone);
        edit_by_phone(mgr, phone);
        break;
    }
}

void delete_contact(ContactManager *mgr)
{
    if(mgr->count == 0){
        printf("No contact data stored yet.\n");
        return;
    }

    printf("How would you like to delete the contact?\n");
    printf("1. With name.\n");
    printf("2. With phone number.\n");

    int choice = input_int("Enter your choice: ", 1, 2);

    switch (choice)
    {
    case 1:
        char name[MAX_LENGTH];
        input_string("Enter the name: ", name, MAX_LENGTH);
        name_format(name);
        delete_by_name(mgr, name);
        break;
    case 2:
        char phone[MAX_LENGTH_PHONE];
        input_string("Enter the phone number: ", phone, MAX_LENGTH_PHONE);
        phone_format(phone);
        delete_by_phone(mgr, phone);
        break;
    }
}

void see_all_contacts(ContactManager *mgr)
{
    if(mgr->count == 0){
        printf("No contact data stored yet.\n");
        return;
    }

    for (int i = 0; i < mgr->count; i++)
    {
        print_contact(&mgr->list[i]);
    }
}

void delete_all_contacts(ContactManager *mgr)
{
    if(mgr->count == 0){
        printf("No contact data stored yet.\n");
        return;
    }

    printf("Do you want to delete all your contacts permanently?\n");
    printf("Answer in yes(y) or no(n): ");
    if (!get_confirmation())
    {
        printf("Contact data will be NOT be deleted as upon your choice.\n");
        return;
    }
    
    printf("All contacts successfully deleted.\n");
    free(mgr->list);
    mgr->count = 0;
    mgr->list = NULL;
}