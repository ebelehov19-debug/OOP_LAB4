#pragma once
#include "Bank_Optional.h"
#include <iosfwd>

class Deposit:public Bank_Optional
{
private:
    double sum_;
    double rate_;
    int years_;
    bool cumulative_;
public:
    Deposit();
    Deposit(double sum, double rate, int years, bool cumulative);
    double metric() const override;

    void read(std::istream& in) override;
    void print(std::ostream& out) const override;

    bool isCumulative() const;

};
