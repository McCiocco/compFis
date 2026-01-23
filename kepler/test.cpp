#include "../lib/numericalMethods.h"


int main() {

    Vector<4> s({-0.44417787896201, 18.8542081567634, -177.689423906611, 2.79240232725923});

    const double c = 0.005 * 1/6;

    std::cout << c * s << std::endl;
    std::cout << (1/6) * 0.005 * s << std::endl;

}