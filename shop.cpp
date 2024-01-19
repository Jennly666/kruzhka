#include "shop.h"
#include <QFile>
#include <QTextStream>

Shop::Shop(QObject* parent)
    : QObject(parent) {
    loadProductsFromFile("products.txt");
}

QVector<Product> Shop::getProducts() const {
    return products;
}

void Shop::processOrder(const QString& username, const QVector<Product>& selectedProducts) {
    for (const Product& selectedProduct : selectedProducts) {
        for (Product& product : products) {
            if (product.getName() == selectedProduct.getName()) {
                product = Product(product.getName(), product.getQuantity() - selectedProduct.getQuantity(), product.getPrice());
                break;
            }
        }
    }

    updateProductsFile();
    generateCheque(username, selectedProducts);
}

void Shop::loadProductsFromFile(const QString& filename) {
    products.clear();

    QFile file(filename);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList parts = line.split("-+_\\=\\|/\\?\\.,~`!");
            if (parts.size() == 3) {
                QString name = parts[0].trimmed();
                int quantity = parts[1].trimmed().toInt();
                double price = parts[2].trimmed().toDouble();

                Product product(name, quantity, price);
                products.push_back(product);
            }
        }
        file.close();
    }
}

void Shop::updateProductsFile() {
    QFile file("products.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        for (const Product& product : products) {
            out << product.getName() << " - " << product.getQuantity() << " | " << product.getPrice() << "\n";
        }
        file.close();
    }
}

void Shop::generateCheque(const QString& username, const QVector<Product>& selectedProducts) {
    QFile file("cheque.txt");
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << "Username: " << username << "\n";
        out << "Selected Products:\n";
        for (const Product& product : selectedProducts) {
            out << "  " << product.getName() << " - " << product.getQuantity() << " - " << product.getPrice() << "\n";
        }
        out << "Total Price: " << QVector<Product>::fromStdVector(selectedProducts.toStdVector()).back().getPrice() << "\n\n";
        file.close();
    }
}
