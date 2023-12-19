#include <random.hpp>

std::random_device random::s_rand_device;

double random::get_double()
{
    const uint64_t rand_uint64 = s_rand_device();
    double rand_double;

    if(rand_uint64)
    {
        return (double)rand_uint64 / s_rand_device.max();
    }
    
    return 0.0;
}

bool random::get_bool()
{
    return s_rand_device() > s_rand_device.max() / 2;
}