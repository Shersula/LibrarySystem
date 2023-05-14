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

    ui->MainSubWindow->hide(); //Прячем доп.окно

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

enum SubWindowType
{
    None = 0,
    GiveBook,
    GiveTicket,
    BookList,
    Info
};

void MainWindow::on_BookGiveBtn_clicked()
{
    if(ui->MainSubWindow->property("Status") == SubWindowType::None)
    {
        ui->MainSubWindow->show();
        ui->MainSubWindow->setProperty("Status", SubWindowType::GiveBook);

        ui->TitleText->setText("Выдача книги [1/2]");

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
        BookTitle->setMaximumHeight(20);

        QLabel* AuthorTitle = new QLabel;
        MainGrid->addWidget(AuthorTitle, row, 1);
        AuthorTitle->setText("Автор");
        AuthorTitle->setStyleSheet("font-family: 'Acme'; font-size: 18px; color: #000000;");

        QLabel* CheckStatus = new QLabel;
        MainGrid->addWidget(CheckStatus, row, 2);
        CheckStatus->setText("Состояние");
        CheckStatus->setStyleSheet("font-family: 'Acme'; font-size: 18px; color: #000000;");

        QSqlQuery query(DB);
        query.exec("SELECT * FROM books");
        while (query.next())
        {
            row++;
            QString Name = query.value("Name").toString();
            QString Author = query.value("Author").toString();

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
    else
    {
        ui->MainSubWindow->hide();
        ui->MainSubWindow->setProperty("Status", SubWindowType::None);
    }
}


void MainWindow::on_OkBtn_clicked()
{
    ui->MainSubWindow->hide();
    ui->MainSubWindow->setProperty("Status", SubWindowType::None);
}

