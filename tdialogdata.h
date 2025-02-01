#ifndef TDIALOGDATA_H
#define TDIALOGDATA_H

#include <QDialog>
#include<QSqlRecord>
namespace Ui {
class TDialogData;
}

class TDialogData : public QDialog
{
    Q_OBJECT

public:
    explicit TDialogData(QWidget *parent = nullptr);
    ~TDialogData();
    QSqlRecord record();
    void setRecord(const QSqlRecord &newRecord);

private slots:
    void on_pushButtonImport_clicked();

    void on_pushButtonClear_clicked();

private:
    QSqlRecord m_record;

private:
    Ui::TDialogData *ui;
};

#endif // TDIALOGDATA_H
