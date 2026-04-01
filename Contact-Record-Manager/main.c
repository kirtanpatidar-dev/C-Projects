/* This is a program for storing contact information.
   It allows the user to add, view and delete contacts  from a file.
   It uses struct to store contact information and file handling to save the data.
   It stores the contact information in a file named "contact.dat" in the same directory as the program.
   Contact information it stored in binary format for efficient storage and retrieval.
   Both name and phone number are unique for each contact and the program will not allow duplicate entries for these fields.
   In future i will make it so phone or name will not be unique and there can be multiple contacts with same name or phone number but for now it is unique for each contact.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

void input_buffer();
void to_upper(char *s);

void input_string(const char *prompt, char *s, int length);
int input_int(const char *prompt, int min, int max);

int get_confirmation();
int is_yesorno_response(char *response);
int is_no_response(char *response);

void print_contact(const Contact *c);
void input_contact(Contact *c);
void input_contact_for_editing(Contact *c, const char *old_name, const char *old_phone);

int is_phone_valid(const char *phone);
int is_email_valid(const char *email);

void name_format(char *name);
void phone_format(char *phone);
void email_format(char *email);

int find_by_name(const char *name);
int find_by_phone(const char *phone);

void view_by_name(const char *name);
void view_by_phone(const char *phone);

void edit_by_name(const char *name);
void edit_by_phone(const char *phone);

void delete_by_name(const char *name);
void delete_by_phone(const char *phone);

int menu();
void add_contact();
void view_contacts();
void edit_contact();
void delete_contact();
void see_all_contacts();
void delete_all_contacts();

int main(void)
{
    printf("                    ***CONTACT RECORD MANAGER***\n\n\n");

    int choice = menu();

    while (choice != MAX_FUNCTION)
    {
        switch (choice)
        {
        case 1:
            add_contact();
            break;
        case 2:
            view_contacts();
            break;
        case 3:
            edit_contact();
            break;
        case 4:
            delete_contact();
            break;
        case 5:
            see_all_contacts();
            break;
        case 6:
            delete_all_contacts();
            break;
        }

        printf("\n\n");
        choice = menu();
    }

    printf("Thank you for using our platform.\n");

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

void input_contact(Contact *c)
{
    while (1)
    {
        input_string("Enter the name: ", c->name, MAX_LENGTH);
        name_format(c->name);

        if (find_by_name(c->name))
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
        else if (find_by_phone(c->phone))
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

void input_contact_for_editing(Contact *c, const char *old_name, const char *old_phone)
{
    while (1)
    {
        input_string("Enter the name: ", c->name, MAX_LENGTH);
        name_format(c->name);

        if (strcmp(c->name, old_name) != 0 && find_by_name(c->name))
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
        else if (strcmp(c->phone, old_phone) != 0 && find_by_phone(c->phone))
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

int find_by_name(const char *name)
{
    FILE *cI = fopen("contact.dat", "rb");
    if (cI == NULL)
        return 0;

    Contact c;

    while (fread(&c, sizeof(c), 1, cI) == 1)
    {
        if (!strcmp(c.name, name))
        {
            fclose(cI);
            return 1;
        }
    }

    fclose(cI);
    return 0;
}

int find_by_phone(const char *phone)
{
    FILE *cI = fopen("contact.dat", "rb");
    if (cI == NULL)
        return 0;

    Contact c;

    while (fread(&c, sizeof(c), 1, cI) == 1)
    {
        if (!strcmp(c.phone, phone))
        {
            fclose(cI);
            return 1;
        }
    }

    fclose(cI);
    return 0;
}

void view_by_name(const char *name)
{
    FILE *contactInfo = fopen("contact.dat", "rb");

    if (contactInfo == NULL)
    {
        printf("Failed to open the file. Try next time.\n");
        return;
    }

    Contact contact;
    int found = 0;

    while (fread(&contact, sizeof(contact), 1, contactInfo) == 1)
    {
        if (!strcmp(name, contact.name))
        {
            print_contact(&contact);
            found = 1;
            break;
        }
    }

    if (found == 0)
    {
        printf("Contact not found.\n");
    }

    fclose(contactInfo);
    contactInfo = NULL;
}

void view_by_phone(const char *phone)
{
    FILE *contactInfo = fopen("contact.dat", "rb");

    if (contactInfo == NULL)
    {
        printf("Failed to open the file. Try next time.\n");
        return;
    }

    Contact contact;
    int found = 0;

    while (fread(&contact, sizeof(contact), 1, contactInfo) == 1)
    {
        if (!strcmp(phone, contact.phone))
        {
            print_contact(&contact);
            found = 1;
            break;
        }
    }

    if (found == 0)
    {
        printf("Contact not found.\n");
    }

    fclose(contactInfo);
    contactInfo = NULL;
}

void edit_by_name(const char *name)
{
    FILE *contactInfo = fopen("contact.dat", "rb");
    FILE *temp = fopen("temp.dat", "wb");

    if (contactInfo == NULL || temp == NULL)
    {
        printf("Failed to open the file. Try next time.\n");
        return;
    }

    printf("Contact to be edited.\n");
    if (!find_by_name(name))
    {
        printf("Editing the contact failed.\n");
        fclose(contactInfo);
        fclose(temp);
        contactInfo = NULL;
        temp = NULL;
        return;
    }
    view_by_name(name);

    Contact originalContact;
    while (fread(&originalContact, sizeof(originalContact), 1, contactInfo) == 1)
    {
        if (!strcmp(name, originalContact.name))
        {
            rewind(contactInfo);
            break;
        }
    }

    Contact contactEdited = originalContact;
    printf("Enter the edited contact data.\n");
    input_contact_for_editing(&contactEdited, originalContact.name, originalContact.phone);

    printf("Edited contact data.\n");
    print_contact(&contactEdited);

    printf("Do you want to save this edited contact data?\n");
    printf("Answer in yes(y) or no(n): ");
    if (!get_confirmation())
    {
        printf("Edited contact data will not be saved and previous data will be kept saved as it is upon your choice.\n");
        fclose(contactInfo);
        fclose(temp);
        contactInfo = NULL;
        temp = NULL;
        return;
    }

    Contact contact;

    while (fread(&contact, sizeof(contact), 1, contactInfo) == 1)
    {
        if (!strcmp(name, contact.name))
        {
            fwrite(&contactEdited, sizeof(contactEdited), 1, temp);
        }
        else
        {
            fwrite(&contact, sizeof(contact), 1, temp);
        }
    }

    printf("Contact successfully edited.\n");

    if (remove("contact.dat") != 0 || rename("temp.dat", "contact.dat") != 0)
    {
        printf("Error occurred while saving the edited contact data.\n");
    }

    fclose(contactInfo);
    fclose(temp);
    contactInfo = NULL;
    temp = NULL;
}

void edit_by_phone(const char *phone)
{
    FILE *contactInfo = fopen("contact.dat", "rb");
    FILE *temp = fopen("temp.dat", "wb");

    if (contactInfo == NULL || temp == NULL)
    {
        printf("Failed to open the file. Try next time.\n");
        return;
    }

    printf("Contact to be edited.\n");
    if (!find_by_phone(phone))
    {
        printf("Editing the contact failed.\n");
        fclose(contactInfo);
        fclose(temp);
        contactInfo = NULL;
        temp = NULL;
        return;
    }
    view_by_phone(phone);

    Contact originalContact;
    while (fread(&originalContact, sizeof(originalContact), 1, contactInfo) == 1)
    {
        if (!strcmp(phone, originalContact.phone))
        {
            rewind(contactInfo);
            break;
        }
    }

    Contact contactEdited = originalContact;
    printf("Enter the edited contact data.\n");
    input_contact_for_editing(&contactEdited, originalContact.name, originalContact.phone);

    printf("Edited contact data.\n");
    print_contact(&contactEdited);
    printf("Do you want to save this edited contact data?\n");
    printf("Answer in yes(y) or no(n): ");
    if (!get_confirmation())
    {
        printf("Edited contact data will not be saved and previous data will be kept saved as it is upon your choice.\n");
        fclose(contactInfo);
        fclose(temp);
        contactInfo = NULL;
        temp = NULL;
        return;
    }

    Contact contact;

    while (fread(&contact, sizeof(contact), 1, contactInfo) == 1)
    {
        if (!strcmp(phone, contact.phone))
        {
            fwrite(&contactEdited, sizeof(contactEdited), 1, temp);
        }
        else
        {
            fwrite(&contact, sizeof(contact), 1, temp);
        }
    }

    printf("Contact successfully edited.\n");

    if (remove("contact.dat") != 0 || rename("temp.dat", "contact.dat") != 0)
    {
        printf("Error occurred while saving the edited contact data.\n");
    }

    fclose(contactInfo);
    fclose(temp);
    contactInfo = NULL;
    temp = NULL;
}

void delete_by_name(const char *name)
{
    FILE *contactInfo = fopen("contact.dat", "rb");
    FILE *temp = fopen("temp.dat", "wb");

    if (contactInfo == NULL || temp == NULL)
    {
        printf("Failed to open the file. Try next time.\n");
        return;
    }

    if (!find_by_name(name))
    {
        printf("Deleting the contact failed.\n");
        fclose(contactInfo);
        fclose(temp);
        contactInfo = NULL;
        temp = NULL;
        return;
    }
    view_by_name(name);

    printf("Do you want to delete this contact permanently?\n");
    printf("Answer in yes(y) or no(n): ");
    if (!get_confirmation())
    {
        printf("Contact data will be NOT deleted as upon your choice.\n");
        fclose(contactInfo);
        fclose(temp);
        contactInfo = NULL;
        temp = NULL;
        return;
    }

    Contact contact;

    while (fread(&contact, sizeof(contact), 1, contactInfo) == 1)
    {
        if (!strcmp(name, contact.name))
        {
            continue;
        }
        else
        {
            fwrite(&contact, sizeof(contact), 1, temp);
        }
    }

    if (remove("contact.dat") != 0 || rename("temp.dat", "contact.dat") != 0)
    {
        printf("Error occurred while saving the edited contact data.\n");
    }
    else
    {
        printf("Contact successfully deleted.\n");
    }

    fclose(contactInfo);
    contactInfo = NULL;
    fclose(temp);
    temp = NULL;
}

void delete_by_phone(const char *phone)
{
    FILE *contactInfo = fopen("contact.dat", "rb");
    FILE *temp = fopen("temp.dat", "wb");

    if (contactInfo == NULL || temp == NULL)
    {
        printf("Failed to open the file. Try next time.\n");
        return;
    }

    if (!find_by_phone(phone))
    {
        printf("Deleting the contact failed.\n");
        fclose(contactInfo);
        fclose(temp);
        contactInfo = NULL;
        temp = NULL;
        return;
    }
    view_by_phone(phone);

    printf("Do you want to delete this contact permanently?\n");
    printf("Answer in yes(y) or no(n): ");
    if (!get_confirmation())
    {
        printf("Contact data will be NOT deleted as upon your choice.\n");
        fclose(contactInfo);
        fclose(temp);
        contactInfo = NULL;
        temp = NULL;
        return;
    }

    Contact contact;

    while (fread(&contact, sizeof(contact), 1, contactInfo) == 1)
    {
        if (!strcmp(phone, contact.phone))
        {
            continue;
        }
        else
        {
            fwrite(&contact, sizeof(contact), 1, temp);
        }
    }

    if (remove("contact.dat") != 0 || rename("temp.dat", "contact.dat") != 0)
    {
        printf("Error occurred while saving the edited contact data.\n");
    }
    else
    {
        printf("Contact successfully deleted.\n");
    }

    fclose(contactInfo);
    contactInfo = NULL;
    fclose(temp);
    temp = NULL;
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

void add_contact()
{
    FILE *contactInfo = fopen("contact.dat", "ab");

    if (contactInfo == NULL)
    {
        printf("Failed to open the file.\n");
        return;
    }

    Contact contact;
    input_contact(&contact);

    printf("Contact to be saved is: \n");
    print_contact(&contact);
    printf("Do you want to save this contact?\n");
    printf("Answer in yes(y) or no(n): ");
    if (!get_confirmation())
    {
        fclose(contactInfo);
        contactInfo = NULL;
        return;
    }

    if (!fwrite(&contact, sizeof(contact), 1, contactInfo))
    {
        printf("Error facing when inputting data into the file.\n");
        fclose(contactInfo);
        contactInfo = NULL;
        return;
    }

    fclose(contactInfo);
    contactInfo = NULL;
}

void view_contacts()
{
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
        view_by_name(name);
        break;
    case 2:
        char phone[MAX_LENGTH_PHONE];
        input_string("Enter the phone number: ", phone, MAX_LENGTH_PHONE);
        phone_format(phone);
        view_by_phone(phone);
        break;
    }
}

void edit_contact()
{
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
        edit_by_name(name);
        break;

    case 2:
        char phone[MAX_LENGTH_PHONE];
        input_string("Enter the phone number: ", phone, MAX_LENGTH_PHONE);
        phone_format(phone);
        edit_by_phone(phone);
        break;
    }
}

void delete_contact()
{
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
        delete_by_name(name);
        break;
    case 2:
        char phone[MAX_LENGTH_PHONE];
        input_string("Enter the phone number: ", phone, MAX_LENGTH_PHONE);
        phone_format(phone);
        delete_by_phone(phone);
        break;
    }
}

void see_all_contacts()
{
    FILE *contactInfo = fopen("contact.dat", "rb");

    if (contactInfo == NULL)
    {
        printf("Failed to open the file. Try next time.\n");
        return;
    }

    Contact contact;
    int found = 0;

    while (fread(&contact, sizeof(contact), 1, contactInfo))
    {
        print_contact(&contact);
        found = 1;
    }

    if(found == 0)
    {
        printf("No contacts found.\n");
    }

    fclose(contactInfo);
    contactInfo = NULL;
}

void delete_all_contacts()
{
    printf("Do you want to delete all your contacts permanently?\n");
    printf("Answer in yes(y) or no(n): ");
    if (!get_confirmation())
    {
        printf("Contact data will be NOT be deleted as upon your choice.\n");
        return;
    }

    if (remove("contact.dat") != 0)
    {
        printf("Error occurred while deleting the contact data.\n");
    }
    else
    {
        printf("All contacts successfully deleted.\n");
    }
}