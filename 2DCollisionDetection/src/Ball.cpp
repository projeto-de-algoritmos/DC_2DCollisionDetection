#include "Ball.hpp"
#include "Assets.hpp"
#include "SolidImage.hpp"

void Ball::makePerfectContactBetweenBalls(Ball * ball1, Ball * ball2)
{
    // double_t x1 = ball1->getXCoordinate();
    // double_t x2 = ball2->getXCoordinate();

    // double_t y1 = ball1->getYCoordinate();
    // double_t y2 = ball2->getYCoordinate();

    // double_t vx1 = ball1->getXVelocity();
    // double_t vx2 = ball2->getXVelocity();

    // double_t vy1 = ball1->getYVelocity();
    // double_t vy2 = ball2->getYVelocity();

    // double_t r = ball1->getRadius() + ball2->getRadius();

    // double_t a = (vx1 * vx1) - (2 * vx1 * vx2) + (vx2 * vx2) + (vy1 * vy1) - (2 * vy1 * vy2) + (vy2 * vy2);
    // double_t b = 2 * (x1 * vx1 - x1 * vx2 - x2 * vx1 + x2 * vx2 + y1 * vy1 - y1 * vy2 - y2 * vy1 + y2 * vy2);
    // double_t c = (x1*x1) - (2 * x1 * x2) + (x2 * x2) + (y1 * y1) - (2 * y1 * y2) + (y2 * y2) - (r*r);

    // double_t delta = sqrt(b*b - (4 * a * c));
    // double_t rollback_delta_time = ((-b) - delta) / (2*a);
    // ball1->setCenter(ball1->getCenter() + ball1->getVelocity() * rollback_delta_time);
    // ball2->setCenter(ball2->getCenter() + ball2->getVelocity() * rollback_delta_time);

    Vector2D cur_dist(ball1->getCenter(), ball2->getCenter());
    Vector2D tru_dist(cur_dist * ((ball1->getRadius() + ball2->getRadius()) / cur_dist.magnitude()));
    Vector2D del_dist((tru_dist - cur_dist) * 0.58);
    ball1->setCenter(ball1->getCenter() - del_dist);
    ball2->setCenter(ball2->getCenter() + del_dist);
}

bool Ball::ballsAreColliding(const Ball * ball1, const Ball * ball2)
{
    double_t x1 = ball1->getXCoordinate();
    double_t y1 = ball1->getYCoordinate();

    double_t x2 = ball2->getXCoordinate();
    double_t y2 = ball2->getYCoordinate();

    int32_t distance = (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);

    return distance <= (ball1->getRadius() + ball2->getRadius()) * (ball1->getRadius() + ball2->getRadius());
}

void Ball::collideBalls(Ball * ball1, Ball * ball2)
{
    if (!ballsAreColliding(ball1, ball2))
        return;

    makePerfectContactBetweenBalls(ball1, ball2);
    
    Vector2D b1c = ball1->getCenter();
    Vector2D b2c = ball2->getCenter();

    Vector2D un(b2c.x() - b1c.x(), b2c.y() - b1c.y());
    un = un * (1/un.magnitude());
    Vector2D ut(-un.y(), un.x());

    double_t v1n = un.innerProduct(ball1->getVelocity());
    double_t v1t = ut.innerProduct(ball1->getVelocity());
    double_t v2n = un.innerProduct(ball2->getVelocity());
    double_t v2t = ut.innerProduct(ball2->getVelocity());

    double_t v1n_ = v2n;
    double_t v2n_ = v1n;
    double_t v1t_ = v1t;
    double_t v2t_ = v2t;

    Vector2D vel1n(un * v1n_);
    Vector2D vel1t(ut * v1t_);
    Vector2D vel2n(un * v2n_);
    Vector2D vel2t(ut * v2t_);
    ball1->setVelocity(vel1n + vel1t);
    ball2->setVelocity(vel2n + vel2t);
}

double_t Ball::getXCoordinate() const noexcept
{
    return _center.x();
}

double_t Ball::getYCoordinate() const noexcept
{
    return _center.y();
}

double_t Ball::getXVelocity() const noexcept
{
    return _velocity.x();
}

double_t Ball::getYVelocity() const noexcept
{
    return _velocity.y();
}

double_t Ball::getRadius() const noexcept
{
    return _ball_radius;
}

Vector2D Ball::getCenter() const noexcept
{
    return _center;
}

void Ball::setCenter(const Vector2D & center) noexcept
{
    _center = center;
}

Vector2D Ball::getVelocity() const noexcept
{
    return _velocity;
}

void Ball::setVelocity(const Vector2D & velocity) noexcept
{
    _velocity = velocity;
}

void Ball::hide()
{
    _ball_highlight->hide();
    _ball_texture->hide();
}

void Ball::show()
{
    _ball_texture->show();
}

void Ball::turnOnBallHighlight()
{
    _ball_highlight->show();
}

void Ball::turnOffBallHighlight()
{
    _ball_highlight->hide();
}

void Ball::updateBallPosition(double_t frame_time)
{
    _center.x(_center.x() + _velocity.x() * frame_time);
    _center.y(_center.y() + _velocity.y() * frame_time);

    if (_center.x() - _ball_radius < 0.0) {
        _center.x(_ball_radius);
        _velocity.x(-_velocity.x());
    }
    else if (_center.x() + _ball_radius > double_t(Assets::TABLE_WIDTH)) {
        _center.x(double_t(Assets::TABLE_WIDTH) - _ball_radius);
        _velocity.x(-_velocity.x());
    }

    if (_center.y() - _ball_radius < 0.0) {
        _center.y(_ball_radius);
        _velocity.y(-_velocity.y());
    }
    else if (_center.y() + _ball_radius > double_t(Assets::TABLE_HEIGHT)) {
        _center.y(double_t(Assets::TABLE_HEIGHT) - _ball_radius);
        _velocity.y(-_velocity.y());
    }

    _ball_texture->setRelativeX(_center.x() - _ball_radius);
    _ball_texture->setGlobalY(_center.y() - _ball_radius);
}

void Ball::updateVelocity(double_t frame_time, double_t friction)
{
    double_t neg_mag = frame_time * friction;
    if (neg_mag >= _velocity.magnitude()) {
        _velocity.x(0);
        _velocity.y(0);
    }
    else {
        _velocity = _velocity + _velocity * (-neg_mag / _velocity.magnitude());
    }
}

Ball * Ball::newBall(double_t x_coordinate,
                     double_t y_coordinate,
                     double_t x_velocity,
                     double_t y_velocity,
                     double_t ball_radius,
                     std::string ball_texture_name,
                     std::string ball_highlight_name)
{
    Ball * ball = new Ball();
    ball->_velocity = Vector2D(x_velocity, y_velocity);
    ball->_center = Vector2D(x_coordinate, y_coordinate);
    ball->_ball_radius = ball_radius;

    ball->_ball_texture = SolidImage::newSolidImage(ball_texture_name, 2*ball_radius, 2*ball_radius);
    ball->_ball_texture->setRelativeX(0);
    ball->_ball_texture->setRelativeY(0);

    ball->_ball_highlight = SolidImage::newSolidImage(ball_highlight_name, 2*ball_radius, 2*ball_radius);
    ball->_ball_highlight->setParent(ball->_ball_texture);
    ball->_ball_highlight->hide();
    ball->_ball_highlight->setRelativeX(0);
    ball->_ball_highlight->setRelativeY(0);

    return ball;
}

Ball::Ball()
{

}

Ball::~Ball()
{

}
