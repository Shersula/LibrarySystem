#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QCheckBox>

QSqlDatabase DB = QSqlDatabase::addDatabase("QMYSQL", "mydb");

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->MainWidget->setCurrentWidget(ui->AuthorizedPage);
    ui->LogInMsg->hide();
    ui->Button_Group->hide();
    ui->TicketInput->setInputMask("99-999-999");
    ui->TicketInput->setCursorPosition(0);

    DB.setHostName("triniti.ru-hoster.com");
    DB.setDatabaseName("rootYJk");
    DB.setUserName("rootYJk");
    DB.setPassword("TY0cnu74j9");

    bool ConnStatus = DB.open();
    if(ConnStatus == true && DB.isOpen()) qDebug("DataBase Connected");
    else qDebug() << "DataBase Error: " << DB.lastError();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::DrowBook(QSqlQuery* query)
{
    int row = 0;

    QGridLayout* MainGrid = new QGridLayout;
    MainGrid->setVerticalSpacing(5);
    MainGrid->setHorizontalSpacing(15);
    MainGrid->setContentsMargins(0, 0, 0, 0);
    ui->scrollAreaWidgetContents->setLayout(MainGrid);

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
}

void MainWindow::on_BookGiveBtn_clicked()
{
    if(ui->MainWidget->currentWidget() == ui->ClearPage)
    {
        ui->MainWidget->setCurrentWidget(ui->GiveBookPage);

        ui->TitleText->setText("Выдача книги [1/2]");

        QSqlQuery query(DB);
        query.exec("SELECT * FROM books");

        DrowBook(&query);

    }
    else
    {
        delete ui->scrollAreaWidgetContents->layout();
        QList<QWidget*> ChildList = ui->scrollAreaWidgetContents->findChildren<QWidget*>();
        foreach (QWidget* i, ChildList)
        {
            delete i;
        }

        ui->MainWidget->setCurrentWidget(ui->ClearPage);
    }
}


void MainWindow::on_OkBtn_clicked()
{

}


void MainWindow::on_SearchBtn_clicked()
{
    delete ui->scrollAreaWidgetContents->layout();
    QList<QWidget*> ChildList = ui->scrollAreaWidgetContents->findChildren<QWidget*>();
    foreach (QWidget* i, ChildList)
    {
        delete i;
    }

    QString Search = ui->SearchLine->text();
    if(Search.length() == 0)
    {
        QSqlQuery query(DB);
        query.exec("SELECT * FROM books");
        DrowBook(&query);
    }
    else
    {
        QSqlQuery query(DB);
        query.exec("SELECT * FROM books WHERE Name LIKE '"+Search+"%' OR Author LIKE '"+Search+"%'");
        DrowBook(&query);
        ui->SearchLine->setText("");
    }

}

void MainWindow::on_HomeBtn_clicked()
{
    delete ui->scrollAreaWidgetContents->layout();
    QList<QWidget*> ChildList = ui->scrollAreaWidgetContents->findChildren<QWidget*>();
    foreach (QWidget* i, ChildList)
    {
        delete i;
    }

    ui->MainWidget->setCurrentWidget(ui->ClearPage);
}


void MainWindow::on_LogInBtn_clicked()
{
    if(ui->TicketInput->text() == "--")
    {
        ui->LogInText->setText("Ошибка: Вы не ввели номер билета");
        ui->LogInMsg->show();
        return void();
    }
    else if(ui->PasswordInput->text().isEmpty())
    {
        ui->LogInText->setText("Ошибка: Вы не ввели пароль");
        ui->LogInMsg->show();
        return void();
    }

    QSqlQuery query(DB);
    query.exec("SELECT * FROM tickets WHERE TicketNumber='" + ui->TicketInput->text() + "' AND Password='" + ui->PasswordInput->text() + "'");

    if(query.next() == false)
    {
        ui->LogInText->setText("Ошибка: Не верный номер билета или пароль");
        ui->LogInMsg->show();
    }
    else
    {
        ui->MainWidget->setCurrentWidget(ui->ClearPage);
        ui->Button_Group->show();
    }
}


void MainWindow::on_Exit_clicked()
{
    ui->MainWidget->setCurrentWidget(ui->AuthorizedPage);
    ui->LogInMsg->hide();
    ui->Button_Group->hide();
}

