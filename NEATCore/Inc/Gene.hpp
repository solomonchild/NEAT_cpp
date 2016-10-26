#pragma once

#include <memory>
#include <functional>

#include "RandomGenerator.hpp"

class Gene
{
public:


    Gene();
    Gene(const Gene&);
    Gene& operator=(const Gene&);
    Gene(std::shared_ptr<RandomGenerator>);

    Gene(Gene&&);
    Gene& operator=(Gene&&);
    ~Gene();

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



private:
    friend std::ostream& operator<<(std::ostream& stream, const Gene&);
    struct Impl;
    std::unique_ptr<Impl> impl_;

};
