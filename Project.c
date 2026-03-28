#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "books.dat"

struct Book {
    char book_id[20];
    char title[50];
    char author[50];
    char publisher[50];
    int quantity;
};

void addBook(void);
void displayBooks(void);
void searchBook(void);
void updateBook(void);
void deleteBook(void);

int main() {
    int choice;
    while (1) {
        printf("\n===== BOOK MANAGEMENT SYSTEM =====\n");
        printf("1. Add New Book\n");
        printf("2. Display All Books\n");
        printf("3. Search Book\n");
        printf("4. Update Book Details\n");
        printf("5. Delete Book Record\n");
        printf("6. Exit\n");
        printf("Enter your choice (1-6): ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            exit(1);
        }
        getchar();
        switch (choice) {
            case 1: addBook(); break;
            case 2: displayBooks(); break;
            case 3: searchBook(); break;
            case 4: updateBook(); break;
            case 5: deleteBook(); break;
            case 6: printf("Exiting program.\n"); exit(0);
            default: printf("Invalid choice.\n");
        }
    }
    return 0;
}

void addBook(void) {
    struct Book book;
    FILE *fp = fopen(FILENAME, "ab");
    if (!fp) { printf("File error.\n"); return; }

    printf("Enter Book ID: ");
    fgets(book.book_id, sizeof(book.book_id), stdin);
    book.book_id[strcspn(book.book_id, "\n")] = '\0';

    printf("Enter Title: ");
    fgets(book.title, sizeof(book.title), stdin);
    book.title[strcspn(book.title, "\n")] = '\0';

    printf("Enter Author: ");
    fgets(book.author, sizeof(book.author), stdin);
    book.author[strcspn(book.author, "\n")] = '\0';

    printf("Enter Publisher: ");
    fgets(book.publisher, sizeof(book.publisher), stdin);
    book.publisher[strcspn(book.publisher, "\n")] = '\0';

    printf("Enter Quantity: ");
    if (scanf("%d", &book.quantity) != 1) {
        printf("Invalid number.\n");
        fclose(fp);
        getchar();
        return;
    }
    getchar();

    fwrite(&book, sizeof(struct Book), 1, fp);
    fclose(fp);
    printf("Book added successfully.\n");
}

void displayBooks(void) {
    struct Book book;
    FILE *fp = fopen(FILENAME, "rb");
    if (!fp) { printf("No books found.\n"); return; }

    printf("\nAll Books:\n");
    printf("------------------------------------------------------------\n");
    while (fread(&book, sizeof(struct Book), 1, fp) == 1) {
        if (strlen(book.book_id) > 0)
            printf("ID: %s | Title: %s | Author: %s | Publisher: %s | Qty: %d\n",
                   book.book_id, book.title, book.author, book.publisher, book.quantity);
    }
    printf("------------------------------------------------------------\n");
    fclose(fp);
}

void searchBook(void) {
    struct Book book;
    FILE *fp = fopen(FILENAME, "rb");
    if (!fp) { printf("No books found.\n"); return; }

    int choice;
    char term[50];
    int found = 0;

    printf("Search by: 1. Book ID  2. Title\nEnter choice: ");
    if (scanf("%d", &choice) != 1) { fclose(fp); return; }
    getchar();

    printf("Enter search term: ");
    fgets(term, sizeof(term), stdin);
    term[strcspn(term, "\n")] = '\0';

    while (fread(&book, sizeof(struct Book), 1, fp) == 1) {
        if ((choice == 1 && strcmp(book.book_id, term) == 0) ||
            (choice == 2 && strcmp(book.title, term) == 0)) {
            printf("Book Found:\n");
            printf("ID: %s\nTitle: %s\nAuthor: %s\nPublisher: %s\nQuantity: %d\n",
                   book.book_id, book.title, book.author, book.publisher, book.quantity);
            found = 1;
            break;
        }
    }
    if (!found) printf("Book not found.\n");
    fclose(fp);
}

void updateBook(void) {
    struct Book book;
    FILE *fp = fopen(FILENAME, "rb+");
    if (!fp) { printf("No books found.\n"); return; }

    char id[20];
    int found = 0;

    printf("Enter Book ID to update: ");
    fgets(id, sizeof(id), stdin);
    id[strcspn(id, "\n")] = '\0';

    while (fread(&book, sizeof(struct Book), 1, fp) == 1) {
        if (strcmp(book.book_id, id) == 0) {
            char temp[50];
            printf("Enter new Title (leave blank to keep): ");
            fgets(temp, sizeof(temp), stdin);
            temp[strcspn(temp, "\n")] = '\0';
            if (strlen(temp)) strcpy(book.title, temp);

            printf("Enter new Author (leave blank to keep): ");
            fgets(temp, sizeof(temp), stdin);
            temp[strcspn(temp, "\n")] = '\0';
            if (strlen(temp)) strcpy(book.author, temp);

            printf("Enter new Publisher (leave blank to keep): ");
            fgets(temp, sizeof(temp), stdin);
            temp[strcspn(temp, "\n")] = '\0';
            if (strlen(temp)) strcpy(book.publisher, temp);

            printf("Enter new Quantity (-1 to keep): ");
            int qty;
            if (scanf("%d", &qty) == 1 && qty >= 0)
                book.quantity = qty;
            getchar();

            fseek(fp, -sizeof(struct Book), SEEK_CUR);
            fwrite(&book, sizeof(struct Book), 1, fp);
            printf("Book updated.\n");
            found = 1;
            break;
        }
    }
    if (!found) printf("Book not found.\n");
    fclose(fp);
}

void deleteBook(void) {
    struct Book book;
    FILE *fp = fopen(FILENAME, "rb");
    FILE *temp = fopen("temp.dat", "wb");
    if (!fp || !temp) { printf("File error.\n"); return; }

    char id[20];
    int found = 0;

    printf("Enter Book ID to delete: ");
    fgets(id, sizeof(id), stdin);
    id[strcspn(id, "\n")] = '\0';

    while (fread(&book, sizeof(struct Book), 1, fp) == 1) {
        if (strcmp(book.book_id, id) == 0) { found = 1; continue; }
        fwrite(&book, sizeof(struct Book), 1, temp);
    }

    fclose(fp);
    fclose(temp);
    remove(FILENAME);
    rename("temp.dat", FILENAME);

    if (found) printf("Book deleted.\n");
    else printf("Book not found.\n");
}