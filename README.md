# Library Management System

**Project 01 of 05 — OOP Class Projects**
**Level:** Beginner – Intermediate
**Language:** C++
**Effort:** 2 – 3 Days

---

## Project Overview

The Library Management System simulates how a real public or school library operates.
It is a console-based application that allows librarians to manage the book catalogue,
register new members, issue books, accept returns, and track overdue fines.

---

## OOP Concepts Covered

| Concept | How It Is Applied |
|---|---|
| Classes & Objects | Book, Member, Transaction, Library each defined as separate classes |
| Encapsulation | Private fields with public getters and setters |
| Inheritance | Librarian inherits from Member |
| Abstraction | Borrow and return logic hidden behind simple method calls |

---

## Class Design

### Book
- **Attributes:** title, author, ISBN, genre, is_available (all private)
- **Methods:** get_title(), get_ISBN(), mark_borrowed(), mark_returned(), display_info()

### Member
- **Attributes:** member_id (private), name, email, borrowed_books[]
- **Methods:** add_borrowed(), remove_borrowed(), view_history(), get_id()

### Librarian (inherits Member)
- **Attributes:** staff_id
- **Extra privileges:** add_book(), remove_book() via Library class

### Transaction
- **Attributes:** transaction_id, book_isbn, member_id, date_issued, due_date
- **Methods:** calculate_fine(), get_status(), display()

### Library
- **Attributes:** books[], members[], transactions[]
- **Methods:** register_member(), issue_book(), accept_return(), search_book(), generate_report()

---

## Features

- Add, remove, and search books by title, author, or ISBN
- Register new members with complete borrowing history
- Issue books with automatic due date set 14 days from today
- Accept book returns and auto-calculate overdue fines (Rs. 5 per day)
- Display all currently borrowed books and pending returns
- Check real-time availability status of any book

---

## How to Compile and Run

```
g++ -std=c++17 -o library_management library_management.cpp
./library_management
```

On Windows:
```
g++ -std=c++17 -o library_management.exe library_management.cpp
library_management.exe
```

---

## Menu Options

```
1. Add Book
2. Remove Book
3. Search Book
4. View All Books
5. Register Member
6. View All Members
7. Issue Book
8. Return Book
9. Generate Report
0. Exit
```

---

## Evaluation Rubric

| Criteria | Marks |
|---|---|
| Correct OOP structure | 20 |
| Encapsulation | 20 |
| Inheritance | 15 |
| Borrow / Return / Fine logic | 25 |
| Menu-driven interface | 10 |
| Code readability and comments | 10 |
| **Total** | **100** |

---

*Learn with Junaid Zaffar — Project 01 of 05 | OOP Class Projects*
