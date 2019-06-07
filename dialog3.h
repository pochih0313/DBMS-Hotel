#pragma once
#ifndef DIALOG3_H
#define DIALOG3_H

#include <QDialog>
#include "mainwindow.h"
#include <QAbstractButton>
#include <QPushButton>

namespace Ui {
class Dialog3;
}

class Dialog3 : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog3(QWidget *parent = nullptr);
    ~Dialog3();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::Dialog3 *ui;
};

#endif // DIALOG3_H
