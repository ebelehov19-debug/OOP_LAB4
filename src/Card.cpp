#include "Card.h"

#include <cmath>
#include <istream>
#include <ostream>
#include <stdexcept>

Bank_Optional* Card::clone() const
{
    return new Card(*this);
}

Card::Card():spending_(0.0),rate_(0.0){}

Card:: Card(double spending,double rate):
    spending_(spending),rate_(rate)
{
        if (!std::isfinite(spending) || !std::isfinite(rate) ||
        spending < 0 || rate < 0){
             throw std::invalid_argument("Invalid card parameters");
        }
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

    in >> std::ws;
    if (in.peek() == 'C') {
        const auto expect = [&in](const char* text) 
        {
            in >> std::ws;
            for (; *text; ++text) {
                char actual;
                if (!in.get(actual) || actual != *text) 
                {
                    in.setstate(std::ios::failbit);
                    return false;
                }
            }
            return true;
        };

        double printedMetric;
        if (!expect("Card:") || !expect("spending=") || !(in >> spending) ||
            !expect("rate=") || !(in >> rate) ||
            !expect("metric=") || !(in >> printedMetric)) {
            return;
        }
    }
    else 
        if (!(in >> spending >> rate)) 
         {
            return;
        }

    try
     {
        Card temp(spending, rate);
        *this = temp;
    }
    catch (const std::invalid_argument&) 
    {
        in.setstate(std::ios::failbit);
    }
}

void Card::print(std::ostream& out) const
{
    out << "Card: spending=" << spending_
        << " rate=" << rate_
        << " metric=" << metric();
}
