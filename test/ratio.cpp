#include "ctd/ratio.hpp"

#pragma warning(push, 0)
#include <gtest/gtest.h>
#pragma warning(pop)

namespace ctd {
    namespace {
        TEST(RatioScale, FloatingPoint) {
            EXPECT_EQ(7.0 * 3.0 / 5.0, (ratio_scale<ratio<3, 5>, double>(7)));
        }

        TEST(RatioScale, RoundingTowardNearest) {
            EXPECT_EQ(1, (ratio_scale<ratio<1, 2>, int, float_round_style::round_to_nearest>(1)));
            EXPECT_EQ(1, (ratio_scale<ratio<-1, 2>, int, float_round_style::round_to_nearest>(-1)));
            EXPECT_EQ(-1, (ratio_scale<ratio<-1, 2>, int, float_round_style::round_to_nearest>(1)));
            EXPECT_EQ(-1, (ratio_scale<ratio<1, 2>, int, float_round_style::round_to_nearest>(-1)));

            EXPECT_EQ(0, (ratio_scale<ratio<1, 3>, int, float_round_style::round_to_nearest>(1)));
            EXPECT_EQ(0, (ratio_scale<ratio<-1, 3>, int, float_round_style::round_to_nearest>(-1)));
            EXPECT_EQ(0, (ratio_scale<ratio<-1, 3>, int, float_round_style::round_to_nearest>(1)));
            EXPECT_EQ(0, (ratio_scale<ratio<1, 3>, int, float_round_style::round_to_nearest>(-1)));
        }

        TEST(RatioScale, RoundingTowardZero) {
            EXPECT_EQ(0, (ratio_scale<ratio<1, 3>, int, float_round_style::round_toward_zero>(1)));
            EXPECT_EQ(0, (ratio_scale<ratio<-1, 3>, int, float_round_style::round_toward_zero>(-1)));
            EXPECT_EQ(0, (ratio_scale<ratio<-1, 3>, int, float_round_style::round_toward_zero>(1)));
            EXPECT_EQ(0, (ratio_scale<ratio<1, 3>, int, float_round_style::round_toward_zero>(-1)));
        }

        TEST(RatioScale, RoundingTowardNegInf) {
            EXPECT_EQ(0, (ratio_scale<ratio<1, 2>, int, float_round_style::round_toward_neg_infinity>(1)));
            EXPECT_EQ(0, (ratio_scale<ratio<-1, 2>, int, float_round_style::round_toward_neg_infinity>(-1)));
            EXPECT_EQ(-1, (ratio_scale<ratio<-1, 2>, int, float_round_style::round_toward_neg_infinity>(1)));
            EXPECT_EQ(-1, (ratio_scale<ratio<1, 2>, int, float_round_style::round_toward_neg_infinity>(-1)));

            EXPECT_EQ(0, (ratio_scale<ratio<1, 3>, int, float_round_style::round_toward_neg_infinity>(1)));
            EXPECT_EQ(0, (ratio_scale<ratio<-1, 3>, int, float_round_style::round_toward_neg_infinity>(-1)));
            EXPECT_EQ(-1, (ratio_scale<ratio<-1, 3>, int, float_round_style::round_toward_neg_infinity>(1)));
            EXPECT_EQ(-1, (ratio_scale<ratio<1, 3>, int, float_round_style::round_toward_neg_infinity>(-1)));
        }

        TEST(RatioScale, RoundingTowardPosInf) {
            EXPECT_EQ(1, (ratio_scale<ratio<1, 3>, int, float_round_style::round_toward_infinity>(1)));
            EXPECT_EQ(1, (ratio_scale<ratio<-1, 3>, int, float_round_style::round_toward_infinity>(-1)));
            EXPECT_EQ(0, (ratio_scale<ratio<-1, 3>, int, float_round_style::round_toward_infinity>(1)));
            EXPECT_EQ(0, (ratio_scale<ratio<1, 3>, int, float_round_style::round_toward_infinity>(-1)));
        }

        TEST(RatioConvert, Identity) {
            // y * [1/3] = 1 * [1/3]
            EXPECT_EQ(1, (ratio_convert<ratio<1, 3>, ratio<1, 3>, int, float_round_style::round_indeterminate>(1)));
        }

        TEST(RatioConvert, NegatedIdentity) {
            // y * [1/3] = -1 * [-1/3]
            EXPECT_EQ(1, (ratio_convert<ratio<1, 3>, ratio<-1, 3>, int, float_round_style::round_indeterminate>(-1)));

            // y * [1/3] = -1 * [1/3]
            EXPECT_EQ(-1, (ratio_convert<ratio<1, 3>, ratio<1, 3>, int, float_round_style::round_indeterminate>(-1)));

            // y * [1/3] = 1 * [-1/3]
            EXPECT_EQ(-1, (ratio_convert<ratio<1, 3>, ratio<-1, 3>, int, float_round_style::round_indeterminate>(1)));

            // y * [-1/3] = 1 * [-1/3]
            EXPECT_EQ(1, (ratio_convert<ratio<-1, 3>, ratio<-1, 3>, int, float_round_style::round_indeterminate>(1)));
        }

        TEST(RatioConvert, LargerRatios) {
            // y * [99/100] = 321 * [49/50]
            // y = 317.76
            EXPECT_EQ(318, (ratio_convert<ratio<99, 100>, ratio<49, 50>, int, float_round_style::round_to_nearest>(321)));
            EXPECT_EQ(317, (ratio_convert<ratio<99, 100>, ratio<49, 50>, int, float_round_style::round_toward_neg_infinity>(321)));
        }

    }
}