#ifndef CUSTOMTABLE_H
#define CUSTOMTABLE_H

#include <QTableWidget>
#include <QTableView>
#include <QHeaderView>
#include <vector>
#include <QObject>
#include <QDebug>
#include <tuple>
class CustomTable: public QObject
{
Q_OBJECT
private:
    QTableWidget * tableView;

private:
    void InitializeTable(const QStringList &headers);


private slots:
    void cellClicked(int row, int column);
public slots:
    void updateTable(const std::vector<std::tuple<QString,bool,bool>> &vectorRowData);
public:
    CustomTable(const QStringList &headers,QObject *_parent=nullptr);
    ~CustomTable();





    QTableWidget * getTablePtr();


};

#endif // CUSTOMTABLE_H
