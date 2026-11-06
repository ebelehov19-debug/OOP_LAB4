#pragma once

#include "Bank_Optional.h"

class Credit : public Bank_Optional
{
private:
    double sum_;
    double rate_;
    int years_;
    bool annuity_;

public:
    Credit();
    Credit(double sum, double rate, int years, bool annuity);

    double metric() const override;
    Bank_Optional* clone() const override;
    void read(std::istream& in) override;
    void print(std::ostream& out) const override;
    bool isAnnuity() const;
};
