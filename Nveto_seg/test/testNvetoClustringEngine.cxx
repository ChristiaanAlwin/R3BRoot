#include <vector>
#include <stdexcept>
#include "NvetoClustringEngine.h"
#include "gtest/gtest.h"

namespace
{

    TEST(testNvetoClustringEngine, basic_int_clustering)
    {
        auto clusteringCondition = [](const int& a, const int& b)
        {
            return std::abs(b - a) <= 1;
        };

        auto clusterer = Nveto_seg::NvetoClustringEngine<int>(clusteringCondition);

        std::vector<int> digis{ 1, 2, 3, 7, 8, 9, 10, 12 };
        auto clusters = clusterer.Clusterize(digis);

        std::vector<std::vector<int>> expected = { { 1, 2, 3 }, { 7, 8, 9, 10 }, { 12 } };

        EXPECT_EQ(clusters, expected);
    }

    TEST(testNvetoClustringEngine, unsorted_object_clustering)
    {
        struct DummyDigi
        {
            int a;
            DummyDigi(const int& _a)
                : a(_a)
            {
            }
            bool operator==(const DummyDigi& rhs) const { return a == rhs.a; }
        };

        auto clusterer = Nveto_seg::NvetoClustringEngine<DummyDigi>();
        clusterer.SetClusteringCondition([](const DummyDigi& a, const DummyDigi& b)
                                         {
                                             return std::abs(b.a - a.a) <= 1;
                                         });

        std::vector<DummyDigi> digis{ 28, 13, 23, 22, 15, 16, 3, 6, 4, 26, 10, 11, 19, 8, 29, 12, 25, 30, 17, 18, 24 };
        std::vector<std::vector<DummyDigi>> clusters = clusterer.Clusterize(digis);

        std::vector<std::vector<DummyDigi>> expected{
            { 28, 29, 30 }, { 22, 23, 24, 25, 26 }, { 4, 3 }, { 10, 11, 12, 13 }, { 8 }, { 19, 18, 17, 16, 15 }, { 6 }
        };
        EXPECT_EQ(clusters, expected);
    }

    TEST(testNvetoClustringEngine, clustering_condition_not_set)
    {
        auto clusterer = Nveto_seg::NvetoClustringEngine<int>();
        std::vector<int> digis{ 1, 2, 3, 7, 8, 9, 10, 12 };
        EXPECT_ANY_THROW(clusterer.Clusterize(digis));
    }

    TEST(testNvetoClustringEngine, check_SatisfiesClusteringCondition)
    {
        auto clusterer = Nveto_seg::NvetoClustringEngine<int>([](const int& a, const int& b)
                                                        {
                                                            return std::abs(b - a) <= 1;
                                                        });
        EXPECT_TRUE(clusterer.SatisfiesClusteringCondition(1, 2));
        EXPECT_FALSE(clusterer.SatisfiesClusteringCondition(1, 3));
    }

} // namespace

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
