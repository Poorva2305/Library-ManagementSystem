#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_BOOKS 100
#define MAX_MEMBERS 50
#define FILENAME "books.dat"
#define MEMBERS_FILE "members.dat"
#define TRANSACTIONS_FILE "transactions.dat"

typedef struct {
    int id;
    char title[100];
    char author[50];
    char isbn[20];
    int available;
    int total_copies;
} Book;

typedef struct {
    int id;
    char name[50];
    char email[50];
    char phone[15];
} Member;

typedef struct {
    int transaction_id;
    int book_id;
    int member_id;
    char issue_date[20];
    char return_date[20];
    int returned;
} Transaction;

Book books[MAX_BOOKS];
Member members[MAX_MEMBERS];
Transaction transactions[MAX_BOOKS];
int book_count = 0;
int member_count = 0;
int transaction_count = 0;

void load_data();
void save_books();
void save_members();
void save_transactions();
void generate_html();
void add_book();
void view_books();
void search_book();
void delete_book();
void add_member();
void view_members();
void issue_book();
void return_book();
void view_transactions();

int main() {
    int choice;
    
    load_data();
    
    while(1) {
        printf("\n========== LIBRARY MANAGEMENT SYSTEM ==========\n");
        printf("1. Add Book\n");
        printf("2. View All Books\n");
        printf("3. Search Book\n");
        printf("4. Delete Book\n");
        printf("5. Add Member\n");
        printf("6. View All Members\n");
        printf("7. Issue Book\n");
        printf("8. Return Book\n");
        printf("9. View Transactions\n");
        printf("10. Generate HTML Report\n");
        printf("0. Exit\n");
        printf("===============================================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();
        
        switch(choice) {
            case 1: add_book(); break;
            case 2: view_books(); break;
            case 3: search_book(); break;
            case 4: delete_book(); break;
            case 5: add_member(); break;
            case 6: view_members(); break;
            case 7: issue_book(); break;
            case 8: return_book(); break;
            case 9: view_transactions(); break;
            case 10: generate_html(); break;
            case 0:
                save_books();
                save_members();
                save_transactions();
                printf("Data saved. Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice!\n");
        }
    }
    
    return 0;
}

void load_data() {
    FILE *fp;
    
    fp = fopen(FILENAME, "rb");
    if(fp != NULL) {
        fread(&book_count, sizeof(int), 1, fp);
        fread(books, sizeof(Book), book_count, fp);
        fclose(fp);
    }
    
    fp = fopen(MEMBERS_FILE, "rb");
    if(fp != NULL) {
        fread(&member_count, sizeof(int), 1, fp);
        fread(members, sizeof(Member), member_count, fp);
        fclose(fp);
    }
    
    fp = fopen(TRANSACTIONS_FILE, "rb");
    if(fp != NULL) {
        fread(&transaction_count, sizeof(int), 1, fp);
        fread(transactions, sizeof(Transaction), transaction_count, fp);
        fclose(fp);
    }
}

void save_books() {
    FILE *fp = fopen(FILENAME, "wb");
    if(fp != NULL) {
        fwrite(&book_count, sizeof(int), 1, fp);
        fwrite(books, sizeof(Book), book_count, fp);
        fclose(fp);
    }
}

void save_members() {
    FILE *fp = fopen(MEMBERS_FILE, "wb");
    if(fp != NULL) {
        fwrite(&member_count, sizeof(int), 1, fp);
        fwrite(members, sizeof(Member), member_count, fp);
        fclose(fp);
    }
}

void save_transactions() {
    FILE *fp = fopen(TRANSACTIONS_FILE, "wb");
    if(fp != NULL) {
        fwrite(&transaction_count, sizeof(int), 1, fp);
        fwrite(transactions, sizeof(Transaction), transaction_count, fp);
        fclose(fp);
    }
}

void add_book() {
    if(book_count >= MAX_BOOKS) {
        printf("Library is full!\n");
        return;
    }
    
    Book b;
    b.id = book_count + 1;
    
    printf("\nEnter book title: ");
    fgets(b.title, 100, stdin);
    b.title[strcspn(b.title, "\n")] = 0;
    
    printf("Enter author name: ");
    fgets(b.author, 50, stdin);
    b.author[strcspn(b.author, "\n")] = 0;
    
    printf("Enter ISBN: ");
    fgets(b.isbn, 20, stdin);
    b.isbn[strcspn(b.isbn, "\n")] = 0;
    
    printf("Enter total copies: ");
    scanf("%d", &b.total_copies);
    b.available = b.total_copies;
    
    books[book_count++] = b;
    save_books();
    
    printf("\n✓ Book added successfully! (ID: %d)\n", b.id);
}

void view_books() {
    if(book_count == 0) {
        printf("\nNo books in library!\n");
        return;
    }
    
    printf("\n========== BOOK LIST ==========\n");
    for(int i = 0; i < book_count; i++) {
        printf("\nBook ID: %d\n", books[i].id);
        printf("Title: %s\n", books[i].title);
        printf("Author: %s\n", books[i].author);
        printf("ISBN: %s\n", books[i].isbn);
        printf("Available: %d/%d\n", books[i].available, books[i].total_copies);
        printf("-----------------------------\n");
    }
}

void search_book() {
    char search[100];
    int found = 0;
    
    printf("\nEnter book title or author to search: ");
    fgets(search, 100, stdin);
    search[strcspn(search, "\n")] = 0;
    
    printf("\n========== SEARCH RESULTS ==========\n");
    for(int i = 0; i < book_count; i++) {
        if(strstr(books[i].title, search) != NULL || strstr(books[i].author, search) != NULL) {
            printf("\nBook ID: %d\n", books[i].id);
            printf("Title: %s\n", books[i].title);
            printf("Author: %s\n", books[i].author);
            printf("ISBN: %s\n", books[i].isbn);
            printf("Available: %d/%d\n", books[i].available, books[i].total_copies);
            printf("-----------------------------\n");
            found = 1;
        }
    }
    
    if(!found) {
        printf("No books found!\n");
    }
}

void delete_book() {
    int id;
    printf("\nEnter book ID to delete: ");
    scanf("%d", &id);
    
    for(int i = 0; i < book_count; i++) {
        if(books[i].id == id) {
            for(int j = i; j < book_count - 1; j++) {
                books[j] = books[j + 1];
            }
            book_count--;
            save_books();
            printf("\n✓ Book deleted successfully!\n");
            return;
        }
    }
    
    printf("\nBook not found!\n");
}

void add_member() {
    if(member_count >= MAX_MEMBERS) {
        printf("Member list is full!\n");
        return;
    }
    
    Member m;
    m.id = member_count + 1;
    
    printf("\nEnter member name: ");
    fgets(m.name, 50, stdin);
    m.name[strcspn(m.name, "\n")] = 0;
    
    printf("Enter email: ");
    fgets(m.email, 50, stdin);
    m.email[strcspn(m.email, "\n")] = 0;
    
    printf("Enter phone: ");
    fgets(m.phone, 15, stdin);
    m.phone[strcspn(m.phone, "\n")] = 0;
    
    members[member_count++] = m;
    save_members();
    
    printf("\n✓ Member added successfully! (ID: %d)\n", m.id);
}

void view_members() {
    if(member_count == 0) {
        printf("\nNo members registered!\n");
        return;
    }
    
    printf("\n========== MEMBER LIST ==========\n");
    for(int i = 0; i < member_count; i++) {
        printf("\nMember ID: %d\n", members[i].id);
        printf("Name: %s\n", members[i].name);
        printf("Email: %s\n", members[i].email);
        printf("Phone: %s\n", members[i].phone);
        printf("--------------------------------\n");
    }
}

void issue_book() {
    int book_id, member_id;
    
    printf("\nEnter book ID: ");
    scanf("%d", &book_id);
    printf("Enter member ID: ");
    scanf("%d", &member_id);
    
    int book_idx = -1, member_idx = -1;
    
    for(int i = 0; i < book_count; i++) {
        if(books[i].id == book_id) {
            book_idx = i;
            break;
        }
    }
    
    for(int i = 0; i < member_count; i++) {
        if(members[i].id == member_id) {
            member_idx = i;
            break;
        }
    }
    
    if(book_idx == -1) {
        printf("\nBook not found!\n");
        return;
    }
    
    if(member_idx == -1) {
        printf("\nMember not found!\n");
        return;
    }
    
    if(books[book_idx].available <= 0) {
        printf("\nBook not available!\n");
        return;
    }
    
    Transaction t;
    t.transaction_id = transaction_count + 1;
    t.book_id = book_id;
    t.member_id = member_id;
    t.returned = 0;
    
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    strftime(t.issue_date, 20, "%Y-%m-%d", tm_info);
    strcpy(t.return_date, "Not Returned");
    
    transactions[transaction_count++] = t;
    books[book_idx].available--;
    
    save_books();
    save_transactions();
    
    printf("\n✓ Book issued successfully! (Transaction ID: %d)\n", t.transaction_id);
}

void return_book() {
    int trans_id;
    
    printf("\nEnter transaction ID: ");
    scanf("%d", &trans_id);
    
    for(int i = 0; i < transaction_count; i++) {
        if(transactions[i].transaction_id == trans_id && !transactions[i].returned) {
            transactions[i].returned = 1;
            
            time_t now = time(NULL);
            struct tm *tm_info = localtime(&now);
            strftime(transactions[i].return_date, 20, "%Y-%m-%d", tm_info);
            
            for(int j = 0; j < book_count; j++) {
                if(books[j].id == transactions[i].book_id) {
                    books[j].available++;
                    break;
                }
            }
            
            save_books();
            save_transactions();
            
            printf("\n✓ Book returned successfully!\n");
            return;
        }
    }
    
    printf("\nTransaction not found or book already returned!\n");
}

void view_transactions() {
    if(transaction_count == 0) {
        printf("\nNo transactions found!\n");
        return;
    }
    
    printf("\n========== TRANSACTIONS ==========\n");
    for(int i = 0; i < transaction_count; i++) {
        printf("\nTransaction ID: %d\n", transactions[i].transaction_id);
        printf("Book ID: %d\n", transactions[i].book_id);
        printf("Member ID: %d\n", transactions[i].member_id);
        printf("Issue Date: %s\n", transactions[i].issue_date);
        printf("Return Date: %s\n", transactions[i].return_date);
        printf("Status: %s\n", transactions[i].returned ? "Returned" : "Issued");
        printf("----------------------------------\n");
    }
}

void generate_html() {
    FILE *fp = fopen("library_report.html", "w");
    if(fp == NULL) {
        printf("Error creating HTML file!\n");
        return;
    }
    
    fprintf(fp, "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n");
    fprintf(fp, "<meta charset=\"UTF-8\">\n");
    fprintf(fp, "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n");
    fprintf(fp, "<title>Library Management Report</title>\n");
    fprintf(fp, "<style>\n");
    fprintf(fp, "* { margin: 0; padding: 0; box-sizing: border-box; }\n");
    fprintf(fp, "body { font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; background: linear-gradient(135deg, #667eea 0%%, #764ba2 100%%); min-height: 100vh; padding: 20px; }\n");
    fprintf(fp, ".container { max-width: 1200px; margin: 0 auto; background: white; border-radius: 15px; box-shadow: 0 20px 60px rgba(0,0,0,0.3); overflow: hidden; }\n");
    fprintf(fp, "header { background: linear-gradient(135deg, #667eea 0%%, #764ba2 100%%); color: white; padding: 40px; text-align: center; }\n");
    fprintf(fp, "h1 { font-size: 2.5em; margin-bottom: 10px; text-shadow: 2px 2px 4px rgba(0,0,0,0.2); }\n");
    fprintf(fp, ".subtitle { font-size: 1.1em; opacity: 0.9; }\n");
    fprintf(fp, ".content { padding: 40px; }\n");
    fprintf(fp, ".section { margin-bottom: 50px; }\n");
    fprintf(fp, "h2 { color: #667eea; margin-bottom: 20px; font-size: 1.8em; border-bottom: 3px solid #667eea; padding-bottom: 10px; }\n");
    fprintf(fp, "table { width: 100%%; border-collapse: collapse; background: white; box-shadow: 0 2px 10px rgba(0,0,0,0.1); border-radius: 8px; overflow: hidden; }\n");
    fprintf(fp, "th { background: linear-gradient(135deg, #667eea 0%%, #764ba2 100%%); color: white; padding: 15px; text-align: left; font-weight: 600; text-transform: uppercase; font-size: 0.9em; letter-spacing: 1px; }\n");
    fprintf(fp, "td { padding: 15px; border-bottom: 1px solid #f0f0f0; }\n");
    fprintf(fp, "tr:hover { background: #f8f9ff; transition: background 0.3s; }\n");
    fprintf(fp, "tr:last-child td { border-bottom: none; }\n");
    fprintf(fp, ".status { padding: 5px 15px; border-radius: 20px; font-size: 0.85em; font-weight: 600; display: inline-block; }\n");
    fprintf(fp, ".status.available { background: #d4edda; color: #155724; }\n");
    fprintf(fp, ".status.issued { background: #fff3cd; color: #856404; }\n");
    fprintf(fp, ".status.returned { background: #d1ecf1; color: #0c5460; }\n");
    fprintf(fp, ".stats { display: grid; grid-template-columns: repeat(auto-fit, minmax(200px, 1fr)); gap: 20px; margin-bottom: 40px; }\n");
    fprintf(fp, ".stat-card { background: linear-gradient(135deg, #667eea 0%%, #764ba2 100%%); color: white; padding: 25px; border-radius: 10px; text-align: center; box-shadow: 0 5px 15px rgba(0,0,0,0.2); }\n");
    fprintf(fp, ".stat-number { font-size: 2.5em; font-weight: bold; margin-bottom: 5px; }\n");
    fprintf(fp, ".stat-label { font-size: 1em; opacity: 0.9; }\n");
    fprintf(fp, "footer { background: #2d3748; color: white; text-align: center; padding: 20px; }\n");
    fprintf(fp, ".empty { text-align: center; padding: 40px; color: #999; font-style: italic; }\n");
    fprintf(fp, "</style>\n</head>\n<body>\n");
    
    fprintf(fp, "<div class=\"container\">\n");
    fprintf(fp, "<header>\n");
    fprintf(fp, "<h1>📚 Library Management System</h1>\n");
    fprintf(fp, "<p class=\"subtitle\">Comprehensive Library Report</p>\n");
    fprintf(fp, "</header>\n");
    
    fprintf(fp, "<div class=\"content\">\n");
    
    // Statistics
    int total_available = 0;
    int total_copies = 0;
    int active_issues = 0;
    
    for(int i = 0; i < book_count; i++) {
        total_available += books[i].available;
        total_copies += books[i].total_copies;
    }
    
    for(int i = 0; i < transaction_count; i++) {
        if(!transactions[i].returned) active_issues++;
    }
    
    fprintf(fp, "<div class=\"stats\">\n");
    fprintf(fp, "<div class=\"stat-card\"><div class=\"stat-number\">%d</div><div class=\"stat-label\">Total Books</div></div>\n", book_count);
    fprintf(fp, "<div class=\"stat-card\"><div class=\"stat-number\">%d</div><div class=\"stat-label\">Total Copies</div></div>\n", total_copies);
    fprintf(fp, "<div class=\"stat-card\"><div class=\"stat-number\">%d</div><div class=\"stat-label\">Members</div></div>\n", member_count);
    fprintf(fp, "<div class=\"stat-card\"><div class=\"stat-number\">%d</div><div class=\"stat-label\">Active Issues</div></div>\n", active_issues);
    fprintf(fp, "</div>\n");
    
    // Books Section
    fprintf(fp, "<div class=\"section\">\n");
    fprintf(fp, "<h2>📖 Books Collection</h2>\n");
    
    if(book_count > 0) {
        fprintf(fp, "<table>\n<tr><th>ID</th><th>Title</th><th>Author</th><th>ISBN</th><th>Available</th><th>Status</th></tr>\n");
        for(int i = 0; i < book_count; i++) {
            fprintf(fp, "<tr><td>%d</td><td>%s</td><td>%s</td><td>%s</td><td>%d/%d</td><td>", 
                    books[i].id, books[i].title, books[i].author, books[i].isbn, 
                    books[i].available, books[i].total_copies);
            if(books[i].available > 0) {
                fprintf(fp, "<span class=\"status available\">Available</span>");
            } else {
                fprintf(fp, "<span class=\"status issued\">All Issued</span>");
            }
            fprintf(fp, "</td></tr>\n");
        }
        fprintf(fp, "</table>\n");
    } else {
        fprintf(fp, "<p class=\"empty\">No books in the library</p>\n");
    }
    
    fprintf(fp, "</div>\n");
    
    // Members Section
    fprintf(fp, "<div class=\"section\">\n");
    fprintf(fp, "<h2>👥 Registered Members</h2>\n");
    
    if(member_count > 0) {
        fprintf(fp, "<table>\n<tr><th>ID</th><th>Name</th><th>Email</th><th>Phone</th></tr>\n");
        for(int i = 0; i < member_count; i++) {
            fprintf(fp, "<tr><td>%d</td><td>%s</td><td>%s</td><td>%s</td></tr>\n",
                    members[i].id, members[i].name, members[i].email, members[i].phone);
        }
        fprintf(fp, "</table>\n");
    } else {
        fprintf(fp, "<p class=\"empty\">No registered members</p>\n");
    }
    
    fprintf(fp, "</div>\n");
    
    // Transactions Section
    fprintf(fp, "<div class=\"section\">\n");
    fprintf(fp, "<h2>📊 Transaction History</h2>\n");
    
    if(transaction_count > 0) {
        fprintf(fp, "<table>\n<tr><th>Trans ID</th><th>Book ID</th><th>Member ID</th><th>Issue Date</th><th>Return Date</th><th>Status</th></tr>\n");
        for(int i = 0; i < transaction_count; i++) {
            fprintf(fp, "<tr><td>%d</td><td>%d</td><td>%d</td><td>%s</td><td>%s</td><td>",
                    transactions[i].transaction_id, transactions[i].book_id, 
                    transactions[i].member_id, transactions[i].issue_date, 
                    transactions[i].return_date);
            if(transactions[i].returned) {
                fprintf(fp, "<span class=\"status returned\">Returned</span>");
            } else {
                fprintf(fp, "<span class=\"status issued\">Issued</span>");
            }
            fprintf(fp, "</td></tr>\n");
        }
        fprintf(fp, "</table>\n");
    } else {
        fprintf(fp, "<p class=\"empty\">No transactions recorded</p>\n");
    }
    
    fprintf(fp, "</div>\n");
    fprintf(fp, "</div>\n");
    
    fprintf(fp, "<footer>\n");
    fprintf(fp, "<p>&copy; 2025 Library Management System. All rights reserved.</p>\n");
    fprintf(fp, "</footer>\n");
    
    fprintf(fp, "</div>\n");
    fprintf(fp, "</body>\n</html>\n");
    
    fclose(fp);
    
    printf("\n✓ HTML report generated successfully! Check 'library_report.html'\n");
}