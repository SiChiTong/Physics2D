#ifndef CIRCLE_POINT_COLLISION_H
#define CIRCLE_POINT_COLLISION_H

#include "Geometry/point.h"
#include "Geometry/circle.h"
#include "Collisions/collision.h"
#include "Dynamics/body.h"

class CirclePointCollision : public Collision
{
public:
  CirclePointCollision(Body& b1, Body& b2) : b1_(b1), b2_(b2) {
    c_ = dynamic_cast<Circle*>(b1.shape());
    p_ = dynamic_cast<Point*>(b2.shape());
  }

  virtual bool detect() const {
    return (p_->origin() - c_->origin()).length() <= c_->radius();
  }

  virtual void apply() const {
    float m1 = b1_.mass();
    float m2 = b2_.mass();

    Vec normal = p_->getNormal(c_->origin());
    if (normal.dot(b2_.velocity() - b1_.velocity()) <= 0.0f)
      return;
//    Vec tangent = normal.perpendicular();

    float v1_n = b1_.velocity().dot(normal);
//    float v1_t = b1_.velocity().dot(tangent);

    float v2_n = b2_.velocity().dot(normal);
//    float v2_t = b2_.velocity().dot(tangent);

    float new_v1_n = (v1_n * (m1 - m2) + 2.0f * m2 * v2_n) / (m1 + m2);
    float new_v2_n = (v2_n * (m2 - m1) + 2.0f * m1 * v1_n) / (m1 + m2);

    b1_.applyImpulse((new_v1_n - v1_n) * normal * m1);
    b2_.applyImpulse((new_v2_n - v2_n) * normal * m2);
  }

private:
  Body& b1_;
  Body& b2_;

  Circle* c_;
  Point* p_;
};

#endif // CIRCLE_POINT_COLLISION_H
