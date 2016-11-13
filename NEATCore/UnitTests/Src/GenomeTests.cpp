#include <gtest/gtest.h>

#include "Genome.hpp"
#include "Parameters.hpp"



TEST(GenomeTest, CompatibilityTestTwoExcessOneDisjoint)
{
    auto generator = std::make_shared<RandomGenerator>();
    Genome::Genes gs1;
    {
        Gene g1;
        g1.innovation(1);
        Gene g2;
        g2.innovation(3);
        gs1.push_back(g1);
        gs1.push_back(g2);
    }

    auto genome1 = Genome(generator, gs1);
    Genome::Genes gs2;
    {
        Gene g1;
        g1.innovation(1);
        Gene g2;
        g2.innovation(2);
        Gene g3;
        g3.innovation(3);
        Gene g4;
        g4.innovation(4);
        Gene g5;
        g5.innovation(5);
        gs2.push_back(g1);
        gs2.push_back(g2);
        gs2.push_back(g3);
        gs2.push_back(g4);
        gs2.push_back(g5);
    }
    auto genome2 = Genome(generator, gs2);
    float distance = genome1.compatibility_distance(genome2);

    ASSERT_EQ(1.2f, distance);
}
TEST(GenomeTest, CompatibilityTestTwoExcessZeroWeight)
{
    auto generator = std::make_shared<RandomGenerator>();
    Genome::Genes gs1;
    {
        Gene g1;
        g1.innovation(1);
        Gene g2;
        g2.innovation(2);
        gs1.push_back(g1);
        gs1.push_back(g2);
    }

    auto genome1 = Genome(generator, gs1);
    Genome::Genes gs2;
    {
        Gene g1;
        g1.innovation(1);
        Gene g2;
        g2.innovation(2);
        Gene g3;
        g3.innovation(3);
        Gene g4;
        g4.innovation(4);
        gs2.push_back(g1);
        gs2.push_back(g2);
        gs2.push_back(g3);
        gs2.push_back(g4);
    }
    auto genome2 = Genome(generator, gs2);
    float distance = genome1.compatibility_distance(genome2);

    ASSERT_EQ(1, distance);
}

TEST(GenomeTest, EvaluateNetworkTest)
{

    class PredictableGenerator: public RandomGenerator
    {
    public:
        PredictableGenerator() :
            index_(0)
        {}

        float get_next(uint16_t rand_max = 1) const override
        {
            auto num = nums[index_++];

            index_ %= nums.size();

            float next = num / 1.0f * static_cast<float>(rand_max);
            return next;
        }

    private:
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
        mutable int index_;

    };
    auto generator = std::make_shared<PredictableGenerator>();
    Gene gene1;
    Gene gene2;
    gene1.weight(0.5);
    gene2.weight(0.3);
    gene1.in(0);
    gene1.out(Parameters::genome_size - 1);
    gene2.in(1);
    gene2.out(Parameters::genome_size - 1);

    Genome genome(generator, {gene1, gene2});
    Outputs expected_outputs = {0.96108983};
    auto outputs = genome.evaluate_network({1, 1});
    ASSERT_FLOAT_EQ(expected_outputs[0], outputs[0]);

}
