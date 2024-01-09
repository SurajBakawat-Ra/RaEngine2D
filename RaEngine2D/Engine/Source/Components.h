#pragma once
#include "Animation.h"
#include "Rigidbody.h"
#include "Assets.h"

class Components
{
public:
    bool has = false;
};


class CTransform : public Components
{
private:
    Vec2 pos = { 0.0, 0.0 };
    Vec2 prevPos = { 0.0, 0.0 };

public:
    void SetPosition(const Vec2& p)
    {
        prevPos = pos;
        pos = p;
    }
    const Vec2& getPos() const { return pos; }
    const Vec2& getPrevPos() const { return prevPos; }
    Vec2 scale = { 1.0, 1.0 };
    Vec2 velocity = { 0.0, 0.0 };
    float angle = 0;
    float aimPos = 0;

    CTransform() { SetPosition(getPos()); }
    explicit CTransform(const Vec2& p) : pos(p) { SetPosition(getPos()); }
    CTransform(const Vec2& p, const Vec2& sp, const Vec2& sc, float a) : pos(p), prevPos(p), velocity(sp), scale(sc), angle(a) { SetPosition(getPos()); }
};


class CBoundingBox : public Components
{
public:
    Vec2 size = { 0.0, 0.0 };

    CBoundingBox() = default;
    explicit CBoundingBox(const Vec2& s) : size(s) {}
};

class CLifeSpan : public Components
{
public:
    sf::Clock clock{};
    float lifespan = 0;

    explicit CLifeSpan(int l) : lifespan(l) { clock.restart(); }
};

class CInput : public Components
{
public:
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;
    bool shoot = false;
    bool canShoot = true;
    bool canJump = true;
    CInput() = default;
};


class CAnimation : public Components
{
public:
    CAnimation() = default;
    Animation animation;
    bool repeat;

    CAnimation(const Animation& animation, bool r) : animation(animation), repeat(r) {}
};

class CState : public Components
{
public:
    CState() = default;
    std::string state;

    size_t frames = 0;
    bool notGrounded = true;
    explicit CState(std::string& s) : state(s) {}
};

class CRigidbody : public Components
{
public:
    CRigidbody(const Rigidbody& r, float g) : rigidbody(r), gravity(g) {}
    Rigidbody rigidbody;
    float gravity;
    float mass;
    bool isKinematic;
    bool freezeRotation;
    bool useGravity;
    float maxVelocity = 10.f;
};
