#include "catch2/catch_test_macros.hpp"
#include <catch2/catch_all.hpp>

#define CONFIG_HPP
#define PIXEL_SIZE 1
#define BRUSH_SPREAD 0
#define BRUSH_DENCITY 1
#define BG_COLOR (SDL_Color) { (Uint8) (0) \
                             , (Uint8) (0) \
                             , (Uint8) (0) \
                             , (Uint8) (255) }
#define SAND_COLOR BG_COLOR
#define WATER_COLOR BG_COLOR
#define STONE_COLOR BG_COLOR
#define TEXT_SIZE 1
#include "../main/classes.cpp"


TEST_CASE("Vector2 != operator works correctly", "[vector2]") {
    Vector2 a = {1, 2};
    Vector2 b = {1, 1};
    Vector2 c = {2, 1};
    Vector2 d;

    CHECK_FALSE( a != a );
    CHECK( a != b );
    CHECK( a != c );
    CHECK( a != d );
}


class TestWorld : public World {
public:
    TestWorld() : World()
    {
        window_size = {3, 3};
        selected_pixel_type = SAND;

        // Initialising pixel_matrix
        pixel_matrix_size = { roundUp((float)window_size.x / PIXEL_SIZE)
                            , roundUp((float)window_size.y / PIXEL_SIZE) };
        pixel_matrix = new Pixel*[pixel_matrix_size.x];
        fillWithVoidPixels(pixel_matrix, pixel_matrix_size);
    }
};


TEST_CASE_METHOD(TestWorld, "World::World() and World::initSDL() work correctly", "[world][init][!mayfail]")
{
    SECTION("initSDL() successful SDL initialisation")
    {
        CHECK(initSDL() == SDL_APP_CONTINUE);
    }

    SECTION("Member definition is correct")
    {
        CHECK(window_size.x == 3);
        CHECK(window_size.y == 3);
        CHECK(pixel_matrix_size.x == 3);
        CHECK(pixel_matrix_size.y == 3);
        CHECK( (int) selected_pixel_type == (int) SAND );
    }

    auto x = GENERATE(0, 1, 2);
    SECTION("Correct filling of pixel_matrix with VOID")
    {
        auto y = GENERATE(0, 1, 2);
        CHECK( (int) pixel_matrix[x][y].type == (int) VOID );
    }
}


TEST_CASE_METHOD(TestWorld, "World::addPixel() works correctly", "[world][addpixel]")
{
    SECTION("Correct Pixel overrides")
    {
        addPixel({1, 1});
        CHECK( (int) pixel_matrix[1][1].type == (int) SAND );

        selected_pixel_type = WATER;
        addPixel({1, 1});
        CHECK( (int) pixel_matrix[1][1].type == (int) SAND );


        addPixel({0, 1});
        CHECK( (int) pixel_matrix[0][1].type == (int) WATER );

        selected_pixel_type = SAND;
        addPixel({0, 1});
        CHECK( (int) pixel_matrix[0][1].type == (int) SAND );

        selected_pixel_type = STONE;

        addPixel({0, 1});
        CHECK( (int) pixel_matrix[0][1].type == (int) STONE );

        selected_pixel_type = SAND;
        addPixel({0, 1});
        CHECK( (int) pixel_matrix[0][1].type == (int) STONE );
    }

    SECTION("Correct out of bounds handling")
    {
        addPixel({-1, 0});
        CHECK( (int) pixel_matrix[0][0].type == (int) SAND );

        addPixel({-123, 345});
        CHECK( (int) pixel_matrix[0][2].type == (int) SAND );
    }
}


TEST_CASE_METHOD(TestWorld, "World::checkPixel() works correctly", "[world][checkpixel]")
{
    addPixel({1, 1});
    selected_pixel_type = WATER;
    addPixel({0, 1});
    selected_pixel_type = STONE;
    addPixel({2, 1});

    auto x = GENERATE(-1, 0, 1, 2, 3);
    SECTION("Correctly works")
    {
        auto y = GENERATE(-1, 0, 1, 2, 3);

        if (x < 0 || x > 2 || y < 0 || y > 2)

            CHECK( (int) checkPixel({x, y}) == (int) SEG_FAULT );

        else if (x == 1 && y == 1 )
            CHECK( (int) checkPixel({x, y}) == (int) SAND );

        else if (x == 0 && y == 1 )
            CHECK( (int) checkPixel({x, y}) == (int) WATER );

        else if (x == 2 && y == 1 )
            CHECK( (int) checkPixel({x, y}) == (int) STONE );

        else
            CHECK( (int) checkPixel({x, y}) == (int) VOID );
    }
}


TEST_CASE_METHOD(TestWorld, "World::recalcWorld() works correctly", "[world][recalcworld]")
{
    SECTION("SAND and WATER pixel physics correct")
    {
        // _S_
        // ___
        // WTW
        addPixel({1, 0});
        selected_pixel_type = STONE;
        addPixel({1, 2});
        selected_pixel_type = WATER;
        addPixel({0, 2});
        addPixel({2, 2});

        recalcWorld();
        // ___
        // _S_
        // WTW
        CHECK( (int) pixel_matrix[1][1].type == (int) SAND );

        recalcWorld();
        // ___    ___
        // S__ or __S
        //
        // WTW    WTW
        CHECK(( (int) pixel_matrix[0][1].type == (int) SAND
               || (int) pixel_matrix[2][1].type == (int) SAND ));

        recalcWorld();
        // ___    ___
        // W__ or __W
        // STW    WTS
        CHECK((
            ( (int) pixel_matrix[0][2].type == (int) SAND
           && (int) pixel_matrix[0][1].type == (int) WATER ) ||
            ( (int) pixel_matrix[2][2].type == (int) SAND

           && (int) pixel_matrix[2][1].type == (int) WATER )
        ));

        recalcWorld();
        CHECK( (int) pixel_matrix[1][1].type == (int) WATER );
    }

    SECTION("STONE pixel physics correct")
    {
        selected_pixel_type = STONE;
        addPixel({1, 1});

        for (int i = 0; i < 20; ++i)
            recalcWorld();

        CHECK( (int) pixel_matrix[1][1].type == (int) STONE );
    }
}

TEST_CASE_METHOD(TestWorld, "World::resizePixelMatrix() works correctly", "[world][resizepixelmatrix]")
{
    // __S
    // __S
    // SSS
    addPixel({2, 0});
    addPixel({2, 1});
    addPixel({2, 2});
    addPixel({1, 2});
    addPixel({0, 2});

    SECTION("Expanding works correct")
    {
        window_size = {4, 4};
        resizePixelMatrix();

        CHECK(pixel_matrix_size.x == 4);
        CHECK(pixel_matrix_size.y == 4);

        for (int x = 0; x < 4; ++x)
        {
            for (int y = 0; y < 4; ++y)
            {
                if ( (y == 2 && x < 3)
                 || (x == 2 && y < 3 ))
                    CHECK( (int) pixel_matrix[x][y].type == (int) SAND );

                else if (y == 3 || x == 3)
                    CHECK( (int) pixel_matrix[x][y].type == (int) VOID );

            }
        }
    }

    SECTION("Shrinking works correct")
    {
        window_size = {2, 2};
        resizePixelMatrix();

        CHECK(pixel_matrix_size.x == 2);
        CHECK(pixel_matrix_size.y == 2);

        for (int x = 0; x < 2; ++x)
            for (int y = 0; y < 2; ++y)
                if (y == 1 || x == 1)
                    CHECK( (int) pixel_matrix[x][y].type == (int) VOID );
    }
}

TEST_CASE_METHOD(TestWorld, "World::clearWorld() works correctly", "[world][clearWorld]")
{
    SECTION("Works correct")
    {
        for (int x = 0; x < 3; ++x)
            for (int y = 0; y < 3; ++y)
                addPixel({x, y});

        clearWorld();

        CHECK(pixel_matrix_size.x == 3);
        CHECK(pixel_matrix_size.y == 3);

        for (int x = 0; x < 3; ++x)
            for (int y = 0; y < 3; ++y)
                CHECK(pixel_matrix[x][y].type == VOID);
    }
}
