#pragma once

#include <iosfwd>

class Bank_Optional
{
    public:
        virtual double metric() const = 0;
        virtual void read(std::istream& in) = 0  ;
        virtual void print(std::ostream& out) const = 0;
        virtual ~Bank_Optional() = default;

        explicit operator double() const ;
        bool operator ==(const Bank_Optional& other) const;    
};

std::ostream& operator<<(std::ostream& out, const Bank_Optional& object);
std::istream& operator>>(std::istream& in, Bank_Optional& object);
