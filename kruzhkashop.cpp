#include "kruzhkashop.h"
#include "ui_kruzhkashop.h"
kruzhkashop::kruzhkashop(QWidget *parent) //создаем окно как таблицу для удобной работы
    : QWidget(parent)
    , ui(new Ui::kruzhkashop)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowMinMaxButtonsHint);//блок свёртывания и сворачивания
    this->setWindowTitle("Магазин товаров");
}

kruzhkashop::~kruzhkashop()
{
    delete ui;
}
void kruzhkashop::readtxt()
{
    QFile file("D:/Рабочий стол/products.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        QStringList parts = line.split(" - ");
        if (parts.size() == 2)
        {
            QString productName = parts[0];
            QStringList priceAndAmount = parts[1].split(" / ");
            if (priceAndAmount.size() == 2)
            {
                Product newproduct;
                QString priceStr = priceAndAmount[1];
                QString amountStr = priceAndAmount[0];

                newproduct.setName(productName);
                newproduct.setPrice(priceStr.toUInt());
                newproduct.setAmount(amountStr.toUInt());

                this->products.append(newproduct);
            }
        }
    }
    file.close();
}
void kruzhkashop::createshop(){

    this->readtxt();

    this->ui->Nick->setText(username);
    this->ui->tableWidget->setRowCount(products.size());
    this->ui->tableWidget->setColumnCount(6);

    this->ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // чтоб таблица была полностью видна

    QStringList nameColumn = {"Название" , "Количество" , "Цена" , "В корзине","",""};
    this->ui->tableWidget->setHorizontalHeaderLabels(nameColumn);



    this->ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //блокировка ячеек

    for (int i = 0; i < products.size(); ++i) {
        QTableWidgetItem *itemName = new QTableWidgetItem(products[i].getName());
        QTableWidgetItem *itemAmount = new QTableWidgetItem(QString::number(products[i].getAmount()));
        QTableWidgetItem *itemPrice = new QTableWidgetItem(QString::number(products[i].getPrice()));
        QTableWidgetItem *itemPicked = new QTableWidgetItem(QString::number(0));

        QPushButton* buttonPlus = new QPushButton("add");
        QPushButton* buttonMinus = new QPushButton("sub");

        // Создаем новый объект, который будет хранить данные строки, чтобы быть переданным в слоты
        QSignalMapper* signalMapperPlus = new QSignalMapper(this);
        QSignalMapper* signalMapperMinus = new QSignalMapper(this);

        // Устанавливаем маппинг для каждой кнопки
        signalMapperPlus->setMapping(buttonPlus, i);
        signalMapperMinus->setMapping(buttonMinus, i);

        // Соединяем сигналы нажатия кнопок с мапперами
        connect(buttonPlus, SIGNAL(clicked()), signalMapperPlus, SLOT(map()));
        connect(buttonMinus, SIGNAL(clicked()), signalMapperMinus, SLOT(map()));

        // Соединяем мапперы со слотами
        connect(signalMapperPlus, SIGNAL(mapped(int)), this, SLOT(on_buttonPlus_clicked(int)));
        connect(signalMapperMinus, SIGNAL(mapped(int)), this, SLOT(on_buttonMinus_clicked(int)));


        this->ui->tableWidget->setItem(i, 0, itemName);
        this->ui->tableWidget->setItem(i, 1, itemAmount);
        this->ui->tableWidget->setItem(i, 2, itemPrice);
        this->ui->tableWidget->setItem(i, 3, itemPicked);
        this->ui->tableWidget->setCellWidget(i, 4, buttonPlus);
        this->ui->tableWidget->setCellWidget(i, 5, buttonMinus);
        this->ui->tableWidget->resizeColumnsToContents();
    }

}
void kruzhkashop::setusername(QString const &name){
    this->username = name;
}
void kruzhkashop::on_buttonPlus_clicked(int row) {
    uint currentPicked = this->ui->tableWidget->item(row, 3)->text().toInt();
    uint availableAmount = this->ui->tableWidget->item(row, 1)->text().toInt(); // Получаем доступное количество товара
    if (currentPicked < availableAmount || (currentPicked+availableAmount == products[row].getAmount() && availableAmount > 0)) {
        this->ui->tableWidget->item(row, 3)->setText(QString::number(currentPicked + 1)); // Увеличиваем количество выбранных товаров
        this->ui->tableWidget->item(row, 1)->setText(QString::number(availableAmount - 1));
    }
}

void kruzhkashop::on_buttonMinus_clicked(int row) {
    uint currentPicked = this->ui->tableWidget->item(row, 3)->text().toInt();
    uint availableAmount = this->ui->tableWidget->item(row, 1)->text().toInt();
    if (currentPicked > 0) {
        this->ui->tableWidget->item(row, 3)->setText(QString::number(currentPicked - 1)); // Уменьшаем количество выбранных товаров
        this->ui->tableWidget->item(row, 1)->setText(QString::number(availableAmount + 1));
    }
}
void kruzhkashop::resetTable() {
    uint totalItems = this->ui->tableWidget->rowCount();
    for (uint i = 0; i < totalItems; ++i) {
        this->ui->tableWidget->item(i, 1)->setText(QString::number(this->products[i].getAmount())); // Восстанавливаем количество товаров на складе
        this->ui->tableWidget->item(i, 3)->setText(QString::number(0)); // Сбрасываем количество выбранных товаров в корзине
    }
}
void kruzhkashop::on_buttonconfirm_clicked() {
    QFile file("D:/Рабочий стол/check.txt");
    QTextStream stream(&file);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        stream << "Имя покупателя: " + username;
        uint totalItems = this->ui->tableWidget->rowCount(); // Получаем общее количество товаров
        uint total = 0;
        for (uint i = 0; i < totalItems; ++i) {
            uint pickedAmount = this->ui->tableWidget->item(i, 3)->text().toInt(); // Получаем количество выбранных товаров
            if (pickedAmount > 0) {
                QString itemName = this->products[i].getName(); // Получаем название товара
                uint itemPrice = this->products[i].getPrice(); // Получаем цену товара
                uint totalPrice = pickedAmount * itemPrice; // Вычисляем общую стоимость товара
                total +=totalPrice;
                // Выполняем действия для подтверждения покупки, например, выводим информацию о товаре и его стоимости
                stream << "\n1):" + itemName + ", Кол-во: " + QString::number(pickedAmount) + " / Итого : " + QString::number(totalPrice);
            }
        }
        if(total == 0){
            QMessageBox::information(this,"ERROR", "Корзина пуста");
            return;
        }
        else{
            QMessageBox::information(this,"Успешная покупка", "Вся информация в чеке");
            stream <<"\nОбщая сумма чека: " + QString::number(total);
            QProcess::startDetached("notepad.exe", QStringList() << "D:/Рабочий стол/check.txt");

        }
    }



    resetTable();
}
