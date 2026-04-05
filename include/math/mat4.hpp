#pragma once

#include "math/vec3.hpp"
#include "math/vec4.hpp"
#include "math/math.hpp"
#include "utility.hpp"

struct Mat4 {
    public:
        constexpr Mat4& operator*=(const f32 s)   noexcept;
        constexpr Mat4& operator*=(const Mat4& m) noexcept;

        constexpr Mat4 operator*(const f32 s)   const noexcept;
        constexpr Mat4 operator*(const Mat4& m) const noexcept;
        constexpr Vec3 operator*(const Vec3& v) const noexcept;

        constexpr Mat4& transpose()  noexcept;
        constexpr Mat4  transposed() const noexcept;

        static constexpr Mat4 identity() noexcept;

        static constexpr Mat4 translate(const Vec3& p)                noexcept;
        static inline    Mat4 rotate(const Vec3& axis, const f32 rad) noexcept;
        static constexpr Mat4 scale(const Vec3& s)                    noexcept;

    public:
        union {
            f32 data[16]{};

            Vec4 rows[4];
        };
};

constexpr Mat4& Mat4::operator*=(const f32 s) noexcept {
    rows[0] *= s;
    rows[1] *= s;
    rows[2] *= s;
    rows[3] *= s;

    return *this;
}
constexpr Mat4& Mat4::operator*=(const Mat4& m) noexcept {
    *this = *this * m;

    return *this;
}

constexpr Mat4 Mat4::operator*(const f32 s) const noexcept {
    return { .rows = { rows[0] * s, rows[1] * s, rows[2] * s, rows[3] * s, } };
}
constexpr Mat4 Mat4::operator*(const Mat4& m) const noexcept {
    Mat4 result;

    for (i32 row = 0; row < 4; ++row) {
        for (i32 col = 0; col < 4; ++col) {
            f32 sum{};

            for (i32 i = 0; i < 4; ++i)
                sum += data[row * 4 + i] * m.data[i * 4 + col];

            result.data[row * 4 + col] = sum;
        }
    }

    return result;
}
constexpr Vec3 Mat4::operator*(const Vec3& v) const noexcept {
    return {
        .data = {
            data[0] * v.x + data[1] * v.y + data[2]  * v.z,
            data[4] * v.x + data[5] * v.y + data[6]  * v.z,
            data[8] * v.x + data[9] * v.y + data[10] * v.z,
        }
    };
}

constexpr Mat4& Mat4::transpose() noexcept {
    Utility::swap(data[1] , data[4] );
    Utility::swap(data[2] , data[8] );
    Utility::swap(data[3] , data[12]);
    Utility::swap(data[6] , data[9] );
    Utility::swap(data[7] , data[13]);
    Utility::swap(data[11], data[14]);

    return *this;
}
constexpr Mat4 Mat4::transposed() const noexcept {
    return {
        .data = {
            data[0], data[4], data[8] , data[12],
            data[1], data[5], data[9] , data[13],
            data[2], data[6], data[10], data[14],
            data[3], data[7], data[11], data[15]
        }
    };
}

constexpr Mat4 Mat4::identity() noexcept {
    return {
        .data = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        }
    };
}

constexpr Mat4 Mat4::translate(const Vec3& p) noexcept {
    return {
        .data = {
            1.0f, 0.0f, 0.0f, p.x ,
            0.0f, 1.0f, 0.0f, p.y ,
            0.0f, 0.0f, 1.0f, p.z ,
            0.0f, 0.0f, 0.0f, 1.0f
        }
    };
}
// rodrigues rotation
inline Mat4 Mat4::rotate(const Vec3& axis, const f32 rad) noexcept {
    const f32 s = Math::sin(rad);
    const f32 c = Math::cos(rad);
    const f32 t = 1 - c;

    const f32 x = axis.x;
    const f32 y = axis.y;
    const f32 z = axis.z;
    const f32 sx = s * x;
    const f32 sy = s * y;
    const f32 sz = s * z;

    const f32 tx  = t * x;
    const f32 ty  = t * y;
    const f32 tz  = t * z;
    const f32 txy = tx * y;
    const f32 txz = tx * z;
    const f32 tyz = ty * z;

    return {
        tx * x + c, txy - sz  , txz + sy  , 0.0f,
        txy + sz  , ty * y + c, tyz - sx  , 0.0f,
        txz - sy  , tyz + sx  , tz * z + c, 0.0f,
        0.0f      , 0.0f      , 0.0f      , 1.0f
    };
}
constexpr Mat4 Mat4::scale(const Vec3& s) noexcept {
    return {
        .data = {
            s.x , 0.0f, 0.0f, 0.0f,
            0.0f, s.y , 0.0f, 0.0f,
            0.0f, 0.0f, s.z , 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        }
    };
}