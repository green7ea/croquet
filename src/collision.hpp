#ifndef COLLISION_H
#define COLLISION_H

class Vector;
class Ball;

void BallObstacle(const Vector &pos,
                  Vector &vel,
                  const Vector &col_normal,
                  float coef);

void BallBall(Ball &ball1, Ball &ball2);

#endif // COLLISION_H
