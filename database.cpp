#include "database.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

bool Database::connect() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("library.db");

    if (!db.open()) {
        qDebug() << "Database open error:" << db.lastError().text();
        return false;
    }

    QSqlQuery query;

    query.exec("CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT)");
    query.exec("CREATE TABLE IF NOT EXISTS books (id INTEGER PRIMARY KEY AUTOINCREMENT, title TEXT, author TEXT)");
    query.exec("CREATE TABLE IF NOT EXISTS rentals (id INTEGER PRIMARY KEY AUTOINCREMENT, user_id INTEGER, book_id INTEGER, "
               "FOREIGN KEY(user_id) REFERENCES users(id), FOREIGN KEY(book_id) REFERENCES books(id))");

    seedTestData();

    return true;
}

void Database::seedTestData() {
    QSqlQuery query;

    query.exec("INSERT INTO users (name) VALUES ('Alice')");
    query.exec("INSERT INTO users (name) VALUES ('Bob')");

    query.exec("INSERT INTO books (title, author) VALUES ('1984', 'George Orwell')");
    query.exec("INSERT INTO books (title, author) VALUES ('Brave New World', 'Aldous Huxley')");

    query.exec("INSERT INTO rentals (user_id, book_id) VALUES (1, 1)");
    query.exec("INSERT INTO rentals (user_id, book_id) VALUES (1, 2)");
}

