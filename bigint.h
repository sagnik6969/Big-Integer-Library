#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

class bigint
{

private:
    bool sign;
    vector<int> number_vector;

    bigint add(const bigint &i1, const bigint &i2) const
    {
        bigint ans;

        long long int carry = 0;

        for (int i = 0; i < max(i1.size(), i2.size()); i++)
        {
            long long curr;

            if (i >= i1.size())
                curr = carry + i2.number_vector[i];

            else if (i >= i2.size())
                curr = carry + i1.number_vector[i];

            else
                curr = carry + i1.number_vector[i] + i2.number_vector[i];

            ans.number_vector.push_back(curr % 10);

            carry = curr / 10;
        }

        while (carry != 0)
        {
            ans.number_vector.push_back(carry % 10);
            carry /= 10;
        }

        return ans;
    }

    bigint subtract(const bigint &i1, const bigint &i2) const
    {
        bool borrow = 0;

        bigint ans;

        for (int i = 0; i < i1.size(); i++)
        {

            int d1 = i1.number_vector[i], d2 = 0;

            if (i < i2.size())
                d2 = i2.number_vector[i];

            if (borrow)
            {
                d1 -= 1;
                borrow = 0;
            }

            if (d1 < d2)
            {

                d1 += 10;
                borrow = 1;
                ans.number_vector.push_back(d1 - d2);
            }

            else
                ans.number_vector.push_back(d1 - d2);
        }

        while (ans.number_vector.size() != 0 && ans.number_vector.back() == 0)
            ans.number_vector.pop_back();

        return ans;
    }

    bigint multiply(const bigint &i1, const bigint &i2)
    {

        auto multiply_with_int = [&](const bigint &obj, const int &num, int zeros, bigint &final_ans)
        {
            auto sum = [&](bigint &b1, bigint &b2)
            {
                int carry = 0;

                int i;

                vector<int> &s1 = b1.number_vector;
                vector<int> &s2 = b2.number_vector;

                for (i = 0; i < s1.size() || i < s2.size(); i++)
                {

                    int curr = carry;

                    if (i < s1.size())
                        curr += s1[i];

                    if (i < s2.size())
                        curr += s2[i];

                    carry = curr / 10;

                    if (i < s1.size())
                        s1[i] = curr % 10;
                    else
                        s1.push_back(curr % 10);
                }

                while (carry != 0)
                {

                    s1.push_back(carry % 10);
                    carry /= 10;
                }
            };

            long long carry = 0;
            bigint ans;

            for (int i = 0; i < zeros; i++)
                ans.number_vector.push_back(0);

            for (int i = 0; i < obj.size(); i++)
            {
                long long curr = carry + num * obj.number_vector[i];
                ans.number_vector.push_back(curr % 10);
                carry = curr / 10;
            }

            while (carry != 0)
            {
                ans.number_vector.push_back(carry % 10);
                carry /= 10;
            }

            while (ans.size() != 0 && ans.number_vector.back() == 0)
                ans.number_vector.pop_back();
            if (ans.number_vector.empty())
                ans.sign = 0;

            //    final_ans = final_ans + ans;
            sum(final_ans, ans);
        };

        bigint ans;
        // ans.number_vector.push_back(0);
        for (int i = 0; i < i1.size(); i++)
        {
            multiply_with_int(i2, i1.number_vector[i], i, ans);
        }

        return ans;
    }

    bigint divide(bigint i1, bigint i2)
    {

        bigint curr = i1;
        long long count = 0;

        while (curr >= i2)
        {
            curr = curr - i2;
            count++;
        }

        bigint ans = bigint(count);

        return ans;
    }

public:
    bigint(long long int num = 0)
    {

        if (num >= 0)
            sign = 0;
        else
            sign = 1;
        if (num < 0)
            num = -num;

        while (num != 0)
        {
            number_vector.push_back(num % 10);
            num /= 10;
        }
    }

    /* Constant member functions are those functions that are denied
    permission to change the values of the data members of their class. */

    int size() const
    {
        return number_vector.size();
    }

    int capacity() const
    {
        return number_vector.capacity();
    }

    bigint absolute_value() const
    {
        bigint ans = *this;
        ans.sign = 0;
        return ans;
    }

    /* When we take const input we are only allowed to call
    only the const member functions of the input object */

    bool operator>(const bigint &obj) const
    {

        if (sign && !obj.sign)
            return false;

        if (!sign && obj.sign)
            return true;

        if (size() > obj.size())
            return !sign;

        else if (size() < obj.size())
            return sign;

        for (int i = size() - 1; i >= 0; i--)
        {

            if (number_vector[i] > obj.number_vector[i])
                return !sign;
            if (number_vector[i] < obj.number_vector[i])
                return sign;
        }

        return false;
    }

    bool operator==(const bigint &obj) const
    {
        if (size() != obj.size() || sign != obj.sign)
            return false;

        for (int i = 0; i < size(); i++)
        {
            if (number_vector[i] != obj.number_vector[i])
                return false;
        }
        return true;
    }

    bool operator!=(const bigint &obj) const
    {
        return !(*this == obj);
    }

    bool operator>=(const bigint &obj) const
    {
        return ((*this > obj) || (*this == obj));
    }

    bool operator<(const bigint &obj) const
    {
        return !(*this >= obj);
    }

    bool operator<=(const bigint &obj) const
    {
        return !(*this > obj);
    }

    bigint operator+(const bigint &obj) const
    {

        bigint ans;

        if (obj.sign == sign)
        {
            ans = add(*this, obj);
            ans.sign = sign;
        }
        else
        {
            if (obj.absolute_value() > absolute_value())
            {
                ans = subtract(obj, *this);
                ans.sign = obj.sign;
            }
            else
            {
                ans = subtract(*this, obj);
                ans.sign = sign;
            }
        }

        // while (ans.size() != 0 && ans.number_vector.back() == 0)
        //     ans.number_vector.pop_back();
        if (ans.number_vector.empty())
            ans.sign = 0;

        return ans;
    }

    bigint operator-(const bigint &obj) const
    {

        bigint ans;

        if (sign != obj.sign)
        {

            ans = add(*this, obj);
            ans.sign = sign;
        }
        else
        {
            if (absolute_value() > obj.absolute_value())
            {
                ans = subtract(*this, obj);
                ans.sign = sign;
            }
            else
            {
                ans = subtract(obj, *this);
                ans.sign = !obj.sign;
            }
        }

        // while (!ans.number_vector.empty() && ans.number_vector.back() == 0)
        //     ans.number_vector.pop_back();

        if (ans.number_vector.empty())
            ans.sign = 0;

        return ans;
    }

    bigint operator*(bigint obj)
    {

        bigint ans;
        if (absolute_value() <= obj.absolute_value())
            ans = multiply(*this, obj);
        else
            ans = multiply(obj, *this);

        if (obj.sign == sign)
            ans.sign = 0;
        else
            ans.sign = 1;

        return ans;
    }

    bigint operator/(bigint obj)
    {

        bigint ans = divide((*this).absolute_value(), obj.absolute_value());

        if (obj.sign == sign)
            ans.sign = 0;
        else
            ans.sign = 1;

        return ans;
    }

    bigint operator++()
    {


        *this = *this + 1;

        return *this;
    }

    bigint operator++(int)
    {

        bigint temp = *this;

        operator++();

        return temp;
    }

    bigint operator--()
    {

        
        *this = *this - 1;

        return *this;
    }

    bigint operator--(int)
    {

        bigint temp = *this;

        operator--();

        return temp;
    }

    bigint operator+=(bigint obj)
    {

        *this = *this + obj;

        return *this;
    }

    bigint operator-=(bigint obj)
    {

        *this = *this - obj;

        return *this;
    }

    void print()
    {
        if (number_vector.empty())
            cout << 0;

        if (sign)
            cout << "-";
        for (int i = size() - 1; i >= 0; i--)
        {
            cout << number_vector[i];
        }
    }

    ostream &write(ostream &os)
    {

        if (number_vector.empty())
            os << 0;

        if (sign)
            os << "-";
        for (int i = size() - 1; i >= 0; i--)
        {
            os << number_vector[i];
        }
        // write stuff to stream
        return os;
    }
};

ostream &operator<<(std::ostream &os, bigint m)
{

    return m.write(os);
}
