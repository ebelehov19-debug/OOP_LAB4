#include "Deposit.h"

#include <cmath>
#include <istream>
#include <ostream>


Bank_Optional* Deposit::clone() const
{
    return new Deposit(*this);
}

Deposit::Deposit()
: sum_(0.0), rate_(0.0), years_(1), cumulative_(false)
{}

Deposit::Deposit(double sum, double rate, int years, int cumulative)
: sum_(sum), rate_(rate), years_(years), cumulative_(cumulative)
{

}

double Deposit::metric() const
{
    if (cumulative_) 
    {
        return sum_ * (std::pow(1.0 + rate_, years_) - 1.0) / years_;
    }

    return sum_ * rate_;
}

bool Deposit::isCumulative() const
{
    return cumulative_;
}

void Deposit::read(std::istream& in)
{
    double sum;
    double rate;
    int years;
    int cumulative;
    if (!(in >> sum >> rate >> years >> cumulative)) 
    {
        return;
    }
    
        Deposit temp(sum, rate, years, cumulative);
        *this = temp;
    
}

void Deposit::print(std::ostream& out) const
{
    const auto previousPrecision = out.precision(6);
    out << "Deposit: sum=" << sum_
        << " rate=" << rate_
        << " years=" << years_
        << " cumulative=" << (cumulative_ ? 1 : 0)
        << " metric=" << metric();
    out.precision(previousPrecision);
}



    
