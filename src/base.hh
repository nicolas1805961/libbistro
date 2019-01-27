#pragma once

#include <cstdint>
#include <initializer_list>
#include <unordered_set>
#include <set>
#include <iterator>
#include <algorithm>
#include <vector>

namespace bistro
{
    /**
    ** Base class.
    **
    ** This class represents a numerical base, along with the means to convert
    ** single digits to and from textual representation.
    **
    ** A valid representation type (for \a Char) has the following properties:
    **   - it can be copied.
    **   - it can be output (with \c operator<< )
    **   - it can be compared to itself (with \c operator== )
    **   - it can be compared to a \c char (with \c operator== )
    **   - it provides a \c hash functor.
    **/
    template <typename Value = uint8_t, typename Char = char>
    class Base
    {
    public:
        /// A digit in textual representation.
        using char_t = Char;

        /// A digit in "value" representation.
        using value_t = Value;

        /// Default constructor.
        Base() = default;

        /// Construct a base from an initializer list.
        Base(std::initializer_list<char_t> list)
        {
            auto it1 = list.begin();
            while (it1 != list.end())
            {
                base_numbers_.first.push_back(*it1);
                it1++;
            }
            auto it2 = std::unique(base_numbers_.first.begin()
                    , base_numbers_.first.end());
            base_numbers_.first.resize(std::distance(base_numbers_.first.begin()
                        ,it2));
            for (long unsigned int i = 0; i < base_numbers_.first.size(); i++)
                base_numbers_.second.insert(i);
        }

        /// Get the numerical base represented.
        size_t get_base_num() const
        {
            return base_numbers_.first.size();
        }

        /**
        ** Add a the representation of a digit.
        **
        ** This will match the representation \a repr of the digit with the
        ** smallest unmatched value, thus increasing the size of the base by 1.
        **
        ** Throws invalid_argument if the representation is reserved (for an
        ** operator) or already matched).
        **/
        void add_digit(char_t repr)
        {
            base_numbers_.second.insert(base_numbers_.first.size());
            auto it1 = base_numbers_.first.begin();
            while (it1 != base_numbers_.first.end())
            {
                if (*it1 == repr)
                    return;
                it1++;
            }
            base_numbers_.first.push_back(repr);
        }

        /// Check wether there is a match for the character representation \a c.
        bool is_digit(char_t c) const
        {
            auto it1 = std::find(base_numbers_.first.begin()
                    , base_numbers_.first.end(), c);
            if (it1 != base_numbers_.first.end())
                return true;
            else
                return false;
        }

        /**
        ** Check wether the character representation \a c is reserved for an
        ** operator.
        **/
        static bool is_operator(char_t c)
        {
            if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%')
                return true;
            else
                return false;
        }

        /**
        ** Get the representation for the value \a i.
        **
        ** \throw std::out_of_range if the value is not in the base.
        **/
        char_t get_digit_representation(value_t i) const
        {
            auto it1 = base_numbers_.first.begin();
            for (auto j = 0; j < i; j++)
                it1++;
            return *it1;
        }

        /**
        ** Get the value for the representation \a i.
        **
        ** \throw std::out_of_range if the representation is not in the base.
        **/
        value_t get_char_value(char_t r) const
        {
            auto it1 = base_numbers_.first.begin();
            while (it1 != base_numbers_.first.end())
            {
                if (*it1 == r)
                    break;
                it1++;
            }
            return std::distance(base_numbers_.first.begin(), it1);
        }

    private:
        std::pair<std::vector<char_t>, std::set<value_t>> base_numbers_;
    };

}
