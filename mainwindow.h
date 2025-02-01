#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QtSql>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public:
    QSqlDatabase DB;
    QSqlQueryModel *qryModel;
    QItemSelectionModel *selectModel;
public:
    void selectData();
    void updateRecord(int recNo);
private slots:
    void on_actOpenDB_triggered();

    void on_actInsert_triggered();

    void on_actEdit_triggered();

    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_actDelete_triggered();

    void on_actAddMoney_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
