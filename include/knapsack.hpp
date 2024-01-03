#ifndef KNAPSACK_H
#define KNAPSACK_H

#include <iostream>
#include <pso.hpp>

class ks_pair
{
public:
    uint64_t weight;
    uint64_t profit;

    ks_pair();
    ks_pair(const uint64_t &_weight, const uint64_t &_profit);
};

class ks_problem
{
private:
    std::vector<ks_pair> m_ks_pairs;
    size_t m_capacity;

public:
    ks_problem(std::istream &_istream);
    const std::vector<ks_pair> &get_ks_pairs() const;
    const size_t &get_capacity() const;
};

class ks_particle : public particle<bool, double, uint64_t>
{
private:
    const ks_problem *m_ks_problem_ptr;

public:
    ks_particle(const ks_problem *_ks_problem_ptr);
    ks_particle(const ks_problem *_ks_problem_ptr, const std::vector<bool> &_position,
        const std::vector<double> &_velocity);
    void update_fitness();
};

class ks_swarm : public swarm<bool, double, uint64_t>
{
private:
    double sigmoid(const double &in);

public:
    double vmax;

    ks_swarm();
    ks_swarm(const std::vector<ks_particle *> &_particle_ptrs);
    void update_position();
};

class ks_bpso : public pso<bool, double, uint64_t>
{
public:
    size_t generation_count;

    ks_bpso(ks_swarm *_swarm_ptr);
    ks_bpso(const size_t &_generation_count, ks_swarm *_swarm_ptr);
    virtual void run();
};

class ks_tvbpso : public ks_bpso
{
public:
    size_t max_iteration;
    double vlow;
    double vhigh;

    ks_tvbpso(ks_swarm *_swarm_ptr);
    ks_tvbpso(const size_t &_generation_count, const size_t &_max_iteration,
        ks_swarm *_swarm_ptr);
    void run();
};

class ks_ntvbpso : public ks_tvbpso
{
public:
    double param;

    ks_ntvbpso(ks_swarm *_swarm_ptr);
    ks_ntvbpso(const double &_param, const size_t &_generation_count, const size_t &_max_iteration,
        ks_swarm *_swarm_ptr);
    void run();
};

#endif