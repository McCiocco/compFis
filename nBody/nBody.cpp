//to compile: 
//g++ -O3 -o anim nBody.cpp -lsfml-graphics -lsfml-window -lsfml-system

#include <SFML/Graphics.hpp>
#include <Eigen/Dense>
#include <random>
#include <vector>
#include <iostream>

//FAI I BENCHMARK

//PHYSICAL PARAMETERS

constexpr unsigned N = 500;
constexpr double FPS = 60;
constexpr double updatesPerFrame = 1;
constexpr double tau = 1 / (FPS * updatesPerFrame);
constexpr double PI = 3.14159265358979323846;
constexpr double G = 100;

constexpr int W = 1800, H = 1000;
constexpr double mToPx = 1; //questo e' il fattore di conversione da metri a pixel
constexpr double simW = W / mToPx;
constexpr double simH = H / mToPx;

constexpr int ANTIAL = 8;
const sf::Color background = sf::Color::Black;
const sf::Color stuff = sf::Color::White;

typedef Eigen::VectorXd VecN;

struct NBodySim {

    double G;
    const double mToPx;
    const double simW = W / mToPx;
    const double simH = H / mToPx;
    // State s;
    // GraphicsVA gVA;
    // GraphicsCirclesTrails gct;

};



struct State {

    VecN x, y, vx, vy;
    VecN prevx, prevy;
    VecN ax, ay;
    VecN m;

    State(): x(N), y(N), vx(N), vy(N), prevx(N), prevy(N), ax(N), ay(N), m(N) {

        galassiaInit();

    }

    void sym8() {

        const double x1 = 0.97000436;
        const double y1 = -0.24308753;
        const double vx1 = 0.46620368;
        const double vy1 = 0.43236573;

        x[0] = x1; y[0] = y1;
        vx[0] = vx1; vy[0] = vy1;

        x[1] = -x1; y[1] = -y1;
        vx[1] = vx1; vy[1] = vy1;

        x[2] = 0; y[2] = 0;
        vx[2] = -2*vx1; vy[2] = -2*vy1;

        m[0] = 1; m[1] = 1; m[2] = 1;

        verletBackwardsStep();

    }

    void circleCheckInit() {

        double s = 0;
        double mass = 1;
        double r = 10;

        for (int j = 1; j < N; j++) {

            s += 1 / std::sin(PI * j / N);

        }
        // omega = 0.1875;

        double v = std::sqrt(G * mass * s / (4 * r));

        for (int i = 0; i < N; i++) {

            double theta = 2*PI / N * i;

            x[i] = r * std::cos(theta);
            y[i] = r * std::sin(theta);

            m[i] = mass;

            vx[i] = std::cos(PI / 2 + theta) * v;
            vy[i] = std::sin(PI / 2 + theta) * v;
            //aggiorna a verlet e fai un check

        }
        
        verletBackwardsStep();

    }

    void solarSystemInit() {

        x[0] = 0; y[0] = 0;
        vx[0] = 0; vy[0] = 0;
        m[0] = 1000; //massa sole

        for (int i = 1; i < N; i++) {

            double r = 100 + 10 * i;
            double theta = (double) std::rand() / RAND_MAX * 2 * PI;

            m[i] = 1;

            x[i] = r * std::cos(theta);
            y[i] = r * std::sin(theta);

            double v = std::sqrt(G * m[0] / r);
            vx[i] = -v * std::sin(theta);
            vy[i] = v * std::cos(theta);

        }
        verletBackwardsStep();

    }

    void randomInit() {

        std::random_device rd;
        std::mt19937 gen(rd());
        
        std::uniform_real_distribution<double> posx(-simW*9/20, simW*9/20);
        std::uniform_real_distribution<double> posy(-simH*9/20, simH*9/20);
        std::uniform_real_distribution<double> vel(-10000, 10000);
        std::uniform_real_distribution<double> mass(10000000000, 20000000000);


        for (int i = 0; i < N; i++) {

            x[i] = posx(gen);
            y[i] = posy(gen);
            vx[i] = vel(gen);
            vy[i] = vel(gen);
            m[i] = mass(gen);

        }

        verletBackwardsStep();

    }

    void galassiaInit() {

        const double M = 50000;
        m.setConstant(1);
        m[0] = M;

        x[0] = 0; y[0] = 0;
        vx[0] = 0; vy[0] = 0;

        std::mt19937 gen(42); 
        std::uniform_real_distribution<double> dist(100, 400);
        std::uniform_real_distribution<double> ang(0.0, 2.0 * PI);

        for (int i = 1; i < N; i++) {

            double r = dist(gen);
            double theta = ang(gen);

            x[i] = r * std::cos(theta);
            y[i] = r * std::sin(theta);

            double v = std::sqrt(G * M / r);
            vx[i] = -v * std::sin(theta);
            vy[i] = v * std::cos(theta);

        }

        verletBackwardsStep();

    }

    void getAcc(double softening = 1) {

        for (int i = 0; i < N; i++) {

            //usa .array() per abilitare il broadcasting
            VecN dx = x.array() - x[i];
            VecN dy = y.array() - y[i];

            //usa il softening per evitare che due corpi sovrapposti facciano esplodere la simulazione
            VecN dsSq = dx.array().square() + dy.array().square() + softening;

            VecN dsInvCube = dsSq.array().pow(-1.5);

            VecN fx = m.array() * dx.array() * dsInvCube.array();
            VecN fy = m.array() * dy.array() * dsInvCube.array();

            fx[i] = 0;
            fy[i] = 0;

            ax[i] = G * fx.sum(); 
            ay[i] = G * fy.sum(); 



        }

    }

    void EulerCromer() {

        getAcc();

        vx += ax  * tau;
        vy += ay * tau;

        x += vx * tau;
        y += vy * tau;

    }
    
    void verletBackwardsStep() {

        getAcc();
        prevx = x - vx * tau + .5 * ax * tau * tau;
        prevy = y - vy * tau + .5 * ay * tau * tau;

    }

    void verlet() {

        getAcc();

        prevx.swap(x);
        prevy.swap(y);

        x = 2*prevx - x + ax * tau * tau;
        y = 2*prevy - y + ay * tau * tau;

    }

};

struct GraphicsVA {

    const State& s;
    const double mToPx;
    sf::VertexArray bodies;
    sf::CircleShape sun;

    GraphicsVA(const State& s_, const double mToPx_): s(s_), mToPx(mToPx_), bodies(sf::Points, N-1) {

        sun.setRadius(10);
        sun.setFillColor(stuff);
        sun.setOrigin({10, 10});
        
        update();

    }

    void update() {

        sun.setPosition({W/2 + s.x[0] * mToPx, H/2 - s.y[0] * mToPx});

        for (int i = 0; i < N-1; i++) {

            bodies[i].position = {W/2 + s.x[i+1] * mToPx, H / 2 - s.y[i+1] * mToPx};

        } 

    }

    void draw(sf::RenderWindow& target) {

        update();
        target.draw(bodies);
        target.draw(sun);

    }

};

struct GraphicsCirclesTrails {

    const State& s;
    const double mToPx;
    std::vector<sf::CircleShape> bodies;
    sf::VertexArray trail;
    unsigned last_updated;
    double r;

    GraphicsCirclesTrails(const State& s_, const double mToPx_): 
        s(s_), mToPx(mToPx_), r(3), bodies(N), trail(sf::Points, 6000), last_updated(0) {

        for (int i = 0; i < N; i++) {

            double newR = r;
            bodies[i].setRadius(newR);
            bodies[i].setFillColor(stuff);
            bodies[i].setOrigin({newR, newR});

        }

        update();

    }

    void update() {

        for (int i = 0; i < N; i++) {

            //qui ci sono delle considerazioni da fare sulle coordinate dello schermo e della sim
            bodies[i].setPosition({W/2 + s.x[i] * mToPx, H / 2 - s.y[i] * mToPx});
            // trail[last_updated].color = stuff;
            // trail[last_updated++].position = bodies[i].getPosition();            

            // last_updated = last_updated % 500;
        }

    } 

    void draw(sf::RenderWindow& target) {

        update();
        for (int i = 0; i < N; i++) {
            
            target.draw(bodies[i]);

        }

        // target.draw(trail);

    }

};




int main() {

    sf::ContextSettings settings;
    settings.antialiasingLevel = ANTIAL;
    sf::RenderWindow window(sf::VideoMode(W, H), "N Body sim", sf::Style::Default, settings);
    window.setFramerateLimit(FPS);
    window.setKeyRepeatEnabled(false);

    State s;
    GraphicsVA g(s);

    bool go = true;
    // std::cout << s.x << std::endl << s.y << std::endl <<  s.x.size() << std::endl;

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
        
            for (int i = 0; i < updatesPerFrame; i++) s.verlet();

        }

        window.clear(background);
        
        g.draw(window);
        
        window.display();
        
    }

}