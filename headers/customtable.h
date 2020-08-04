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
    //Object name | Visible | Selected
    std::vector<std::tuple<QString,bool,bool>> vectorRowData;

private:
    void InitializeTable(const QStringList &headers);
    void updateTable();

private slots:
    void cellClicked(int row, int column);
public:
    CustomTable(const QStringList &headers,QObject *_parent=nullptr);
    ~CustomTable();

public slots:
    void addRowData(const QString &objectName);




    QTableWidget * getTablePtr();


};

#endif // CUSTOMTABLE_H
