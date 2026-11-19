#include "Credit.h"

#include <cmath>
#include <istream>
#include <ostream>


Bank_Optional* Credit::clone() const
{
    return new Credit(*this);
}

Credit::Credit():sum_(0.0),rate_(0.0),years_(1),annuity_(false)
{}

Credit::Credit(double sum, double rate, int years, int annuity)
: sum_(sum), rate_(rate), years_(years), annuity_(annuity)
{

}
double Credit::metric() const
{
    if (!annuity_) 
    {
        return sum_ * rate_;
    }

    if (rate_ == 0.0) 
    {
        return sum_ / years_;
    }

    const double r = rate_ / 12.0;
    const double n = years_ * 12.0;
    const double payment =
        sum_ * r / (1.0 - std::pow(1.0 + r, -n));

    return 12.0 * payment;
}

bool Credit::isAnnuity() const
{
    return annuity_;
}

void Credit::read(std::istream& in)
{
    double sum;
    double rate;
    int years;
    int annuity;
    if (!(in >> sum >> rate >> years >> annuity)) 
    {
        return;
    }
    
        Credit temp(sum, rate, years, annuity);
        *this = temp;
    
}

void Credit::print(std::ostream& out) const
{
    const auto previousPrecision = out.precision(6);
    out << "Credit: sum=" << sum_
        << " rate=" << rate_
        << " years=" << years_
        << " annuity=" << (annuity_ ? 1 : 0)
        << " metric=" << metric();
    out.precision(previousPrecision);
}



    

