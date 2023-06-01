#ifndef PAGEFUNCTION_H
#define PAGEFUNCTION_H

#include <QMainWindow>
#include <QGridLayout>
#include <QSqlQuery>
#include <QLabel>
#include <QCheckBox>
#include <QRadioButton>
#include <QObject>
#include <QSpinBox>
#include <QPushButton>
#include <QDateEdit>

class PageFunction
{
public:
    PageFunction();

    //Список билетов
    static QGridLayout* DrowUsersList(QSqlQuery* query);
    static QGridLayout* DrowBookList(QSqlQuery* query, int Role);

    //Выдача книг
    static QGridLayout* DrowBook(QSqlQuery* query, int type = 0);
    static QGridLayout* DrowUsers(QSqlQuery* query);
    static QGridLayout* DrowBookCounter(QSqlQuery* query, int Role);
};

#endif // PAGEFUNCTION_H
