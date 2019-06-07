#include "dialog3.h"
#include "ui_dialog3.h"

Dialog3::Dialog3(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog3)
{
    ui->setupUi(this);
}

Dialog3::~Dialog3()
{
    MainWindow conn;
    delete ui;
}

void Dialog3::on_buttonBox_clicked(QAbstractButton *button) //新增額外服務資料
{
    MainWindow conn;
    if(!conn.connOpen()) {
        qDebug()<<"Failed";
    }

    conn.connOpen();

    QSqlQuery* qry = new QSqlQuery(conn.mydb);

    QString no, service_name, guest_id, price, employee_id;
    no = ui->lineEdit->text();
    service_name = ui->lineEdit_2->text();
    guest_id = ui->lineEdit_3->text();
    price = ui->lineEdit_4->text();
    employee_id = ui->lineEdit_5->text();

    if(ui->buttonBox->button(QDialogButtonBox::Ok)  == button)   //判断按下的是否為"確定”按鈕
    {
        qry->exec("insert into SERVICE (Service_no, Service_name, Guest_id, Service_price, Employee_id) values('"+no+"', '"+service_name+"', '"+guest_id+"', '"+price+"', '"+employee_id+"')");
    }
}
