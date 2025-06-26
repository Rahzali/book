#ifndef RENT_H
#define RENT_H

#include <QSortFilterProxyModel>

class Rent : public QSortFilterProxyModel {
    Q_OBJECT

public:
    explicit Rent(QObject *parent = nullptr);

    void setUserFilter(const QString &text) { userFilter = text; invalidateFilter(); }
    void setBookFilter(const QString &text) { bookFilter = text; invalidateFilter(); }

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    QString userFilter;
    QString bookFilter;
};

#endif
