#ifndef RANDOM_H
#define RANDOM_H

#include <random>

class random
{
private:
    static std::random_device s_rand_device;

public:
    static double get_double();
    static bool get_bool();
};

#endif