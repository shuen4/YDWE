#pragma once
template <typename T>
class qmatrix
{
public:
    typedef T value_type[3][3];

public:
    qmatrix(T* data)
        : data_(data)
    { }

    qmatrix<T>& operator =(const value_type& r)
    {
        value_type& m = *(value_type*)data_;
        for (size_t i = 0; i < 3; ++i)
        {
            for (size_t j = 0; j < 3; ++j)
            {
                m[i][j] = r[i][j];
            }
        }
        return *this;
    }

    qmatrix<T>& operator *=(const value_type& r)
    {
        value_type& m = *(value_type*)data_;
        value_type l;
        for (size_t i = 0; i < 3; ++i)
        {
            for (size_t j = 0; j < 3; ++j)
            {
                l[i][j] = m[i][j];
            }
        }
        for (size_t i = 0; i < 3; ++i)
        {
            for (size_t j = 0; j < 3; ++j)
            {
                T n(0);
                for (size_t k = 0; k < 3; ++k)
                {
                    n += l[i][k] * r[k][j];
                }
                m[i][j] = n;
            }
        }
        return *this;
    }

private:
    T* data_;
};

#define M_PI       3.14159265358979323846   // pi