#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <unistd.h>
#include <vector>

int main(){
    sf::RenderWindow window(sf::VideoMode(1800, 720), "Earth-2");
    std::vector<std::vector<int>>coords;
    sf::Vector2i position;

    char lastC='W';
    int s=20;
    std::vector<sf::RectangleShape> rectangles;
    std::vector<sf::RectangleShape> recback;
    window.clear();
    sf::RectangleShape rectangle;
    while (window.isOpen()){
        sf::Event event;
        while (window.pollEvent(event)){
            if(s<0){
                s=20;
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)&&sf::Keyboard::isKeyPressed(sf::Keyboard::Z)){
                if(rectangles.size()>=1){
                    recback.push_back(rectangles[rectangles.size()-1]);
                    rectangles.pop_back();
                }
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::R)){
                if(recback.size()>=1){
                    rectangles.push_back(recback[recback.size()-1]);
                    recback.pop_back();
                }
            }
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::MouseWheelMoved)
            {
                s-=event.mouseWheel.delta*10;
            }
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                recback.clear();
                position=sf::Mouse::getPosition(window);
                rectangle.setSize(sf::Vector2f(s, s));
                rectangle.setPosition(position.x-s/2, position.y-s/2);
                rectangles.push_back(rectangle);
            }
            if(sf::Mouse::isButtonPressed(sf::Mouse::Right)){
                if(lastC=='W'){
                    rectangle.setFillColor(sf::Color(255, 0, 0));
                    lastC='R';
                }
                else if(lastC=='R'){
                    rectangle.setFillColor(sf::Color(0, 255, 0));
                    lastC='G';
                }
                else if(lastC=='G'){
                    rectangle.setFillColor(sf::Color(0, 0, 255));
                    lastC='B';
                }
                else if(lastC=='B'){
                    rectangle.setFillColor(sf::Color(0, 0, 0));
                    lastC='Z';
                }
                else{
                    rectangle.setFillColor(sf::Color(255, 255, 255));
                    lastC='W';
                }
            }
            window.clear();
            for(int i=0; i<rectangles.size(); ++i){
                window.draw(rectangles[i]);
            }
            window.display();
        }
    }
    return 0;
}