#include "database.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

bool Database::connect()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("library.db");

    if (!db.open()) {
        qDebug() << "Database open error:" << db.lastError().text();
        return false;
    }

    QSqlQuery query;

    // --- Create Tables ---
    query.exec(R"(
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL
        )
    )");

    query.exec(R"(
        CREATE TABLE IF NOT EXISTS books (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            title TEXT NOT NULL,
            author TEXT
        )
    )");

    query.exec(R"(
        CREATE TABLE IF NOT EXISTS rentals (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            user_id INTEGER,
            book_id INTEGER,
            rent_date DATE DEFAULT CURRENT_DATE,
            return_date DATE,
            FOREIGN KEY(user_id) REFERENCES users(id),
            FOREIGN KEY(book_id) REFERENCES books(id)
        )
    )");

    seedTestData();

    return true;
}

void Database::seedTestData()
{
    QSqlQuery query;

    // --- Seed Users ---
    query.exec("SELECT COUNT(*) FROM users");
    if (query.next() && query.value(0).toInt() == 0) {
        query.exec("INSERT INTO users (name) VALUES ('Alice')");
        query.exec("INSERT INTO users (name) VALUES ('Bob')");
        query.exec("INSERT INTO users (name) VALUES ('Charlie')");
    }

    // --- Seed Books ---
    query.exec("SELECT COUNT(*) FROM books");
    if (query.next() && query.value(0).toInt() == 0) {
        query.exec("INSERT INTO books (title, author) VALUES ('1984', 'George Orwell')");
        query.exec("INSERT INTO books (title, author) VALUES ('Brave New World', 'Aldous Huxley')");
        query.exec("INSERT INTO books (title, author) VALUES ('Fahrenheit 451', 'Ray Bradbury')");
        query.exec("INSERT INTO books (title, author) VALUES ('To Kill a Mockingbird', 'Harper Lee')");
        query.exec("INSERT INTO books (title, author) VALUES ('The Hobbit', 'J.R.R. Tolkien')");
    }

    // --- Seed Rentals ---
    query.exec("SELECT COUNT(*) FROM rentals");
    if (query.next() && query.value(0).toInt() == 0) {
        // Alice rents '1984' (id = 1)
        query.exec("INSERT INTO rentals (user_id, book_id) VALUES (1, 1)");

        // Bob rents 'Brave New World' (id = 2)
        query.exec("INSERT INTO rentals (user_id, book_id) VALUES (2, 2)");

        // Charlie rented and returned 'Fahrenheit 451' (id = 3)
        query.exec("INSERT INTO rentals (user_id, book_id, return_date) VALUES (3, 3, DATE('now', '-10 days'))");
    }
}
