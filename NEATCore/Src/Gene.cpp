#include "Gene.hpp"

struct Gene::Impl
{
    bool is_enabled_ = true;
    float weight_ = 0.0f;
    unsigned innovation_ = 0;
    unsigned in_ = 0;
    unsigned out_ = 0;
};


Gene::Gene()
:impl_(new Impl)
{
}

Gene::~Gene()
{
}

bool Gene::is_enabled() const
{
    return impl_->is_enabled_;
}

float Gene::weight() const
{
    return impl_->weight_;
}

unsigned Gene::innovation() const
{
    return impl_->innovation_;
}

unsigned Gene::in() const
{
    return impl_->in_;
}

unsigned Gene::out() const
{
    return impl_->out_;
}

void Gene::is_enabled(bool val)
{
   impl_->is_enabled_ = val;
}

void Gene::weight(float val)
{
    impl_->weight_ = val;
}

void Gene::innovation(unsigned val)
{
    impl_->innovation_ = val;
}

void Gene::in(unsigned val)
{
    impl_->in_ = val;
}

void Gene::out(unsigned val)
{
    impl_->out_ = val;
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
