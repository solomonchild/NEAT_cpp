#pragma once

#include <memory>
#include <functional>

#include "RandomGenerator.hpp"

class Gene
{
public:
    bool is_enabled() const;
    void is_enabled(bool);

    float weight() const;
    void weight(float);

    unsigned innovation() const;
    void innovation(unsigned);

    unsigned in() const;
    void in(unsigned);

    unsigned out() const;
    void out(unsigned);


    static std::unique_ptr<Gene, std::function<void(Gene*)>> create(const RandomGenerator&);

private:
    Gene();
    ~Gene();
    struct Impl;
    std::unique_ptr<Impl> impl_;

};
