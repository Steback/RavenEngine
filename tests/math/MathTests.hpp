#ifndef RAVENENGINE_MATHTESTS_HPP
#define RAVENENGINE_MATHTESTS_HPP


#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

#include "math/Vector2.hpp"
#include "math/Vector3.hpp"


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
    }

} // namespace re


#endif //RAVENENGINE_MATHTESTS_HPP
