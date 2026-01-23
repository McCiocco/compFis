#ifndef LURCH_MATRIXVIEW
#define LURCH_MATRIXVIEW

#include "matrix.h"

template <const unsigned M, const unsigned N>
struct MatrixView {

    Matrix<M, N>& m;

    MatrixView(Matrix& m_);
    
    virtual unsigned int f(const unsigned i, const unsigned j) const;

    virtual unsigned int g(const unsigned i, const unsigned j) const;

    const double& operator[](const unsigned i, const unsigned j) const;

    double& operator[](const unsigned i, const unsigned j);


};







#endif