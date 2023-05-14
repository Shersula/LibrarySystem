#ifndef PAGEFUNCTION_H
#define PAGEFUNCTION_H

#include <QMainWindow>
#include <QGridLayout>
#include <QSqlQuery>
#include <QLabel>
#include <QCheckBox>

class PageFunction
{
public:
    PageFunction();
    static QGridLayout* DrowBook(QSqlQuery* query);
};

#endif // PAGEFUNCTION_H
