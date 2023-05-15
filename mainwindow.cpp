#include "mainwindow.h"
#include "pagefunction.h"
#include "./ui_mainwindow.h"
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlError>
#include <QMessageBox>

QSqlDatabase DB = QSqlDatabase::addDatabase("QMYSQL", "mydb");

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ChangePage(ui->AuthorizedPage);

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

void MainWindow::ClearWidget(QWidget* area)
{
    if(area->layout() != nullptr) delete area->layout();
    QList<QWidget*> ChildList = area->findChildren<QWidget*>(Qt::FindDirectChildrenOnly);
    foreach (QWidget* i, ChildList)
    {
        if(i->layout() != nullptr) delete i->layout();
        delete i;
    }
}

void MainWindow::ChangePage(QWidget* page)
{
    if(ui->MainWidget->currentWidget() == ui->GiveBookPage) ClearWidget(ui->scrollAreaGiveBook);
    else if(ui->MainWidget->currentWidget() == ui->AuthorizedPage) ui->Button_Group->show();

    if(page == ui->ClearPage)
    {
        ui->TitleText->setText("Главная");

        ui->MainWidget->setCurrentWidget(ui->ClearPage);
    }
    else if(page == ui->GiveBookPage)
    {
        ui->TitleText->setText("Выдача книги [1/3]");

        ui->MainWidget->setCurrentWidget(ui->GiveBookPage);

        TempUser->UserIntMap.clear();
        ui->GiveBookPage->setProperty("PageStatus", 1);
    }
    else if(page == ui->AuthorizedPage)
    {
        delete TempUser;
        TempUser = nullptr;

        ui->TitleText->setText("Авторизация");

        ui->MainWidget->setCurrentWidget(ui->AuthorizedPage);
        ui->LogInMsg->hide();
        ui->Button_Group->hide();
        ui->TicketInput->setInputMask("99-999-999");
        ui->TicketInput->setCursorPosition(0);
    }
}

//////////////////////////////////Окно выдачи книг/////////////////////////////////////
void MainWindow::on_BookGiveBtn_clicked()
{
    if(ui->MainWidget->currentWidget() == ui->GiveBookPage) ChangePage(ui->ClearPage);
    else
    {
        ChangePage(ui->GiveBookPage);

        QSqlQuery query(DB);
        query.exec("SELECT * FROM books");
        ui->scrollAreaGiveBook->setLayout(PageFunction::DrowBook(&query));
    }
}


void MainWindow::on_GiveBookOkBtn_clicked()
{
    if(ui->GiveBookPage->property("PageStatus") == 1)
    {
        QList<QCheckBox*> CheckBoxList = ui->scrollAreaGiveBook->findChildren<QCheckBox*>();
        foreach(QCheckBox* i, CheckBoxList)
        {
            if(i->isChecked()) TempUser->UserIntMap.insert("BookID", i->property("ID").toInt());
        }
        if(TempUser->UserIntMap.empty() == true)
        {
            QMessageBox::warning(this, "Предупреждение", "Вы должны выбрать хотя-бы одну книгу", QMessageBox::Cancel);
            return void();
        }
        ClearWidget(ui->scrollAreaGiveBook);

        QSqlQuery query(DB);
        query.exec("SELECT * FROM tickets");
        ui->scrollAreaGiveBook->setLayout(PageFunction::DrowUsers(&query));

        ui->TitleText->setText("Выдача книги [2/3]");
        ui->GiveBookPage->setProperty("PageStatus", 2);
    }
    else if(ui->GiveBookPage->property("PageStatus") == 2)
    {
        QList<QPushButton*> ButtonList = ui->scrollAreaGiveBook->findChildren<QPushButton*>();
        int length = TempUser->UserIntMap.size();
        foreach(QPushButton* i, ButtonList)
        {
            if(i->isChecked()) TempUser->UserIntMap.insert("UserID", i->property("ID").toInt());
        }
        if(length == TempUser->UserIntMap.size())
        {
            QMessageBox::warning(this, "Предупреждение", "Вы должны выбрать пользователя которому следует выдать книгу", QMessageBox::Cancel);
            return void();
        }

        ClearWidget(ui->scrollAreaGiveBook);

        QSqlQuery query(DB);
        QString queryStr = "SELECT * FROM books WHERE";
        QList<int> BookIDList = TempUser->UserIntMap.values("BookID");
        for (int i = 0; i < BookIDList.size(); ++i)
        {
            if(i == 0) queryStr += " ID = " + QString::number(BookIDList[i]);
            else queryStr += " OR ID = " + QString::number(BookIDList[i]);
        }

        TempUser->UserIntMap.remove("BookID");

        query.exec(queryStr);
        ui->scrollAreaGiveBook->setLayout(PageFunction::DrowBookCounter(&query));

        ui->TitleText->setText("Выдача книги [3/3]");
        ui->GiveBookPage->setProperty("PageStatus", 3);
    }
    else if(ui->GiveBookPage->property("PageStatus") == 3)
    {
        QList<QSpinBox*> SpinBoxList = ui->scrollAreaGiveBook->findChildren<QSpinBox*>();
        QList<QDateEdit*> DateEditList = ui->scrollAreaGiveBook->findChildren<QDateEdit*>();
        QString FinalMessage;
        foreach(QSpinBox* i, SpinBoxList)
        {
            if(i->value() > 0)
            {
                QSqlQuery query(DB);
                query.exec(QString("INSERT INTO `studentbook`(`Student`, `Book`, `Count`, `ReturnDate`) VALUES (%1,%2,%3,%4)")
                                .arg(TempUser->UserIntMap.value("UserID"))
                                .arg(i->property("ID").toInt())
                                .arg(i->value())
                                .arg(DateEditList[SpinBoxList.indexOf(i)]->date().toString("yyyyMMdd")));

                query.exec(QString("UPDATE `books` SET `Count` = `Count`-%1 WHERE `ID` = %2")
                                .arg(i->value())
                                .arg(i->property("ID").toInt()));

                query.exec("SELECT `Name` FROM `books` WHERE `ID` = " + i->property("ID").toString());
                if(query.next())
                {
                    FinalMessage += QString("%1 (%2шт.) до %3\n")
                                        .arg(query.value("Name").toString())
                                        .arg(i->value())
                                        .arg(DateEditList[SpinBoxList.indexOf(i)]->date().toString("dd.MM.yyyy"));
                }
            }
        }

        if(FinalMessage.isEmpty() == true) QMessageBox::information(this, "Выдача книг завершена", "Вы завершили выдачу книг");
        else
        {
            QSqlQuery query(DB);
            query.exec("SELECT `FirstName`, `MiddleName`, `LastName` FROM `tickets` WHERE `ID` = " + QString::number(TempUser->UserIntMap.value("UserID")));
            if(query.next())
            {
                FinalMessage = QString("Студенту %1 %2 %3 (ID %4) были выданы книги:\n")
                                   .arg(query.value("FirstName").toString())
                                   .arg(query.value("MiddleName").toString())
                                   .arg(query.value("LastName").toString())
                                   .arg(TempUser->UserIntMap.value("UserID")) + FinalMessage;
            }

            QMessageBox::information(this, "Выдача книг завершена", FinalMessage);
        }

        ChangePage(ui->ClearPage);
    }
}

void MainWindow::on_GiveBookSearchBtn_clicked()
{
    ClearWidget(ui->scrollAreaGiveBook);
    
    QString Search = ui->GiveBookSearchLine->text();
    if(Search.length() == 0)
    {
        QSqlQuery query(DB);
        query.exec("SELECT * FROM books");
        ui->scrollAreaGiveBook->setLayout(PageFunction::DrowBook(&query));
    }
    else
    {
        QSqlQuery query(DB);
        query.exec("SELECT * FROM books WHERE Name LIKE '"+Search+"%' OR Author LIKE '"+Search+"%'");
        ui->scrollAreaGiveBook->setLayout(PageFunction::DrowBook(&query));
        ui->GiveBookSearchLine->setText("");
    }

}

void MainWindow::on_GiveBookHomeBtn_clicked() {ChangePage(ui->ClearPage);}
///////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_TicketGiveBtn_clicked()
{

}

///////////////////////////////////////////Авторизация////////////////////////////
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
        TempUser = new User(query.value("ID").toInt());
        ChangePage(ui->ClearPage);
    }
}

void MainWindow::on_Exit_clicked() {ChangePage(ui->AuthorizedPage);}
/////////////////////////////////////////////////////////////////////////////////////
