#pragma once
#ifndef DIALOG2_H
#define DIALOG2_H

#include <QDialog>
#include "mainwindow.h"
#include <QAbstractButton>
#include <QPushButton>

namespace Ui {
class Dialog2;
}

class Dialog2 : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog2(QWidget *parent = nullptr);
    ~Dialog2();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::Dialog2 *ui;
};

#endif // DIALOG2_H
