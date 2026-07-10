
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>
#include <windows.h>


#define MAX_NAME    100
#define MAX_ADDR    200
#define MAX_MAIL    100
#define MAX_PHONE   20
#define MAX_PWD     100

#define DATA_FILE     "records.dat"
#define TEMP_FILE     "records.tmp"
#define CSV_FILE      "records_export.csv"
#define PASSWORD_FILE "password.dat"

#define COLOR_DEFAULT 7
#define COLOR_GREEN   10
#define COLOR_RED     12
#define COLOR_YELLOW  14
#define COLOR_CYAN    11


typedef struct {
    char name[MAX_NAME];
    char address[MAX_ADDR];
    char email[MAX_MAIL];
    char phone[MAX_PHONE];
} Record;


void setColor(int color);
void printHeader(const char *title);
void pauseScreen(void);
void clearInputBuffer(void);
void trimNewline(char *str);
void toLowerStr(const char *src, char *dst);
int  strcicmp(const char *a, const char *b);
void getStringInput(char *buffer, int size, const char *prompt);
void readMaskedInput(char *buffer, int size);

int  authenticate(void);
void setupPassword(void);

void mainMenu(void);
void addRecord(void);
void searchRecord(void);
void modifyRecord(void);
void deleteRecord(void);
void listRecords(void);
void sortRecordsByName(void);
void exportToCSV(void);
void exitProgram(void);


int main(void) {
    if (authenticate()) {
        mainMenu();
    }
    return 0;
}


void setColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (WORD)color);
}

void printHeader(const char *title) {
    setColor(COLOR_CYAN);
    printf("========================================================================\n");
    printf("   %s\n", title);
    printf("========================================================================\n");
    setColor(COLOR_DEFAULT);
    printf("\n");
}

void pauseScreen(void) {
    printf("\nPress any key to continue...");
    getch();
}


void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void trimNewline(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

void toLowerStr(const char *src, char *dst) {
    int i;
    for (i = 0; src[i]; i++) {
        dst[i] = (char)tolower((unsigned char)src[i]);
    }
    dst[i] = '\0';
}


int strcicmp(const char *a, const char *b) {
    while (*a && *b) {
        int diff = tolower((unsigned char)*a) - tolower((unsigned char)*b);
        if (diff != 0) return diff;
        a++;
        b++;
    }
    return tolower((unsigned char)*a) - tolower((unsigned char)*b);
}


void getStringInput(char *buffer, int size, const char *prompt) {
    printf("%s", prompt);
    fgets(buffer, size, stdin);
    trimNewline(buffer);
}


void readMaskedInput(char *buffer, int size) {
    int i = 0;
    char ch;
    while (1) {
        ch = getch();
        if (ch == '\r') {
            break;
        } else if (ch == '\b') {
            if (i > 0) {
                i--;
                printf("\b \b");
            }
        } else if (i < size - 1 && ch >= 32 && ch <= 126) {
            buffer[i++] = ch;
            printf("*");
        }
    }
    buffer[i] = '\0';
}


void setupPassword(void) {
    char pwd[MAX_PWD], confirm[MAX_PWD];

    system("cls");
    printHeader("First-Time Setup");
    printf("No password found. Let's create one for your phonebook.\n\n");

    while (1) {
        printf("Create a password: ");
        readMaskedInput(pwd, MAX_PWD);
        printf("\nConfirm password : ");
        readMaskedInput(confirm, MAX_PWD);
        printf("\n\n");

        if (strlen(pwd) == 0) {
            setColor(COLOR_RED);
            printf("Password cannot be empty. Try again.\n\n");
            setColor(COLOR_DEFAULT);
            continue;
        }
        if (strcmp(pwd, confirm) != 0) {
            setColor(COLOR_RED);
            printf("Passwords do not match. Try again.\n\n");
            setColor(COLOR_DEFAULT);
            continue;
        }
        break;
    }

    FILE *fp = fopen(PASSWORD_FILE, "w");
    if (fp == NULL) {
        printf("Error: could not save password file.\n");
        Sleep(1500);
        return;
    }
    fprintf(fp, "%s", pwd);
    fclose(fp);

    setColor(COLOR_GREEN);
    printf("Password created successfully!\n");
    setColor(COLOR_DEFAULT);
    Sleep(1200);
}


int authenticate(void) {
    FILE *fp = fopen(PASSWORD_FILE, "r");
    if (fp == NULL) {
        setupPassword();
        fp = fopen(PASSWORD_FILE, "r");
        if (fp == NULL) {
            printf("Fatal error: could not access password file.\n");
            return 0;
        }
    }

    char stored[MAX_PWD];
    if (fgets(stored, MAX_PWD, fp) == NULL) {
        stored[0] = '\0';
    }
    trimNewline(stored);
    fclose(fp);

    int attempts = 3;
    while (attempts > 0) {
        system("cls");
        printHeader("Phonebook Manager - Login");
        printf("Enter password (%d attempt%s remaining): ", attempts, attempts == 1 ? "" : "s");

        char pwd[MAX_PWD];
        readMaskedInput(pwd, MAX_PWD);
        printf("\n");

        if (strcmp(pwd, stored) == 0) {
            setColor(COLOR_GREEN);
            printf("\nAccess granted!\n");
            setColor(COLOR_DEFAULT);
            Sleep(700);
            return 1;
        }

        attempts--;
        setColor(COLOR_RED);
        printf("\nIncorrect password.\n");
        setColor(COLOR_DEFAULT);
        Sleep(1000);
    }

    printf("\nToo many failed attempts. Exiting.\n");
    Sleep(1500);
    return 0;
}


void mainMenu(void) {
    int choice;

    while (1) {
        system("cls");
        printHeader("Phonebook Manager - Main Menu");
        printf(" 1. Add a record\n");
        printf(" 2. Search a record\n");
        printf(" 3. Modify a record\n");
        printf(" 4. Delete a record\n");
        printf(" 5. List all records\n");
        printf(" 6. Sort records by name\n");
        printf(" 7. Export records to CSV\n");
        printf(" 8. Exit\n");
        printf("\nEnter your choice: ");

        if (scanf("%d", &choice) != 1) {
            choice = -1;
        }
        clearInputBuffer();

        switch (choice) {
            case 1: addRecord();         break;
            case 2: searchRecord();      break;
            case 3: modifyRecord();      break;
            case 4: deleteRecord();      break;
            case 5: listRecords();       break;
            case 6: sortRecordsByName(); break;
            case 7: exportToCSV();       break;
            case 8: exitProgram();       return;
            default:
                setColor(COLOR_RED);
                printf("\nInvalid input. Please choose 1-8.\n");
                setColor(COLOR_DEFAULT);
                Sleep(1200);
        }
    }
}


void addRecord(void) {
    char more = 'y';

    while (more == 'y' || more == 'Y') {
        system("cls");
        printHeader("Add New Record");

        Record r;
        getStringInput(r.name,    MAX_NAME,  "Enter name         : ");
        getStringInput(r.address, MAX_ADDR,  "Enter address       : ");
        getStringInput(r.email,   MAX_MAIL,  "Enter email address : ");
        getStringInput(r.phone,   MAX_PHONE, "Enter phone number  : ");

        FILE *fp = fopen(DATA_FILE, "ab");
        if (fp == NULL) {
            setColor(COLOR_RED);
            printf("\nError: unable to open the data file.\n");
            setColor(COLOR_DEFAULT);
            pauseScreen();
            return;
        }
        fwrite(&r, sizeof(Record), 1, fp);
        fclose(fp);

        setColor(COLOR_GREEN);
        printf("\nRecord added successfully!\n");
        setColor(COLOR_DEFAULT);

        printf("\nAdd another record? (y/n): ");
        scanf(" %c", &more);
        clearInputBuffer();
    }
}


void searchRecord(void) {
    system("cls");
    printHeader("Search Record");

    FILE *fp = fopen(DATA_FILE, "rb");
    if (fp == NULL) {
        printf("No records found. The phonebook is empty.\n");
        pauseScreen();
        return;
    }

    char query[MAX_NAME];
    getStringInput(query, MAX_NAME, "Enter a name (or part of a name) to search: ");

    char lowerQuery[MAX_NAME];
    toLowerStr(query, lowerQuery);

    Record r;
    int found = 0;
    printf("\n");

    while (fread(&r, sizeof(Record), 1, fp) == 1) {
        char lowerName[MAX_NAME];
        toLowerStr(r.name, lowerName);

        if (strstr(lowerName, lowerQuery) != NULL) {
            found++;
            setColor(COLOR_GREEN);
            printf("--- Match #%d ---\n", found);
            setColor(COLOR_DEFAULT);
            printf("  Name    : %s\n", r.name);
            printf("  Address : %s\n", r.address);
            printf("  Email   : %s\n", r.email);
            printf("  Phone   : %s\n\n", r.phone);
        }
    }
    fclose(fp);

    if (found == 0) {
        setColor(COLOR_RED);
        printf("No matching records found.\n");
        setColor(COLOR_DEFAULT);
    } else {
        printf("%d record(s) found.\n", found);
    }
    pauseScreen();
}


void modifyRecord(void) {
    system("cls");
    printHeader("Modify Record");

    FILE *fp = fopen(DATA_FILE, "rb");
    if (fp == NULL) {
        printf("No records found.\n");
        pauseScreen();
        return;
    }

    char query[MAX_NAME];
    getStringInput(query, MAX_NAME, "Enter the exact name of the record to modify: ");

    FILE *temp = fopen(TEMP_FILE, "wb");
    if (temp == NULL) {
        printf("Error: unable to create temporary file.\n");
        fclose(fp);
        pauseScreen();
        return;
    }

    Record r;
    int found = 0;

    while (fread(&r, sizeof(Record), 1, fp) == 1) {
        if (!found && strcicmp(r.name, query) == 0) {
            found = 1;

            printf("\nCurrent details:\n");
            printf("  Name    : %s\n  Address : %s\n  Email   : %s\n  Phone   : %s\n",
                   r.name, r.address, r.email, r.phone);
            printf("\nEnter new details (press ENTER to keep the current value):\n\n");

            char buf[MAX_ADDR];

            printf("Name [%s]: ", r.name);
            fgets(buf, MAX_NAME, stdin);
            trimNewline(buf);
            if (strlen(buf) > 0) strcpy(r.name, buf);

            printf("Address [%s]: ", r.address);
            fgets(buf, MAX_ADDR, stdin);
            trimNewline(buf);
            if (strlen(buf) > 0) strcpy(r.address, buf);

            printf("Email [%s]: ", r.email);
            fgets(buf, MAX_MAIL, stdin);
            trimNewline(buf);
            if (strlen(buf) > 0) strcpy(r.email, buf);

            printf("Phone [%s]: ", r.phone);
            fgets(buf, MAX_PHONE, stdin);
            trimNewline(buf);
            if (strlen(buf) > 0) strcpy(r.phone, buf);

            setColor(COLOR_GREEN);
            printf("\nRecord updated successfully!\n");
            setColor(COLOR_DEFAULT);
        }
        fwrite(&r, sizeof(Record), 1, temp);
    }

    fclose(fp);
    fclose(temp);

    remove(DATA_FILE);
    rename(TEMP_FILE, DATA_FILE);

    if (!found) {
        setColor(COLOR_RED);
        printf("\nNo record found with that name.\n");
        setColor(COLOR_DEFAULT);
    }
    pauseScreen();
}


void deleteRecord(void) {
    system("cls");
    printHeader("Delete Record");

    FILE *fp = fopen(DATA_FILE, "rb");
    if (fp == NULL) {
        printf("No records found.\n");
        pauseScreen();
        return;
    }

    char query[MAX_NAME];
    getStringInput(query, MAX_NAME, "Enter the exact name of the record to delete: ");

    FILE *temp = fopen(TEMP_FILE, "wb");
    if (temp == NULL) {
        printf("Error: unable to create temporary file.\n");
        fclose(fp);
        pauseScreen();
        return;
    }

    Record r;
    int matchFound = 0;
    int deleted = 0;

    while (fread(&r, sizeof(Record), 1, fp) == 1) {
        if (!matchFound && strcicmp(r.name, query) == 0) {
            matchFound = 1;

            printf("\nFound record:\n");
            printf("  Name : %s\n  Phone: %s\n", r.name, r.phone);
            printf("\nConfirm delete? (y/n): ");

            char confirm;
            scanf(" %c", &confirm);
            clearInputBuffer();

            if (confirm == 'y' || confirm == 'Y') {
                deleted = 1;
                continue; 
            }
        }
        fwrite(&r, sizeof(Record), 1, temp);
    }

    fclose(fp);
    fclose(temp);

    remove(DATA_FILE);
    rename(TEMP_FILE, DATA_FILE);

    if (deleted) {
        setColor(COLOR_GREEN);
        printf("\nRecord deleted successfully!\n");
    } else if (matchFound) {
        printf("\nDeletion cancelled.\n");
    } else {
        setColor(COLOR_RED);
        printf("\nNo record found with that name.\n");
    }
    setColor(COLOR_DEFAULT);
    pauseScreen();
}


void listRecords(void) {
    system("cls");
    printHeader("All Records");

    FILE *fp = fopen(DATA_FILE, "rb");
    if (fp == NULL) {
        printf("No records found. The phonebook is empty.\n");
        pauseScreen();
        return;
    }

    Record r;
    int count = 0;

    printf("%-4s %-20s %-25s %-25s %-15s\n", "No.", "Name", "Address", "Email", "Phone");
    printf("------------------------------------------------------------------------------------------\n");

    while (fread(&r, sizeof(Record), 1, fp) == 1) {
        count++;
        printf("%-4d %-20.20s %-25.25s %-25.25s %-15.15s\n",
               count, r.name, r.address, r.email, r.phone);
    }
    fclose(fp);

    printf("------------------------------------------------------------------------------------------\n");
    if (count == 0) {
        printf("No records to display.\n");
    } else {
        printf("Total records: %d\n", count);
    }
    pauseScreen();
}

/* ---------------------------------------------------------------------- */
/*  Sort                                                                    */
/* ---------------------------------------------------------------------- */
int compareRecordsByName(const void *a, const void *b) {
    return strcicmp(((const Record *)a)->name, ((const Record *)b)->name);
}

void sortRecordsByName(void) {
    system("cls");
    printHeader("Sort Records");

    FILE *fp = fopen(DATA_FILE, "rb");
    if (fp == NULL) {
        printf("No records found.\n");
        pauseScreen();
        return;
    }

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    int count = (int)(size / sizeof(Record));

    if (count == 0) {
        printf("No records to sort.\n");
        fclose(fp);
        pauseScreen();
        return;
    }

    Record *records = malloc(count * sizeof(Record));
    if (records == NULL) {
        printf("Error: not enough memory.\n");
        fclose(fp);
        pauseScreen();
        return;
    }

    fread(records, sizeof(Record), count, fp);
    fclose(fp);

    qsort(records, count, sizeof(Record), compareRecordsByName);

    fp = fopen(DATA_FILE, "wb");
    if (fp != NULL) {
        fwrite(records, sizeof(Record), count, fp);
        fclose(fp);
    }
    free(records);

    setColor(COLOR_GREEN);
    printf("%d record(s) sorted alphabetically by name!\n", count);
    setColor(COLOR_DEFAULT);
    pauseScreen();
}


void exportToCSV(void) {
    system("cls");
    printHeader("Export to CSV");

    FILE *fp = fopen(DATA_FILE, "rb");
    if (fp == NULL) {
        printf("No records found.\n");
        pauseScreen();
        return;
    }

    FILE *csv = fopen(CSV_FILE, "w");
    if (csv == NULL) {
        printf("Error: unable to create CSV file.\n");
        fclose(fp);
        pauseScreen();
        return;
    }

    fprintf(csv, "Name,Address,Email,Phone\n");

    Record r;
    int count = 0;
    while (fread(&r, sizeof(Record), 1, fp) == 1) {
        fprintf(csv, "%s,%s,%s,%s\n", r.name, r.address, r.email, r.phone);
        count++;
    }
    fclose(fp);
    fclose(csv);

    setColor(COLOR_GREEN);
    printf("%d record(s) exported to %s\n", count, CSV_FILE);
    setColor(COLOR_DEFAULT);
    pauseScreen();
}


void exitProgram(void) {
    system("cls");
    setColor(COLOR_CYAN);
    printf("Thank you for using Phonebook Manager. Goodbye!\n");
    setColor(COLOR_DEFAULT);
    Sleep(1200);
}
