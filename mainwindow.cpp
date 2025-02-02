#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QFileDialog>
#include<QMessageBox>
#include"tdialogdata.h"
#include<QSqlQuery>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->tableView);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setAlternatingRowColors(true);

   //ui->tableView->setStyleSheet("alternate-background-color:rgb(217,225,240);background-color:rgb(255,255,255)");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::selectData()
{
    qryModel=new QSqlQueryModel();
    selectModel=new QItemSelectionModel(qryModel,this);
    ui->tableView->setModel(qryModel);
    ui->tableView->setSelectionModel(selectModel);
    qryModel->setQuery("SELECT empNo,Name,Gender,Birthday,Province,Department,"
                        "Salary FROM employee order by empNo");
    if(qryModel->lastError().isValid())
    {
        QMessageBox::information(this,"error","数据库表格读取失败\n"+qryModel->lastError().text());
        return;
    }
    QSqlRecord rec=qryModel->record();
    qryModel->setHeaderData(rec.indexOf("empNo"),Qt::Horizontal,"工号");
    qryModel->setHeaderData(rec.indexOf("Name"),Qt::Horizontal,"姓名");
    qryModel->setHeaderData(rec.indexOf("Gender"),Qt::Horizontal,"性别");
    qryModel->setHeaderData(rec.indexOf("Birthday"),Qt::Horizontal,"出生日期");
    qryModel->setHeaderData(rec.indexOf("Province"),Qt::Horizontal,"省份");
    qryModel->setHeaderData(rec.indexOf("Department"),Qt::Horizontal,"部门");
    qryModel->setHeaderData(rec.indexOf("Salary"),Qt::Horizontal,"工资");
    ui->actOpenDB->setEnabled(false);
    ui->actAddMoney->setEnabled(true);
    ui->actEdit->setEnabled(true);
    ui->actDelete->setEnabled(true);
    ui->actInsert->setEnabled(true);
}

void MainWindow::updateRecord(int recNo)
{
    QSqlRecord curRec=qryModel->record(recNo);
    int empNo=curRec.value("EmpNo").toInt();

    QSqlQuery query;
    query.prepare("select *from employee where EmpNo=:ID");
    query.bindValue(":ID",empNo);
    query.exec();
    query.first();
    if(!query.isValid())
        return;
    curRec=query.record();
    TDialogData *dataDialog=new TDialogData(this);
    dataDialog->setRecord(curRec);
    if(dataDialog->exec()==QDialog::Accepted)
    {
        QSqlRecord recData=dataDialog->record();
        query.prepare("update employee set Name=:Name,Gender=:Gender,"
                      "Birthday=:Birthday,Province=:Province,"
                      "Department=:Department,Salary=:Salary,"
                      "Memo=:Memo,Photo=:Photo "
                      "where EmpNo=:EmpNo"
                      );
        query.bindValue(":EmpNo",recData.value("EmpNo"));
        query.bindValue(":Name",recData.value("Name"));
        query.bindValue(":Gender",recData.value("Gender"));
        query.bindValue(":Birthday",recData.value("Birthday"));
        query.bindValue(":province",recData.value("Province"));
        query.bindValue(":Department",recData.value("Department"));
        query.bindValue(":Salary",recData.value("Salary"));
        query.bindValue(":Memo",recData.value("Memo"));
        query.bindValue(":Photo",recData.value("Photo"));
        if(!query.exec())
        {
            QMessageBox::critical(this,"错误","更新数据错误\n"+query.lastError().text());
        }
        else
        {
            qryModel->setQuery(qryModel->query().executedQuery());
        }
    }


}

void MainWindow::on_actOpenDB_triggered()
{
    QString aFile=QFileDialog::getOpenFileName(this,"选择文件","","SQLite数据库(*.db3)");
    if(aFile.isEmpty())
        return;

    DB=QSqlDatabase::addDatabase("QSQLITE");
    DB.setDatabaseName(aFile);
    if(DB.open())
        selectData();
    else
        QMessageBox::warning(this,"警告","打开数据文件失败");

}
void MainWindow::on_actInsert_triggered()
{
    QSqlQuery query;
    query.exec("select * from employee where EmpNo=-1");
    QSqlRecord curRec=query.record();
    curRec.setValue("EmpNo",3000+qryModel->rowCount());

    TDialogData *dataDialog=new TDialogData(this);
    dataDialog->setRecord(curRec);
    if(dataDialog->exec()==QDialog::Accepted)
    {
        QSqlRecord recData=dataDialog->record();
        query.prepare("INSERT INTO employee (EmpNo,Name,Gender,Birthday,Province,"
                      "Department,Salary,Memo,Photo)"
                      "VALUES(:EmpNo,:Name,:Gender,:Birthday,:Province,"
                      ":Department,:Salary,:Memo,:Photo)");
        query.bindValue(":EmpNo",recData.value("EmpNo"));
        query.bindValue(":Name",recData.value("Name"));
        query.bindValue(":Gender",recData.value("Gender"));
        query.bindValue(":Birthday",recData.value("Birthday"));
        query.bindValue(":province",recData.value("Province"));
        query.bindValue(":Department",recData.value("Department"));
        query.bindValue(":Salary",recData.value("Salary"));
        query.bindValue(":Memo",recData.value("Memo"));
         query.bindValue(":Photo",recData.value("Photo"));
        if(!query.exec())
         {
             QMessageBox::critical(this,"错误","插入数据错误\n"+query.lastError().text());
         }
        else
        {
            qryModel->setQuery(qryModel->query().executedQuery());
        }
    }
    delete dataDialog;
}


void MainWindow::on_actEdit_triggered()
{
    int curRecNo=selectModel->currentIndex().row();
    updateRecord(curRecNo);
}


void MainWindow::on_tableView_doubleClicked(const QModelIndex &index)
{
    updateRecord(index.row());
}


void MainWindow::on_actDelete_triggered()
{
    int curNo=selectModel->currentIndex().row();
    QSqlRecord curRec=qryModel->record(curNo);
    if(curRec.isEmpty())
        return;
    int empNo=curRec.value("EmpNo").toInt();
    QSqlQuery query;
    query.prepare("delete from employee where EmpNo=:ID");
    query.bindValue(":ID",empNo);
    if(!query.exec())
        QMessageBox::critical(this,"error","删除失败\n"+query.lastError().text());
    else
    {
        qryModel->setQuery(qryModel->query().lastQuery());
        qryModel->query().executedQuery();
    }
}


void MainWindow::on_actAddMoney_triggered()
{
    QSqlQuery qryUpdate;
    qryUpdate.exec("UPDATE employee SET Salary=Salary+1000");
    qryModel->setQuery(qryModel->query().lastQuery());
    // qryModel->query().executedQuery();
}

