#include "pagefunction.h"

PageFunction::PageFunction()
{
}


QGridLayout* PageFunction::DrowUsers(QSqlQuery* query)
{
    int row = 0;

    QGridLayout* MainGrid = new QGridLayout;
    MainGrid->setVerticalSpacing(5);
    MainGrid->setHorizontalSpacing(15);
    MainGrid->setContentsMargins(0, 0, 0, 0);
    MainGrid->setAlignment(Qt::AlignTop | Qt::AlignCenter);

    QLabel* TicketTitle = new QLabel;
    TicketTitle->setText("Номер билета");
    TicketTitle->setStyleSheet("font-family: 'Acme'; font-size: 18px; color: #000000;");
    TicketTitle->setAlignment(Qt::AlignCenter);
    MainGrid->addWidget(TicketTitle, row, 0);

    QLabel* NameTitle = new QLabel;
    NameTitle->setText("Фио");
    NameTitle->setStyleSheet("font-family: 'Acme'; font-size: 18px; color: #000000;");
    NameTitle->setAlignment(Qt::AlignCenter);
    MainGrid->addWidget(NameTitle, row, 1);

    QLabel* CheckStatus = new QLabel;
    CheckStatus->setText("Состояние");
    CheckStatus->setStyleSheet("font-family: 'Acme'; font-size: 18px; color: #000000;");
    CheckStatus->setAlignment(Qt::AlignCenter);
    MainGrid->addWidget(CheckStatus, row, 2);

    while (query->next())
    {
        row++;
        int ID = query->value("ID").toInt();
        QString Ticket = query->value("TicketNumber").toString();
        QString Name = query->value("FirstName").toString() + " " + query->value("MiddleName").toString() + " " + query->value("LastName").toString();

        QLabel* TicketNumber = new QLabel;
        TicketNumber->setText(Ticket);
        TicketNumber->setStyleSheet("font-family: 'Acme'; font-size: 18px; color: #000000;");
        TicketNumber->setAlignment(Qt::AlignCenter);
        MainGrid->addWidget(TicketNumber, row, 0);

        QLabel* UserName = new QLabel;
        UserName->setText(Name);
        UserName->setStyleSheet("font-family: 'Acme'; font-size: 18px; color: #000000;");
        UserName->setAlignment(Qt::AlignCenter);
        MainGrid->addWidget(UserName, row, 1);

        QPushButton* Status = new QPushButton();
        Status->setCheckable(true);
        Status->setAutoExclusive(true);
        Status->setMaximumSize(100, 20);
        Status->setCursor(Qt::PointingHandCursor);
        Status->setText("Выбрать");
        Status->setStyleSheet("QPushButton{background-color: rgb(135, 156, 207); border-color: rgb(0, 0, 0); border: solid; border-width: 2px; border-radius: 15px;}\
        QPushButton:hover{background-color: rgb(85, 98, 130);}\
        QPushButton:pressed{border: unset;}\
        QPushButton:checked{border: unset; background-color: rgb(85, 98, 130);}");
        Status->setProperty("ID", ID);
        MainGrid->addWidget(Status, row, 2);
    }

    return MainGrid;
}

QGridLayout* PageFunction::DrowBook(QSqlQuery* query)
{
    int row = 0;

    QGridLayout* MainGrid = new QGridLayout;
    MainGrid->setVerticalSpacing(5);
    MainGrid->setHorizontalSpacing(15);
    MainGrid->setContentsMargins(0, 0, 0, 0);
    MainGrid->setAlignment(Qt::AlignTop | Qt::AlignCenter);

    QLabel* BookTitle = new QLabel;
    BookTitle->setText("Название книги");
    BookTitle->setStyleSheet("font-family: 'Acme'; font-size: 18px; color: #000000;");
    BookTitle->setAlignment(Qt::AlignCenter);
    MainGrid->addWidget(BookTitle, row, 0);

    QLabel* AuthorTitle = new QLabel;
    AuthorTitle->setText("Автор");
    AuthorTitle->setStyleSheet("font-family: 'Acme'; font-size: 18px; color: #000000;");
    AuthorTitle->setAlignment(Qt::AlignCenter);
    MainGrid->addWidget(AuthorTitle, row, 1);

    QLabel* CountTitle = new QLabel;
    CountTitle->setText("Количество");
    CountTitle->setStyleSheet("font-family: 'Acme'; font-size: 18px; color: #000000;");
    CountTitle->setAlignment(Qt::AlignCenter);
    MainGrid->addWidget(CountTitle, row, 2);

    QLabel* CheckStatus = new QLabel;
    CheckStatus->setText("Состояние");
    CheckStatus->setStyleSheet("font-family: 'Acme'; font-size: 18px; color: #000000;");
    CheckStatus->setAlignment(Qt::AlignCenter);
    MainGrid->addWidget(CheckStatus, row, 3);

    while (query->next())
    {
        row++;
        int ID = query->value("ID").toInt();
        QString BookCount = query->value("Count").toString();
        QString Name = query->value("Name").toString();
        QString Author = query->value("Author").toString();

        QLabel* BookName = new QLabel;
        BookName->setText(Name);
        BookName->setStyleSheet("font-family: 'Acme'; font-size: 18px; color: #000000;");
        BookName->setAlignment(Qt::AlignCenter);
        MainGrid->addWidget(BookName, row, 0);

        QLabel* AuthorName = new QLabel;
        AuthorName->setText(Author);
        AuthorName->setStyleSheet("font-family: 'Acme'; font-size: 18px; color: #000000;");
        AuthorName->setAlignment(Qt::AlignCenter);
        MainGrid->addWidget(AuthorName, row, 1);

        QLabel* Count = new QLabel;
        Count->setText(BookCount);
        Count->setStyleSheet("font-family: 'Acme'; font-size: 18px; color: #000000;");
        Count->setAlignment(Qt::AlignCenter);
        MainGrid->addWidget(Count, row, 2);

        QWidget* StatusWidget = new QWidget;
        QHBoxLayout* StatusBox = new QHBoxLayout();
        StatusBox->setAlignment(Qt::AlignCenter);
        StatusWidget->setLayout(StatusBox);
        QCheckBox* Status = new QCheckBox;
        StatusBox->addWidget(Status);
        Status->setProperty("ID", ID);
        MainGrid->addWidget(StatusWidget, row, 3);
    }

    return MainGrid;
}

QGridLayout* PageFunction::DrowBookCounter(QSqlQuery* query)
{
    int row = 0;

    QGridLayout* MainGrid = new QGridLayout;
    MainGrid->setVerticalSpacing(5);
    MainGrid->setHorizontalSpacing(15);
    MainGrid->setContentsMargins(0, 0, 0, 0);
    MainGrid->setAlignment(Qt::AlignTop | Qt::AlignCenter);

    QLabel* BookTitle = new QLabel;
    BookTitle->setText("Название книги");
    BookTitle->setStyleSheet("font-family: 'Acme'; font-size: 18px; color: #000000;");
    BookTitle->setAlignment(Qt::AlignCenter);
    MainGrid->addWidget(BookTitle, row, 0);

    QLabel* AuthorTitle = new QLabel;
    AuthorTitle->setText("Автор");
    AuthorTitle->setStyleSheet("font-family: 'Acme'; font-size: 18px; color: #000000;");
    AuthorTitle->setAlignment(Qt::AlignCenter);
    MainGrid->addWidget(AuthorTitle, row, 1);

    QLabel* CountTitle = new QLabel;
    CountTitle->setText("Количество");
    CountTitle->setStyleSheet("font-family: 'Acme'; font-size: 18px; color: #000000;");
    CountTitle->setAlignment(Qt::AlignCenter);
    MainGrid->addWidget(CountTitle, row, 2);

    QLabel* CheckStatus = new QLabel;
    CheckStatus->setText("Количетсво для выдачи");
    CheckStatus->setStyleSheet("font-family: 'Acme'; font-size: 18px; color: #000000;");
    CheckStatus->setAlignment(Qt::AlignCenter);
    MainGrid->addWidget(CheckStatus, row, 3);

    while (query->next())
    {
        row++;
        int ID = query->value("ID").toInt();
        QString BookCount = query->value("Count").toString();
        QString Name = query->value("Name").toString();
        QString Author = query->value("Author").toString();

        QLabel* BookName = new QLabel;
        BookName->setText(Name);
        BookName->setStyleSheet("font-family: 'Acme'; font-size: 18px; color: #000000;");
        BookName->setAlignment(Qt::AlignCenter);
        MainGrid->addWidget(BookName, row, 0);

        QLabel* AuthorName = new QLabel;
        AuthorName->setText(Author);
        AuthorName->setStyleSheet("font-family: 'Acme'; font-size: 18px; color: #000000;");
        AuthorName->setAlignment(Qt::AlignCenter);
        MainGrid->addWidget(AuthorName, row, 1);

        QLabel* Count = new QLabel;
        Count->setText(BookCount);
        Count->setStyleSheet("font-family: 'Acme'; font-size: 18px; color: #000000;");
        Count->setAlignment(Qt::AlignCenter);
        MainGrid->addWidget(Count, row, 2);

        QSpinBox* Counter = new QSpinBox();
        Counter->setAlignment(Qt::AlignCenter);
        Counter->setStyleSheet("QSpinBox{font-family: 'Acme'; font-size: 18px; color: #000000; margin-top: 5px; margin-bottom: 5px; margin-left: 60px; margin-right: 60px;}\
        QSpinBox::up-button {width: 25; subcontrol-origin: margin; subcontrol-position: center top; bottom: 5px}\
        QSpinBox::down-button {width: 25; subcontrol-origin: margin; subcontrol-position: center bottom; top: 5px}");
        Counter->setMinimum(0);
        Counter->setMaximum(BookCount.toInt());
        Counter->setProperty("ID", ID);
        MainGrid->addWidget(Counter, row, 3);
    }

    return MainGrid;
}
