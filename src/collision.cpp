#include "collision.hpp"

#include "audio.hpp"
#include "ball.hpp"
#include "game.hpp"
#include "vector.hpp"

void BallObstacle(const Vector &pos,
                  Vector &vel,
                  const Vector &col_normal,
                  float coef)
{
    const Vector orig_vel = vel;
    const Vector proj_vn = vel.dot(col_normal) * col_normal;
    vel += -2.f * proj_vn;
    vel *= coef;

    char log[2048] = "";
    snprintf(log, 2048,
             "Collision de la balle au point (%f, %f, %f) avec "
             "une vitesse initiale de (%f, %f, %f).\r\n"
             "La vitesse finale est de (%f, %f, %f).\r\n",
             pos.x, pos.y, pos.z, orig_vel.x, orig_vel.y, orig_vel.z, vel.x,
             vel.x, vel.z);
    Game &game = Game::getGame();
    game.log(log);
}

void BallBall(Ball &ball1, Ball &ball2)
{
    // See http://www.plasmaphysics.org.uk/programs/coll2d_cpp.htm for
    // an explanation of the optimisations.
    static const float coeff = 0.87f;
    const Vector ball1_pos = ball1.getRelPos();
    const Vector ball2_pos = ball2.getRelPos();

    // Balls are moving appart
    const Vector posDiff = ball2_pos - ball1_pos;
    const Vector velDiff = ball2.velocity - ball1.velocity;

    if (velDiff.x * posDiff.x + velDiff.y * posDiff.y >= 0.f) return;

    const Vector orig_vel1 = ball1.velocity;
    const Vector orig_vel2 = ball2.velocity;

    const Vector correction = (ball1.velocity + ball2.velocity) / 2.f;
    const float a = posDiff.y / posDiff.x;
    const float dvx2 = -2.f * (velDiff.x + a * velDiff.y) / ((1 + a * a) * 2.f);
    ball1.velocity -= Vector(dvx2, a * dvx2);
    ball2.velocity += Vector(dvx2, a * dvx2);

    ball1.velocity = (ball1.velocity - correction) * coeff + correction;
    ball2.velocity = (ball2.velocity - correction) * coeff + correction;

    // Player gets to play again
    Game &game = Game::getGame();
    game.signalExtraTurn();

    // Play sound
    Audio &audio = Audio::getAudio();
    audio.playSound(5, 200.f,
                    getAmplitude((ball1_pos + ball2_pos) / 2.f, velDiff));

    char log[2048] = "";
    snprintf(log, 2048,
             "Collision balle balle :\r\n"
             "\t%s\r\n"
             "\t\tpos = (%f, %f, %f)\r\n"
             "\t\tvel initiale = (%f, %f, %f)\r\n"
             "\t\tvel finale = (%f, %f, %f)\r\n"
             "\t%s\r\n"
             "\t\tpos = (%f, %f, %f)\r\n"
             "\t\tvel initiale = (%f, %f, %f)\r\n"
             "\t\tvel finale = (%f, %f, %f)\r\n",
             ball1.isBlue() ? "Bleu" : "Jaune", ball1_pos.x, ball1_pos.y,
             ball1_pos.z, orig_vel1.x, orig_vel1.y, orig_vel1.z,
             ball1.velocity.x, ball1.velocity.y, ball1.velocity.z,
             ball2.isBlue() ? "Bleu" : "Jaune", ball2_pos.x, ball2_pos.y,
             ball2_pos.z, orig_vel2.x, orig_vel2.y, orig_vel2.z,
             ball2.velocity.x, ball2.velocity.y, ball2.velocity.z);
    game.log(log);
}
