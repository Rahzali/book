#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "database.h"
#include "rent.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QStringListModel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    if (Database::connect()) {
        qDebug() << "Database connected.";
    }

    ui->userList->hide();
    ui->userEdit->hide();
    ui->bookList->hide();
    ui->bookEdit->hide();
    ui->rentTable->hide();
    ui->userEdit2->hide();
    ui->bookEdit2->hide();
    ui->rentalText1->hide();
    ui->rentalText2->hide();
    ui->addBookEdit->hide();
    ui->addBookButton->hide();
    ui->addUserEdit->hide();
    ui->addUserButton->hide();
    ui->removeBookCombo->hide();
    ui->removeBookButton->hide();
    ui->removeUserCombo->hide();
    ui->removeUserButton->hide();
    ui->userText->hide();
    ui->userText2->hide();
    ui->bookText->hide();
    ui->bookText2->hide();
    ui->rentUserCombo->hide();
    ui->rentBookCombo->hide();
    ui->rentButton->hide();
    ui->returnUserCombo->hide();
    ui->returnBookCombo->hide();
    ui->returnButton->hide();

    connect(ui->showUsers, &QPushButton::clicked, this, &MainWindow::showUsers);
    connect(ui->showBooks, &QPushButton::clicked, this, &MainWindow::showBooks);
    connect(ui->showRentals, &QPushButton::clicked, this, &MainWindow::showRentals);

    connect(ui->userEdit, &QLineEdit::textChanged, this, &MainWindow::filterUsers);
    connect(ui->bookEdit, &QLineEdit::textChanged, this, &MainWindow::filterBooks);
    connect(ui->userEdit2, &QLineEdit::textChanged, this, &MainWindow::filterRentals);
    connect(ui->bookEdit2, &QLineEdit::textChanged, this, &MainWindow::filterRentals);

    connect(ui->rentButton, &QPushButton::clicked, this, &MainWindow::rentBook);
    connect(ui->returnButton, &QPushButton::clicked, this, &MainWindow::returnSelectedRental);
    connect(ui->returnUserCombo, &QComboBox::currentIndexChanged, this, &MainWindow::updateReturnBooksForUser);
    connect(ui->addUserButton, &QPushButton::clicked, this, &MainWindow::addUser);
    connect(ui->addBookButton, &QPushButton::clicked, this, &MainWindow::addBook);
    connect(ui->removeUserButton, &QPushButton::clicked, this, &MainWindow::removeUser);
    connect(ui->removeBookButton, &QPushButton::clicked, this, &MainWindow::removeBook);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete usersModel;
    delete booksModel;
    delete rentalsModel;
    delete usersProxy;
    delete booksProxy;
    delete rentalsProxy;
}

void MainWindow::showUsers()
{
    delete usersModel;
    delete usersProxy;

    usersModel = new QStringListModel(this);
    QStringList users;

    QSqlQuery query("SELECT name FROM users ORDER BY name");
    while (query.next()) {
        users << query.value(0).toString();
    }

    usersModel->setStringList(users);

    usersProxy = new QSortFilterProxyModel(this);
    usersProxy->setSourceModel(usersModel);
    usersProxy->setFilterCaseSensitivity(Qt::CaseInsensitive);

    ui->userList->setModel(usersProxy);

    ui->userList->show();
    ui->userEdit->show();
    ui->addUserEdit->show();
    ui->addUserButton->show();
    ui->removeUserCombo->clear();
    QSqlQuery uq("SELECT name FROM users ORDER BY name");
    while (uq.next()) {
        ui->removeUserCombo->addItem(uq.value(0).toString());
    }
    ui->removeUserCombo->show();
    ui->removeUserButton->show();
    ui->userText->show();
    ui->userText2->show();

    ui->bookList->hide();
    ui->bookEdit->hide();
    ui->rentTable->hide();
    ui->userEdit2->hide();
    ui->bookEdit2->hide();
    ui->addBookEdit->hide();
    ui->addBookButton->hide();
    ui->removeBookCombo->hide();
    ui->removeBookButton->hide();
    ui->bookText->hide();
    ui->bookText2->hide();
    ui->rentBookCombo->hide();
    ui->returnBookCombo->hide();
    ui->rentUserCombo->hide();
    ui->returnUserCombo->hide();

}

void MainWindow::showBooks()
{
    delete booksModel;
    delete booksProxy;

    booksModel = new QStringListModel(this);
    QStringList books;

    QSqlQuery query("SELECT title FROM books ORDER BY title");
    while (query.next()) {
        books << query.value(0).toString();
    }

    booksModel->setStringList(books);

    booksProxy = new QSortFilterProxyModel(this);
    booksProxy->setSourceModel(booksModel);
    booksProxy->setFilterCaseSensitivity(Qt::CaseInsensitive);

    ui->bookList->setModel(booksProxy);

    ui->bookList->show();
    ui->bookEdit->show();
    ui->addBookEdit->show();
    ui->addBookButton->show();
    ui->removeBookCombo->clear();
    QSqlQuery bq("SELECT title FROM books ORDER BY title");
    while (bq.next()) {
        ui->removeBookCombo->addItem(bq.value(0).toString());
    }
    ui->removeBookCombo->show();
    ui->removeBookButton->show();
    ui->bookText->show();
    ui->bookText2->show();

    ui->addUserEdit->hide();
    ui->addUserButton->hide();
    ui->userList->hide();
    ui->userEdit->hide();
    ui->rentTable->hide();
    ui->userEdit2->hide();
    ui->bookEdit2->hide();
    ui->removeUserCombo->hide();
    ui->removeUserButton->hide();
    ui->userText->hide();
    ui->userText2->hide();
    ui->rentBookCombo->hide();
    ui->returnBookCombo->hide();
    ui->rentUserCombo->hide();
    ui->returnUserCombo->hide();
}

void MainWindow::showRentals()
{
    delete rentalsModel;
    delete rentalsProxy;

    rentalsModel = new QSqlQueryModel(this);
    rentalsModel->setQuery(R"(
        SELECT users.name AS "User", books.title AS "Book"
        FROM rentals
        JOIN users ON users.id = rentals.user_id
        JOIN books ON books.id = rentals.book_id
        WHERE return_date IS NULL
        ORDER BY users.name
    )");

    rentalsProxy = new Rent(this);
    rentalsProxy->setSourceModel(rentalsModel);
    ui->rentTable->setModel(rentalsProxy);
    ui->rentTable->resizeColumnsToContents();

    ui->rentTable->show();
    ui->userEdit2->show();
    ui->bookEdit2->show();

    ui->rentUserCombo->show();
    ui->rentBookCombo->show();
    ui->rentButton->show();

    ui->returnUserCombo->show();
    ui->returnBookCombo->show();
    ui->returnButton->show();

    loadRentForm();


    ui->userList->hide();
    ui->userEdit->hide();
    ui->bookList->hide();
    ui->bookEdit->hide();

    ui->addUserEdit->hide();
    ui->addUserButton->hide();
    ui->removeUserCombo->hide();
    ui->removeUserButton->hide();
    ui->addBookEdit->hide();
    ui->addBookButton->hide();
    ui->removeBookCombo->hide();
    ui->removeBookButton->hide();
}


void MainWindow::filterUsers(const QString &text)
{
    if (usersProxy)
        usersProxy->setFilterFixedString(text);
}

void MainWindow::filterBooks(const QString &text)
{
    if (booksProxy)
        booksProxy->setFilterFixedString(text);
}

void MainWindow::filterRentals()
{
    if (!rentalsProxy) return;

    rentalsProxy->setUserFilter(ui->userEdit2->text());
    rentalsProxy->setBookFilter(ui->bookEdit2->text());
}

void MainWindow::loadRentForm()
{
    refreshUserList();
    refreshAvailableBooks();

    ui->returnUserCombo->clear();
    QSqlQuery query(R"(
        SELECT DISTINCT users.id, users.name
        FROM rentals
        JOIN users ON users.id = rentals.user_id
        WHERE return_date IS NULL
        ORDER BY users.name
    )");

    while (query.next()) {
        ui->returnUserCombo->addItem(query.value(1).toString(), query.value(0));
    }

    updateReturnBooksForUser();
}

void MainWindow::refreshUserList()
{
    ui->rentUserCombo->clear();
    QSqlQuery query("SELECT id, name FROM users");
    while (query.next()) {
        ui->rentUserCombo->addItem(query.value(1).toString(), query.value(0));
    }
}

void MainWindow::refreshAvailableBooks()
{
    ui->rentBookCombo->clear();
    QSqlQuery query(R"(
        SELECT id, title FROM books
        WHERE id NOT IN (
            SELECT book_id FROM rentals WHERE return_date IS NULL
        )
    )");
    while (query.next()) {
        ui->rentBookCombo->addItem(query.value(1).toString(), query.value(0));
    }
}

void MainWindow::rentBook()
{
    int userId = ui->rentUserCombo->currentData().toInt();
    int bookId = ui->rentBookCombo->currentData().toInt();

    if (userId == 0 || bookId == 0) return;

    QSqlQuery query;
    query.prepare("INSERT INTO rentals (user_id, book_id) VALUES (?, ?)");
    query.addBindValue(userId);
    query.addBindValue(bookId);
    query.exec();

    refreshAvailableBooks();
    loadRentForm();
    refreshRentalsTable();
}

void MainWindow::updateReturnBooksForUser()
{
    ui->returnBookCombo->clear();
    int userId = ui->returnUserCombo->currentData().toInt();

    QSqlQuery query;
    query.prepare(R"(
        SELECT books.id, books.title
        FROM rentals
        JOIN books ON books.id = rentals.book_id
        WHERE rentals.user_id = ? AND return_date IS NULL
    )");
    query.addBindValue(userId);
    query.exec();

    while (query.next()) {
        ui->returnBookCombo->addItem(query.value(1).toString(), query.value(0));
    }
}

void MainWindow::returnSelectedRental()
{
    int userId = ui->returnUserCombo->currentData().toInt();
    int bookId = ui->returnBookCombo->currentData().toInt();

    if (userId == 0 || bookId == 0) return;

    QSqlQuery query;
    query.prepare(R"(
        UPDATE rentals
        SET return_date = CURRENT_DATE
        WHERE user_id = ? AND book_id = ? AND return_date IS NULL
    )");
    query.addBindValue(userId);
    query.addBindValue(bookId);
    query.exec();

    refreshAvailableBooks();
    loadRentForm();
    refreshRentalsTable();
}

void MainWindow::addUser()
{
    QString name = ui->addUserEdit->text().trimmed();
    if (name.isEmpty()) return;

    QSqlQuery query;
    query.prepare("INSERT INTO users (name) VALUES (?)");
    query.addBindValue(name);

    if (!query.exec()) {
        qDebug() << "Failed to add user:" << query.lastError().text();
    } else {
        ui->addUserEdit->clear();
        showUsers();
    }
}
void MainWindow::addBook()
{
    QString title = ui->addBookEdit->text().trimmed();
    if (title.isEmpty()) return;

    QSqlQuery query;
    query.prepare("INSERT INTO books (title) VALUES (?)");
    query.addBindValue(title);

    if (!query.exec()) {
        qDebug() << "Failed to add book:" << query.lastError().text();
    } else {
        ui->addBookEdit->clear();
        showBooks();
    }
}
void MainWindow::refreshRentalsTable()
{
    if (!rentalsModel) {
        rentalsModel = new QSqlQueryModel(this);
    }

    rentalsModel->setQuery(R"(
        SELECT users.name AS "User", books.title AS "Book"
        FROM rentals
        JOIN users ON users.id = rentals.user_id
        JOIN books ON books.id = rentals.book_id
        WHERE return_date IS NULL
        ORDER BY users.name
    )");

    if (!rentalsProxy) {
        rentalsProxy = new Rent(this);
        ui->rentTable->setModel(rentalsProxy);
    }

    rentalsProxy->setSourceModel(rentalsModel);
    ui->rentTable->resizeColumnsToContents();
}
void MainWindow::removeUser()
{
    QString name = ui->removeUserCombo->currentText().trimmed();
    if (name.isEmpty()) return;

    QSqlQuery query;
    query.prepare("DELETE FROM users WHERE name = ?");
    query.addBindValue(name);

    if (!query.exec()) {
        qDebug() << "Failed to remove user:" << query.lastError().text();
    } else {
        showUsers();
    }
}

void MainWindow::removeBook()
{
    QString title = ui->removeBookCombo->currentText().trimmed();
    if (title.isEmpty()) return;

    QSqlQuery query;
    query.prepare("DELETE FROM books WHERE title = ?");
    query.addBindValue(title);

    if (!query.exec()) {
        qDebug() << "Failed to remove book:" << query.lastError().text();
    } else {
        showBooks();
    }
}

