#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <iomanip>

using namespace std;

// ─────────────────────────────────────────
// CLASS: Book
// ─────────────────────────────────────────
class Book {
private:
    string title;
    string author;
    string ISBN;
    string genre;
    bool is_available;

public:
    Book(string t, string a, string isbn, string g = "General")
        : title(t), author(a), ISBN(isbn), genre(g), is_available(true) {}

    // Getters
    string get_title()  const { return title; }
    string get_author() const { return author; }
    string get_ISBN()   const { return ISBN; }
    string get_genre()  const { return genre; }
    bool   get_availability() const { return is_available; }

    // Setters
    void mark_borrowed() { is_available = false; }
    void mark_returned() { is_available = true; }

    void display_info() const {
        cout << "  Title  : " << title << "\n"
             << "  Author : " << author << "\n"
             << "  ISBN   : " << ISBN << "\n"
             << "  Genre  : " << genre << "\n"
             << "  Status : " << (is_available ? "Available" : "Borrowed") << "\n";
    }
};

// ─────────────────────────────────────────
// CLASS: Member
// ─────────────────────────────────────────
class Member {
private:
    string member_id;

public:
    string name;
    string email;
    vector<string> borrowed_books; // stores ISBNs

    Member(string id, string n, string e)
        : member_id(id), name(n), email(e) {}

    // Getters / Setters
    string get_id()    const { return member_id; }
    string get_name()  const { return name; }
    string get_email() const { return email; }

    void add_borrowed(const string& isbn) {
        borrowed_books.push_back(isbn);
    }

    void remove_borrowed(const string& isbn) {
        for (int i = 0; i < (int)borrowed_books.size(); i++) {
            if (borrowed_books[i] == isbn) {
                borrowed_books.erase(borrowed_books.begin() + i);
                return;
            }
        }
    }

    void view_history() const {
        if (borrowed_books.empty()) {
            cout << "  No books currently borrowed.\n";
        } else {
            cout << "  Currently borrowed ISBNs:\n";
            for (const string& isbn : borrowed_books)
                cout << "    - " << isbn << "\n";
        }
    }

    virtual void display_info() const {
        cout << "  Member ID : " << member_id << "\n"
             << "  Name      : " << name << "\n"
             << "  Email     : " << email << "\n";
    }

    virtual ~Member() {}
};

// ─────────────────────────────────────────
// CLASS: Librarian (inherits Member)
// ─────────────────────────────────────────
class Librarian : public Member {
public:
    string staff_id;

    Librarian(string id, string n, string e, string sid)
        : Member(id, n, e), staff_id(sid) {}

    void display_info() const override {
        Member::display_info();
        cout << "  Staff ID  : " << staff_id << " (Librarian)\n";
    }

    // Librarian-only privileges are called via Library class methods below
};

// ─────────────────────────────────────────
// CLASS: Transaction
// ─────────────────────────────────────────
class Transaction {
private:
    static int counter;
    string transaction_id;
    string book_isbn;
    string book_title;
    string member_id;
    time_t date_issued;
    time_t due_date;
    bool returned;

public:
    Transaction(const string& isbn, const string& btitle, const string& mid)
        : book_isbn(isbn), book_title(btitle), member_id(mid), returned(false) {
        counter++;
        transaction_id = "TXN" + to_string(counter);
        date_issued = time(nullptr);
        due_date = date_issued + (14 * 24 * 60 * 60); // 14 days
    }

    string get_transaction_id() const { return transaction_id; }
    string get_book_isbn()      const { return book_isbn; }
    string get_member_id()      const { return member_id; }
    bool   is_returned()        const { return returned; }

    void mark_returned() { returned = true; }

    double calculate_fine() const {
        if (returned) return 0.0;
        time_t now = time(nullptr);
        if (now <= due_date) return 0.0;
        double overdue_seconds = difftime(now, due_date);
        int overdue_days = (int)(overdue_seconds / (24 * 60 * 60));
        return overdue_days * 5.0; // Rs. 5 per day
    }

    string get_status() const {
        if (returned) return "Returned";
        double fine = calculate_fine();
        if (fine > 0) return "Overdue";
        return "Active";
    }

    void display() const {
        char issued_buf[20], due_buf[20];
        strftime(issued_buf, sizeof(issued_buf), "%Y-%m-%d", localtime(&date_issued));
        strftime(due_buf,    sizeof(due_buf),    "%Y-%m-%d", localtime(&due_date));

        cout << "  Transaction ID : " << transaction_id << "\n"
             << "  Book ISBN      : " << book_isbn << "\n"
             << "  Book Title     : " << book_title << "\n"
             << "  Member ID      : " << member_id << "\n"
             << "  Date Issued    : " << issued_buf << "\n"
             << "  Due Date       : " << due_buf << "\n"
             << "  Status         : " << get_status() << "\n"
             << "  Fine           : Rs. " << fixed << setprecision(2) << calculate_fine() << "\n";
    }
};

int Transaction::counter = 0;

// ─────────────────────────────────────────
// CLASS: Library
// ─────────────────────────────────────────
class Library {
public:
    vector<Book*>        books;
    vector<Member*>      members;
    vector<Transaction*> transactions;

    ~Library() {
        for (Book* b        : books)        delete b;
        for (Member* m      : members)      delete m;
        for (Transaction* t : transactions) delete t;
    }

    // ── Book management (Librarian only in a real system) ──

    void add_book(const string& title, const string& author,
                  const string& isbn,  const string& genre = "General") {
        books.push_back(new Book(title, author, isbn, genre));
        cout << "Book added: " << title << "\n";
    }

    void remove_book(const string& isbn) {
        for (int i = 0; i < (int)books.size(); i++) {
            if (books[i]->get_ISBN() == isbn) {
                if (!books[i]->get_availability()) {
                    cout << "Cannot remove a currently borrowed book.\n";
                    return;
                }
                cout << "Removed book: " << books[i]->get_title() << "\n";
                delete books[i];
                books.erase(books.begin() + i);
                return;
            }
        }
        cout << "Book with ISBN " << isbn << " not found.\n";
    }

    // ── Member management ──

    void register_member(const string& id, const string& name, const string& email) {
        members.push_back(new Member(id, name, email));
        cout << "Member registered: " << name << "\n";
    }

    // ── Search ──

    void search_book(const string& query) const {
        bool found = false;
        cout << "\nSearch results for \"" << query << "\":\n";
        for (const Book* b : books) {
            if (b->get_title().find(query)  != string::npos ||
                b->get_author().find(query) != string::npos ||
                b->get_ISBN().find(query)   != string::npos) {
                b->display_info();
                cout << "  ----------\n";
                found = true;
            }
        }
        if (!found) cout << "  No books matched.\n";
    }

    // ── Issue / Return ──

    void issue_book(const string& member_id, const string& isbn) {
        Member* member = find_member(member_id);
        Book*   book   = find_book(isbn);

        if (!member) { cout << "Member not found.\n"; return; }
        if (!book)   { cout << "Book not found.\n";   return; }
        if (!book->get_availability()) {
            cout << "Book is currently not available.\n"; return;
        }

        book->mark_borrowed();
        member->add_borrowed(isbn);
        transactions.push_back(new Transaction(isbn, book->get_title(), member_id));
        cout << "Book \"" << book->get_title() << "\" issued to " << member->get_name() << ".\n";
    }

    void accept_return(const string& member_id, const string& isbn) {
        Member* member = find_member(member_id);
        Book*   book   = find_book(isbn);

        if (!member) { cout << "Member not found.\n"; return; }
        if (!book)   { cout << "Book not found.\n";   return; }

        // Find the matching open transaction
        Transaction* txn = nullptr;
        for (Transaction* t : transactions) {
            if (t->get_book_isbn() == isbn &&
                t->get_member_id() == member_id &&
                !t->is_returned()) {
                txn = t;
                break;
            }
        }
        if (!txn) { cout << "No active transaction found for this book and member.\n"; return; }

        double fine = txn->calculate_fine();
        txn->mark_returned();
        book->mark_returned();
        member->remove_borrowed(isbn);

        cout << "Book \"" << book->get_title() << "\" returned by " << member->get_name() << ".\n";
        if (fine > 0)
            cout << "Overdue fine: Rs. " << fixed << setprecision(2) << fine << "\n";
        else
            cout << "No fine.\n";
    }

    // ── Reports ──

    void generate_report() const {
        cout << "\n===== Library Report =====\n";
        cout << "Total books    : " << books.size() << "\n";
        cout << "Total members  : " << members.size() << "\n";
        cout << "Transactions   : " << transactions.size() << "\n";

        cout << "\n-- Currently Borrowed Books --\n";
        bool any = false;
        for (const Transaction* t : transactions) {
            if (!t->is_returned()) {
                t->display();
                cout << "  ----------\n";
                any = true;
            }
        }
        if (!any) cout << "  None.\n";
    }

    void view_all_members() const {
        cout << "\n-- All Members --\n";
        if (members.empty()) { cout << "  None registered.\n"; return; }
        for (const Member* m : members) {
            m->display_info();
            m->view_history();
            cout << "  ----------\n";
        }
    }

    void view_all_books() const {
        cout << "\n-- All Books --\n";
        if (books.empty()) { cout << "  No books in catalogue.\n"; return; }
        for (const Book* b : books) {
            b->display_info();
            cout << "  ----------\n";
        }
    }

private:
    Member* find_member(const string& id) const {
        for (Member* m : members)
            if (m->get_id() == id) return m;
        return nullptr;
    }

    Book* find_book(const string& isbn) const {
        for (Book* b : books)
            if (b->get_ISBN() == isbn) return b;
        return nullptr;
    }
};

// ─────────────────────────────────────────
// MENU HELPERS
// ─────────────────────────────────────────

void print_menu() {
    cout << "\n===== Library Management System =====\n"
         << "1. Add Book\n"
         << "2. Remove Book\n"
         << "3. Search Book\n"
         << "4. View All Books\n"
         << "5. Register Member\n"
         << "6. View All Members\n"
         << "7. Issue Book\n"
         << "8. Return Book\n"
         << "9. Generate Report\n"
         << "0. Exit\n"
         << "Choose an option: ";
}

// ─────────────────────────────────────────
// MAIN
// ─────────────────────────────────────────

int main() {
    Library lib;

    // Seed some sample data
    lib.add_book("The Alchemist",    "Paulo Coelho", "ISBN001", "Fiction");
    lib.add_book("Clean Code",       "Robert Martin","ISBN002", "Technology");
    lib.add_book("Atomic Habits",    "James Clear",  "ISBN003", "Self-Help");
    lib.register_member("M001", "Ali Hassan",  "ali@mail.com");
    lib.register_member("M002", "Sara Khan",   "sara@mail.com");

    int choice;
    do {
        print_menu();
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            string title, author, isbn, genre;
            cout << "Title  : "; getline(cin, title);
            cout << "Author : "; getline(cin, author);
            cout << "ISBN   : "; getline(cin, isbn);
            cout << "Genre  : "; getline(cin, genre);
            lib.add_book(title, author, isbn, genre);

        } else if (choice == 2) {
            string isbn;
            cout << "Enter ISBN to remove: "; getline(cin, isbn);
            lib.remove_book(isbn);

        } else if (choice == 3) {
            string query;
            cout << "Enter title / author / ISBN: "; getline(cin, query);
            lib.search_book(query);

        } else if (choice == 4) {
            lib.view_all_books();

        } else if (choice == 5) {
            string id, name, email;
            cout << "Member ID : "; getline(cin, id);
            cout << "Name      : "; getline(cin, name);
            cout << "Email     : "; getline(cin, email);
            lib.register_member(id, name, email);

        } else if (choice == 6) {
            lib.view_all_members();

        } else if (choice == 7) {
            string mid, isbn;
            cout << "Member ID : "; getline(cin, mid);
            cout << "ISBN      : "; getline(cin, isbn);
            lib.issue_book(mid, isbn);

        } else if (choice == 8) {
            string mid, isbn;
            cout << "Member ID : "; getline(cin, mid);
            cout << "ISBN      : "; getline(cin, isbn);
            lib.accept_return(mid, isbn);

        } else if (choice == 9) {
            lib.generate_report();

        } else if (choice != 0) {
            cout << "Invalid option. Try again.\n";
        }

    } while (choice != 0);

    cout << "Exiting. Goodbye.\n";
    return 0;
}
