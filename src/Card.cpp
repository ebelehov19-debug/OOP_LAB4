#include "Card.h"

#include <cmath>
#include <istream>
#include <ostream>


Bank_Optional* Card::clone() const
{
    return new Card(*this);
}

Card::Card():spending_(0.0),rate_(0.0){}

Card:: Card(double spending,double rate):
spending_(spending),rate_(rate)
{

}

double Card::metric() const
{
    return spending_*rate_;
}

bool Card::hasCashback() const
{
    return rate_>0;
}

void Card::read(std::istream& in)
{
    double spending;
    double rate;
    if (!(in >> spending >> rate)) 
    {
        return;
    }
    
        Card temp(spending, rate);
        *this = temp;
    
}

void Card::print(std::ostream& out) const
{
    const auto previousPrecision = out.precision(6);
    out << "Card: spending=" << spending_
        << " rate=" << rate_
        << " metric=" << metric();
    out.precision(previousPrecision);
}
