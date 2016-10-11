#include "Gene.hpp"

struct Gene::Impl
{
    bool is_enabled_ = true;
    float weight_ = 0.0f;
    unsigned innovation_ = 0;
};


Gene::Gene()
:impl_(new Impl)
{
}

Gene::~Gene()
{
}

bool Gene::is_enabled()
{
    return impl_->is_enabled_;
}

float Gene::weight()
{
    return impl_->weight_;
}

std::unique_ptr<Gene, std::function<void(Gene*)>> Gene::create(const RandomGenerator& generator)
{
    auto del = [] (Gene* gen)
    {
        delete gen;
    };
    auto gene = std::unique_ptr<Gene, std::function<void(Gene*)>>(new Gene, del);
    gene->impl_->weight_ = generator.get_next();
    return gene;
}
