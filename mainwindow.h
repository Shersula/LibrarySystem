#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlQuery>
#include <QCheckBox>
#include "user.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:


    void on_LogInBtn_clicked();

    void on_Exit_clicked();

    void on_BookGiveBtn_clicked();

    void on_TicketGiveBtn_clicked();

    void on_GiveBookSearchBtn_clicked();

    void on_GiveBookOkBtn_clicked();

    void on_GiveBookHomeBtn_clicked();

    void on_GiveTicketHomeBtn_clicked();

    void on_GiveTicketSearchBtn_clicked();

    void on_GiveTicketBookBtn_clicked();

    void on_GiveTicketDelBtn_clicked();

    void on_GiveTicketOkBtn_clicked();

    void on_CreateTicketBtn_clicked();

    void on_GiveTicketBackBtn_clicked();

    void on_GiveTicketOkBtn_2_clicked();

    void on_AdminCheck_stateChanged(int status);

    void on_GiveTicketChangePswdBtn_clicked();

    void on_BookListBtn_clicked();

    void on_BooksHomeBtn_clicked();

    void on_BooksSearchBtn_clicked();

    void on_BooksAddBtn_clicked();

    void on_AddBookBackBtn_clicked();

    void on_AddBookOkBtn_clicked();

    void on_BooksEditBtn_clicked();

    void on_BooksDelBtn_clicked();

private:
    Ui::MainWindow *ui;
    void ClearWidget(QWidget* area);
    void ChangePage(QWidget* page);
    User* TempUser = nullptr;
};
#endif // MAINWINDOW_H
