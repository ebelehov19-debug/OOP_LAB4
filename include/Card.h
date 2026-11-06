#pragma once

#include "Bank_Optional.h"

class Card:public Bank_Optional
{
private:
    double spending_;
    double rate_;
public:
    Card();
    Card(double spending,double rate);
    double metric() const override;
    Bank_Optional* clone() const override;
    void read(std::istream& in) override;
    void print(std::ostream& out) const override;
    bool hasCashback()const;
};


