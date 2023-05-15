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
    void on_BookGiveBtn_clicked();

    void on_OkBtn_clicked();

    void on_SearchBtn_clicked();

    void on_HomeBtn_clicked();

    void on_LogInBtn_clicked();

    void on_Exit_clicked();
private:
    Ui::MainWindow *ui;
    void ClearWidget(QWidget* area);
    void ChangePage(QWidget* page);
    User* TempUser = nullptr;
};
#endif // MAINWINDOW_H
