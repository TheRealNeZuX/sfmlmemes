#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include <cstdlib>
#include <SFML/Audio.hpp>
#include <string>
#include <iostream>
using namespace sf;
using namespace std;

struct Block{
    int rotation = 2;
    int Lastrotation = 2;
    int prize = 0;
    Vector2f LastPos;
    /*
     0 - UP
     1 - DOWN
     2 - RIGHT
     3 - LEFT
     */
    RectangleShape self;
    Block(Color color = Color(0, 0, 0), float x = 0, float y = 0){
        self.setPosition(x, y);
        self.setFillColor(color);
        self.setSize(Vector2f(20.f, 20.f));
    }
    void SetColor(Color color){
        self.setFillColor(color);
    }
    void SetPos(sf::Vector2f pos){
        self.setPosition(pos);
    }
    Vector2f GetPos(){
        return self.getPosition();
    }
};
class Snake {

public:
    vector <Block> body;
    Vector2f speed{0, 0};
    Snake(float x, float y){
        body.emplace_back(Block(Color(255, 222, 0), x, y));
        for (int i = 0; i < 3; ++i) {
            body.emplace_back(Block(Color(85, 255, 0), x-22.f*float(i+1), y));
        }
    }
    void increase(){
        Block block(Color(85, 255, 0));
        Block last = body[body.size()-1];
        if(last.rotation == 0){
            block.SetPos(last.GetPos()+Vector2f{0, -22});
        }else if(last.rotation == 1){
            block.SetPos(last.GetPos()+Vector2f{0, 22});
        }else if(last.rotation == 2){
            block.SetPos(last.GetPos()+Vector2f{-22, 0});
        }else if(last.rotation == 3){
            block.SetPos(last.GetPos()+Vector2f{22, 0});
        }
        block.rotation = last.rotation;
        body.push_back(block);
    }
    void move(Keyboard::Key pressed, float sleep_prom){
        Thread a(&sleep, seconds(0.1));
        a.launch();
        map<Keyboard::Key, int> key{
            {Keyboard::W, 0},
            {Keyboard::S, 1},
            {Keyboard::D, 2},
            {Keyboard::A, 3},
        };
        map <int, Vector2f> turn{
                {0, {0, -22}},
                {1, {0, 22}},
                {2, {22, 0}},
                {3, {-22, 0}}
        };
        speed = turn[key[pressed]];
        body[0].Lastrotation = key[pressed];
        body[0].rotation = key[pressed];
        body[0].LastPos = body[0].GetPos();
        body[0].SetPos((Vector2f)body[0].GetPos()+speed);
        for (int i = 1; i < body.size(); ++i) {
            body[i].Lastrotation = body[i].rotation;
            body[i].LastPos = body[i].GetPos();
            body[i].SetPos(body[i-1].LastPos);
            body[i].rotation = body[i-1].Lastrotation;
        }
    }
};

vector <Block> spawn(int count){
    vector <Block> food;
    for(int i = 0; i < count; ++i){
        food.emplace_back(Block(Color(255, 0, 68), (float)(rand()%680), (float)(rand()%320)));
    }
    return food;
}
bool touch(Block first, Block second){
    if((first.GetPos().x <= second.GetPos().x+20&&first.GetPos().x+20 >= second.GetPos().x)&&(first.GetPos().y <= second.GetPos().y+20 && first.GetPos().y+20 >= second.GetPos().y)){
        return true;
    }
    return false;
}

int main() {
    RenderWindow window(VideoMode(700, 400), "Snake");
    Snake snake(250, 400);
    int points = snake.body.size()-4;
    //Rules
    bool suicide = true;
    bool walls = true;
    bool killed = false;
    //Rules
    //Time
    float Tick;
    sf::Clock clock;
    float LastFrame = 0;
    //Time
    //Sounds
    sf::SoundBuffer end_sound;
    end_sound.loadFromFile("../files/gameover.wav");
    Sound esound, fsound;
    esound.setBuffer(end_sound);
    sf::SoundBuffer food_sound;
    food_sound.loadFromFile("../files/food.wav");
    fsound.setBuffer(food_sound);

    sf::Music music;
    music.openFromFile("../files/music.ogg");
    music.setLoop(true);
    music.play();
    //Sounds
    //Text
    Text end, score;
    end.setString("Game Over");
    score.setPosition(10, 10);
    score.setFillColor(Color::White);
    score.setCharacterSize(25);
    Font font;
    font.loadFromFile("../files/JapariSans.otf");
    score.setFont(font);
    end.setPosition(200, 135);
    end.setFont(font);
    end.setCharacterSize(75);
    end.setFillColor(Color::Red);
    //Text
    Keyboard::Key pressed;
    vector <Block> food = spawn(5);
    while(window.isOpen()&&!killed) {
        points = snake.body.size()-4;
        score.setString("Score: "+to_string(points));
        Event event{};
        Tick = (float)clock.getElapsedTime().asSeconds() - LastFrame;
        LastFrame = (float)clock.getElapsedTime().asSeconds();
        snake.move(pressed, 0.1);
        cout<<Tick<<endl;
        for (int i = 0; i < food.size(); ++i) {
            if(touch(snake.body[0], food[i])){
                snake.increase();
                food.erase(food.begin()+i);
                fsound.play();
            }
        }
        if(food.size()==0){
            food = spawn(5);
        }
        while(window.pollEvent(event)) {
            /*if(Keyboard::isKeyPressed(sf::Keyboard::Space)){
                snake.increase();
            }*/
            if(event.type == Event::Closed){
                window.close();
            }
            if(Keyboard::isKeyPressed(Keyboard::W)){
                pressed = Keyboard::W;
            }
            if(Keyboard::isKeyPressed(Keyboard::A)){
                pressed = Keyboard::A;
            }
            if(Keyboard::isKeyPressed(Keyboard::D)){
                pressed = Keyboard::D;
            }
            if(Keyboard::isKeyPressed(Keyboard::S)){
                pressed = Keyboard::S;
            }
        }
        window.clear();
        for (int i = 0; i < snake.body.size(); ++i) {
            window.draw(snake.body[i].self);
        }
        for (int i = 0; i < food.size(); ++i) {
            window.draw(food[i].self);
        }
        if(suicide){
            for (int i = 1; i < snake.body.size(); ++i) {
                if(touch(snake.body[0], snake.body[i])){
                    killed = true;
                    window.draw(end);
                }
            }
        }
        if(walls){
            if((snake.body[0].GetPos().x < 0 || snake.body[0].GetPos().x > 680)||(snake.body[0].GetPos().y < 0 || snake.body[0].GetPos().y > 380)){
                window.draw(end);
                killed = true;
            }
        }
        window.draw(score);
        window.display();
        if(killed){
            music.stop();
            esound.play();
            Thread a(&sleep, seconds(3));
            a.launch();
        }
    }
    return 0;
}
