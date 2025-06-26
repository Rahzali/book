#include "rent.h"

Rent::Rent(QObject *parent)
    : QSortFilterProxyModel(parent) {}

bool Rent::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    QModelIndex userIndex = sourceModel()->index(source_row, 0, source_parent);
    QModelIndex bookIndex = sourceModel()->index(source_row, 1, source_parent);

    QString userText = sourceModel()->data(userIndex).toString();
    QString bookText = sourceModel()->data(bookIndex).toString();

    return userText.contains(userFilter, Qt::CaseInsensitive) &&
           bookText.contains(bookFilter, Qt::CaseInsensitive);
}
