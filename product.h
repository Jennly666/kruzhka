#ifndef PRODUCT_H
#define PRODUCT_H
#include <QString>
class Product
{
public:
    Product();
    uint getAmount();
    uint getPrice();
    QString getName();

    void setName(QString const &n);
    void setPrice (uint const &p);
    void setAmount (uint const &a);
private:
    QString name;
    uint price;
    uint amount;
};

#endif // PRODUCT_H
