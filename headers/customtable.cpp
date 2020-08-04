#include "customtable.h"

void CustomTable::InitializeTable(const QStringList &headers)
{
    tableView->setColumnCount(4);
    tableView->setShowGrid(false); // Включаем сетку
    tableView->verticalHeader()->setVisible(false);
    // Разрешаем выделение только одного элемента
    // tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    // Разрешаем выделение построчно
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // Устанавливаем заголовки колонок
    tableView->setHorizontalHeaderLabels(headers);


}

CustomTable::CustomTable(const QStringList &headers,QObject *_parent):QObject(_parent)
{
    tableView=new QTableWidget();
    InitializeTable(headers);

   connect(tableView,&QTableWidget::cellChanged,this, &CustomTable::cellClicked);

}

CustomTable::~CustomTable()
{
    delete tableView;
}

void CustomTable::addRowData(const QString &rowData)
{
    vectorRowData.push_back({rowData,true,false});
    updateTable();
}



void CustomTable::updateTable()
{
    for(int i=0;i<vectorRowData.size();++i)
    {
        tableView->insertRow(i);

        QTableWidgetItem *itemIndex = new QTableWidgetItem();
       // item->data(Qt::CheckStateRole);
       itemIndex->setFlags(Qt::ItemIsEnabled);
       itemIndex->setText(QString::number(i));

        tableView->setItem(i,0, itemIndex);

        QTableWidgetItem *item = new QTableWidgetItem();
       // item->data(Qt::CheckStateRole);
       item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);
        if(std::get<1>(vectorRowData[i]))
        item->setCheckState(Qt::Checked);
        else  item->setCheckState(Qt::Unchecked);

        tableView->setItem(i,1, item);

        QTableWidgetItem *itemName = new QTableWidgetItem();
       // item->data(Qt::CheckStateRole);
       itemName->setFlags(Qt::ItemIsEnabled);
       itemName->setText(std::get<0>(vectorRowData[i]));

        tableView->setItem(i,2, itemName);
        tableView->resizeColumnsToContents();

        QTableWidgetItem *itemSelected = new QTableWidgetItem();
       // item->data(Qt::CheckStateRole);
       itemSelected->setFlags(Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);
       if(std::get<2>(vectorRowData[i]))
       itemSelected->setCheckState(Qt::Checked);
       else  itemSelected->setCheckState(Qt::Unchecked);

        tableView->setItem(i,3, itemSelected);


    }
//    for(int i = 0;i<10; i++){
//        // Вставляем строку
//        tableView->insertRow(i);
//        /* Устанавливаем в первую колонку id забирая его из результата SQL-запроса
//            * Эта колонка будет скрыта
//            * */

//        tableView->setItem(i,0, new QTableWidgetItem("index"));

//        // Создаём элемент, который будет выполнять роль чекбокса
//        QTableWidgetItem *item = new QTableWidgetItem();
//        item->data(Qt::CheckStateRole);
//        /* Проверяем, на статус нечетности, если нечетное устройство, то
//            * выставляем состояние чекбокса в Checked, иначе в Unchecked
//            * */
//        item->setCheckState(Qt::Checked);
//        //   item->setCheckState(Qt::Unchecked);

//        // Устанавливаем чекбокс во вторую колонку


//        tableView->setItem(i,1, item);
//        // Далее забираем все данные из результата запроса и устанавливаем в остальные поля
//        tableView->setItem(i,2, new QTableWidgetItem("Name"));
//        //tableView->setItem(i,3, new QTableWidgetItem(query.value(3).toString()));
//        //tableView->setItem(i,4, new QTableWidgetItem(query.value(4).toString()));
//        tableView->resizeColumnsToContents();
    //    }
}

void CustomTable::cellClicked(int row, int column)
{
    qDebug()<<"Column clicked"<<row<<" "<<column;
}

QTableWidget *CustomTable::getTablePtr()
{ return tableView;

}
