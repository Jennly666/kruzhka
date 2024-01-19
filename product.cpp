#include "product.h"

Product::Product() {}

QString Product::getName(){
    return this->name;
}

uint Product::getAmount(){
    return this->amount;
}

uint Product::getPrice(){
    return this->price;
}

void Product::setName(const QString &n){
    this->name = n;
}
void Product::setPrice(const uint &p){
    this->price = p;
}
void Product::setAmount(const uint &a){
    this->amount = a;
}
