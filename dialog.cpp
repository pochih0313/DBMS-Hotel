#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}


void Dialog::on_buttonBox_clicked(QAbstractButton *button)
{
    MainWindow conn;
    if(!conn.connOpen()) {
        qDebug()<<"Failed";
    }

    QSqlQuery* qry = new QSqlQuery(conn.mydb);

    QString no, employee_id, guest_name, guest_id, phone, date, no_of_day, no_of_guest, room_no;
    no = ui->lineEdit->text();
    employee_id = ui->lineEdit_2->text();
    guest_name = ui->lineEdit_3->text();
    guest_id = ui->lineEdit_4->text();
    phone = ui->lineEdit_5->text();
    date = ui->dateEdit->text();
    no_of_day = ui->lineEdit_7->text();
    no_of_guest = ui->lineEdit_8->text();
    room_no = ui->lineEdit_9->text();

    if(ui->buttonBox->button(QDialogButtonBox::Ok)  == button)   //判断按下的是否為"確定”按鈕
    {
        qry->exec("select count(*) from GUEST where Guest_id='"+guest_id+"'");
        if(qry->next()) {
            if(qry->value(0).toInt() == 0) {
                qry->exec("insert into GUEST (Guest_id,Guest_name,Phone_no) values('"+guest_id+"', '"+guest_name+"', '"+phone+"')");
            }
            else {
                qDebug() << "guest's ID exists";
            }
        }
        qry->exec("insert into RESERVATION (Reservation_no, Guest_id, Check_in_date, No_of_days, No_of_guest, Room_no, Employee_id) values('" +no+"', '"+guest_id+"', '"+date+"', '"+no_of_day+"', '"+no_of_guest+"', '"+room_no+"', '"+employee_id+"')");
    }
}
