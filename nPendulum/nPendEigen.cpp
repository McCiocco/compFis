//to compile: 
//g++ -O3 -o anim nPendulum.cpp ../lib/numericalMethods.cpp -lsfml-graphics -lsfml-window -lsfml-system

#include <SFML/Graphics.hpp>
#include <Eigen/Dense>
#include <iostream>

// PHYSICAL PARAMETERS

constexpr double PI = 3.141592654;
constexpr double g = 9.81;
constexpr unsigned N = 100;
constexpr double FPS = 60;
constexpr double updatesPerFrame = N / 20 + 1;
constexpr double tau = 1/(FPS * updatesPerFrame);
constexpr double l = 1.0/N;
constexpr double gtoL = g / l;
constexpr double m = 1;

constexpr int W = 1200, H = 800;
constexpr int ANTIAL = 8;
const sf::Color background = sf::Color::Black;
const sf::Color stuff = sf::Color::White;

typedef Eigen::Vector<double, N> VecN;
typedef Eigen::Vector<double, 2*N> state;
typedef Eigen::Matrix<double, N, N> MatN;
// typedef Eigen::MatrixXd MatN;
// typedef Eigen::VectorXd VecN;
// typedef Eigen::VectorXd state;

MatN getW(const double m, const double l);
const MatN A = getW(m, l);


struct Graphics {

    const state& s;
    sf::RectangleShape ropes[N];
    double len;

    Graphics(const state& s_): s(s_) {

        len = H * 3 / 5 / N;

        for (int i = 0; i < N; i++) {

            
            ropes[i].setSize({len, 1});
            ropes[i].setOrigin({0.5, 0});
            ropes[i].setFillColor(stuff);

        }

    } 

    void draw(sf::RenderWindow& target) {

        double x = W/2, y = H/5;
        for (int i = 0; i < N; i++) {

            ropes[i].setPosition(x, y);
            ropes[i].setRotation(90 - s(i) * 180 / PI);

            x += len * std::sin(s(i));
            y += len * std::cos(s(i));

            target.draw(ropes[i]);

        }


    }

};

MatN getW(const double m , const double l) {

    MatN res;
    for (int i = 0; i < N; i++) for (int j = 0; j < N; j++) res(i, j) = (N - std::max(i, j));
    return res;

}

state fGem(const state& s) {

    VecN theta = s.head(N), omega = s.tail(N);

    MatN M, C;
    VecN gVec;
    
    for (int i = 0; i < N; i++) for (int j = 0; j < N; j++) M(i, j) = A(i, j) * std::cos(theta(i) - theta(j));
    for (int i = 0; i < N; i++) for (int j = 0; j < N; j++) C(i, j) = A(i, j) * std::sin(theta(i) - theta(j)) * omega(j);
    for (int i = 0; i < N; i++) gVec(i) = (N - i) * g / (m*l) * std::sin(theta(i));
    
    state newS;
    newS << omega, M.ldlt().solve(-C * omega - gVec);

    return newS;

}

state rk4Pend(const state& x, const double tau) {

    state f1 = fGem(x);
    state f2 = fGem(x + .5 * tau * f1);
    state f3 = fGem(x + .5 * tau * f2);
    state f4 = fGem(x + tau * f3);

    return x + (tau / 6.0) * (f1 + 2.0*f2 + 2.0*f3 + f4);

}

int main() {

    sf::ContextSettings settings;
    settings.antialiasingLevel = ANTIAL;
    sf::RenderWindow window(sf::VideoMode(W, H), "N Pendulum", sf::Style::Default, settings);
    window.setFramerateLimit(FPS);
    window.setKeyRepeatEnabled(false);

    state s;
    for (int i = 0; i < N; i++) s(i) = PI/3;
    Graphics g(s);


    bool go = true;


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

        if (go) {
        
            for (int i = 0; i < updatesPerFrame; i++) s = rk4Pend(s, tau); 

        }

        window.clear(background);
        
        g.draw(window);
        
        window.display();
        
    }

}
