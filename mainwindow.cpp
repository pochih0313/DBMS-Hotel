#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->comboBox->addItem("---View Tables---");
    ui->comboBox->addItem("Guests");
    ui->comboBox->addItem("Employee");
    ui->comboBox->addItem("Room");
    ui->comboBox->addItem("Room reservation");
    ui->comboBox->addItem("Service reservation");

    mydb = QSqlDatabase::addDatabase("QSQLITE");
    mydb.setDatabaseName("D:/QT/workspace/database/Hotel/hotel.db");

    if(!mydb.open()) {
        qDebug()<<"Failed";
        return;
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    QSqlQueryModel* modal = new QSqlQueryModel();
    QSqlQuery* qry = new QSqlQuery(mydb);

    switch(index) {
        case 1: qry->prepare("select * from GUEST"); break;
        case 2: qry->prepare("select * from EMPLOYEE"); break;
        case 3: qry->prepare("select * from ROOM"); break;
        case 4: qry->prepare("select * from RESERVATION"); break;
        case 5: qry->prepare("select * from SERVICE"); break;
        default:;
    }

    qry->exec();
    modal->setQuery(*qry);
    ui->tableView->setModel(modal);
}

void MainWindow::on_pushButton_clicked()
{
    QSqlQueryModel* modal = new QSqlQueryModel();
    QSqlQuery* qry = new QSqlQuery(mydb);

    if(ui->radioButton->isChecked()) {
        if(ui->lineEdit->text() != "") {
            qry->prepare("select * from GUEST where Guest_name='"+ui->lineEdit->text()+"'");
        }
    }

    qDebug()<<ui->lineEdit->text();

    qry->exec();
    modal->setQuery(*qry);
    ui->tableView->setModel(modal);
    ui->lineEdit->setText("");
}
