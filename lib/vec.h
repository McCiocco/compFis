// Vector class for computing and shit with doubles

#ifndef LURCH_VEC
#define LURCH_VEC

#include <initializer_list>
#include <iostream>
#include <math.h>


template <const unsigned N>
struct Vector {

    double arr[N];

    Vector(); 

    Vector(std::initializer_list<double> l);

    // No size check
    Vector(const double _arr[]);

    inline const double& operator[](const unsigned i) const;

    inline double& operator[](const unsigned i);

    Vector operator+(const Vector& b) const;

    Vector operator-(const Vector& b) const;

    double operator*(const Vector& b) const; //scalar product

    Vector operator*(const double t) const; 

    Vector& operator+=(const Vector& b);

    Vector& operator-=(const Vector& b);

    Vector& operator*=(const double t);

    inline double norm() const;

    Vector& operator=(const Vector& b);

    bool operator==(const Vector& b) const;

    bool operator!=(const Vector& b) const;

};

template <const unsigned N>
std::ostream& operator<<(std::ostream& s, const Vector<N>& v);

template <const unsigned N>
Vector<N> operator*(const double t, const Vector<N>& b);

//start of implementation

template <const unsigned N>
Vector<N>::Vector(const double _a[]) {

    for (int i = 0; i < N; i++) arr[i] = _a[i];

}

template <const unsigned N>
Vector<N>::Vector() {

    for (int i = 0; i < N; i++) arr[i] = 0;

}

template <const unsigned N>
Vector<N>::Vector(std::initializer_list<double> l) {

    auto it = l.begin();
    for (int i = 0; i < N; i++) arr[i] = *it, it++;

}


template <const unsigned N>
inline double& Vector<N>::operator[](const unsigned i) {

    return arr[i];

}

template <const unsigned N>
inline const double& Vector<N>::operator[](const unsigned i) const {
    
    return arr[i];

}

template <const unsigned N>
Vector<N> Vector<N>::operator+(const Vector<N>& b) const {
    
    Vector<N> result;

    for (int i = 0; i < N; i++)

        result.arr[i] = arr[i] + b.arr[i];

    return result;

}

template <const unsigned N>
Vector<N> Vector<N>::operator-(const Vector<N>& b) const {
    
    Vector<N> result;

    for (int i = 0; i < N; i++)

        result.arr[i] = arr[i] - b.arr[i];
    
        return result;

    }

template <const unsigned N>
Vector<N> Vector<N>::operator*(const double t) const {
    
    Vector<N> result;

    for (int i = 0; i < N; i++)
    
        result.arr[i] = arr[i] * t;
    
    return result;

}

template <const unsigned N>
Vector<N> operator*(const double t, const Vector<N>& b) {

    return b * t;

}

template <const unsigned N>
double Vector<N>::operator*(const Vector<N>& b) const {

    double result = 0;

    for (int i = 0; i < N; i++)

        result += arr[i] * b.arr[i];

    return result;

}

template <const unsigned N>
Vector<N>& Vector<N>::operator+=(const Vector<N>& b) {
    
    for (int i = 0; i < N; ++i)
    
        arr[i] += b.arr[i];
    
    return *this;

}

template <const unsigned N>
Vector<N>& Vector<N>::operator-=(const Vector<N>& b) {
    
    for (int i = 0; i < N; ++i)
    
        arr[i] -= b.arr[i];
    
    return *this;

}

template <const unsigned N>
Vector<N>& Vector<N>::operator*=(const double t) {
    
    for (int i = 0; i < N; ++i)
    
        arr[i] *= t;
    
    return *this;

}

template <const unsigned N>
inline double Vector<N>::norm() const {

    return std::sqrt(*this * *this);

}

template <const unsigned N>
Vector<N>& Vector<N>::operator=(const Vector<N>& b) {

    for (int i = 0; i < N; i++)

        arr[i] = b.arr[i];

    return *this;

}

template <const unsigned N>
bool Vector<N>::operator==(const Vector<N>& b) const {

    for (int i = 0; i < N; ++i)

        if (arr[i] != b.arr[i]) return false;

    return true;

}

template <const unsigned N>
bool Vector<N>::operator!=(const Vector<N>& b) const {

    return !(*this == b);

}

template <const unsigned N>
std::ostream& operator<<(std::ostream& s, const Vector<N>& b) {

    for (int i = 0; i < N; i++)

        s << b.arr[i] << " ";

    return s;

}


#endif