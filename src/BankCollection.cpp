#include "BankCollection.h"

#include <limits>
#include <stdexcept>
#include <utility>

BankCollection::BankCollection():data_(nullptr),size_(0),capacity_(0) {}

BankCollection::~BankCollection()
{
    for (std::size_t i = 0; i < size_; ++i)
    {
        delete data_[i];
    }
    delete[] data_;
}

BankCollection:: BankCollection(const BankCollection& other):data_(nullptr), size_(0), capacity_(other.size_)
{
    if (capacity_ == 0)
        return;
    data_ = new Bank_Optional*[capacity_];

    try 
    {
        for (; size_ < other.size_; ++size_) 
        {
            data_[size_] = other.data_[size_]->clone();
            if (!data_[size_]) 
            {
                throw std::runtime_error("clone() returned nullptr");
            }
        }
    }
    catch (...) 
    {
        for (std::size_t i = 0; i < size_; ++i) 
        {
            delete data_[i];
        }
        delete[] data_;
        throw;
    }
}
BankCollection:: BankCollection(BankCollection&& other) noexcept
    : data_(other.data_), size_(other.size_), capacity_(other.capacity_)
{
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
}

void BankCollection::swap(BankCollection& other) noexcept
{
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
}
BankCollection& BankCollection::operator=(const BankCollection& other)
{
    BankCollection tmp(other);
    swap(tmp);                   
    return *this;
}
BankCollection& BankCollection::operator=(BankCollection&& other) noexcept
{
    if (this != &other)
    {
        BankCollection tmp(std::move(other));
        swap(tmp);                             
    }
    return *this;
}

void BankCollection::reserve(std::size_t capacity)
{
    if (capacity <= capacity_) 
    {
        return;
    }
    auto** newData = new Bank_Optional*[capacity];
    for (std::size_t i = 0; i < size_; ++i) 
    {
        newData[i] = data_[i];
    }
    delete[] data_;
    data_ = newData;
    capacity_ = capacity;
}

void BankCollection::add(Bank_Optional* object)
{
    if (!object) {
        throw std::invalid_argument("Cannot add nullptr");
    }
    for (std::size_t i = 0; i < size_; ++i) 
    {
        if (data_[i] == object) 
        {
            throw std::invalid_argument("Object already belongs to this collection");
        }
    }
    if (size_ == capacity_) {
        const auto maxCapacity = std::numeric_limits<std::size_t>::max() /
                                 sizeof(Bank_Optional*);
        if (size_ >= maxCapacity) 
        {
            throw std::length_error("Collection is too large");
        }
        const auto newCapacity = capacity_ == 0 ? 1 :
            (capacity_ > maxCapacity / 2 ? maxCapacity : capacity_ * 2);
        reserve(newCapacity);
    }
    data_[size_++] = object;
}

void BankCollection::remove(std::size_t index)
{
    if (index >= size_) {
        throw std::out_of_range("Collection index out of range");
    }
    delete data_[index];
    for (std::size_t i = index; i + 1 < size_; ++i) 
    {
        data_[i] = data_[i + 1];
    }
    data_[--size_] = nullptr;
}

std::size_t BankCollection::size() const
{
    return size_;
}

Bank_Optional& BankCollection::at(std::size_t index)
{
    if (index >= size_) 
    {
        throw std::out_of_range("Collection index out of range");
    }
    return *data_[index];
}

const Bank_Optional& BankCollection::at(std::size_t index) const
{
    if (index >= size_) 
    {
        throw std::out_of_range("Collection index out of range");
    }
    return *data_[index];
}

double BankCollection::totalMetric() const
{
    double total = 0;
    for (std::size_t i = 0; i < size_; ++i) 
    {
        total += data_[i]->metric();
    }
    return total;
}

void BankCollection::sortByMetric()
{
    for (std::size_t i = 1; i < size_; ++i) 
    {
        auto* current = data_[i];
        const double metric = current->metric();
        std::size_t j = i;
        while (j > 0 && data_[j - 1]->metric() > metric) 
        {
            std::swap(data_[j], data_[j - 1]);
            --j;
        }
    }
}
