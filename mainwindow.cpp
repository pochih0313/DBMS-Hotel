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


    connOpen(); //open database (in mainwindow.h)

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_comboBox_currentIndexChanged(int index) //顯示每個table內容
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
        // 住客資料查詢:輸入名字
        if(ui->lineEdit->text() != "") {
            qry->exec("select * from GUEST where Guest_name='"+ui->lineEdit->text()+"'");
        }
    }
    else if(ui->radioButton_2->isChecked()) {
        // 訂單查詢:輸入名字
        if(ui->lineEdit->text() != "") {
            // 搜尋符合名字的所有訂單
            qry->exec("select Reservation_no,Guest_name,RESERVATION.Guest_id,Phone_no,Check_in_date,No_of_days,No_of_guest,Room_no from RESERVATION JOIN GUEST on RESERVATION.Guest_id = GUEST.Guest_id where Guest_name ='"+ui->lineEdit->text()+"'");
        }
    }
    else if(ui->radioButton_3->isChecked()) {
        // 訂單查詢:輸入房型
        if(ui->lineEdit->text() != "") {
            // 搜尋符合房型的所有訂單: IN
            qry->exec("select Reservation_no,Guest_name,RESERVATION.Guest_id,Phone_no,Check_in_date,No_of_days,No_of_guest,Room_no from RESERVATION JOIN GUEST on RESERVATION.Guest_id = GUEST.Guest_id where Room_no in(select Room_no from ROOM where Room_type='"+ui->lineEdit->text()+"')");
        }
    }
    else if(ui->radioButton_4->isChecked()) {
        // 服務總金額:輸入名字
        if(ui->lineEdit->text() != "") {
            // 搜尋符合名字的所有服務金額加總: SUM
            qry->exec("select Reservation_no,Guest_name,RESERVATION.Guest_id,Check_in_date,No_of_days,RESERVATION.Room_no,sum(Service_price) as 'Total_price' from RESERVATION JOIN GUEST on RESERVATION.Guest_id = GUEST.Guest_id JOIN SERVICE on RESERVATION.Guest_id = SERVICE.Guest_id where Guest_name = '"+ui->lineEdit->text()+"' group by SERVICE.Guest_id");
        }
    }
    else if(ui->radioButton_5->isChecked()) {
        // 當日最高消費訂單:輸入日期
        if(ui->lineEdit->text() != "") {
            // 搜尋符合日期的最高消費訂單: MAX
            qry->exec("select Reservation_no,RESERVATION.Guest_id,Check_in_date,No_of_days,RESERVATION.No_of_guest,RESERVATION.Room_no,Room_type,max(Price) as 'max_price' from RESERVATION JOIN ROOM on RESERVATION.Room_no = ROOM.Room_no where Check_in_date = '"+ui->lineEdit->text()+"'");
        }
    }
    else if(ui->radioButton_6->isChecked()) {
        // 當日最低消費訂單:輸入日期
        if(ui->lineEdit->text() != "") {
            // 搜尋符合日期的最低消費訂單: MIN
            qry->exec("select Reservation_no,RESERVATION.Guest_id,Check_in_date,No_of_days,RESERVATION.No_of_guest,RESERVATION.Room_no,Room_type,min(Price) as 'max_price' from RESERVATION JOIN ROOM on RESERVATION.Room_no = ROOM.Room_no where Check_in_date = '"+ui->lineEdit->text()+"'");
        }
    }

    qDebug()<<ui->lineEdit->text();

    modal->setQuery(*qry);
    ui->tableView->setModel(modal);
    ui->lineEdit->setText("");
}

void MainWindow::on_pushButton_2_clicked() // 按鈕:訂單輸入
{
    connClose();

    Dialog dialog;
    dialog.setModal(true);
    dialog.exec(); // 開新視窗
}

void MainWindow::on_pushButton_4_clicked() // 按鈕:刪除訂單
{
    connClose();

    Dialog2 dialog2;
    dialog2.setModal(true);
    dialog2.exec(); // 開新視窗
}

void MainWindow::on_pushButton_5_clicked() // 按鈕:非負責員工訂單
{
    QSqlQueryModel* modal = new QSqlQueryModel();
    QSqlQuery* qry = new QSqlQuery(mydb);
    // 搜尋所有職位不是'員工'的負責訂單: NOT IN
    qry->exec("select RESERVATION.Employee_id,Reservation_no,Guest_name,RESERVATION.Guest_id,Phone_no,Check_in_date,No_of_days,No_of_guest,Room_no from RESERVATION JOIN GUEST on RESERVATION.Guest_id = GUEST.Guest_id where Employee_id not in(select Employee_id from EMPLOYEE where Position='員工')");
    modal->setQuery(*qry);
    ui->tableView->setModel(modal);
}

void MainWindow::on_pushButton_6_clicked() // 按鈕:額外服務-住客名單
{
    QSqlQueryModel* modal = new QSqlQueryModel();
    QSqlQuery* qry = new QSqlQuery(mydb);
    // 有在'SERVICE' TABLE的所有住客: EXISTS
    qry->exec("select Guest_name,Guest_id,Phone_no from GUEST where exists(select * from SERVICE where GUEST.Guest_id = SERVICE.Guest_id)");
    modal->setQuery(*qry);
    ui->tableView->setModel(modal);
}

void MainWindow::on_pushButton_7_clicked() // 按鈕:無額外服務-住客名單
{
    QSqlQueryModel* modal = new QSqlQueryModel();
    QSqlQuery* qry = new QSqlQuery(mydb);
    // 沒有在'SERVICE' TABLE的所有住客: EXISTS
    qry->exec("select Guest_name,Guest_id,Phone_no from GUEST where not exists(select * from SERVICE where GUEST.Guest_id = SERVICE.Guest_id)");
    modal->setQuery(*qry);
    ui->tableView->setModel(modal);
}

void MainWindow::on_pushButton_8_clicked() // 按鈕:住房次數
{
    QSqlQueryModel* modal = new QSqlQueryModel();
    QSqlQuery* qry = new QSqlQuery(mydb);
    // 列出每個住客的訂單數量: COUNT
    qry->exec("select Guest_name,RESERVATION.Guest_id,count(RESERVATION.Guest_id) as 'Total_times' from RESERVATION JOIN GUEST on RESERVATION.Guest_id = GUEST.Guest_id group by RESERVATION.Guest_id");
    modal->setQuery(*qry);
    ui->tableView->setModel(modal);
}

void MainWindow::on_pushButton_9_clicked() // 按鈕:額外服務次數
{
    QSqlQueryModel* modal = new QSqlQueryModel();
    QSqlQuery* qry = new QSqlQuery(mydb);
    // 列出每個住客的額外服務數量: COUNT
    qry->exec("select Guest_name,GUEST.Guest_id,count(SERVICE.Guest_id) as 'No_of_services' from GUEST JOIN SERVICE on GUEST.Guest_id = SERVICE.Guest_id group by GUEST.Guest_id");
    modal->setQuery(*qry);
    ui->tableView->setModel(modal);
}

void MainWindow::on_pushButton_10_clicked() // 按鈕:單日平均住宿消費
{
    QSqlQueryModel* modal = new QSqlQueryModel();
    QSqlQuery* qry = new QSqlQuery(mydb);
    // 列出每日的平均房間收入: AVG
    qry->exec("select Check_in_date,avg(Price) as 'Average' from RESERVATION JOIN ROOM on RESERVATION.Room_no = ROOM.Room_no group by Check_in_date");
    qDebug() << qry;
    modal->setQuery(*qry);
    ui->tableView->setModel(modal);
}

void MainWindow::on_pushButton_11_clicked() // 按鈕:住宿金額滿3000
{
    QSqlQueryModel* modal = new QSqlQueryModel();
    QSqlQuery* qry = new QSqlQuery(mydb);
    // 列出住宿金額滿3000的住客及總金額: HAVING, SUM
    qry->exec("select Guest_name,RESERVATION.Guest_id,sum(Price) as 'Total_amount' from RESERVATION JOIN GUEST on RESERVATION.Guest_id = GUEST.Guest_id JOIN ROOM on RESERVATION.Room_no = ROOM.Room_no group by RESERVATION.Guest_id having sum(Price) > 3000");
    qDebug() << qry;
    modal->setQuery(*qry);
    ui->tableView->setModel(modal);
}

void MainWindow::on_pushButton_3_clicked() // 按鈕:加購服務
{
    connClose();

    Dialog3 dialog3;
    dialog3.setModal(true);
    dialog3.exec(); // 開新視窗
}

void MainWindow::on_pushButton_12_clicked() //透過介面輸入指令
{
    QSqlQueryModel* modal = new QSqlQueryModel();
    QSqlQuery* qry = new QSqlQuery(mydb);
    qry->exec(ui->textEdit->toPlainText());
    qDebug() << qry;
    modal->setQuery(*qry);
    ui->tableView->setModel(modal);
    ui->textEdit->setText("");
}
