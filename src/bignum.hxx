#pragma once
#include "bignum.hh"
#include <iterator>
#include <fstream>
#include <iostream>

    template <typename T>
bistro::BigNum<T>::BigNum(std::size_t base) : base_(base), positive_(true)
{}
template <typename T>
    template <typename Base>
bistro::BigNum<T>::BigNum(std::istream& in, const Base& b)
{
    positive_ = true;
    base_ = b.get_base_num();
    std::string line;
    for (auto i = 0; i < 3; i++)
        getline(in,line);
    auto i = 0;
    while (!b.is_operator(line[i]) && b.is_digit(line[i]))
    {
        set_digit(i,b.get_char_value(line[i]));
        i++;
    }
}
template <typename T>
typename bistro::BigNum<T>::self_t bistro::BigNum<T>::clone() const
{
    BigNum clone(base_);
    clone.positive_ = positive_;
    clone.number_ = number_;
    return clone;
}
template <typename T>
typename bistro::BigNum<T>::index_t bistro::BigNum<T>::get_num_digits() const
{
    return number_.size();
}
template <typename T>
typename bistro::BigNum<T>::digit_t bistro::BigNum<T>::get_digit(index_t i) const
{
    auto it = number_.begin();
    std::advance(it, i);
    return *it;
}
    template <typename T>
void bistro::BigNum<T>::set_digit(index_t i, digit_t d)
{
    auto k = i - number_.size();
    if (k > 0)
    {
        for (std::size_t j = 0; j <  k; j++)
            number_.push_back(0);
        number_.push_back(d);
    }
    else if (k == 0)
        number_.push_back(d);
    else
    {
        auto it = number_.begin();
        std::advance(it, i);
        *it = d;
    }
    auto it2 = number_.begin();
    while (*it2 == 0)
    {
        number_.erase(it2);
        it2++;
    }
}
template <typename T>
bool bistro::BigNum<T>::is_positive() const
{
    return positive_;
}
    template <typename T>
void bistro::BigNum<T>::set_positive(bool p)
{
    positive_ = p;
}
template <typename T>
bool bistro::BigNum<T>::operator>(const self_t& other) const
{
    if (this->is_positive() && !other.is_positive())
        return true;
    else if (!this->is_positive() && other.is_positive())
        return false;
    else
    {
        if (number_.size() > other.number_.size())
            return true;
        else if (number_.size() < other.number_.size())
            return false;
        else
        {
            auto it1 = number_.begin();
            auto it2 = other.number_.begin();
            while (it1 != number_.end())
            {
                if (*it1 > *it2)
                    return true;
                else if (*it1 < *it2)
                    return false;
                it1++;
                it2++;
            }
        }
    }
}
template <typename T>
bool bistro::BigNum<T>::operator==(const self_t& other) const
{
    if (this->is_positive() && other.is_positive())
    {
        if (number_.size() == other.number_.size())
        {
            auto it1 = number_.begin();
            auto it2 = other.number_.begin();
            while (it1 != number_.end())
            {
                if (*it1 != *it2)
                    return false;
                it1++;
                it2++;
            }
            return true;
        }
    }
    return false;
}
template <typename T>
bistro::BigNum<T>::operator bool() const
{
    if (number_[0] != 0)
        return true;
    else
        return false;
}
template <typename T>
template <typename Base>
std::ostream& bistro::BigNum<T>::print(std::ostream& out, const Base& b) const
{
    auto it1 = number_.begin();
    while (it1 != number_.end())
    {
        out << b.get_digit_representation(*it1);
        it1++;
    }
    return out;
}
template <typename T>
typename bistro::BigNum<T>::self_t bistro::BigNum<T>
::operator+(const self_t& other) const
{
    auto retenue = 0;
    size_t sum = 0;
    auto digit = 0;
    BigNum result(base_);
    auto it1 = other.number_.rbegin();
    auto it2 = number_.rbegin();
    while (it1 != other.number_.rend() && it2 != number_.rend())
    {
        sum = *it1 + *it2 + retenue;
        digit = sum % base_;
        result.number_.insert(result.number_.begin(), digit);
        if (sum >= base_)
            retenue = 1;
        else
            retenue = 0;
        it1++;
        it2++;
    }
    if (it1 != other.number_.rend())
    {
        while (it1 != other.number_.rend())
        {
            sum = *it1 + retenue;
            digit = sum % base_;
            result.number_.insert(result.number_.begin(), digit);
            if (sum >= base_)
                retenue = 1;
            else
                retenue = 0;
            it1++;
        }
    }
    else if (it2 != number_.rend())
    {
        while (it2 != number_.rend())
        {
            sum = *it2 + retenue;
            digit = sum % base_;
            result.number_.insert(result.number_.begin(), digit);
            if (sum >= base_)
                retenue = 1;
            else
                retenue = 0;
            it2++;
        }
    }
    if (retenue == 1)
        result.number_.insert(result.number_.begin(), retenue);

    return result;
}
