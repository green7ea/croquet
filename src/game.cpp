#include "game.hpp"

#include "arrow.hpp"
#include "audio.hpp"
#include "ball.hpp"
#include "mallet.hpp"
#include "nodecamera.hpp"
#include "nodelookingcamera.hpp"
#include "peg.hpp"
#include "ring.hpp"
#include "surface.hpp"
#include "wall.hpp"

#include <SDL.h>

Game *Game::self = NULL;

Game &Game::getGame()
{
    if (!self)
    {
        self = new Game();
    }
    return *self;
}

void Game::destroyGame()
{
    if (self)
    {
        delete self;
        self = NULL;
    }
}

static void setBallVelocity(const Vector &mallet_velocity, Ball &ball)
{
    const float coeff_restitution = 0.3f;

    // Play sound
    Audio &audio = Audio::getAudio();
    audio.playSound(1, 200.f, getAmplitude(ball.velocity, mallet_velocity));

    ball.velocity
        = (mallet_velocity * Mallet::mass * coeff_restitution) / Ball::mass;

    Game &game = Game::getGame();
    char log[1024] = "";
    snprintf(log, 1024,
             "La balle %s a ete frappe et a une vitesse initiale de (%f, %f, "
             "%f)m/s.\r\n",
             ball.isBlue() ? "bleu" : "jaune", ball.velocity.x, ball.velocity.y,
             ball.velocity.z);
    game.log(log);
}

void Game::update()
{
    const unsigned int time = SDL_GetTicks();
    const float new_delta = (float)(time - deltat) * 0.001f;

    Ball *ball = isBluesTurn ? dynamic_cast<Ball *>(blueBall.get())
                             : dynamic_cast<Ball *>(redBall.get());

    scenegraph.collision(*dynamic_cast<Ball *>(blueBall.get()), new_delta);
    scenegraph.collision(*dynamic_cast<Ball *>(redBall.get()), new_delta);
    scenegraph.update(new_delta);
    deltat = time;

    (*current)(*ball, new_delta);
}

void Game::draw(SDL_Surface *screen)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, 1, 0.1f, 1000.f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    cam->draw();
    scenegraph.draw();
}

void Game::keypress(int key)
{
    switch (key)
    {
    case 'q':
        cam->keyMove(Vector(0.f, 0.f, 1.f));
        break;
    case 'e':
        cam->keyMove(Vector(0.f, 0.f, -1.f));
        break;
    case 'w':
        cam->keyMove(Vector(1.f, 0.f, 0.f));
        break;
    case 'a':
        cam->keyMove(Vector(0.f, 1.f, 0.f));
        break;
    case 's':
        cam->keyMove(Vector(-1.f, 0.f, 0.f));
        break;
    case 'd':
        cam->keyMove(Vector(0.f, -1.f, 0.f));
        break;
    case 'o':
        cam = std::unique_ptr<Camera>(new Camera(*cam));
    default:
        break;
    }
}

void Game::mouseDown(int button, int x, int y)
{
    switch (button)
    {
    case 1:
        if (current == &prehit)
        {
            current = &hitting;
        }
        hit_time = SDL_GetTicks();
        break;
    case 3:
        SDL_ShowCursor(false);
        isMouseDown = true;
        mouseX = x;
        mouseY = y;
        break;
    }
}

void Game::mouseUp(int button, int x, int y)
{
    switch (button)
    {
    case 1:
        if (current == &hitting)
        {
            current = &hit;
        }
        break;
    case 3:
        SDL_ShowCursor(true);
        isMouseDown = false;
        break;
    default:
        break;
    }
}

void Game::mouseMove(int x, int y, int xrel, int yrel)
{
    if (isMouseDown)
    {
        if (mouseX != x || mouseY != y)
        {
            SDL_WarpMouse(mouseX, mouseY);
            cam->mouseMove(-xrel, -yrel);
        }
    }
}

void Game::signalExtraTurn()
{
    extraTurn = true;
}

void Game::setTurn()
{
    scenegraph.remove(arrow);
    scenegraph.remove(mallet_offset);
    // scenegraph.remove( mallet );

    Arrow *arrow_ptr = dynamic_cast<Arrow *>(arrow.get());
    arrow_ptr->setBlue(isBluesTurn);

    auto &ring = isBluesTurn ? blueRings.front() : redRings.front();
    ring->addChild(arrow);

    auto &ball = isBluesTurn ? blueBall : redBall;

    const Vector diff = ring->getAbsPos() - ball->getAbsPos();

    // We add an offset to the theta or it's too easy to get it in the target as
    // everything is already aligned.
    const float theta = atan2(-diff.y, -diff.x) - 0.1;

    cam = std::unique_ptr<Camera>(
        new NodeCamera(ball, 1.f, 2.f, theta, Vector(0.f, 0.f, 0.5f)));

    ball->setOrientation(Quaternion());
    ball->addChild(mallet_offset);
    ball->setVisible(true);
}

void Game::signalGoal(const Ball &ball, const Node *ring)
{
    const bool blue = ball.isBlue();

    char log[1024] = "";
    snprintf(log, 1024, "Cerceau pour %s!\r\n", blue ? "bleu" : "jaune");
    this->log(log);

    std::list<std::shared_ptr<Node>> *rings = blue ? &blueRings : &redRings;

    if (ring == rings->front().get())
    {
        scenegraph.remove(arrow);
        rings->pop_front();

        if (rings->empty())
        {
            running = false;
            snprintf(log, 1024, "%s emporte la partie!\r\n",
                     blue ? "Bleu" : "Jaune");
            Game &game = Game::getGame();
            game.log(log);
        }
        else
        {
            if (isBluesTurn)
            {
                if (ball.isBlue())
                {
                    signalExtraTurn();
                }
                blueRings.front()->addChild(arrow);
            }
            else
            {
                if (!ball.isBlue())
                {
                    signalExtraTurn();
                }
                redRings.front()->addChild(arrow);
            }
        }
    }
}

void Game::log(const std::string &text)
{
    if (log_file)
    {
        fprintf(log_file, "%s", text.c_str());
    }
    else
    {
        fprintf(stderr, "%s", text.c_str());
    }
}

Game::Game()
    : running(true)
    , scenegraph(NULL)
    , cam(new Camera())
    , model()
    , view()
    , isMouseDown(false)
    , isBluesTurn(true)
    , extraTurn(false)
    , malletSpeed(0.f)
{
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glClearColor(0.f, 0.f, 0.f, 0.f);

    Vector dir(cos(60.f * 3.1415f / 180.f) * sin(45.f * 3.1415f / 180.f),
               -sin(60.f * 3.1415f / 180.f) * sin(45.f * 3.1415f / 180.f),
               cos(45.f * 3.1415f / 180.f));
    dir /= dir.length();

    const float proj[]
        = {1.f,           0.f,           0.f, 0.f, 0.f, 1.f, 0.f,   0.f,
           dir.x / dir.z, dir.y / dir.z, 0.f, 0.f, 0.f, 0.f, 0.01f, 1.f};
    groundProjection = Matrix(proj);

    Node *surface = new Surface(&scenegraph);
    scenegraph.addChild(std::shared_ptr<Node>(surface));

    blueBall = std::shared_ptr<Node>(new Ball(surface, true));
    blueBall->setRelPos(Vector(-4.f, -7.f, Ball::radius));
    surface->addChild(blueBall);
    blueBall->setVisible(false);

    redBall = std::shared_ptr<Node>(new Ball(surface, false));
    redBall->setRelPos(Vector(-4.f, -7.f, Ball::radius));
    surface->addChild(redBall);
    redBall->setVisible(false);

    // 1
    std::shared_ptr<Node> ring(new Ring(surface));
    ring->setRelPos(Vector(-3.5f, -5.25f, 0.f));
    surface->addChild(ring);
    blueRings.push_back(ring);
    redRings.push_back(ring);

    // 2
    ring = std::shared_ptr<Node>(new Ring(surface));
    ring->setRelPos(Vector(-3.5f, 5.25f, 0.f));
    surface->addChild(ring);
    blueRings.push_back(ring);
    redRings.push_back(ring);

    // 3
    ring = std::shared_ptr<Node>(new Ring(surface));
    ring->setRelPos(Vector(3.5f, 5.25f, 0.f));
    surface->addChild(ring);
    blueRings.push_back(ring);
    redRings.push_back(ring);

    // 4
    ring = std::shared_ptr<Node>(new Ring(surface));
    ring->setRelPos(Vector(3.5f, -5.25f, 0.f));
    surface->addChild(ring);
    blueRings.push_back(ring);
    redRings.push_back(ring);

    // 5
    ring = std::shared_ptr<Node>(new Ring(surface));
    ring->setRelPos(Vector(0.f, -3.5f, 0.f));
    surface->addChild(ring);
    blueRings.push_back(ring);
    redRings.push_back(ring);

    // 6
    ring = std::shared_ptr<Node>(new Ring(surface));
    ring->setRelPos(Vector(0.f, 3.5f, 0.f));
    surface->addChild(ring);
    blueRings.push_back(ring);
    redRings.push_back(ring);

    const float offset = 0.1f; // in metres
    // Walls
    const float w = Surface::width * 0.5f;
    const float h = Surface::height * 0.5f;
    surface->addChild(std::shared_ptr<Node>(new Wall(
        surface, Vector(-w, -h + offset, 0.f), Vector(-w, h - offset, 0.f))));
    surface->addChild(std::shared_ptr<Node>(new Wall(
        surface, Vector(w - offset, -h, 0.f), Vector(-w + offset, -h, 0.f))));
    surface->addChild(std::shared_ptr<Node>(new Wall(
        surface, Vector(w, h - offset, 0.f), Vector(w, -h + offset, 0.f))));
    surface->addChild(std::shared_ptr<Node>(new Wall(
        surface, Vector(-w + offset, h, 0.f), Vector(w - offset, h, 0.f))));
    // Diagonal Walls
    surface->addChild(std::shared_ptr<Node>(new Wall(
        surface, Vector(-w + offset, -h, 0.f), Vector(-w, -h + offset, 0.f))));

    surface->addChild(std::shared_ptr<Node>(new Wall(
        surface, Vector(w, -h + offset, 0.f), Vector(w - offset, -h, 0.f))));

    surface->addChild(std::shared_ptr<Node>(new Wall(
        surface, Vector(w - offset, h, 0.f), Vector(w, h - offset, 0.f))));

    surface->addChild(std::shared_ptr<Node>(new Wall(
        surface, Vector(-w, h - offset, 0.f), Vector(-w + offset, h, 0.f))));

    // Rest
    surface->addChild(std::shared_ptr<Node>(new Peg(surface)));

    arrow = std::shared_ptr<Node>(new Arrow(surface));
    arrow->setRelPos(Vector(0.f, 0.f, Ring::height));

    mallet_offset = std::shared_ptr<Node>(new Node(surface));
    mallet = std::shared_ptr<Node>(new Mallet(surface));
    mallet->setRelPos(
        Vector(Mallet::cylinder_length / 2.f + Ball::radius, 0.f, 0.f));
    mallet_offset->addChild(mallet);

    setTurn();
    initLambdas();
    log_file = fopen("partie.txt", "w+");
    deltat = SDL_GetTicks();
}

Game::~Game()
{
    if (log_file)
    {
        fclose(log_file);
    }
}

void Game::initLambdas()
{
    prehit = [&](Ball &ball, float delta_t) {
        Vector dir = -1.f * this->cam->dir;
        dir.z = 0.f;
        dir /= dir.length();
        const Vector up(0.f, 0.f, 1.f);
        const Vector left = up.cross(dir);

        const Matrix mat(dir, left, up);

        mallet_offset->setOrientation(Quaternion(mat));
    };

    hitting = [&](Ball &ball, float delta_t) {
        const unsigned int delay = SDL_GetTicks() - hit_time;

        if (delay < 2000)
        {
            Vector pos = mallet->getRelPos();
            pos += Vector(0.2f * delta_t, 0.f, 0.f);
            mallet->setRelPos(pos);
        }
        malletSpeed = 0.f;
    };

    hit = [&](Ball &ball, float delta_t) {
        const float start = Mallet::cylinder_length / 2.f + Ball::radius;
        Vector pos = mallet->getRelPos();
        if (pos.x > start)
        {
            malletSpeed += 10.f * delta_t;
            pos -= malletSpeed * Vector(1.f * delta_t, 0.f, 0.f);
            mallet->setRelPos(pos);
            if (pos.x <= start)
            {
                pos.x = start;

                Vector dir_normal = cam->dir;
                dir_normal.z = 0.f;
                dir_normal /= dir_normal.length();

                setBallVelocity(malletSpeed * dir_normal, ball);
                extraTurn = false;
                current = &watch;

                cam = std::unique_ptr<Camera>(
                    new NodeLookingCamera(&ball, *cam));

                scenegraph.remove(mallet_offset);
                // TODO fix
                // const Vector abs_pos = mallet->getAbsPos();
                // scenegraph.addChild( mallet );
                // mallet->setAbsPos( abs_pos );
            }
        }
    };

    watch = [&](Ball &ball, float delta_t) {
        Ball *blue = dynamic_cast<Ball *>(blueBall.get());
        Ball *red = dynamic_cast<Ball *>(redBall.get());

        if (blue->velocity.length() < 0.001f && red->velocity.length() < 0.001f)
        {
            char log[1024];

            blue->velocity = 0.f;
            red->velocity = 0.f;
            if (!extraTurn)
            {
                isBluesTurn = !isBluesTurn;
            }
            else
            {
                char log[1024];
                snprintf(log, 1024, "%s a un tour de plus.\r\n",
                         isBluesTurn ? "Bleu" : "Jaune");
                this->log(log);
            }

            const Vector posBlue = blueBall->getRelPos();
            const Vector posRed = redBall->getRelPos();
            snprintf(log, 1024,
                     "Position finale de la balle bleu (%f, %f, %f)\r\n"
                     "Position finale de la balle jaune (%f, %f, %f)\r\n",
                     posBlue.x, posBlue.y, posBlue.z, posRed.x, posRed.y,
                     posRed.z);
            this->log(log);
            setTurn();
            current = &prehit;
        }
    };

    current = &prehit;
}
