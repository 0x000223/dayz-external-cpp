//
// Created by Max on 06/05/2022.
//

#ifndef DAYZ_EXTERNAL_CPP_MATH_H
#define DAYZ_EXTERNAL_CPP_MATH_H

#include <cmath>

/**
 * @brief
 */
struct vector2
{
    float x, y;

    static vector2 zero;

    bool operator !() const {
        return !this->x && !this->y;
    }

    vector2 operator +(vector2 other) const {
        return vector2 {
                .x = this->x + other.x,
                .y = this->y + other.y
        };
    }

    vector2 operator*(float scalar) const {
        return vector2 {
                .x = this->x * scalar,
                .y = this->y * scalar
        };
    }

    vector2 operator*(const vector2 other) const {
        return vector2 {
            .x = this->x * other.x,
            .y = this->y * other.y
        };
    }

    bool operator ==(const vector2 other) const {
        if (this->x == other.x
            && this->y == other.y) {
            return true;
        }
        return false;
    }

    float distance(const vector2 other) const {
        auto const a = this->x - other.x;
        auto const b = this->y - other.y;

        return sqrtf(a * a + b * b);
    }
};

/**
 * @brief
 */
struct vector3
{
    float x, y, z;

    static vector3 zero;

    float dot(vector3 other) const {
        return this->x * other.x + this->y * other.y + this->z * other.z;
    }

    vector3 operator -(const vector3 other) const {
        return vector3 {
                .x = this->x - other.x,
                .y = this->y - other.y,
                .z = this->z - other.z,
        };
    }

    vector3 operator *(float other) const {
        return vector3 {
            .x = this->x * other,
            .y = this->y * other,
            .z = this->z * other
        };
    }

    vector3 operator*(const vector3 other) const {
        return vector3 {
            .x = this->x * other.x,
            .y = this->y * other.y,
            .z = this->z * other.z
        };
    }

    bool operator ==(const vector3 other) const {
        if (this->x == other.x
            && this->y == other.y
            && this->z == other.z) {
            return true;
        }
        return false;
    }

    float distance(const vector3 other) const {
        auto const a = this->x - other.x;
        auto const b = this->y - other.y;
        auto const c = this->z - other.z;

        return sqrtf(a * a + b * b + c * c);
    }

    float length() const {
        return sqrtf(powf(this->x, 2.f) + powf(this->y, 2.f) + powf(this->z, 2.f));
    }

    static float distance(vector3& first, vector3& second) {
        auto const a = first.x - second.x;
        auto const b = first.y - second.y;
        auto const c = first.z - second.z;

        return sqrtf(a * a + b * b + c * c);
    }
};

struct matrix44 {
    float m[12];
};

#endif //DAYZ_EXTERNAL_CPP_MATH_H
