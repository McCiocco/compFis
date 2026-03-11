//to compile: 
//g++ -O3 -o anim nPendulum.cpp ../lib/numericalMethods.cpp -lsfml-graphics -lsfml-window -lsfml-system

#include "../lib/numericalMethods.h"
#include <SFML/Graphics.hpp>
#include <fstream>
#include "../lib/matrixUtils.h"


// PHYSICAL PARAMETERS

constexpr double PI = 3.141592654;
constexpr double g = 9.81;
constexpr double l = 1;
constexpr double FPS = 60;
// constexpr double tau = 1/FPS;
constexpr unsigned N = 15;
constexpr double gtoL = g / l;
constexpr double m = 1;
bool isNan = false;

constexpr int W = 1200, H = 800;
constexpr int ANTIAL = 8;
const sf::Color background = sf::Color::Black;
const sf::Color stuff = sf::Color::White;


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

struct Graphics {

    const Vector<2*N>& s;
    sf::RectangleShape ropes[N];
    double len;

    Graphics(const Vector<2*N>& s_): s(s_) {

        len = H * 2 / 5 / N;

        for (int i = 0; i < N; i++) {

            
            ropes[i].setSize({len, 1});
            ropes[i].setOrigin({0.5, 0});
            ropes[i].setFillColor(stuff);

        }

    } 

    void draw(sf::RenderWindow& target) {

        double x = W/2, y = H/3;
        for (int i = 0; i < N; i++) {

            ropes[i].setPosition(x, y);
            ropes[i].setRotation(90 - s[i] * 180 / PI);

            x += len * std::sin(s[i]);
            y += len * std::cos(s[i]);

            target.draw(ropes[i]);

        }



    }

};

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
    bool flag = false;
    for (int i = 0; i < N; i++) if (std::isnan(thetaDotDot[i])) flag = true;
    if (flag && ~isNan) {

        std::cout << std::fixed << std::setprecision(8);
        std::cout << "M:" << std::endl << M << std::endl;
        std::cout << "C:" << std::endl << C << std::endl;
        std::cout << theta << std::endl;
        std::cout << omega<< std::endl;
        std::cout << C * omega << std::endl;
        std::cout << thetaDotDot << std::endl;
        isNan = true;

    }
    
    return concat(omega, thetaDotDot);

}

int main() {

    sf::ContextSettings settings;
    settings.antialiasingLevel = ANTIAL;
    sf::RenderWindow window(sf::VideoMode(W, H), "N Pendulum", sf::Style::Default, settings);
    window.setFramerateLimit(FPS);
    window.setKeyRepeatEnabled(false);

    Vector<2*N> s;
    for (int i = 0; i < N; i++) s[i] = PI *0.9;
    Graphics g(s);

    double t = 0;
    double tau = 1/FPS;

    bool go = true;
    long long unsigned count = 0;


    while (window.isOpen()) {

        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                
                window.close();
                break;
            
            case sf::Event::KeyPressed:

                if (event.key.scancode == sf::Keyboard::Scan::Escape) {

                    window.close();

                } else if (event.key.scancode == sf::Keyboard::Scan::P) { 
                    
                    go = !go;
                
                } 
    
                break;

            default:
                break;
            }
            
        }

        // calculations

        if (go && !isNan) {
        
            s = rk4(s, t, tau, fGem<N>); 

            t += tau;

        }

        window.clear(background);
        
        //std::cout << "y" << std::endl;
        g.draw(window);
        
        window.display();
        
    }

}