#include "galaxy.h"

constexpr int ANTIAL = 8;
const sf::Color background = sf::Color::Black;

int main() {

    Eigen::setNbThreads(1);
    omp_set_num_threads(std::max(omp_get_max_threads() - 1, 1));

    sf::ContextSettings settings;
    settings.antialiasingLevel = ANTIAL;
    sf::RenderWindow window(sf::VideoMode(W, H), "N Body sim", sf::Style::Default, settings);
    window.setFramerateLimit(FPS);
    window.setKeyRepeatEnabled(false);

    State s = stateInit();
    Graphics g(s);

    sf::RectangleShape fog({W, H});
    fog.setFillColor(sf::Color(0, 0, 0, 50));

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

            for (int i = 0; i < updatesPerFrame; i++) s.verlet();
    	    window.draw(fog);

        }

        //window.clear(background);

        g.draw(window);

        window.display();

    }

}
