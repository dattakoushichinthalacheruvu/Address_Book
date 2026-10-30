#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<stdlib.h>

#define MAX 100

struct Contact {
    char name[50];
    char phone[15];
    char email[50];
};

struct AddressBook {
    struct Contact contacts[MAX];
    int count;
};

// Function declarations
int isValidName(char *name);
int isValidPhone(char *phone, struct AddressBook *book, int skipIndex);
int isValidEmail(char *email, struct AddressBook *book, int skipIndex);
int findContactsByName(char *name, struct AddressBook *book, int result[]);
void saveToFile(struct AddressBook *book);
void loadFromFile(struct AddressBook *book);

void addContact(struct AddressBook *book) {
    struct Contact newC;

    // Name
    do {
        printf("Enter name: ");
        scanf(" %[^\n]", newC.name);
    } while(!isValidName(newC.name));

    // Phone
    do {
        printf("Enter 10 digit phone: ");
        scanf("%s", newC.phone);
    } while(!isValidPhone(newC.phone, book, -1));

    // Email
    do {
        printf("Enter @gmail.com email: ");
        scanf("%s", newC.email);
    } while(!isValidEmail(newC.email, book, -1));

    book->contacts[book->count++] = newC;
    saveToFile(book);
    printf("................successfully added....................\n");
}

void listContacts(struct AddressBook *book) {
    if(book->count == 0) {
        printf("No contacts found\n");
        return;
    }
    for(int i=0; i<book->count; i++) {
        printf("\n--- Contact %d ---\n", i+1);
        printf("Name : %s\n", book->contacts[i].name);
        printf("Phone: %s\n", book->contacts[i].phone);
        printf("Email: %s\n", book->contacts[i].email);
    }
}

void searchContact(struct AddressBook *book) {
    int choice;
    printf("Search by 1.Name 2.Phone 3.Email: ");
    scanf("%d", &choice);

    char key[50];
    printf("Enter search term: ");
    scanf(" %[^\n]", key);

    int found = 0;
    for(int i=0; i<book->count; i++) {
        int match = 0;
        if(choice==1 && strstr(book->contacts[i].name, key)) match=1;
        if(choice==2 && strcmp(book->contacts[i].phone, key)==0) match=1;
        if(choice==3 && strcmp(book->contacts[i].email, key)==0) match=1;

        if(match) {
            found = 1;
            printf("\nName : %s\nPhone: %s\nEmail: %s\n",
            book->contacts[i].name, book->contacts[i].phone, book->contacts[i].email);
        }
    }
    if(!found) printf("Contact not found\n");
}

void editContact(struct AddressBook *book) {
    char name[50];
    printf("Enter name to edit: ");
    scanf(" %[^\n]", name);

    int match[100], c=0;
    c = findContactsByName(name, book, match);
    if(c==0) { printf("Not found\n"); return; }

    for(int i=0; i<c; i++)
        printf("%d. %s | %s | %s\n", i+1, book->contacts[match[i]].name, book->contacts[match[i]].phone, book->contacts[match[i]].email);

    int ch, idx;
    printf("Pick number to edit: ");
    scanf("%d",&ch);
    if(ch<1 || ch>c){ printf("Invalid choice\n"); return; }
    idx = match[ch-1];

    printf("Edit 1.Name 2.Phone 3.Email: ");
    scanf("%d",&ch);

    if(ch==1){
        char newName[50];
        do{
            printf("Enter new name: ");
            scanf(" %[^\n]", newName);
        }while(!isValidName(newName));
        strcpy(book->contacts[idx].name, newName);
    }
    else if(ch==2){
        char newPhone[15];
        do{
            printf("Enter new phone: ");
            scanf("%s", newPhone);
        }while(!isValidPhone(newPhone, book, idx));
        strcpy(book->contacts[idx].phone, newPhone);
    }
    else if(ch==3){
        char newEmail[50];
        do{
            printf("Enter new email: ");
            scanf("%s", newEmail);
        }while(!isValidEmail(newEmail, book, idx));
        strcpy(book->contacts[idx].email, newEmail);
    }

    saveToFile(book);
    printf("Contact updated!\n");
}

void deleteContact(struct AddressBook *book) {
    char name[50];
    printf("Enter name to delete: ");
    scanf(" %[^\n]", name);

    int match[100], c=0;
    c = findContactsByName(name, book, match);
    if(c==0) { printf("Not found\n"); return; }

    for(int i=0; i<c; i++)
        printf("%d. %s | %s | %s\n", i+1, book->contacts[match[i]].name, book->contacts[match[i]].phone, book->contacts[match[i]].email);

    int ch, idx;
    printf("Pick number to delete: ");
    scanf("%d",&ch);
    if(ch<1 || ch>c){ printf("Invalid choice\n"); return; }
    idx = match[ch-1];

    printf("Confirm delete yes/no: ");
    char confirm[5];
    scanf("%s", confirm);
    if(strcmp(confirm,"yes")==0){
        for(int i=idx; i<book->count-1; i++)
            book->contacts[i] = book->contacts[i+1];
        book->count--;
        saveToFile(book);
        printf("Contact deleted\n");
    }
    else{
        printf("Deletion cancelled\n");
    }
}

// VALIDATION FUNCTIONS
int isValidName(char *name) {
    if(strlen(name)==0) {
        printf("Name cannot be empty\n");
        return 0;
    }
    for(int i=0; name[i]; i++)
        if(!isalpha(name[i]) && name[i]!=' ' && name[i]!='.') {
            printf("Name can only contain alphabets, space and dot\n");
            return 0;
        }
    return 1;
}

int isValidPhone(char *phone, struct AddressBook *book, int skip) {
    if(strlen(phone)!=10) {
        printf("Phone must be 10 digits\n");
        return 0;
    }
    for(int i=0; phone[i]; i++)
        if(!isdigit(phone[i])) {
            printf("Phone must contain only digits\n");
            return 0;
        }
    for(int i=0; i<book->count; i++)
        if(i!=skip && strcmp(book->contacts[i].phone, phone)==0) {
            printf("Phone number already exists\n");
            return 0;
        }
    return 1;
}

int isValidEmail(char *email, struct AddressBook *book, int skip) {
    char *at = strstr(email, "@gmail.com");
    if(at==NULL || strlen(at)!=10) {
        printf("Email must end with @gmail.com\n");
        return 0;
    }
    for(int i=0; email[i]!='@'; i++)
        if(!islower(email[i]) &&!isdigit(email[i])) {
            printf("Email username must be lowercase and digits only\n");
            return 0;
        }
    for(int i=0; i<book->count; i++)
        if(i!=skip && strcmp(book->contacts[i].email, email)==0) {
            printf("Email already exists\n");
            return 0;
        }
    return 1;
}

int findContactsByName(char *name, struct AddressBook *book, int result[]) {
    int c=0;
    for(int i=0; i<book->count; i++)
        if(strstr(book->contacts[i].name, name)) result[c++] = i;
    return c;
}

// FILE HANDLING
void saveToFile(struct AddressBook *book) {
    FILE *fp = fopen("contacts.dat", "wb");
    if(fp==NULL){ printf("Error saving file\n"); return; }
    fwrite(book, sizeof(struct AddressBook), 1, fp);
    fclose(fp);
}

void loadFromFile(struct AddressBook *book) {
    FILE *fp = fopen("contacts.dat", "rb");
    if(fp) {
        fread(book, sizeof(struct AddressBook), 1, fp);
        fclose(fp);
    }
    else book->count = 0;
}

int main() {
    struct AddressBook book;
    loadFromFile(&book);
    int choice;
    while(1) {
        printf("\n===== MENU =====\n");
        printf("1.Add Contact\n2.List Contacts\n3.Search Contact\n4.Edit Contact\n5.Delete Contact\n6.Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);
        switch(choice) {
            case 1: addContact(&book); break;
            case 2: listContacts(&book); break;
            case 3: searchContact(&book); break;
            case 4: editContact(&book); break;
            case 5: deleteContact(&book); break;
            case 6: printf("Thank you\n"); exit(0);
            default: printf("Invalid choice\n");
        }
    }
}