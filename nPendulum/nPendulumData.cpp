//to compile: 
//g++ -O3 -o anim nPendulum.cpp ../lib/numericalMethods.cpp -lsfml-graphics -lsfml-window -lsfml-system

#include "../lib/numericalMethods.h"
#include <fstream>
#include "../lib/matrixUtils.h"


// PHYSICAL PARAMETERS

constexpr double PI = 3.141592654;
constexpr double g = 9.81;
constexpr double l = 1;
constexpr double tau = 0.001;
constexpr double FPS = 60;
constexpr unsigned N = 15;
constexpr double gtoL = g / l;
constexpr double m = 1;

constexpr unsigned nSteps = 60000;

template <const unsigned N>
Matrix<N, N> Mcos(const Vector<N>& theta) {

    Matrix<N, N> ans;
    for (int i = 0; i < N; i++) ans[i][i] = std::cos(theta[i]);
    return ans;

}

template <const unsigned N>
Matrix<N, N> Msin(const Vector<N>& theta) {

    Matrix<N, N> ans;
    for (int i = 0; i < N; i++) ans[i][i] = std::sin(theta[i]);
    return ans;

}

template <const unsigned N>
Vector<2*N> f(const Vector<2*N>& s, const double t) {

    Vector<N> theta, omega, omega2;
    for (int i = 0; i < N; i++) 
        omega2[i] = s[N + i] * s[N + i], omega[i] = s[N+i], theta[i] = s[i];

    const Matrix<N, N> T = upperTriangular<N>(1) * lowerTriangular<N>(1);

    const Matrix<N, N> C = Mcos(theta);
    const Matrix<N, N> S = Msin(theta);
    const Matrix<N, N> A = C * T * C + S * T * S;
    const Matrix<N, N> B = S * T * C - C * T * S;
    Vector<N> vT;
    for (int i = 0; i < N; i++) vT[i] = (N - i + 1) * std::sin(theta[i]);
    
    const Vector<N> thetaDotDot =  inverse(A) * (-gtoL * vT + B * omega2);

    return concat(omega, thetaDotDot);


}

template <const unsigned N>
Vector<2*N> fGem(const Vector<2*N>& s, const double t) {

    Vector<N> theta, omega, omega2;
    for (int i = 0; i < N; i++) 
        omega2[i] = s[N + i] * s[N + i], omega[i] = s[N+i], theta[i] = s[i];

    Matrix<N, N> W, M, C;
    Vector<N> gVec;
    
    for (int i = 0; i < N; i++) for (int j = 0; j < N; j++) W[i][j] = m * l * l * (N - std::max(i, j));
    for (int i = 0; i < N; i++) for (int j = 0; j < N; j++) M[i][j] = W[i][j] * std::cos(theta[i] - theta[j]);
    for (int i = 0; i < N; i++) for (int j = 0; j < N; j++) C[i][j] = W[i][j] * std::sin(theta[i] - theta[j]) * omega[j];
    for (int i = 0; i < N; i++) gVec[i] = (N - i) * g * l * std::sin(theta[i]);
    
    Vector<N> thetaDotDot = inverse(M) * (C * (-1) * omega - gVec);
    
    return concat(omega, thetaDotDot);

}

template <const unsigned N>
Vector<2*N> fPaper(const Vector<2*N>& s, const double t) {

    Vector<N> theta, omega, omega2, thetaDotDot;
    for (int i = 0; i < N; i++) 
        omega2[i] = s[N + i] * s[N + i], omega[i] = s[N+i], theta[i] = s[i];

    for (int i = 0; i < N; i++) {

        for (int j = 0; j < N; j++) {

            ;

        }

    }

}

void outputData() {

    double t = 0;
    Vector<2*N> state;
    for (int i = 0; i < N; i++) state[i] = PI, state[N + i] = 0;

    double thetas[N][nSteps];
    for (int i = 0; i < N; i++) thetas[i][0] = state[i];
    
    Vector<2*N> (*fptr)(const Vector<2*N>&, const double);
    fptr = &f<N>;

    for (int i = 1; i < nSteps; i++) {

        state = rk4<2*N>(state, t, tau, fptr);

        if (i % 10 == 0) for (int j = 0; j < N; j++) thetas[j][i] = state[j];
        t += tau;

    }

    std::ofstream outf;
    outf.open("nPendulum.txt", std::ios::in | std::ios::trunc);

    outf << std::setprecision(15);
    outf << N << " " << nSteps << std::endl;
    for (int i = 0; i < N; i++) {

        for (int j = 0; j < nSteps; j++) outf << thetas[i][j] << " ";
        outf << std::endl;

    }


}

int main() {

    outputData();
    
}