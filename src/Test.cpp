//Test scaffold for TriangleTest

#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "Triangle.cpp"

TEST_CASE("Triangle is equilateral", "[determineTriangleType]")
{
    Triangle t(4, 4, 4);
    t.determineTriangleType();
    REQUIRE(t.type == Triangle::TriangleType::equilateral);
}

TEST_CASE("Triangle is isosceles", "[determineTriangleType]")
{
    Triangle t(5, 5, 6);
    t.determineTriangleType();
    REQUIRE(t.type == Triangle::TriangleType::isosceles);
}

TEST_CASE("Triangle is scalene", "[determineTriangleType]")
{
    Triangle t(3, 4, 5);
    t.determineTriangleType();
    REQUIRE(t.type == Triangle::TriangleType::scalene);
}

TEST_CASE("Triangle lengths are invalid", "[determineTriangleType]")
{
    Triangle t(1, 2, 3);
    t.determineTriangleType();
    REQUIRE(t.type == Triangle::TriangleType::invalid);
}

TEST_CASE("Negative side lengths are invalid", "[determineTriangleType]")
{
    Triangle t(-3, -4, -5);
    t.determineTriangleType();
    REQUIRE(t.type == Triangle::TriangleType::invalid);
}