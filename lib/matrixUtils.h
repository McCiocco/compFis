#ifndef LURCH_MAT_UTILS
#define LURCH_MAT_UTILS

#include "matrix.h"
#include <utility>

//DECLARATIONS

template <const unsigned N>
Matrix<N, N> I();

template <const unsigned N>
struct PLU {

    Matrix<N, N> P, L, U;

    PLU(const Matrix<N, N>& A);

};

template<const unsigned N>
Vector<N> forwardSubstitution(const Matrix<N, N>& L, const Vector<N>& b);

template<const unsigned N>
Vector<N> backSubstitution(const Matrix<N, N>& U, const Vector<N>& y);

template<const unsigned N>
Vector<N> LUSolve(const Matrix<N, N>& A, const Vector<N>& b);

template <const unsigned N>
Matrix<N, N> inverse(const Matrix<N, N>& A);

// IMPLEMENTATION


template <const unsigned N>
Matrix<N, N> I() {

    Matrix<N, N> ans;
    for (int i = 0; i < N; i++) ans[i][i] = 1;

    return ans;

}


template <const unsigned N>
PLU<N>::PLU(const Matrix<N, N>& A) {

    L = I<N>();
    P = I<N>();
    U = A;

    int k;

    for (int i = 0; i < N; i++) {
        
        k = i;
        while (U[i][i] == 0) {

            U.rowSwap(i, k+1);
            P.rowSwap(i, k+1);
            k++;

        }

        for (int j = i+1; j < N; j++) {

            L[j][i] = U[j][i]/U[i][i];
            U.setRow(j, U.row(j) - U.row(i) * L[j][i]);

        }

    }

}

template<const unsigned N>
Vector<N> forwardSubstitution(const Matrix<N, N>& L, const Vector<N>& b) {

    Vector<N> y;
    y[0] = b[0] / L[0][0];
    double s;
    for (int i = 1; i < N; i++) {

        s = 0;
        for (int j = 0; j < i; j++) s += L[i][j] * y[j];
        y[i] = (b[i] - s) / L[i][i];

    }    

    return y;

}

template<const unsigned N>
Vector<N> backSubstitution(const Matrix<N, N>& U, const Vector<N>& y) {

    Vector<N> x;
    x[N-1] = y[N-1] / U[N-1][N-1];
    double s;
    for (int i = N-2; i >= 0; i--) {

        s = 0;
        for (int j = i+1; j < N; j++) s += U[i][j] * x[j];
        x[i] = (y[i] - s) / U[i][i];

    }

    return x;

}

template<const unsigned N>
Vector<N> PLUSolve(const Matrix<N, N>& A, const Vector<N>& b) {

    PLU plu(A);
    Vector<N> y = forwardSubstitution(plu.L, plu.P * b);

    return backSubstitution(plu.U, y);

}




template <const unsigned N>
Matrix<N, N> inverse(const Matrix<N, N>& A) {

    Matrix<N, N> AInv;
    PLU plu(A);
    Matrix B = I<N>();

    for (int j = 0; j < N; j++) {

        AInv.setCol(j, PLUSolve(A, B.col(j)));

    }

    return AInv;

    
}






#endif