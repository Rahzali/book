#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "database.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    if (Database::connect()) {
        loadData();
    } else {
        qDebug() << "Database connection failed.";
    }

    connect(ui->toggleButton, &QPushButton::clicked, this, &MainWindow::toggleTableVisibility);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete model;
}

void MainWindow::toggleTableVisibility()
{
    bool isVisible = ui->tableView->isVisible();
    ui->tableView->setVisible(!isVisible);

    if (isVisible)
        ui->toggleButton->setText("Show Table");
    else
        ui->toggleButton->setText("Hide Table");
}


void MainWindow::loadData()
{
    if (model) {
        delete model;
    }

    model = new QSqlQueryModel(this);
    model->setQuery(R"(
        SELECT users.name AS "User",
               COALESCE(books.title, '(no rentals)') AS "Book Title"
        FROM users
        LEFT JOIN rentals ON users.id = rentals.user_id
        LEFT JOIN books ON rentals.book_id = books.id
        ORDER BY users.name
    )");

    if (model->lastError().isValid()) {
        qDebug() << "Query Error:" << model->lastError().text();
    }

    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();
}
