#include "matrix.h"
#include "matrixUtils.h"
#include <iomanip>

using namespace std;

int main() {

    Matrix<3, 3> m = {{1, 2, 3}, {2, 3, 1}, {-2, 3, -2}};
    Matrix<3, 3> n;

    Matrix<2,2> A = {{-1, 1.5}, {1, -1}};
    Vector<3> b = {1, 2, 3};

    cout << fixed << setw(10) << inverse(m) * m << endl;

}