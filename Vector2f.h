#pragma once
#include <math.h>
#include <cassert>

struct Vector2f
{
    float x;
    float y;

    Vector2f(float x, float y) 
    {
        this->x = x;
        this->y = y;
    };

    Vector2f(int x, int y)
    {
        this->x = x;
        this->y = y;
    };

    Vector2f operator + (Vector2f v)
    {
        return Vector2f(x + v.x, y + v.y);
    }
    Vector2f& operator += (Vector2f v)
    {
        x += v.x;
        y += v.y;
        return *this;
    }

    Vector2f operator - (Vector2f v)
    {
        return Vector2f(x - v.x, y - v.y);
    }
    Vector2f& operator -= (Vector2f v)
    {
        x -= v.x;
        y -= v.y;
        return *this;
    }

    Vector2f operator * (float v)
    {
        return Vector2f(x * v, y * v);
    }
    Vector2f& operator *= (float v)
    {
        x *= v;
        y *= v;
        return *this;
    }

    Vector2f operator / (float v)
    {
        return Vector2f(x / v, y / v);
    }
    Vector2f& operator /= (float v)
    {
        x /= v;
        y /= v;
        return *this;
    }

    float operator * (Vector2f v)
    {
        return x * v.x + y * v.y;
    }

    float mod()
    {
        return pow(x * x + y * y, 0.5);
    }

    float projection( Vector2f axis)
    {
        float scalarComposition = *this * axis;
        float moduleAxis = axis.mod();

        assert(moduleAxis != 0);

        return scalarComposition / moduleAxis;
    }

    Vector2f norm()
    {
        return (*this) / this->mod();
    }
};

Vector2f operator * (float f, Vector2f v)
{
    return Vector2f(f * v.x, f * v.y);
}