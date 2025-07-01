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

    // USERS
    query.exec(R"(
CREATE TABLE IF NOT EXISTS users (
    id   INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL
))");

    // AUTHORS
    query.exec(R"(
CREATE TABLE IF NOT EXISTS authors (
    id   INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL
))");

    // GENRES
    query.exec(R"(
CREATE TABLE IF NOT EXISTS genres (
    id   INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL
))");

    // BOOKS
    query.exec(R"(
CREATE TABLE IF NOT EXISTS books (
    id    INTEGER PRIMARY KEY AUTOINCREMENT,
    title TEXT NOT NULL
))");

    // COMMENTS
    query.exec(R"(
CREATE TABLE IF NOT EXISTS comments (
    id      INTEGER PRIMARY KEY AUTOINCREMENT,
    user_id INTEGER,
    book_id INTEGER,
    content TEXT NOT NULL,
    FOREIGN KEY(user_id) REFERENCES users(id),
    FOREIGN KEY(book_id) REFERENCES books(id)
))");

    // RENTALS
    query.exec(R"(
CREATE TABLE IF NOT EXISTS rentals (
    id          INTEGER PRIMARY KEY AUTOINCREMENT,
    user_id     INTEGER,
    book_id     INTEGER,
    rent_date   DATE DEFAULT CURRENT_DATE,
    return_date DATE,
    FOREIGN KEY(user_id) REFERENCES users(id),
    FOREIGN KEY(book_id) REFERENCES books(id)
))");

    // FAVOURITES
    query.exec(R"(
CREATE TABLE IF NOT EXISTS favourites
(
    user_id INTEGER NOT NULL,
    book_id INTEGER NOT NULL,
    FOREIGN KEY(user_id) REFERENCES users (id),
    FOREIGN KEY(book_id) REFERENCES books (id),
    PRIMARY KEY (user_id, book_id)
);
))");

    // AUTHORS_BOOKS_JOIN_TABLE
    query.exec(R"(
CREATE TABLE IF NOT EXISTS authors_books_join_table (
    authors_id INTEGER,
    book_id    INTEGER,
    FOREIGN KEY(authors_id) REFERENCES authors(id),
    FOREIGN KEY(book_id) REFERENCES books(id),
    PRIMARY KEY(authors_id, book_id)
))");

    // GENRES_BOOKS_JOIN_TABLE
    query.exec(R"(
CREATE TABLE IF NOT EXISTS genres_books_join_table (
    genres_id INTEGER,
    book_id   INTEGER,
    FOREIGN KEY(genres_id) REFERENCES genres(id),
    FOREIGN KEY(book_id) REFERENCES books(id),
    PRIMARY KEY(genres_id, book_id)
))");

    seedTestData();

    return true;
}

void Database::seedTestData()
{
    QSqlQuery query;
    QVariant userId1, userId2, userId3;
    QVariant authorId1, authorId2, authorId3, authorId4, authorId5;
    QVariant genreId1, genreId2, genreId3;
    QVariant bookId1, bookId2, bookId3, bookId4, bookId5;

    // USERS
    query.exec("SELECT COUNT(*) FROM users");
    if (query.next() && query.value(0).toInt() == 0) {
        query.exec("INSERT INTO users (name) VALUES ('Alice')");
        userId1 = query.lastInsertId();
        query.exec("INSERT INTO users (name) VALUES ('Bob')");
        userId2 = query.lastInsertId();
        query.exec("INSERT INTO users (name) VALUES ('Charlie')");
        userId3 = query.lastInsertId();
    }

    // AUTHORS
    query.exec("SELECT COUNT(*) FROM authors");
    if (query.next() && query.value(0).toInt() == 0) {
        query.exec("INSERT INTO authors (name) VALUES ('George Orwell')");
        authorId1 = query.lastInsertId();
        query.exec("INSERT INTO authors (name) VALUES ('Aldous Huxley')");
        authorId2 = query.lastInsertId();
        query.exec("INSERT INTO authors (name) VALUES ('Ray Bradbury')");
        authorId3 = query.lastInsertId();
        query.exec("INSERT INTO authors (name) VALUES ('Harper Lee')");
        authorId4 = query.lastInsertId();
        query.exec("INSERT INTO authors (name) VALUES ('J.R.R. Tolkien')");
        authorId5 = query.lastInsertId();
    }

    // GENRES
    query.exec("SELECT COUNT(*) FROM genres");
    if (query.next() && query.value(0).toInt() == 0) {
        query.exec("INSERT INTO genres (name) VALUES ('Dystopian')");
        genreId1 = query.lastInsertId();
        query.exec("INSERT INTO genres (name) VALUES ('Fantasy')");
        genreId2 = query.lastInsertId();
        query.exec("INSERT INTO genres (name) VALUES ('Classic')");
        genreId3 = query.lastInsertId();
    }

    // BOOKS & JOINS
    query.exec("SELECT COUNT(*) FROM books");
    if (query.next() && query.value(0).toInt() == 0) {
        // 1984
        query.exec("INSERT INTO books (title) VALUES ('1984')");
        bookId1 = query.lastInsertId();
        query.prepare("INSERT INTO authors_books_join_table (authors_id, book_id) VALUES (?, ?)");
        query.addBindValue(authorId1);
        query.addBindValue(bookId1);
        query.exec();
        query.prepare("INSERT INTO genres_books_join_table (genres_id, book_id) VALUES (?, ?)");
        query.addBindValue(genreId1);
        query.addBindValue(bookId1);
        query.exec();

        // Brave New World
        query.exec("INSERT INTO books (title) VALUES ('Brave New World')");
        bookId2 = query.lastInsertId();
        query.prepare("INSERT INTO authors_books_join_table (authors_id, book_id) VALUES (?, ?)");
        query.addBindValue(authorId2);
        query.addBindValue(bookId2);
        query.exec();
        query.prepare("INSERT INTO genres_books_join_table (genres_id, book_id) VALUES (?, ?)");
        query.addBindValue(genreId1);
        query.addBindValue(bookId2);
        query.exec();

        // Fahrenheit 451
        query.exec("INSERT INTO books (title) VALUES ('Fahrenheit 451')");
        bookId3 = query.lastInsertId();
        query.prepare("INSERT INTO authors_books_join_table (authors_id, book_id) VALUES (?, ?)");
        query.addBindValue(authorId3);
        query.addBindValue(bookId3);
        query.exec();
        query.prepare("INSERT INTO genres_books_join_table (genres_id, book_id) VALUES (?, ?)");
        query.addBindValue(genreId1);
        query.addBindValue(bookId3);
        query.exec();

        // To Kill a Mockingbird
        query.exec("INSERT INTO books (title) VALUES ('To Kill a Mockingbird')");
        bookId4 = query.lastInsertId();
        query.prepare("INSERT INTO authors_books_join_table (authors_id, book_id) VALUES (?, ?)");
        query.addBindValue(authorId4);
        query.addBindValue(bookId4);
        query.exec();
        query.prepare("INSERT INTO genres_books_join_table (genres_id, book_id) VALUES (?, ?)");
        query.addBindValue(genreId3);
        query.addBindValue(bookId4);
        query.exec();

        // The Hobbit
        query.exec("INSERT INTO books (title) VALUES ('The Hobbit')");
        bookId5 = query.lastInsertId();
        query.prepare("INSERT INTO authors_books_join_table (authors_id, book_id) VALUES (?, ?)");
        query.addBindValue(authorId5);
        query.addBindValue(bookId5);
        query.exec();
        query.prepare("INSERT INTO genres_books_join_table (genres_id, book_id) VALUES (?, ?)");
        query.addBindValue(genreId2);
        query.addBindValue(bookId5);
        query.exec();
    }

    // RENTALS
    query.exec("SELECT COUNT(*) FROM rentals");
    if (query.next() && query.value(0).toInt() == 0) {
        query.prepare("INSERT INTO rentals (user_id, book_id, rent_date) VALUES (?, ?, CURRENT_DATE)");
        query.addBindValue(userId1);
        query.addBindValue(bookId1);
        query.exec();

        query.prepare("INSERT INTO rentals (user_id, book_id, rent_date) VALUES (?, ?, CURRENT_DATE)");
        query.addBindValue(userId2);
        query.addBindValue(bookId2);
        query.exec();

        query.prepare(
            "INSERT INTO rentals (user_id, book_id, rent_date, return_date) VALUES (?, ?, DATE('now', '-15 days'), DATE('now', '-10 days'))");
        query.addBindValue(userId3);
        query.addBindValue(bookId3);
        query.exec();
    }
}
