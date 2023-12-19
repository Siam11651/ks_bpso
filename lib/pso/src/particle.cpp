#include <particle.hpp>

template <typename P, typename V, typename F>
particle<P, V, F>::particle() {}

template <typename P, typename V, typename F>
particle<P, V, F>::particle(const std::vector<P> &_position, const std::vector<V> &_velocity) :
    m_position(_position),
    m_velocity(_velocity) {}

template <typename P, typename V, typename F>
std::vector<P> &particle<P, V, F>::position()
{
    return m_position;
}

template <typename P, typename V, typename F>
const std::vector<P> &particle<P, V, F>::const_position() const
{
    return m_position;
}

template <typename P, typename V, typename F>
std::optional<std::vector<P>> &particle<P, V, F>::best_position()
{
    return m_best_position;
}

template <typename P, typename V, typename F>
const std::optional<std::vector<P>> &particle<P, V, F>::const_best_position() const
{
    return m_best_position;
}

template <typename P, typename V, typename F>
std::vector<V> &particle<P, V, F>::velocity()
{
    return m_velocity;
}

template <typename P, typename V, typename F>
const std::vector<V> &particle<P, V, F>::const_velocity() const
{
    return m_velocity;
}

template <typename P, typename V, typename F>
std::optional<F> &particle<P, V, F>::best_fitness()
{
    return m_best_fitness;
}

template <typename P, typename V, typename F>
const std::optional<F> &particle<P, V, F>::const_best_fitness() const
{
    return m_best_fitness;
}

template class particle<bool, double, uint64_t>;