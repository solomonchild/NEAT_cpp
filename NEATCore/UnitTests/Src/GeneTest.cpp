#include <gtest/gtest.h>

#include "Gene.hpp"
#include "RandomGenerator.hpp"


std::vector<float> nums =
{
    0.7206788849676746, 0.3794265886957935, 0.524354878268552,
    0.26652269250181404, 0.985476169993312, 0.35077350707990473,
    0.48199812164623024, 0.9097862606875186, 0.572809447763906,
    0.09223247986163774, 0.9907758677874439, 0.5661027952446488,
    0.5354741901702641, 0.08183928136759533, 0.10827764903404846,
    0.9153483058998877, 0.7096771784088359, 0.3768665774047917,
    0.9088489554659702, 0.541847151481221, 0.3324676892791174,
    0.08573976578035081, 0.4346904882405004, 0.4425963777966423,
    0.007839112864302278, 0.6765720081256908, 0.13433449286497512,
    0.2680338300383408, 0.19926490709416267, 0.3110849197789698
};

class PredictableGenerator: public RandomGenerator
{
public:
    PredictableGenerator() :
        index_(0)
    {}

    float get_next(uint16_t rand_max) const override
    {
        auto num = nums[index_++];
        index_ %= nums.size();
        return num;
    }

private:
    mutable int index_;

};

TEST(GeneTest, ConstructGeneWithPredictableGenerator)
{
    PredictableGenerator generator;
    std::vector<Gene> genes;
    for(unsigned i = 0; i < 10; ++i)
    {
        genes.push_back(Gene(generator));
    }

    size_t i = 0;
    for(auto& it : genes)
    {
        ASSERT_TRUE(it.is_enabled());
        ASSERT_EQ(nums[i++], it.weight());
    }
}
