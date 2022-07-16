#include <SFML/Graphics.hpp>
#include <vector>
#include <chrono>
#include <thread>

int main(){
    sf::RenderWindow window(sf::VideoMode(1800, 720), "CIRCLES");
    sf::CircleShape circle;
    std::vector<sf::CircleShape> circles;
    while (window.isOpen()){
        for (int i = 0; i < circles.size(); ++i) {
            if(circles[i].getRadius()==0){
                circles.erase(circles.begin()+i);
            }else{
                circles[i].setRadius(circles[i].getRadius()-1);
                circles[i].setPosition(circles[i].getPosition().x+1, circles[i].getPosition().y+1);
            }
        }
        sf::Event event;
        while(window.pollEvent(event)){
            if(event.type==sf::Event::Closed){
                window.close();
            }
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                circle.setRadius(40);
                circle.setPosition(sf::Mouse::getPosition(window).x-40, sf::Mouse::getPosition(window).y-40);
                circle.setFillColor(sf::Color(160, 180, 250));
                circles.push_back(circle);
            }

        }
        window.clear();
        for (int i = 0; i < circles.size(); ++i) {
            window.draw(circles[i]);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(15));
        window.display();
    }
}
