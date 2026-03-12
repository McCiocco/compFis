#include "../state.h"
#include <SFML/Graphics.hpp>

constexpr double r = 2;
constexpr double mass = 1;

const sf::Color stuff = sf::Color::White;

struct Graphics {

    const State& s;
    std::vector<sf::CircleShape> bodies; //attento agli includes di vector
    
    Graphics(const State&);

    void update();

    void draw(sf::RenderWindow&);

};

State stateInit();
