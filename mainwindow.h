#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringListModel>
#include <QSqlQueryModel>
#include "rent.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void showUsers();
    void showBooks();
    void showRentals();

    void filterUsers(const QString &text);
    void filterBooks(const QString &text);
    void filterRentals();

    void loadRentForm();
    void rentBook();
    void returnSelectedRental();
    void updateReturnBooksForUser();
    void showManagePanel();
    void hideManagePanel();



private:
    Ui::MainWindow *ui;

    QStringListModel *usersModel = nullptr;
    QStringListModel *booksModel = nullptr;
    QSqlQueryModel *rentalsModel = nullptr;

    QSortFilterProxyModel *usersProxy = nullptr;
    QSortFilterProxyModel *booksProxy = nullptr;
    Rent *rentalsProxy = nullptr;

    void refreshUserList();
    void refreshAvailableBooks();
};
#endif // MAINWINDOW_H
