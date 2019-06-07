#include "dialog2.h"
#include "ui_dialog2.h"

Dialog2::Dialog2(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog2)
{
    ui->setupUi(this);
}

Dialog2::~Dialog2()
{
    MainWindow conn;
    delete ui;
}

void Dialog2::on_buttonBox_clicked(QAbstractButton *button) // 刪除訂單
{
    MainWindow conn;
    if(!conn.connOpen()) {
        qDebug()<<"Failed";
    }

    conn.connOpen();

    QSqlQuery* qry = new QSqlQuery(conn.mydb);

    QString no;
    no = ui->lineEdit->text();

    if(ui->buttonBox->button(QDialogButtonBox::Ok)  == button)   //判断按下的是否為"確定”按鈕
    {
        // 判斷輸入訂單編號是否在TABLE裡
        qry->exec("select count(*) from RESERVATION where Reservation_no='"+no+"'");
        if(qry->next()) {
            if(qry->value(0).toInt() != 0) {
                qry->exec("delete from RESERVATION where Reservation_no="+no+""); //刪除此訂單
            }
            else {
                qDebug() << "Reservation not exists";
            }
        }
    }
}
