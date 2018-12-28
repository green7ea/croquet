#ifndef GAME_H
#define GAME_H

#include "arrow.hpp"
#include "camera.hpp"
#include "matrix.hpp"
#include "node.hpp"
#include "ring.hpp"

#include <SDL.h>
#include <functional>
#include <list>

class Game
{
public:
    static Game &getGame();
    static void destroyGame();

    void init();
    void update();
    void draw(SDL_Surface *screen);
    void keypress(int key);
    void mouseDown(int button, int x, int y);
    void mouseUp(int button, int x, int y);
    void mouseMove(int x, int y, int xrel, int yrel);
    void signalExtraTurn();
    void setTurn();
    void signalGoal(const Ball &ball, const Node *ring);
    void log(const std::string &text);

    Matrix groundProjection;

    bool running;

private:
    Game();
    ~Game();
    void initLambdas();

    static Game *self;

    Node scenegraph;
    Camera *cam;
    Matrix model;
    Matrix view;

    std::function<void(Ball &, float)> prehit;
    std::function<void(Ball &, float)> hitting;
    std::function<void(Ball &, float)> hit;
    std::function<void(Ball &, float)> watch;
    std::function<void(Ball &, float)> *current;

    bool isMouseDown;
    int mouseX;
    int mouseY;

    Vector camMove;

    unsigned int deltat;
    unsigned int hit_time;

    bool isBluesTurn;
    bool extraTurn;
    float malletSpeed;
    std::shared_ptr<Node> blueBall;
    std::shared_ptr<Node> redBall;

    std::list<std::shared_ptr<Node>> blueRings;
    std::list<std::shared_ptr<Node>> redRings;

    std::shared_ptr<Node> arrow;
    std::shared_ptr<Node> mallet;
    std::shared_ptr<Node> mallet_offset;

    FILE *log_file;
};

#endif // GAME_H
