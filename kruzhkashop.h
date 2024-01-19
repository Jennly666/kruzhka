#ifndef KRUZHKASHOP_H
#define KRUZHKASHOP_H

#include<QWidget>
#include<QString>
#include<QTableWidget>
#include<QDebug>
#include<QHBoxLayout>
#include<QPushButton>
#include<QSignalMapper>
#include<fstream>
#include<QFile>
#include<QMessageBox>
#include <QProcess>
#include "product.h"
namespace Ui {
class kruzhkashop;
}

class kruzhkashop : public QWidget
{
    Q_OBJECT

public:
    kruzhkashop(QWidget *parent = nullptr);
    ~kruzhkashop();

    void readtxt();
    void createshop();
    void setusername(QString const &name);

    void resetTable();
private slots:
    void on_buttonPlus_clicked(int row);
    void on_buttonMinus_clicked(int row);

    void on_buttonconfirm_clicked();

private:
    Ui::kruzhkashop *ui;
    QString username;
    QList<Product> products;
};

#endif // KRUZHKASHOP_H
