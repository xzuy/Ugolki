#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "ai.h"
#include <vector>

TEST_SUITE("distanceToCorner") {

    TEST_CASE("Black player distances") {
        CHECK(distanceToCorner(0, 0, 'B') == 0);    
        CHECK(distanceToCorner(0, 1, 'B') == 1);    
        CHECK(distanceToCorner(3, 4, 'B') == 7);    
        CHECK(distanceToCorner(7, 7, 'B') == 14);   
    }


    TEST_CASE("Symmetry") {
        CHECK(distanceToCorner(0, 0, 'B') == 0);
        CHECK(distanceToCorner(0, 0, 'W') == 14);

        CHECK(distanceToCorner(7, 7, 'B') == 14);
        CHECK(distanceToCorner(7, 7, 'W') == 0);
    }

    TEST_CASE("Edge positions") {
        CHECK(distanceToCorner(0, 5, 'B') == 5);    
        CHECK(distanceToCorner(5, 0, 'B') == 5);   
        CHECK(distanceToCorner(0, 5, 'W') == 9);    
        CHECK(distanceToCorner(5, 0, 'W') == 9);    
    }
}

