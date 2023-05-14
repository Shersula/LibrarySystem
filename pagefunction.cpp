#include "pagefunction.h"

PageFunction::PageFunction()
{

}
QGridLayout* PageFunction::DrowBook(QSqlQuery* query)
{
    int row = 0;

    QGridLayout* MainGrid = new QGridLayout;
    MainGrid->setVerticalSpacing(5);
    MainGrid->setHorizontalSpacing(15);
    MainGrid->setContentsMargins(0, 0, 0, 0);

    QLabel* BookTitle = new QLabel;
    MainGrid->addWidget(BookTitle, row, 0);
    BookTitle->setText("Название книги");
    BookTitle->setStyleSheet("font-family: 'Acme'; font-size: 18px; color: #000000;");

    QLabel* AuthorTitle = new QLabel;
    MainGrid->addWidget(AuthorTitle, row, 1);
    AuthorTitle->setText("Автор");
    AuthorTitle->setStyleSheet("font-family: 'Acme'; font-size: 18px; color: #000000;");

    QLabel* CheckStatus = new QLabel;
    MainGrid->addWidget(CheckStatus, row, 2);
    CheckStatus->setText("Состояние");
    CheckStatus->setStyleSheet("font-family: 'Acme'; font-size: 18px; color: #000000;");

    while (query->next())
    {
        row++;
        QString Name = query->value("Name").toString();
        QString Author = query->value("Author").toString();

        QLabel* BookName = new QLabel;
        MainGrid->addWidget(BookName, row, 0);
        BookName->setText(Name);
        BookName->setStyleSheet("font-family: 'Acme'; font-size: 18px; color: #000000;");

        QLabel* AuthorName = new QLabel;
        MainGrid->addWidget(AuthorName, row, 1);
        AuthorName->setText(Author);
        AuthorName->setStyleSheet("font-family: 'Acme'; font-size: 18px; color: #000000;");

        QCheckBox* Status = new QCheckBox;
        MainGrid->addWidget(Status, row, 2);
    }
    return MainGrid;
}
