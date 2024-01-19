#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowMinMaxButtonsHint);//блок свёртывания и сворачивания
    shop = new kruzhkashop();
    QWidget::setFixedSize(window()->width() , window()->height());
    this->setWindowTitle("Регистрация");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_registrationButton_clicked()
{
    if(ui->nickname->text().isEmpty() || ui->password->text().isEmpty()){
        QMessageBox::information(this,"ERROR", "Данные введены не полностью");
        return;
    }
    QString userName = ui->nickname->text();
    shop->setusername(userName);
    shop->show();
    shop->createshop();
    this->close();
}
