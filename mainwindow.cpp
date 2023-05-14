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

void MainWindow::ClearWindow(QWidget* area)
{
    delete area->layout();
    QList<QWidget*> ChildList = area->findChildren<QWidget*>();
    foreach (QWidget* i, ChildList)
    {
        delete i;
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

        ui->scrollAreaWidgetContents->setLayout(PageFunction::DrowBook(&query));

    }
    else
    {
        ClearWindow(ui->scrollAreaWidgetContents);

        ui->MainWidget->setCurrentWidget(ui->ClearPage);
    }
}


void MainWindow::on_OkBtn_clicked()
{

}


void MainWindow::on_SearchBtn_clicked()
{
    ClearWindow(ui->scrollAreaWidgetContents);

    QString Search = ui->SearchLine->text();
    if(Search.length() == 0)
    {
        QSqlQuery query(DB);
        query.exec("SELECT * FROM books");
        ui->scrollAreaWidgetContents->setLayout(PageFunction::DrowBook(&query));
    }
    else
    {
        QSqlQuery query(DB);
        query.exec("SELECT * FROM books WHERE Name LIKE '"+Search+"%' OR Author LIKE '"+Search+"%'");
        ui->scrollAreaWidgetContents->setLayout(PageFunction::DrowBook(&query));
        ui->SearchLine->setText("");
    }

}

void MainWindow::on_HomeBtn_clicked()
{
    ClearWindow(ui->scrollAreaWidgetContents);

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

