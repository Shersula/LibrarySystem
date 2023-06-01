#include "pagefunction.h"

PageFunction::PageFunction()
{
}

//////////////////////////////////Список билетов///////////////////////////////////////////////
QGridLayout* PageFunction::DrowUsersList(QSqlQuery* query)
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

    QLabel* StudentTicketTitle = new QLabel;
    StudentTicketTitle->setText("Номер студ. билета");
    StudentTicketTitle->setStyleSheet("font-family: 'Acme'; font-size: 18px; color: #000000;");
    StudentTicketTitle->setAlignment(Qt::AlignCenter);
    MainGrid->addWidget(StudentTicketTitle, row, 1);

    QLabel* NameTitle = new QLabel;
    NameTitle->setText("ФИО");
    NameTitle->setStyleSheet("font-family: 'Acme'; font-size: 18px; color: #000000;");
    NameTitle->setAlignment(Qt::AlignCenter);
    MainGrid->addWidget(NameTitle, row, 2);

    QLabel* IssueDateTitle = new QLabel;
    IssueDateTitle->setText("Дата выдачи");
    IssueDateTitle->setStyleSheet("font-family: 'Acme'; font-size: 18px; color: #000000;");
    IssueDateTitle->setAlignment(Qt::AlignCenter);
    MainGrid->addWidget(IssueDateTitle, row, 3);

    QLabel* Function = new QLabel;
    Function->setText("Состояние");
    Function->setStyleSheet("font-family: 'Acme'; font-size: 18px; color: #000000;");
    Function->setAlignment(Qt::AlignCenter);
    MainGrid->addWidget(Function, row, 4);

    while (query->next())
    {
        row++;
        int ID = query->value("ID").toInt();
        QString Ticket = query->value("TicketNumber").toString();
        QString StudentTicket = query->value("StudentIDNumber").toString();
        QDate IssueDate = query->value("IssueDate").toDate();
        QString Name = query->value("LastName").toString() + " " + query->value("FirstName").toString() + " " + query->value("MiddleName").toString();

        QLabel* TicketNumber = new QLabel;
        TicketNumber->setText(Ticket);
        TicketNumber->setStyleSheet("font-family: 'Acme'; font-size: 18px; color: #000000;");
        TicketNumber->setAlignment(Qt::AlignCenter);
        MainGrid->addWidget(TicketNumber, row, 0);

        QLabel* StudentTicketNumber = new QLabel;
        StudentTicketNumber->setText(StudentTicket);
        StudentTicketNumber->setStyleSheet("font-family: 'Acme'; font-size: 18px; color: #000000;");
        StudentTicketNumber->setAlignment(Qt::AlignCenter);
        MainGrid->addWidget(StudentTicketNumber, row, 1);

        QLabel* UserName = new QLabel;
        UserName->setText(Name);
        UserName->setStyleSheet("font-family: 'Acme'; font-size: 18px; color: #000000;");
        UserName->setAlignment(Qt::AlignCenter);
        MainGrid->addWidget(UserName, row, 2);

        QLabel* Date = new QLabel;
        Date->setText(IssueDate.toString("dd.MM.yyyy"));
        Date->setStyleSheet("font-family: 'Acme'; font-size: 18px; color: #000000;");
        Date->setAlignment(Qt::AlignCenter);
        MainGrid->addWidget(Date, row, 3);

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
        MainGrid->addWidget(Status, row, 4);
    }

    return MainGrid;
}

QGridLayout* PageFunction::DrowBookList(QSqlQuery* query, int Role)
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

    QLabel* ReturnDateTitle = new QLabel;
    ReturnDateTitle->setText("Дата возврата");
    ReturnDateTitle->setStyleSheet("font-family: 'Acme'; font-size: 18px; color: #000000;");
    ReturnDateTitle->setAlignment(Qt::AlignCenter);
    MainGrid->addWidget(ReturnDateTitle, row, 3);

    if(Role == 1)
    {
        QLabel* ReturnStatusTitle = new QLabel;
        ReturnStatusTitle->setText("Возврат");
        ReturnStatusTitle->setStyleSheet("font-family: 'Acme'; font-size: 18px; color: #000000;");
        ReturnStatusTitle->setAlignment(Qt::AlignCenter);
        MainGrid->addWidget(ReturnStatusTitle, row, 4);
    }

    while (query->next())
    {
        row++;
        int ID = query->value("ID").toInt();
        int UserRole = query->value("Role").toInt();
        QString BookCount = query->value("Count").toString();
        QString Name = query->value("Name").toString();
        QString Author = query->value("Author").toString();
        QDate IssueDate = query->value("ReturnDate").toDate();

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

        QLabel* Date = new QLabel;
        if(UserRole == 0)
        {
            if(IssueDate < QDate::currentDate())
            {
                Date->setText(IssueDate.toString("dd.MM.yyyy") + " (Просрочено)");
                Date->setStyleSheet("font-family: 'Acme'; font-size: 18px; color: #ff0000;");
            }
            else
            {
                Date->setText(IssueDate.toString("dd.MM.yyyy"));
                Date->setStyleSheet("font-family: 'Acme'; font-size: 18px; color: #000000;");
            }
        }
        else
        {
            Date->setText("Бессрочно");
            Date->setStyleSheet("font-family: 'Acme'; font-size: 18px; color: #000000;");
        }
        Date->setAlignment(Qt::AlignCenter);
        MainGrid->addWidget(Date, row, 3);

        if(Role == 1)
        {
            QWidget* StatusWidget = new QWidget;
            QHBoxLayout* StatusBox = new QHBoxLayout();
            StatusBox->setAlignment(Qt::AlignCenter);
            StatusWidget->setLayout(StatusBox);
            QCheckBox* Status = new QCheckBox;
            Status->setStyleSheet("QCheckBox::indicator{border:unset; background: #98A3BE; border-radius: 5px;}\
            QCheckBox::indicator:checked{border:solid; background: #98A3BE; border-width: 2px; border-radius: 5px;}");
            StatusBox->addWidget(Status);
            Status->setProperty("ID", ID);
            MainGrid->addWidget(StatusWidget, row, 4);
        }
    }

    return MainGrid;
}

//////////////////////////////////Выдача книг///////////////////////////////////////////////
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
    NameTitle->setText("ФИО");
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
        int Role = query->value("Role").toInt();
        QString Ticket = query->value("TicketNumber").toString();
        QString Name = query->value("LastName").toString() + " " + query->value("FirstName").toString() + " " + query->value("MiddleName").toString();

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
        Status->setProperty("Role", Role);
        MainGrid->addWidget(Status, row, 2);
    }

    return MainGrid;
}

QGridLayout* PageFunction::DrowBook(QSqlQuery* query, int type)
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

        if(type == 0)
        {
            QWidget* StatusWidget = new QWidget;
            QHBoxLayout* StatusBox = new QHBoxLayout();
            StatusBox->setAlignment(Qt::AlignCenter);
            StatusWidget->setLayout(StatusBox);
            QCheckBox* Status = new QCheckBox;
            Status->setStyleSheet("QCheckBox::indicator{border:unset; background: #98A3BE; border-radius: 5px;}\
            QCheckBox::indicator:checked{border:solid; background: #98A3BE; border-width: 2px; border-radius: 5px;}");
            StatusBox->addWidget(Status);
            Status->setProperty("ID", ID);
            MainGrid->addWidget(StatusWidget, row, 3);
        }
        else
        {
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
            MainGrid->addWidget(Status, row, 3);
        }
    }

    return MainGrid;
}

QGridLayout* PageFunction::DrowBookCounter(QSqlQuery* query, int Role)
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

    if(Role == 0)
    {
        QLabel* ReturnDateTitle = new QLabel;
        ReturnDateTitle->setText("Дата возврата");
        ReturnDateTitle->setStyleSheet("font-family: 'Acme'; font-size: 18px; color: #000000;");
        ReturnDateTitle->setAlignment(Qt::AlignCenter);
        MainGrid->addWidget(ReturnDateTitle, row, 4);
    }

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
        QSpinBox::up-button{width: 25; subcontrol-origin: margin; subcontrol-position: center top; bottom: 5px}\
        QSpinBox::down-button{width: 25; subcontrol-origin: margin; subcontrol-position: center bottom; top: 5px}\
        QSpinBox::up-button:hover{bottom: 6px;}\
        QSpinBox::down-button:hover{top: 6px;}\
        QSpinBox::up-button:pressed{bottom: 5px;}\
        QSpinBox::down-button:pressed{top: 5px;}");
        Counter->setMinimum(0);
        Counter->setMaximum(BookCount.toInt());
        Counter->setProperty("ID", ID);
        MainGrid->addWidget(Counter, row, 3);

        if(Role == 0)
        {
            QDateEdit* ReturnDate = new QDateEdit;
            ReturnDate->setAlignment(Qt::AlignCenter);
            ReturnDate->setMinimumDate(QDate::currentDate().addDays(5));
            ReturnDate->setMaximumDate(QDate::currentDate().addMonths(3));
            ReturnDate->setStyleSheet("QDateEdit{font-family: 'Acme'; font-size: 18px; color: #000000;}\
            QDateEdit::up-button{width: 25;}\
            QDateEdit::down-button{width: 25;}\
            QDateEdit::up-button:hover{bottom: 1px;}\
            QDateEdit::down-button:hover{top: 1px;}\
            QDateEdit::up-button:pressed{bottom: 0px;}\
            QDateEdit::down-button:pressed{top: 0px;}");
            MainGrid->addWidget(ReturnDate, row, 4);
        }
    }

    return MainGrid;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
