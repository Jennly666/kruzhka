#ifndef SHOP_H
#define SHOP_H

#include <QObject>
#include <QVector>
#include "product.h"

class Shop : public QObject {
    Q_OBJECT
public:
    Shop(QObject* parent = nullptr);

    QVector<Product> getProducts() const;

public slots:
    void processOrder(const QString& username, const QVector<Product>& selectedProducts);

private:
    QVector<Product> products;

    void loadProductsFromFile(const QString& filename);
    void updateProductsFile();
    void generateCheque(const QString& username, const QVector<Product>& selectedProducts);
};

#endif // SHOP_H
