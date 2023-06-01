#include "mainwindow.h"
#include "pagefunction.h"
#include "./ui_mainwindow.h"
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlError>
#include <QMessageBox>
#include <QInputDialog>

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

void MainWindow::keyPressEvent(QKeyEvent* e)
{
    if(ui->MainWidget->currentWidget() == ui->AuthorizedPage && e->key() == Qt::Key_Return) on_LogInBtn_clicked();
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
    ////////////////////////////////////////////////////////////Прошлая страница////////////////////////////////////////////////////////////
    if(ui->MainWidget->currentWidget() == ui->GiveBookPage) ClearWidget(ui->scrollAreaGiveBook);
    else if(ui->MainWidget->currentWidget() == ui->BooksPage) ClearWidget(ui->scrollAreaBooks);
    else if(ui->MainWidget->currentWidget() == ui->GiveTicketPage) ClearWidget(ui->scrollAreaGiveTicket);
    else if(ui->MainWidget->currentWidget() == ui->CreateTicketPage)
    {
        ui->TicketNumberInput->setText("");
        ui->FirstNameInput->setText("");
        ui->LastNameInput->setText("");
        ui->MiddleNameInput->setText("");
        ui->StudNumberInput->setText("");
        ui->PasswordInput_2->setText("");
        ui->StudentCheck->setChecked(true);
        ui->StudNumber->show();
    }
    else if(ui->MainWidget->currentWidget() == ui->AddBookPage)
    {
        ui->BookNameInput->setText("");
        ui->AuthorInput->setText("");
        ui->BookCountInput->setValue(0);
    }
    else if(ui->MainWidget->currentWidget() == ui->AuthorizedPage)
    {
        ui->Button_Group->show();
        if(TempUser != nullptr)
        {
            if(TempUser->getRole() == 0 || TempUser->getRole() == 2)
            {
                ui->BookGiveBtn->hide();
                ui->TicketGiveBtn->hide();
                ui->BookListBtn->setToolTip("<html><head/><body><p>Список книг</p></body></html>");
            }
            else
            {
                ui->BookGiveBtn->show();
                ui->TicketGiveBtn->show();
                ui->BookListBtn->setToolTip("<html><head/><body><p>Управление книгами</p></body></html>");
            }
        }
    }
    ////////////////////////////////////////////////////////////Новая страница////////////////////////////////////////////////////////////
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
    else if(page == ui->AddBookPage)
    {
        ui->TitleText->setText("Добавление книги");
        ui->MainWidget->setCurrentWidget(ui->AddBookPage);
    }
    else if(page == ui->GiveTicketPage)
    {
        ui->TitleText->setText("Список билетов");

        ui->MainWidget->setCurrentWidget(ui->GiveTicketPage);

        ui->GiveTicketChangePswdBtn->show();
        ui->GiveTicketDelBtn->show();
        ui->GiveTicketBookBtn->show();
        ui->CreateTicketBtn->show();
        ui->GiveTicketOkBtn->hide();
        ui->GiveTicketSearchLine->setPlaceholderText("Введите ФИО студента/номер билета");

        ui->GiveTicketHomeBtn->setIcon(QIcon(":/res/img/icons/HomeIcon.png"));
        TempUser->UserIntMap.clear();
        ui->GiveTicketPage->setProperty("PageStatus", 1);
    }
    else if(page == ui->CreateTicketPage)
    {
        ui->TitleText->setText("Выдача библиотечного билета");

        ui->TicketNumberInput->setInputMask("99-999-999");
        ui->TicketNumberInput->setCursorPosition(0);

        ui->StudNumberInput->setInputMask("99-99999");

        ui->MainWidget->setCurrentWidget(ui->CreateTicketPage);
    }
    else if(page == ui->BooksPage)
    {
        if(TempUser->getRole() == 0 || TempUser->getRole() == 2)
        {
            ui->BooksDelBtn->hide();
            ui->BooksEditBtn->hide();
            ui->BooksAddBtn->hide();
        }
        else
        {
            ui->BooksDelBtn->show();
            ui->BooksEditBtn->show();
            ui->BooksAddBtn->show();
        }

        TempUser->UserIntMap.clear();

        ui->TitleText->setText("Список книг");

        ui->MainWidget->setCurrentWidget(ui->BooksPage);
    }
    else if(page == ui->InfoPage)
    {
        ui->TitleText->setText("Справка");
        ui->MainWidget->setCurrentWidget(ui->InfoPage);

        if(TempUser->getRole() == 1)
        {
            ui->InfoLabel->setText("<html><head/><body>\
            <p align=\"center\"><span style=\" font-size:18pt; font-weight:700;\">Информация(Администратор)</span></p>\
            <p><span style=\" font-size:12pt;\">Данное приложение предоставляет возможность взаимодействие с электронной базой библиотеки.\
            <br/><br/>Роль Администратор позволяет выполнять следующие действия:\
            <br/>-Выдача книг на электронный библиотечный билет студента<br/>\
            -Возврат книг закрепленных за студентом<br/>\
            -Просмотр сроков ворзврата и книг закрепленных за студентом<br/><br/>\
            -Создание электронного библиотечного билета для студента<br/>\
            -Удаление электронного библиотечного билета<br/>\
            -Смена пароля от электронного библиотечного билета<br/><br/>\
            -Добавление новыйх книг в информационную базу библиотеки<br/>\
            -Редактирование данных о уже добавленных книгах<br/>\
            -Удаление записей о добавленых книгах</span></p></body></html>");
        }
        else if(TempUser->getRole() == 0)
        {
            ui->InfoLabel->setText("<html><head/><body><p align=\"center\"><span style=\" font-size:18pt; font-weight:700;\">Информация(Студент)</span></p>\
            <p><span style=\" font-size:12pt;\">Данное приложение предоставляет возможность взаимодействие с электронной базой библиотеки.<br/><br/>\
            Роль Студент позволяет просматривать срок возврата и список книг закрепленных за вами.<br/><br/></span>\
            <span style=\" font-size:12pt; font-weight:700; color:#ff0000;\">В случае утери доступа к аккаунту обратитесь к администрации библиотеки для<br/>восстановления.</span></p></body></html> ");
        }
        else if(TempUser->getRole() == 2)
        {
            ui->InfoLabel->setText("<html><head/><body><p align=\"center\"><span style=\" font-size:18pt; font-weight:700;\">Информация(Преподаватель)</span></p>\
            <p><span style=\" font-size:12pt;\">Данное приложение предоставляет возможность взаимодействие с электронной базой библиотеки.<br/><br/>\
            Роль Преподаватель позволяет просматривать список книг закрепленных за вами.<br/><br/></span>\
            <span style=\" font-size:12pt; font-weight:700; color:#ff0000;\">В случае утери доступа к аккаунту обратитесь к администрации библиотеки для<br/>восстановления.</span></p></body></html> ");
        }
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
                TempUser->UserIntMap.insert("UserRole", i->property("Role").toInt());
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
        ui->scrollAreaGiveBook->setLayout(PageFunction::DrowBookCounter(&query, TempUser->UserIntMap.value("UserRole")));

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
                                .arg(TempUser->UserIntMap.value("UserRole") == 0 ? DateEditList[SpinBoxList.indexOf(i)]->date().toString("yyyyMMdd") : "00000000"));

                query.exec(QString("UPDATE `books` SET `Count` = `Count`-%1 WHERE `ID` = %2")
                                .arg(i->value())
                                .arg(i->property("ID").toInt()));

                query.exec("SELECT `Name` FROM `books` WHERE `ID` = " + i->property("ID").toString());
                if(query.next())
                {
                    FinalMessage += QString("%1 (%2шт.) до %3\n")
                                        .arg(query.value("Name").toString())
                                        .arg(i->value())
                                        .arg(TempUser->UserIntMap.value("UserRole") == 0 ? DateEditList[SpinBoxList.indexOf(i)]->date().toString("dd.MM.yyyy") : "Бессрочно");
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
                QString RoleName;
                if(TempUser->UserIntMap.value("UserRole") == 0) RoleName = "Студенту";
                else if(TempUser->UserIntMap.value("UserRole") == 2) RoleName = "Преподавателю";
                else RoleName = "Администратору";

                FinalMessage = QString("%1 %2 %3 %4 (ID %5) были выданы книги:\n")
                                   .arg(RoleName)
                                   .arg(query.value("LastName").toString())
                                   .arg(query.value("FirstName").toString())
                                   .arg(query.value("MiddleName").toString())
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
    QSqlQuery query(DB);

    if(Search.length() == 0) query.exec("SELECT * FROM books");
    else query.exec("SELECT * FROM books WHERE Name LIKE '%"+Search+"%' OR Author LIKE '%"+Search+"%'");

    ui->scrollAreaGiveBook->setLayout(PageFunction::DrowBook(&query));
    ui->GiveBookSearchLine->setText("");
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

void MainWindow::on_CreateTicketBtn_clicked()
{
    if(ui->MainWidget->currentWidget() == ui->GiveTicketPage && ui->GiveTicketPage->property("PageStatus") == 1) ChangePage(ui->CreateTicketPage);
}

void MainWindow::on_GiveTicketBackBtn_clicked()
{
    if(ui->MainWidget->currentWidget() == ui->CreateTicketPage)
    {
        ChangePage(ui->GiveTicketPage);
        QSqlQuery query(DB);
        query.exec("SELECT * FROM tickets");
        ui->scrollAreaGiveTicket->setLayout(PageFunction::DrowUsersList(&query));
    }
}

void MainWindow::on_GiveTicketOkBtn_2_clicked()
{
    if(ui->MainWidget->currentWidget() == ui->CreateTicketPage)
    {
        //12-123-123
        if(ui->TicketNumberInput->text() == "--")
        {
            QMessageBox::warning(this, "Предупреждение", "Вы не указали номер библеотечного билета", QMessageBox::Cancel);
            return void();
        }
        else if(ui->TicketNumberInput->text().length() < 10)
        {
            QMessageBox::warning(this, "Предупреждение", "Необходимо указать полный номер библеотечного билета", QMessageBox::Cancel);
            return void();
        }

        if(ui->FirstNameInput->text().isEmpty() == true)
        {
            QMessageBox::warning(this, "Предупреждение", "Вы не указали имя пользователя", QMessageBox::Cancel);
            return void();
        }

        if(ui->LastNameInput->text().isEmpty() == true)
        {
            QMessageBox::warning(this, "Предупреждение", "Вы не указали фамилию пользователя", QMessageBox::Cancel);
            return void();
        }

        if(ui->StudNumberInput->text() == "-" && ui->StudentCheck->isChecked() == true)
        {
            QMessageBox::warning(this, "Предупреждение", "Вы не указали номер студенческого билета", QMessageBox::Cancel);
            return void();
        }
        else if(ui->StudNumberInput->text().length() < 8 && ui->StudentCheck->isChecked() == true)
        {
            QMessageBox::warning(this, "Предупреждение", "Необходимо указать полный номер студенческого билета", QMessageBox::Cancel);
            return void();
        }

        if(ui->PasswordInput_2->text().isEmpty() == true)
        {
            QMessageBox::warning(this, "Предупреждение", "Вы не указали пароль для входа", QMessageBox::Cancel);
            return void();
        }

        QSqlQuery query(DB);
        query.exec("SELECT * FROM `tickets` WHERE `TicketNumber` = '" + ui->TicketNumberInput->text() + "'");
        if(query.next() == true)
        {
            QMessageBox::warning(this, "Предупреждение", "Такой номер библеотечного билета уже существует", QMessageBox::Cancel);
            return void();
        }

        QString StudNumber;
        int Role;
        if(ui->StudentCheck->isChecked() == true)
        {
            query.exec("SELECT * FROM `tickets` WHERE `StudentIDNumber` = '" + ui->StudNumberInput->text() + "'");
            if(query.next() == true)
            {
                QMessageBox::warning(this, "Предупреждение", "Пользователь с таким студенческим билетом уже существует", QMessageBox::Cancel);
                return void();
            }

            StudNumber = ui->StudNumberInput->text();
            Role = 0;
        }
        else if(ui->AdminCheck->isChecked())
        {
            StudNumber = "XX-ADMIN";
            Role = 1;
        }
        else
        {
            StudNumber = "TEACHER";
            Role = 2;
        }
        query.exec(QString("INSERT INTO `tickets`(`TicketNumber`, `FirstName`, `MiddleName`, `LastName`, `StudentIDNumber`, `Password`, `Role`, `IssueDate`) VALUES ('%1', '%2', '%3', '%4', '%5', '%6', %7, '%8')")
        .arg(ui->TicketNumberInput->text())
        .arg(ui->FirstNameInput->text())
        .arg(ui->MiddleNameInput->text())
        .arg(ui->LastNameInput->text())
        .arg(StudNumber)
        .arg(ui->PasswordInput_2->text())
        .arg(Role)
        .arg(QDate::currentDate().toString("yyyyMMdd")));


        if(Role == 0) StudNumber = "Студент";
        else if(Role == 1) StudNumber = "Администратор";
        else if(Role == 2) StudNumber = "Преподаватель";

        QMessageBox::information(this, QString("Билет для %1 %2 %3").arg(ui->LastNameInput->text()).arg(ui->FirstNameInput->text()).arg(ui->MiddleNameInput->text()),
        QString("Билет для %1 %2 %3 успешно создан\nНомер билета: %5\nРоль: %6\nПароль: %7\n\nДата создания: %8")
        .arg(ui->LastNameInput->text())
        .arg(ui->FirstNameInput->text())
        .arg(ui->MiddleNameInput->text())
        .arg(ui->TicketNumberInput->text())
        .arg(StudNumber)
        .arg(ui->PasswordInput_2->text())
        .arg(QDate::currentDate().toString("dd.MM.yyyy")));

        ChangePage(ui->CreateTicketPage);
    }
}

void MainWindow::on_AdminCheck_stateChanged(int status)
{
    if(status == Qt::Checked)
    {
        ui->StudNumber->hide();
        ui->StudNumberInput->setText("");
    }
}

void MainWindow::on_TeacherCheck_stateChanged(int status)
{
    if(status == Qt::Checked)
    {
        ui->StudNumber->hide();
        ui->StudNumberInput->setText("");
    }
}

void MainWindow::on_StudentCheck_stateChanged(int status)
{
    if(status == Qt::Checked)  ui->StudNumber->show();
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

            query.exec("SELECT * FROM `studentbook` WHERE `Student` = " + i->property("ID").toString());
            if(query.next() == true)
            {
                QMessageBox::warning(this, "Предупреждение", "Вы не можете удалить аккаунт за которым закреплены книги", QMessageBox::Cancel);
                return void();
            }

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

void MainWindow::on_GiveTicketChangePswdBtn_clicked()
{
    QList<QPushButton*> ButtonList = ui->scrollAreaGiveTicket->findChildren<QPushButton*>();
    foreach(QPushButton* i, ButtonList)
    {
        if(i->isChecked())
        {
            QSqlQuery query(DB);

            query.exec("SELECT `FirstName`, `MiddleName`, `LastName` FROM `tickets` WHERE ID = " + i->property("ID").toString());
            if(query.next() == true)
            {
                while(true)
                {
                    QString FirstName = query.value("FirstName").toString();
                    QString MiddleName = query.value("MiddleName").toString();
                    QString LastName = query.value("LastName").toString();

                    bool ok;
                    QString NewPswd = QInputDialog::getText(this, QString("Смена пароля для %1 %2 %3").arg(LastName).arg(FirstName).arg(MiddleName), "Введите новый пароль", QLineEdit::Normal, QString(), &ok);

                    if(ok == false) return void();

                    if(NewPswd.isEmpty() == true)
                    {
                        QMessageBox::warning(this, "Предупреждение", "Вы ничего не ввели", QMessageBox::Cancel);
                        continue;
                    }
                    else if(NewPswd.length() > 32)
                    {
                        QMessageBox::warning(this, "Предупреждение", "Пароль не должен быть длинее 32 символов", QMessageBox::Cancel);
                        continue;
                    }
                    else
                    {
                        query.exec(QString("UPDATE `tickets` SET `Password`= '%1' WHERE `ID` = '%2'").arg(NewPswd).arg(i->property("ID").toString()));

                        QMessageBox::information(this, QString("Билет %1 %2 %3").arg(LastName).arg(FirstName).arg(MiddleName),
                        QString("Пароль для %1 %2 %3 успешно изменен\n\nНовый пароль: %4")
                        .arg(LastName)
                        .arg(FirstName)
                        .arg(MiddleName)
                        .arg(NewPswd));

                        break;
                    }
                }
            }
            return void();
        }
    }

    QMessageBox::warning(this, "Предупреждение", "Вы должны выбрать пользователя для которого хотите сменить пароль", QMessageBox::Cancel);
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

            ui->GiveTicketPage->setProperty("PageStatus", 2);

            TempUser->UserIntMap.insert("UserID", i->property("ID").toInt());

            query.exec("SELECT `FirstName`, `MiddleName`, `LastName` FROM `tickets` WHERE ID = " + i->property("ID").toString());
            if(query.next() == true) ui->TitleText->setText(QString("Книги %1 %2 %3").arg(query.value("LastName").toString()).arg(query.value("FirstName").toString()).arg(query.value("MiddleName").toString()));

            query.exec(QString("SELECT `tickets`.`FirstName`, `tickets`.`MiddleName`, `tickets`.`LastName`, `tickets`.`Role`, `studentbook`.`ID`, `studentbook`.`ReturnDate`, `studentbook`.`Count`, `books`.`Name`, `books`.`Author` FROM `tickets`\
            INNER JOIN `studentbook` ON `tickets`.`ID` = `studentbook`.`Student`\
            INNER JOIN `books` ON `books`.`ID` = `studentbook`.`Book`\
            WHERE `tickets`.`ID` = '%1'").arg(i->property("ID").toString()));

            ClearWidget(ui->scrollAreaGiveTicket);
            ui->scrollAreaGiveTicket->setLayout(PageFunction::DrowBookList(&query, TempUser->getRole()));

            ui->GiveTicketSearchLine->setPlaceholderText("Введите название книги/автора");
            ui->GiveTicketChangePswdBtn->hide();
            ui->GiveTicketDelBtn->hide();
            ui->GiveTicketBookBtn->hide();
            ui->CreateTicketBtn->hide();
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
                    FinalMessage += QString("%1 (%2шт.) была возвращена библиотеке\n")
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
            query.exec("SELECT `tickets`.`FirstName`, `tickets`.`MiddleName`, `tickets`.`LastName`, `tickets`.`Role`, `studentbook`.`ID`, `studentbook`.`ReturnDate`, `studentbook`.`Count`, `books`.`Name`, `books`.`Author` FROM `tickets`\
            INNER JOIN `studentbook` ON `tickets`.`ID` = `studentbook`.`Student`\
            INNER JOIN `books` ON `books`.`ID` = `studentbook`.`Book`\
            WHERE `tickets`.`ID` = '" + QString::number(TempUser->UserIntMap.value("UserID")) + "'");
            ui->scrollAreaGiveTicket->setLayout(PageFunction::DrowBookList(&query, TempUser->getRole()));
        }
    }
}

void MainWindow::on_GiveTicketSearchBtn_clicked()
{
    ClearWidget(ui->scrollAreaGiveTicket);

    QString Search = ui->GiveTicketSearchLine->text();

    QSqlQuery query(DB);

    if(ui->GiveTicketPage->property("PageStatus") == 1)
    {
        if(Search.length() == 0) query.exec("SELECT * FROM tickets");
        else query.exec(QString("SELECT * FROM tickets WHERE `TicketNumber` LIKE '%%1%' OR `FirstName` LIKE '%%1%' OR `MiddleName` LIKE '%%1%' OR `LastName` LIKE '%%1%' OR `StudentIDNumber` LIKE '%%1%'").arg(Search));

        ui->scrollAreaGiveTicket->setLayout(PageFunction::DrowUsersList(&query));
        ui->GiveTicketSearchLine->setText("");
    }
    else if(ui->GiveTicketPage->property("PageStatus") == 2)
    {
        if(Search.length() == 0)
        {
            query.exec(QString("SELECT `tickets`.`FirstName`, `tickets`.`MiddleName`, `tickets`.`LastName`, `tickets`.`Role`, `studentbook`.`ID`, `studentbook`.`ReturnDate`, `studentbook`.`Count`, `books`.`Name`, `books`.`Author` FROM `tickets`\
            INNER JOIN `studentbook` ON `tickets`.`ID` = `studentbook`.`Student`\
            INNER JOIN `books` ON `books`.`ID` = `studentbook`.`Book`\
            WHERE `tickets`.`ID` = '%1'").arg(TempUser->UserIntMap.value("UserID")));
        }
        else
        {
            query.exec(QString("SELECT `tickets`.`FirstName`, `tickets`.`MiddleName`, `tickets`.`LastName`, `tickets`.`Role`, `studentbook`.`ID`, `studentbook`.`ReturnDate`, `studentbook`.`Count`, `books`.`Name`, `books`.`Author` FROM `tickets`\
            INNER JOIN `studentbook` ON `tickets`.`ID` = `studentbook`.`Student`\
            INNER JOIN `books` ON `books`.`ID` = `studentbook`.`Book`\
            WHERE `tickets`.`ID` = '%1' AND (`books`.`Name` LIKE '%%2%' OR `books`.`Author` LIKE '%%2%')").arg(TempUser->UserIntMap.value("UserID")).arg(Search));
        }

        ui->scrollAreaGiveTicket->setLayout(PageFunction::DrowBookList(&query, TempUser->getRole()));
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
///////////////////////////////////////////Меню книг//////////////////////////////////

void MainWindow::on_BookListBtn_clicked()
{
    if(ui->MainWidget->currentWidget() == ui->BooksPage) ChangePage(ui->ClearPage);
    else
    {
        ChangePage(ui->BooksPage);

        if(TempUser->getRole() == 0 || TempUser->getRole() == 2)
        {
            QSqlQuery query(DB);
            query.exec(QString("SELECT `tickets`.`FirstName`, `tickets`.`MiddleName`, `tickets`.`LastName`, `tickets`.`Role`, `studentbook`.`ID`, `studentbook`.`ReturnDate`, `studentbook`.`Count`, `books`.`Name`, `books`.`Author` FROM `tickets`\
            INNER JOIN `studentbook` ON `tickets`.`ID` = `studentbook`.`Student`\
            INNER JOIN `books` ON `books`.`ID` = `studentbook`.`Book`\
            WHERE `tickets`.`ID` = '%1'").arg(TempUser->getID()));

            ui->scrollAreaBooks->setLayout(PageFunction::DrowBookList(&query, TempUser->getRole()));

            query.exec(QString("SELECT `FirstName`, `MiddleName`, `LastName` FROM `tickets` WHERE ID = %1").arg(TempUser->getID()));
            if(query.next() == true) ui->TitleText->setText(QString("Книги %1 %2 %3").arg(query.value("LastName").toString()).arg(query.value("FirstName").toString()).arg(query.value("MiddleName").toString()));
        }
        else
        {
            QSqlQuery query(DB);
            query.exec("SELECT * FROM books");
            ui->scrollAreaBooks->setLayout(PageFunction::DrowBook(&query, 1));
        }
    }
}

void MainWindow::on_BooksSearchBtn_clicked()
{
    ClearWidget(ui->scrollAreaBooks);

    QString Search = ui->BooksSearchLine->text();
    QSqlQuery query(DB);

    if(TempUser->getRole() == 0 || TempUser->getRole() == 2)
    {
        if(Search.length() == 0)
        {
            query.exec(QString("SELECT `tickets`.`FirstName`, `tickets`.`MiddleName`, `tickets`.`LastName`, `tickets`.`Role`, `studentbook`.`ID`, `studentbook`.`ReturnDate`, `studentbook`.`Count`, `books`.`Name`, `books`.`Author` FROM `tickets`\
            INNER JOIN `studentbook` ON `tickets`.`ID` = `studentbook`.`Student`\
            INNER JOIN `books` ON `books`.`ID` = `studentbook`.`Book`\
            WHERE `tickets`.`ID` = '%1'").arg(TempUser->getID()));
        }
        else
        {
            query.exec(QString("SELECT `tickets`.`FirstName`, `tickets`.`MiddleName`, `tickets`.`LastName`, `tickets`.`Role`, `studentbook`.`ID`, `studentbook`.`ReturnDate`, `studentbook`.`Count`, `books`.`Name`, `books`.`Author` FROM `tickets`\
            INNER JOIN `studentbook` ON `tickets`.`ID` = `studentbook`.`Student`\
            INNER JOIN `books` ON `books`.`ID` = `studentbook`.`Book`\
            WHERE `tickets`.`ID` = '%1' AND (`books`.`Name` LIKE '%%2%' OR `books`.`Author` LIKE '%%2%')").arg(TempUser->getID()).arg(Search));
        }

        ui->scrollAreaBooks->setLayout(PageFunction::DrowBookList(&query, TempUser->getRole()));
    }
    else
    {
        if(Search.length() == 0) query.exec("SELECT * FROM books");
        else query.exec("SELECT * FROM books WHERE Name LIKE '%"+Search+"%' OR Author LIKE '%"+Search+"%'");

        ui->scrollAreaBooks->setLayout(PageFunction::DrowBook(&query, 1));
    }
    ui->BooksSearchLine->setText("");
}

void MainWindow::on_BooksHomeBtn_clicked() {ChangePage(ui->ClearPage);}

void MainWindow::on_BooksAddBtn_clicked() {ChangePage(ui->AddBookPage);}

void MainWindow::on_AddBookBackBtn_clicked()
{
    ChangePage(ui->BooksPage);
    QSqlQuery query(DB);
    query.exec("SELECT * FROM books");
    ui->scrollAreaBooks->setLayout(PageFunction::DrowBook(&query, 1));
}

void MainWindow::on_AddBookOkBtn_clicked()
{
    if(ui->BookNameInput->text().isEmpty() == true)
    {
        QMessageBox::warning(this, "Предупреждение", "Вы не ввели название книги", QMessageBox::Cancel);
        return void();
    }
    else if(ui->AuthorInput->text().isEmpty() == true)
    {
        QMessageBox::warning(this, "Предупреждение", "Вы не ввели автора", QMessageBox::Cancel);
        return void();
    }

    QSqlQuery query(DB);
    query.exec(QString("SELECT * FROM books WHERE `Name` = '%1'").arg(ui->BookNameInput->text()));

    if(query.next() == true)
    {
        if(TempUser->UserIntMap.contains("BookID") == true)
        {
            if(query.value("ID").toInt() != TempUser->UserIntMap.value("BookID"))
            {
                QMessageBox::warning(this, "Предупреждение", "Книга с таким названием уже есть в базе данных", QMessageBox::Cancel);
                return void();
            }
        }
        else
        {
            QMessageBox::warning(this, "Предупреждение", "Книга с таким названием уже есть в базе данных", QMessageBox::Cancel);
            return void();
        }
    }

    if(TempUser->UserIntMap.contains("BookID") == true)
    {
        query.exec(QString("UPDATE `books` SET `Name` = '%1',`Author` = '%2',`Count` = %3 WHERE `ID` = %4").arg(ui->BookNameInput->text()).arg(ui->AuthorInput->text()).arg(ui->BookCountInput->value()).arg(TempUser->UserIntMap.value("BookID")));

        QMessageBox::information(this, "Книга успешно отредактирована", QString("Книга %1 (%2) в количестве %3шт. успешно отредактирована").arg(ui->BookNameInput->text()).arg(ui->AuthorInput->text()).arg(ui->BookCountInput->value()));
    }
    else
    {
        query.exec(QString("INSERT INTO `books`(`Name`, `Author`, `Count`) VALUES ('%1','%2',%3)").arg(ui->BookNameInput->text()).arg(ui->AuthorInput->text()).arg(ui->BookCountInput->value()));

        QMessageBox::information(this, "Книга успешно добавлена", QString("Книга %1 (%2) в количестве %3шт. успешно создана").arg(ui->BookNameInput->text()).arg(ui->AuthorInput->text()).arg(ui->BookCountInput->value()));
    }
    ChangePage(ui->BooksPage);
    query.exec("SELECT * FROM books");
    ui->scrollAreaBooks->setLayout(PageFunction::DrowBook(&query, 1));
}

void MainWindow::on_BooksEditBtn_clicked()
{
    QList<QPushButton*> ButtonList = ui->scrollAreaBooks->findChildren<QPushButton*>();
    foreach(QPushButton* i, ButtonList)
    {
        if(i->isChecked())
        {
            QSqlQuery query(DB);

            TempUser->UserIntMap.insert("BookID", i->property("ID").toInt());

            query.exec(QString("SELECT * FROM books WHERE `ID` = %1").arg(i->property("ID").toInt()));

            ChangePage(ui->AddBookPage);

            if(query.next() == true)
            {
                ui->BookNameInput->setText(query.value("Name").toString());
                ui->AuthorInput->setText(query.value("Author").toString());
                ui->BookCountInput->setValue(query.value("Count").toInt());
                ui->TitleText->setText(QString("Редактирование книги %1").arg(query.value("Name").toString()));
            }
            return void();
        }
    }

    QMessageBox::warning(this, "Предупреждение", "Вы не выбрали книгу для редактирования", QMessageBox::Cancel);
    return void();
}

void MainWindow::on_BooksDelBtn_clicked()
{
    QList<QPushButton*> ButtonList = ui->scrollAreaBooks->findChildren<QPushButton*>();
    foreach(QPushButton* i, ButtonList)
    {
        if(i->isChecked())
        {
            QSqlQuery query(DB);

            query.exec(QString("SELECT `studentbook`.*, `tickets`.`FirstName`, `tickets`.`MiddleName`, `tickets`.`LastName` FROM `studentbook`\
            INNER JOIN `tickets` ON `studentbook`.`Student` = `tickets`.`ID` WHERE `Book` = %1").arg(i->property("ID").toInt()));

            QString ErrorMsg;
            while(query.next())
            {
                if(ErrorMsg.isEmpty()) ErrorMsg = "Невозможно удалить данную книгу так как ее не сдали следующие пользователи:\n";

                ErrorMsg += QString("%1 %2 %3 (%4шт.)").arg(query.value("LastName").toString()).arg(query.value("FirstName").toString()).arg(query.value("MiddleName").toString()).arg(query.value("Count").toString());
            }

            if(ErrorMsg.isEmpty() == false)
            {
                QMessageBox::warning(this, "Невозможно удалить книгу", ErrorMsg, QMessageBox::Cancel);
                return void();
            }

            query.exec(QString("SELECT * FROM books WHERE `ID` = %1").arg(i->property("ID").toInt()));

            if(query.next() == true)
            {
                QMessageBox::information(this, "Книга успешно удалена", QString("Книга %1 (%2) в количестве %3шт. успешно удалена").arg(query.value("Name").toString()).arg(query.value("Author").toString()).arg(query.value("Count").toInt()));
            }

            query.exec(QString("DELETE FROM `books` WHERE `ID` = %1").arg(i->property("ID").toInt()));

            ChangePage(ui->BooksPage);
            query.exec("SELECT * FROM books");
            ui->scrollAreaBooks->setLayout(PageFunction::DrowBook(&query, 1));
            return void();
        }
    }

    QMessageBox::warning(this, "Предупреждение", "Вы не выбрали книгу для удаления", QMessageBox::Cancel);
    return void();
}
///////////////////////////////////////////Информация////////////////////////////
void MainWindow::on_InfoBtn_clicked()
{
    if(ui->MainWidget->currentWidget() == ui->InfoPage) ChangePage(ui->ClearPage);
    else ChangePage(ui->InfoPage);
}

void MainWindow::on_InfoHomeBtn_clicked() {ChangePage(ui->ClearPage);}
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
        ui->AccountText->setText(QString("%1 %2 %3").arg(query.value("LastName").toString()).arg(query.value("FirstName").toString()).arg(query.value("MiddleName").toString()));
        ChangePage(ui->ClearPage);
    }
}

void MainWindow::on_Exit_clicked() {ChangePage(ui->AuthorizedPage);}
/////////////////////////////////////////////////////////////////////////////////////
