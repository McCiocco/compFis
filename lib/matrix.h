///BRUMA

#ifndef LURCH_MAT
#define LURCH_MAT

#include <iostream>
#include <initializer_list>
#include <math.h>
#include "vec.h"

template <const unsigned M, const unsigned N>
struct Matrix{

    double arr[M][N];

    Matrix();

    Matrix(const int i);

    Matrix(std::initializer_list<std::initializer_list<double>> l);

    Matrix(const double _mat[M][N]);

    //this is meant to be used as mat[i][j]
    const double* operator[](const unsigned i) const;

    //idem as above
    double* operator[](const unsigned i);

    Vector<N> row(const unsigned i) const;

    Vector<M> col(const unsigned i) const;

    void setRow(const unsigned i, const Vector<N>& v);

    void setCol(const unsigned i, const Vector<N>& v);

    void rowSwap(const unsigned i, const unsigned j);

    
    
    Matrix operator+(const Matrix& m) const;

    Matrix operator-(const Matrix& m) const;

    Vector<M> operator*(const Vector<N>& v) const;

    Matrix operator*(const double k) const;
    
    Matrix operator/(const double k) const;

    Matrix& operator+=(const Matrix& m);

    Matrix& operator-=(const Matrix& m);

    Matrix& operator*=(const double k);

    Matrix& operator/=(const double k);
    


    Matrix& operator=(const Matrix& m);

    bool operator==(const Matrix& m) const;
    
    bool operator!=(const Matrix& m) const;


};

template <const unsigned M, const unsigned N, const unsigned L>
Matrix<M, L> operator*(const Matrix<M, N>& A, const Matrix<N, L>& b);

template <const unsigned M, const unsigned N>
std::ostream& operator<<(std::ostream& s, const Matrix<M, N>& m);

template <const unsigned M, const unsigned N>
Vector<M> operator*(const Vector<N>& v, const Matrix<M, N>& m);

template <const unsigned M, const unsigned N>
Matrix<M, N> operator*(const double k, const Matrix<M, N>& m);

// START OF IMPLEMENTATION

template <const unsigned M, const unsigned N>
std::ostream& operator<<(std::ostream& s, const Matrix<M, N>& m) {

    for (int i = 0; i < M; i++) {

        for (int j = 0; j < N; j++) s << m.arr[i][j] << " ";

        s << std::endl;

        
    }
    return s;

}

template <const unsigned M, const unsigned N>
Matrix<M, N>::Matrix(): arr() {}

template <const unsigned M, const unsigned N>
Matrix<M, N>::Matrix(const int i) {
    
    for (int i = 0; i < M; i++) for (int j = 0; j < N; j++) arr[i][j] = i;

}

template <const unsigned M, const unsigned N>
Matrix<M, N>::Matrix(std::initializer_list<std::initializer_list<double>> l) {

    auto it1 = l.begin();
    auto it2 = it1->begin();
    for (int i = 0; i < M; i++) {

        for (int j = 0; j < N; j++) {

            arr[i][j] = *(it2++);

        }

        it1++;
        it2 = it1->begin();

    }

}

template <const unsigned M, const unsigned N>
Matrix<M, N>::Matrix(const double _mat[M][N]) {

    for (int i = 0; i < M; i++) for (int j = 0; j < N; j++) arr[i][j] = _mat[i][j];

}

template <const unsigned M, const unsigned N>
const double* Matrix<M, N>::operator[](const unsigned i) const {

    return arr[i];

}

template <const unsigned M, const unsigned N>
double* Matrix<M, N>::operator[](const unsigned i) {

    return arr[i];

}


template <const unsigned M, const unsigned N>
Vector<N> Matrix<M, N>::row(const unsigned i) const {

    return Vector<N>(arr[i]);

}


template <const unsigned M, const unsigned N>
Vector<M> Matrix<M, N>::col(const unsigned i) const {

    Vector<M> a;

    for (int j = 0; j < M; j++) a[j] = arr[j][i];
    
    return a;


}

template <const unsigned M, const unsigned N>
void Matrix<M, N>::setRow(const unsigned i, const Vector<N>& v) {

    for (int j = 0; j < N; j++) arr[i][j] = v[j];

}

template <const unsigned M, const unsigned N>
void Matrix<M, N>::setCol(const unsigned i, const Vector<N>& v) {

    for (int j = 0; j < N; j++) arr[j][i] = v[j];

}


template <const unsigned M, const unsigned N>
void Matrix<M, N>::rowSwap(const unsigned i, const unsigned j) {
    
    double temp[N];
    for (int k = 0; k < N; k++) temp[k] = arr[i][k];

    for (int k = 0; k < N; k++) arr[i][k] = arr[j][k];
    for (int k = 0; k < N; k++) arr[j][k] = temp[k];

}



template <const unsigned M, const unsigned N>
Matrix<M, N> Matrix<M, N>::operator+(const Matrix<M, N>& m) const {

    Matrix<M, N> sum;
    for (int i = 0; i < M; i++) for (int j = 0; j < N; j++) 
        sum[i][j] = arr[i][j] + m[i][j];
        
    return sum;

}

template <const unsigned M, const unsigned N>
Matrix<M, N> Matrix<M, N>::operator-(const Matrix<M, N>& m) const {

    Matrix<M, N> diff;
    for (int i = 0; i < M; i++) for (int j = 0; j < N; j++) 
        diff[i][j] = arr[i][j] - m[i][j];
        
    return diff;

}

template <const unsigned M, const unsigned N>
Vector<M> Matrix<M, N>::operator*(const Vector<N>& v) const {

    Vector<M> prod;
    for (int i = 0; i < M; i++) prod[i] = this->row(i) * v;

    return prod;

}

template <const unsigned M, const unsigned N>
Vector<M> operator*(const Vector<N>& v, const Matrix<M, N>& m) {

    return m * v;

}

template <const unsigned M, const unsigned N>
Matrix<M, N> Matrix<M, N>::operator*(const double k) const {

    Matrix<M, N> prod;
    for (int i = 0; i < M; i++) for (int j = 0; j < N; j++)
        prod[i][j] = arr[i][j] * k;

    return prod;

}

template <const unsigned M, const unsigned N>
Matrix<M, N> operator*(const double k, const Matrix<M, N>& m) {

    return m * k;

}

template <const unsigned M, const unsigned N>
Matrix<M, N> Matrix<M, N>::operator/(const double k) const {

    Matrix<M, N> prod;
    for (int i = 0; i < M; i++) for (int j = 0; j < N; j++)
        prod[i][j] = arr[i][j] / k;

    return prod;

}

template <const unsigned M, const unsigned N, const unsigned L>
Matrix<M, L> operator*(const Matrix<M, N>& A, const Matrix<N, L>& B) {

    Matrix<M, L> prod;
    for (int i = 0; i < M; i++) for (int j = 0; j < L; j++)
        prod[i][j] = A.row(i) * B.col(j);

    return prod;

}



template <const unsigned M, const unsigned N>
Matrix<M, N>& Matrix<M, N>::operator+=(const Matrix& m) {

    for (int i = 0; i < M; i++) for (int j = 0; j < N; j++)
        arr[i][j] += m[i][j];
    
    return *this;

}

template <const unsigned M, const unsigned N>
Matrix<M, N>& Matrix<M, N>::operator-=(const Matrix& m) {

    for (int i = 0; i < M; i++) for (int j = 0; j < N; j++)
        arr[i][j] -= m[i][j];
    
    return *this;

}

template <const unsigned M, const unsigned N>
Matrix<M, N>& Matrix<M, N>::operator*=(const double k) {

    for (int i = 0; i < M; i++) for (int j = 0; j < N; j++)
        arr[i][j] *= k;
    
    return *this;

}

template <const unsigned M, const unsigned N>
Matrix<M, N>& Matrix<M, N>::operator/=(const double k) {

    for (int i = 0; i < M; i++) for (int j = 0; j < N; j++)
        arr[i][j] /= k;
    
    return *this;

}

template <const unsigned M, const unsigned N>
Matrix<M, N>& Matrix<M, N>::operator=(const Matrix& m) {

    for (int i = 0; i < M; i++) for (int j = 0; j < N; j++)
        arr[i][j] = m[i][j];

    return *this;

}

template <const unsigned M, const unsigned N>
bool Matrix<M, N>::operator==(const Matrix& m) const {

    for (int i = 0; i < M; i++) for (int j = 0; j < N; j++)
        if (arr[i][j] != m[i][j]) return false;

    return true;

}

template <const unsigned M, const unsigned N>
bool Matrix<M, N>::operator!=(const Matrix& m) const {

    return ~(*this == m);

}




#endif