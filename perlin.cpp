#include <array>
#include <cmath>
#include <iostream>

template <typename T> struct perlin_noise
{
    static_assert(std::is_floating_point<T>() == true);
    perlin_noise()
    {
        for (size_t i = 0; i < p.size() / 2; ++i)
        {
            p[256 + i] = p[i];
        }
    }
    auto noise(T x, T y, T z) const -> T
    {
        // clang-format off
        int X = static_cast<int>(std::floor(x)) & 255,  // FIND UNIT CUBE THAT
            Y = static_cast<int>(std::floor(y)) & 255,  // CONTAINS POINT.
            Z = static_cast<int>(std::floor(z)) & 255;
            x -= std::floor(x);                         // FIND RELATIVE X,Y,Z
            y -= std::floor(y);                         // OF POINT IN CUBE.
            z -= std::floor(z);
        T u = fade(x),                                  // COMPUTE FADE CURVES
          v = fade(y),                                  // FOR EACH OF X,Y,Z.
          w = fade(z);
        int A = p[X  ]+Y, AA = p[A]+Z, AB = p[A+1]+Z,   // HASH COORDINATES OF
          B = p[X+1]+Y, BA = p[B]+Z, BB = p[B+1]+Z;     // THE 8 CUBE CORNERS,

      return lerp(w, lerp(v, lerp(u, grad(p[AA  ], x  , y  , z   ),  // AND ADD
                                     grad(p[BA  ], x-1, y  , z   )), // BLENDED
                             lerp(u, grad(p[AB  ], x  , y-1, z   ),  // RESULTS
                                     grad(p[BB  ], x-1, y-1, z   ))),// FROM  8
                     lerp(v, lerp(u, grad(p[AA+1], x  , y  , z-1 ),  // CORNERS
                                     grad(p[BA+1], x-1, y  , z-1 )), // OF CUBE
                             lerp(u, grad(p[AB+1], x  , y-1, z-1 ),
                                     grad(p[BB+1], x-1, y-1, z-1 ))));
        // clang-format on
    }
    auto fade(T t) const -> T { return t * t * t * (t * (t * 6 - 15) + 10); }
    auto lerp(T t, T a, T b) const -> T { return a + t * (b - a); }
    auto grad(int hash, T x, T y, T z) const -> T
    {
        // clang-format off
        int h = hash & 15;        // CONVERT LO 4 BITS OF HASH CODE
        T u = h < 8 ? x : y,      // INTO 12 GRADIENT DIRECTIONS.
        v = h < 4 ? y : h == 12 || h == 14 ? x : z;
        return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
        // clang-format on
    }

    auto print_lerp(T t, T a, T b) const -> void
    {
        std::cout << "lerp(" << t << ", " << a << ", " << b << ") => " << lerp(t, a, b) << "\n";
    }
    auto print_noise(T x, T y, T z) const -> void
    {
        std::cout << "noise(" << x << ", " << y << ", " << z << ") => " << noise(x, y, z) << '\n';
    }
    auto print_fade(T t) const -> void { std::cout << "fade(" << t << ") => " << fade(t) << '\n'; }
    auto print_grad(int hash, T x, T y, T z) const -> void
    {
        std::cout << "grad(" << hash << ", " << x << ", " << y << ", " << z
                  << ") = " << grad(hash, x, y, z) << '\n';
    }

    // The permutation table
    std::array<int, 512> p{
        151, 160, 137, 91,  90,  15,  131, 13,  201, 95,  96,  53,  194, 233, 7,   225, 140, 36,
        103, 30,  69,  142, 8,   99,  37,  240, 21,  10,  23,  190, 6,   148, 247, 120, 234, 75,
        0,   26,  197, 62,  94,  252, 219, 203, 117, 35,  11,  32,  57,  177, 33,  88,  237, 149,
        56,  87,  174, 20,  125, 136, 171, 168, 68,  175, 74,  165, 71,  134, 139, 48,  27,  166,
        77,  146, 158, 231, 83,  111, 229, 122, 60,  211, 133, 230, 220, 105, 92,  41,  55,  46,
        245, 40,  244, 102, 143, 54,  65,  25,  63,  161, 1,   216, 80,  73,  209, 76,  132, 187,
        208, 89,  18,  169, 200, 196, 135, 130, 116, 188, 159, 86,  164, 100, 109, 198, 173, 186,
        3,   64,  52,  217, 226, 250, 124, 123, 5,   202, 38,  147, 118, 126, 255, 82,  85,  212,
        207, 206, 59,  227, 47,  16,  58,  17,  182, 189, 28,  42,  223, 183, 170, 213, 119, 248,
        152, 2,   44,  154, 163, 70,  221, 153, 101, 155, 167, 43,  172, 9,   129, 22,  39,  253,
        19,  98,  108, 110, 79,  113, 224, 232, 178, 185, 112, 104, 218, 246, 97,  228, 251, 34,
        242, 193, 238, 210, 144, 12,  191, 179, 162, 241, 81,  51,  145, 235, 249, 14,  239, 107,
        49,  192, 214, 31,  181, 199, 106, 157, 184, 84,  204, 176, 115, 121, 50,  45,  127, 4,
        150, 254, 138, 236, 205, 93,  222, 114, 67,  29,  24,  72,  243, 141, 128, 195, 78,  66,
        215, 61,  156, 180,
    };
};

auto main() -> int
try
{
    const auto pn = perlin_noise<double>();
    pn.print_fade(0.75);
    pn.print_lerp(0.5, 50, 100);
    pn.print_lerp(0.25, 50, 25);
    pn.print_lerp(0.25, -50, 25);
    pn.print_noise(0, 0, 0);
    pn.print_noise(1, 1, 1);
    pn.print_noise(-1, -1, -1);
    pn.print_noise(0.5, 0.5, 0.5);
    pn.print_noise(0.1, 0.1, 0.1);
    pn.print_noise(3.14, 42, 0);
    pn.print_noise(-4.20, 10, 0);
}
catch (const std::exception &e)
{
    std::cerr << e.what() << '\n';
}
