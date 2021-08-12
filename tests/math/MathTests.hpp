#ifndef RAVENENGINE_MATHTESTS_HPP
#define RAVENENGINE_MATHTESTS_HPP


#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"
#include "fmt/printf.h"

#include "math/Vector2.hpp"
#include "math/Vector3.hpp"
#include "math/Vector4.hpp"
#include "math/Matrix3.hpp"
#include "math/Matrix4.hpp"


namespace re {

    TEST_CASE("Vector2 Tests") {
        re::Vector2 a = {1.0f, 1.0f};
        re::Vector2 b = {2.0f, 2.0f};

        CHECK(a == re::Vector2(1.0f, 1.0f));
        CHECK(a != b);
        CHECK(a + 1.0f == b);
        CHECK(a + b == re::Vector2(3.0f, 3.0f));
        CHECK(b - 1.0f == a);
        CHECK(a - b == re::Vector2(-1.0f, -1.0f));
        CHECK(a * 2.0f == b);
        CHECK(a * b == 4);
        CHECK(b / 2.0f == a);
        CHECK(b.lengthSqrt() == 8);
    }


    TEST_CASE("Vector3 Tests") {
        re::Vector3 a = {1.0f, 1.0f, 1.0f};
        re::Vector3 b = {2.0f, 2.0f, 2.0f};

        CHECK(a == re::Vector3(1.0f, 1.0f, 1.0f));
        CHECK(a != b);
        CHECK(a + 1.0f == b);
        CHECK(a + b == re::Vector3(3.0f, 3.0f, 3.0f));
        CHECK(b - 1.0f == a);
        CHECK(a - b == re::Vector3(-1.0f, -1.0f, -1.0f));
        CHECK(a * 2.0f == b);
        CHECK(a * b == 6);
        CHECK(b / 2.0f == a);
        CHECK(b.lengthSqrt() == 12);
        CHECK(a.cross(b) == re::vec3(0.0f, 0.0f, 0.0f));
    }

    TEST_CASE("Vector4 Tests") {
        re::Vector4 a = {1.0f, 1.0f, 1.0f, 1.0f};
        re::Vector4 b = {2.0f, 2.0f, 2.0f, 2.0f};

        CHECK(a == re::Vector4(1.0f, 1.0f, 1.0f, 1.0f));
        CHECK(a != b);
        CHECK(a + 1.0f == b);
        CHECK(a + b == re::Vector4(3.0f, 3.0f, 3.0f, 3.0f));
        CHECK(b - 1.0f == a);
        CHECK(a - b == re::Vector4(-1.0f, -1.0f, -1.0f, -1.0f));
        CHECK(a * 2.0f == b);
        CHECK(a * b == 8);
        CHECK(b / 2.0f == a);
        CHECK(b.lengthSqrt() == 16);
    }

    TEST_CASE("Matrix3 Tests") {
        re::mat3 a = {
                { 1.0f, 1.0f, 1.0f },
                { 1.0f, 1.0f, 1.0f },
                { 1.0f, 1.0f, 1.0f }
        };

        re::mat3 b = {
                { 2.0f, 2.0f, 2.0f },
                { 2.0f, 2.0f, 2.0f },
                { 2.0f, 2.0f, 2.0f }
        };

        re::mat3 c = {
                { 6, 1, 1 },
                { 4, -2, 5 },
                { 2, 8, 7 }
        };

        CHECK(a != b);
        CHECK(a + b == re::mat3({3, 3, 3}, {3, 3, 3}, {3, 3, 3}));
        CHECK(a - b == re::mat3({-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}));
        CHECK(a * 2 == b);
        CHECK(b * re::vec3(2, 2, 2) == re::vec3(12, 12, 12));
        CHECK(a * b == re::mat3({6, 6, 6}, {6, 6, 6}, {6, 6, 6}));
        CHECK(b / 2 == a);
        CHECK(c.determinant() == -306);
        CHECK(c.adjugate() == re::mat3({-54, 1, 7}, {-18, 40, -26}, {36, -46, -16}));
    }

    TEST_CASE("Matrix4 Tests") {
        re::mat4 a = {
                { 1.0f, 1.0f, 1.0f, 1.0f },
                { 1.0f, 1.0f, 1.0f, 1.0f },
                { 1.0f, 1.0f, 1.0f, 1.0f },
                { 1.0f, 1.0f, 1.0f, 1.0f }
        };

        re::mat4 b = {
                { 2.0f, 2.0f, 2.0f, 2.0f },
                { 2.0f, 2.0f, 2.0f, 2.0f },
                { 2.0f, 2.0f, 2.0f, 2.0f },
                { 2.0f, 2.0f, 2.0f, 2.0f }
        };

        re::mat4 c = {
                { 4, 3, 2, 2 },
                { 0, 1, -3, 3 },
                { 0, -1, 3, 3 },
                { 0, 3, 1, 1 }
        };

        CHECK(a != b);
        CHECK(a + b == re::mat4({3, 3, 3, 3}, {3, 3, 3, 3}, {3, 3, 3, 3}, {3, 3, 3, 3}));
        CHECK(a - b == re::mat4({-1, -1, -1, -1}, {-1, -1, -1, -1}, {-1, -1, -1, -1}, {-1, -1, -1, -1}));
        CHECK(a * 2 == b);
        CHECK(b * re::vec4(2, 2, 2, 2) == re::vec4(16, 16, 16, 16));
        CHECK(a * b == re::mat4({8, 8, 8, 8}, {8, 8, 8, 8}, {8, 8, 8, 8}, {8, 8, 8, 8}));
        CHECK(b / 2 == a);
        CHECK(c.determinant() == -240);
        CHECK(c.adjugate() == re::mat4({-60, 0, 18, 66}, {0, 0, 24, -72}, {0, 40, -32, -24}, {0, -40, -40, 0}));
    }

} // namespace re


#endif //RAVENENGINE_MATHTESTS_HPP
