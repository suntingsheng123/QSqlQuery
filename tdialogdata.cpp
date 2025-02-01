#include "tdialogdata.h"
#include "ui_tdialogdata.h"
#include <QPixmap>
#include<QBitArray>
#include<QFileDialog>
TDialogData::TDialogData(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TDialogData)
{
    ui->setupUi(this);
}

TDialogData::~TDialogData()
{
    delete ui;
}

QSqlRecord TDialogData::record()
{
    m_record.setValue("EmpNo",ui->spinBoxEmpNo->value());
    m_record.setValue("Name",ui->lineEditName->text());
    m_record.setValue("Gender",ui->comboBoxSex->currentText());
    m_record.setValue("Birthday",ui->dateTimeEdit->date());
    m_record.setValue("Province",ui->comboBoxBirth->currentText());
    m_record.setValue("Department",ui->comboBoxDepartment->currentText());
    m_record.setValue("Salary",ui->spinBoxSalary->value());
    m_record.setValue("Memo",ui->plainTextEdit->toPlainText());
    return m_record;
}

void TDialogData::setRecord(const QSqlRecord &newRecord)
{
    ui->spinBoxEmpNo->setEnabled(false);
    ui->spinBoxEmpNo->setValue(newRecord.value("EmpNo").toInt());
    ui->lineEditName->setText(newRecord.value("Name").toString());
    ui->comboBoxSex->setCurrentText(newRecord.value("Gender").toString());
    ui->dateTimeEdit->setDate(newRecord.value("Birthday").toDate());
    ui->comboBoxBirth->setCurrentText(newRecord.value("Province").toString());
    ui->comboBoxDepartment->setCurrentText(newRecord.value("Department").toString());
    ui->spinBoxSalary->setValue(newRecord.value("Salary").toInt());
    ui->plainTextEdit->setPlainText(newRecord.value("Memo").toString());
    QVariant va=newRecord.value("Photo");
    if(va.isValid())
    {
        QPixmap pic;
        pic.loadFromData(va.toByteArray());
        ui->labelpic->setPixmap(pic.scaledToWidth(ui->labelpic->size().width()));

    }
    m_record = newRecord;
}

void TDialogData::on_pushButtonImport_clicked()
{
    QString aFile=QFileDialog::getOpenFileName(this,"选择文件","","照片(*.jpg)");
    if(aFile.isEmpty())
        return;
    QFile file(aFile);
    if(!file.open(QIODevice::ReadOnly))
    {
        return;
    }
    QByteArray data=file.readAll();
    file.close();
    m_record.setValue("Photo",data);
    QPixmap pic;
    pic.loadFromData(data);
    ui->labelpic->setPixmap(pic.scaledToWidth(ui->labelpic->size().width()));

}


void TDialogData::on_pushButtonClear_clicked()
{
    m_record.setNull("Photo");
    ui->labelpic->clear();
}

