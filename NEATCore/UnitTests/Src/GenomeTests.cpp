#include <gtest/gtest.h>

#include "Genome.hpp"
#include "Parameters.hpp"


namespace
{
class PredictableGenerator: public RandomGenerator
{
public:
    PredictableGenerator(const std::vector<float>& nums) :
            index_(0)
            ,nums_(nums)
        {}


        float get_next(uint16_t rand_max = 1) const override
        {
            assert(nums_.size() != 0);
            auto num = nums_[index_++];

            index_ %= nums_.size();

            float next = num / 1.0f * static_cast<float>(rand_max);
            return next;
        }

    private:
        mutable int index_;
        std::vector<float> nums_;

};
}

TEST(GenomeTest, CompatibilityTestTwoExcessOneDisjointWeightDifference)
{
    auto generator = std::make_shared<RandomGenerator>();
    Genome::Genes gs1;
    {
        Gene g1;
        g1.innovation(1);
        g1.weight(1.2f);
        Gene g2;
        g2.innovation(3);
        g2.weight(.98f);
        gs1.push_back(g1);
        gs1.push_back(g2);
    }

    auto genome1 = Genome(generator, gs1);
    Genome::Genes gs2;
    {
        Gene g1;
        g1.innovation(1);
        g1.weight(0.5f);
        Gene g2;
        g2.innovation(2);
        g2.weight(0.4f);
        Gene g3;
        g3.innovation(3);
        g3.weight(0.2f);
        Gene g4;
        g4.innovation(4);
        g4.weight(1.2f);
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

    ASSERT_EQ(1.3184f, distance);

}

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
    auto generator = std::make_shared<PredictableGenerator>(nums);
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

TEST(GenomeTest, CrossoverTest)
{
    std::vector<float> nums = {1};
    auto generator = std::make_shared<PredictableGenerator>(nums);

    Genome g1 { generator,
        {
            {generator, 1.0f, 1, 1, 4},
            {generator, 1.0f, 2, 2, 4},
            {generator, 1.0f, 4, 2, 5, false},
            {generator, 1.0f, 5, 3, 5},
            {generator, 1.0f, 6, 4, 5},

        }
    };

    Genome g2 { generator,
        {
            {generator, 2.0f, 1, 1, 4, false},
            {generator, 2.0f, 2, 2, 4},
            {generator, 2.0f, 3, 3, 4},
            {generator, 2.0f, 4, 2, 5, false},
            {generator, 2.0f, 6, 4, 5},
            {generator, 2.0f, 7, 1, 6},
            {generator, 2.0f, 8, 6, 4},

        }
    };
    std::cout << g1 << std::endl;
    std::cout << g2 << std::endl;
    auto resulting_genome = g1.crossover(g2);
    std::cout << resulting_genome;
    Genome expected_genome { generator,
        {
            {generator, 1.0f, 1, 1, 4},
            {generator, 2.0f, 2, 2, 4},
            {generator, 1.0f, 4, 2, 5, false},
            {generator, 1.0f, 5, 3, 5},
            {generator, 2.0f, 6, 4, 5},
        }
   };
   ASSERT_EQ(expected_genome, resulting_genome);
}
