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

class PageFunction
{
public:
    PageFunction();
    static QGridLayout* DrowBook(QSqlQuery* query);
    static QGridLayout* DrowUsers(QSqlQuery* query);
    static QGridLayout* DrowBookCounter(QSqlQuery* query);
};

#endif // PAGEFUNCTION_H
