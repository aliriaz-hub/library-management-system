# Library Management System
**Language:** C++

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

## Features

- Add, remove, and search books by title, author, or ISBN
- Register new members with complete borrowing history
- Issue books with automatic due date set 14 days from today
- Accept book returns and auto-calculate overdue fines (Rs. 5 per day)
- Display all currently borrowed books and pending returns
- Check real-time availability status of any book

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

