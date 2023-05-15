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
    if(ui->MainWidget->currentWidget() == ui->GiveBookPage)
    {
        TempUser->UserIntMap.clear();
        ClearWidget(ui->scrollAreaGiveBook);
    }
    else if(ui->MainWidget->currentWidget() == ui->GiveTicketPage)
    {
        ClearWidget(ui->scrollAreaGiveTicket);
    }
    else if(ui->MainWidget->currentWidget() == ui->AuthorizedPage)
    {
        ui->Button_Group->show();
        if(TempUser->getRole() == 0)
        {
            ui->BookGiveBtn->hide();
            ui->TicketGiveBtn->hide();
        }
        else
        {
            ui->BookGiveBtn->show();
            ui->TicketGiveBtn->show();
        }
    }

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
    else if(page == ui->GiveTicketPage)
    {
        ui->TitleText->setText("Список билетов");

        ui->MainWidget->setCurrentWidget(ui->GiveTicketPage);

        ui->GiveTicketDelBtn->show();
        ui->GiveTicketBookBtn->show();
        ui->GiveTicketOkBtn->hide();
        ui->GiveTicketHomeBtn->setIcon(QIcon(":/res/img/icons/HomeIcon.png"));
        TempUser->UserIntMap.clear();
        ui->GiveTicketPage->setProperty("PageStatus", 1);
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
            if(i->isChecked())
            {
                TempUser->UserIntMap.insert("UserID", i->property("ID").toInt());
                break;
            }
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
        query.exec("SELECT * FROM books WHERE Name LIKE '%"+Search+"%' OR Author LIKE '%"+Search+"%'");
        ui->scrollAreaGiveBook->setLayout(PageFunction::DrowBook(&query));
        ui->GiveBookSearchLine->setText("");
    }
}

void MainWindow::on_GiveBookHomeBtn_clicked() {ChangePage(ui->ClearPage);}
/////////////////////////////////Cписок билетов///////////////////////////////////////////

void MainWindow::on_TicketGiveBtn_clicked()
{
    if(ui->MainWidget->currentWidget() == ui->GiveTicketPage) ChangePage(ui->ClearPage);
    else
    {
        ChangePage(ui->GiveTicketPage);
        QSqlQuery query(DB);
        query.exec("SELECT * FROM tickets");
        ui->scrollAreaGiveTicket->setLayout(PageFunction::DrowUsersList(&query));
    }
}

void MainWindow::on_GiveTicketDelBtn_clicked()
{
    QList<QPushButton*> ButtonList = ui->scrollAreaGiveTicket->findChildren<QPushButton*>();
    foreach(QPushButton* i, ButtonList)
    {
        if(i->isChecked())
        {
            if(TempUser->getID() == i->property("ID").toInt())
            {
                QMessageBox::warning(this, "Предупреждение", "Вы не можете удалить аккаунт на котором сейчас находитесь", QMessageBox::Cancel);
                return void();
            }

            QSqlQuery query(DB);
            query.exec("DELETE FROM `tickets` WHERE `ID` = " + i->property("ID").toString());

            ChangePage(ui->GiveTicketPage);
            query.exec("SELECT * FROM tickets");
            ui->scrollAreaGiveTicket->setLayout(PageFunction::DrowUsersList(&query));
            return void();
        }
    }

    QMessageBox::warning(this, "Предупреждение", "Вы должны выбрать пользователя которого хотите удалить", QMessageBox::Cancel);
    return void();
}

void MainWindow::on_GiveTicketBookBtn_clicked()
{
    QList<QPushButton*> ButtonList = ui->scrollAreaGiveTicket->findChildren<QPushButton*>();
    foreach(QPushButton* i, ButtonList)
    {
        if(i->isChecked())
        {
            QSqlQuery query(DB);
            query.exec(QString("SELECT `tickets`.`FirstName`, `tickets`.`MiddleName`, `tickets`.`LastName`, `studentbook`.`ID`, `studentbook`.`ReturnDate`, `studentbook`.`Count`, `books`.`Name`, `books`.`Author` FROM `tickets`\
            INNER JOIN `studentbook` ON `tickets`.`ID` = `studentbook`.`Student`\
            INNER JOIN `books` ON `books`.`ID` = `studentbook`.`Book`\
            WHERE `tickets`.`ID` = '%1'").arg(i->property("ID").toString()));

            ui->GiveTicketPage->setProperty("PageStatus", 2);

            TempUser->UserIntMap.insert("UserID", i->property("ID").toInt());

            ClearWidget(ui->scrollAreaGiveTicket);
            ui->scrollAreaGiveTicket->setLayout(PageFunction::DrowBookList(&query));

            if(query.last() == true) ui->TitleText->setText(QString("Книги %1 %2 %3").arg(query.value("FirstName").toString()).arg(query.value("MiddleName").toString()).arg(query.value("LastName").toString()));

            ui->GiveTicketDelBtn->hide();
            ui->GiveTicketBookBtn->hide();
            ui->GiveTicketOkBtn->show();
            ui->GiveTicketHomeBtn->setIcon(QIcon(":/res/img/icons/BackIco.png"));
            return void();
        }
    }

    QMessageBox::warning(this, "Предупреждение", "Вы должны выбрать пользователя у которого хотите просмотреть список книг", QMessageBox::Cancel);
    return void();
}

void MainWindow::on_GiveTicketOkBtn_clicked()
{
    if(ui->GiveTicketPage->property("PageStatus") == 2)
    {
        QList<QCheckBox*> CheckBoxList = ui->scrollAreaGiveTicket->findChildren<QCheckBox*>();
        QString FinalMessage;
        foreach(QCheckBox* i, CheckBoxList)
        {
            if(i->isChecked())
            {
                QSqlQuery query(DB);
                query.exec("SELECT `studentbook`.`Count`, `books`.`Name`, `books`.`ID` AS 'BookID'  FROM `studentbook` INNER JOIN `books` ON `books`.`ID` = `studentbook`.`Book` WHERE `studentbook`.`ID` = " + i->property("ID").toString());
                if(query.next())
                {
                    FinalMessage += QString("%1 (%2шт.) были возвращены библиотеке\n")
                    .arg(query.value("Name").toString())
                    .arg(query.value("Count").toString());

                    query.exec(QString("UPDATE `books` SET `Count` = `Count`+%1 WHERE `ID` = %2")
                    .arg(query.value("Count").toString())
                    .arg(query.value("BookID").toString()));

                    query.exec("DELETE FROM `studentbook` WHERE `ID` = " + i->property("ID").toString());
                }
            }
        }

        if(FinalMessage.isEmpty() == true)
        {
            QMessageBox::warning(this, "Предупреждение", "Вы должны выбрать хотя-бы одну книгу", QMessageBox::Cancel);
            return void();
        }
        else
        {
            QMessageBox::information(this, "Возврат книг", FinalMessage);

            ClearWidget(ui->scrollAreaGiveTicket);

            QSqlQuery query(DB);
            query.exec("SELECT `tickets`.`FirstName`, `tickets`.`MiddleName`, `tickets`.`LastName`, `studentbook`.`ID`, `studentbook`.`ReturnDate`, `studentbook`.`Count`, `books`.`Name`, `books`.`Author` FROM `tickets`\
            INNER JOIN `studentbook` ON `tickets`.`ID` = `studentbook`.`Student`\
            INNER JOIN `books` ON `books`.`ID` = `studentbook`.`Book`\
            WHERE `tickets`.`ID` = '" + QString::number(TempUser->UserIntMap.value("UserID")) + "'");
            ui->scrollAreaGiveTicket->setLayout(PageFunction::DrowBookList(&query));
        }
    }
}

void MainWindow::on_GiveTicketSearchBtn_clicked()
{
    ClearWidget(ui->scrollAreaGiveTicket);

    QString Search = ui->GiveTicketSearchLine->text();
    if(Search.length() == 0)
    {
        QSqlQuery query(DB);
        query.exec("SELECT * FROM tickets");
        ui->scrollAreaGiveTicket->setLayout(PageFunction::DrowUsersList(&query));
    }
    else
    {
        QSqlQuery query(DB);
        query.exec(QString("SELECT * FROM tickets WHERE `TicketNumber` LIKE '%%1%' OR `FirstName` LIKE '%%1%' OR `MiddleName` LIKE '%%1%' OR `LastName` LIKE '%%1%' OR `StudentIDNumber` LIKE '%%1%'").arg(Search));
        ui->scrollAreaGiveTicket->setLayout(PageFunction::DrowUsersList(&query));
        ui->GiveTicketSearchLine->setText("");
    }
}

void MainWindow::on_GiveTicketHomeBtn_clicked()
{
    if(ui->GiveTicketPage->property("PageStatus") == 1) ChangePage(ui->ClearPage);
    else if(ui->GiveTicketPage->property("PageStatus") == 2)
    {
        ChangePage(ui->GiveTicketPage);

        QSqlQuery query(DB);
        query.exec("SELECT * FROM tickets");
        ui->scrollAreaGiveTicket->setLayout(PageFunction::DrowUsersList(&query));
    }
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
        TempUser = new User(query.value("ID").toInt(), query.value("Role").toInt());
        ui->AccountText->setText(QString("%1 %2 %3").arg(query.value("FirstName").toString()).arg(query.value("MiddleName").toString()).arg(query.value("LastName").toString()));
        ChangePage(ui->ClearPage);
    }
}

void MainWindow::on_Exit_clicked() {ChangePage(ui->AuthorizedPage);}
/////////////////////////////////////////////////////////////////////////////////////
