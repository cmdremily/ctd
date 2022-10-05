#include "ctd/units.hpp"

#pragma warning(push, 0)
#include <gtest/gtest.h>
#pragma warning(pop)

using namespace ctd::unit_literals;

namespace ctd {
    namespace {

        TEST(QuantityTest, Addition) {
            mass<int> ans = mass<int, kilo>(2) - mass<int>(10);
            EXPECT_EQ(1990, ans.count());

            EXPECT_EQ(1001_g, 1_kg + 1_g);
        }

        TEST(QuantityTest, Additions_ScaleOverflow) {
            using capacitance =
                capacitance<uint32_t, ratio<999999999999999999, 1000000000000000000>::type>;

            ASSERT_EQ(capacitance(2), capacitance(1) + capacitance(1));
        }

        TEST(QuantityTest, Subtraction) {
            mass<int> ans = mass<int, kilo>(2) - mass<int>(10);
            EXPECT_EQ(1990, ans.count());

            EXPECT_EQ(999_g, 1_kg - 1_g);
        }

        TEST(QuantityTest, SubtractionScaleOverflowBug) { EXPECT_EQ(1999_pF, 2_nF - 1_pF); }

        TEST(QuantityTest, Multiplication) {
            time<int, milli> ans = capacitance<int, micro>(100) * resistance<int, kilo>(20);
            EXPECT_EQ(2000, ans.count());

            EXPECT_EQ(200_N, 20_kg * 10_m_s2);
            EXPECT_EQ(10000_mV, 500_mA * 20_Ohm);
            EXPECT_EQ(10_V, 500_mA * 20_Ohm);
        }

        TEST(QuantityTest, MultiplicationByScalar) {
            EXPECT_EQ(200_kg, 20_kg * 10);
            EXPECT_EQ(2500_mA, 5 * 500_mA);
        }

        TEST(QuantityTest, Division) {
            speed<int> ans = length<int, kilo>(100) / time<int>(20);
            EXPECT_EQ(5000, ans.count());

            EXPECT_EQ(25_uA, 500_mV / 20_kOhm);
        }

       TEST(QuantityTest, DivisionByScalar) {
            EXPECT_EQ(25_mV, 500_mV / 20);
        }

        TEST(QuantityTest, DivisionInversionByScalar) { EXPECT_EQ(20_Hz, 2 / 100_ms); }

        TEST(QuantityTest, ScaleChange) {
            auto f = frequency<uint32_t, ratio<1>>(100_kHz);
            EXPECT_EQ(100000, f.count());
        }

        TEST(QuantityTest, ScaleChangeCustomScale) {
            using two_mA = current<int, ratio<2, 1000>>;
            two_mA cut = 500_mA;

            EXPECT_EQ(250, cut.count());
        }

        TEST(QuantityTest, EqualitySelf) { EXPECT_EQ(1_m, 1_m); }

        TEST(QuantityTest, EqualityScale) {
            EXPECT_EQ(1_F, 1000_mF);
            EXPECT_EQ(1000_uF, 1_mF);
            EXPECT_NE(1_F, 1000_uF);
            EXPECT_NE(1000_uF, 1_F);
        }

        TEST(QuantityTest, LessThanScaled) { EXPECT_LT(1_mF, 1_F); }

        TEST(QuantityTest, LessEqualsScaled) {
            EXPECT_LE(1_F, 1000_mF);
            EXPECT_LE(999_mF, 1_F);
        }

        TEST(QuantityTest, GreaterThanScaled) { EXPECT_GT(1_mF, 1_pF); }

        TEST(QuantityTest, GreaterEqualsScaled) {
            EXPECT_GE(1_F, 1000_mF);
            EXPECT_GE(1_F, 999_mF);
        }

        TEST(QuantityTest, MakeUnity) {
            constexpr auto volts = 123_mV;
            auto unity_volts = make_unity_valued<volts.count()>(volts);

            EXPECT_EQ(1, unity_volts.count());
            static_assert(is_same_v<units::volt, decltype(unity_volts)::units>,
                "Wrong unit for make unity");
            EXPECT_EQ(volts, unity_volts);
        }
    }
}