#ifndef MODELLING_ASSIGNMENTS_ASSIGNMENT1_INCLUDE_DE_BOOR_HPP
#define MODELLING_ASSIGNMENTS_ASSIGNMENT1_INCLUDE_DE_BOOR_HPP

#pragma once

#include <atlas/math/Math.hpp>
#include <atlas/core/Macros.hpp>
#include <atlas/core/Float.hpp>
#include <atlas/core/Numeric.hpp>

#include <vector>

namespace assignment1
{
    template <std::size_t degree>
    struct DeBoorCox
    {
        static float eval(int i, float t, std::vector<float> const& knots)
        {
            float numeratorA = t - knots[i - 1];
            float denomA = knots[i + degree - 2] - knots[i - 1];
            float numeratorB = knots[i + degree - 1] - t;
            float denomB = knots[i + degree - 1] - knots[i];

            float weightA, weightB;
            weightA = weightB = 0.0f;

            if (denomA != 0.0f)
            {
                weightA = numeratorA / denomA * 
                    DeBoorCox<degree - 1>::eval(i, t, knots);
            }

            if (denomB != 0.0f)
            {
                weightB = numeratorB / denomB *
                    DeBoorCox<degree - 1>::eval(i + 1, t, knots);
            }

            return weightA + weightB;
        }
    };

    template<>
    struct DeBoorCox<1>
    {
        static float eval(int i, float t, std::vector<float> const& knots)
        {
            using atlas::core::geq;
            if (geq(t, knots[i - 1]) && t < knots[i])
            {
                return 1.0f;
            }
            else
            {
                return 0.0f;
            }
        }
    };

    template <std::size_t degree>
    void createSpline(std::vector<atlas::math::Point> const& ctrlPts,
        std::vector<float> const& knots,
        std::vector<atlas::math::Point>& splineData, int step, 
        bool parametric = false)
    {
        namespace math = atlas::math;

        // Check if we have enough control points for the degree.
        std::size_t knotSize = degree + ctrlPts.size();
        if (ctrlPts.size() >= degree && knots.size() == knotSize)
        {
            splineData.clear();

            if (parametric)
            {
                for (std::size_t k = 1; k <= ctrlPts.size(); ++k)
                {
                    for (int i = 0; i < step; ++i)
                    {
                        float t = ((float)i / step) * 
                            (ctrlPts.size() - (degree - 1)) + degree - 1;
                        float weight = DeBoorCox<degree>::eval((int)k, t, knots);
                        math::Point pt(t, weight, 0.0f);
                        splineData.push_back(pt);
                    }
                }

                return;
            }

            for (int i = 0; i < step; ++i)
            {
                float t = ((float)i / step) * (ctrlPts.size() - (degree - 1)) +
                    degree - 1;
                math::Point pt(0.0f);

                for (std::size_t k = 1; k <= ctrlPts.size(); ++k)
                {
                    float weight = DeBoorCox<degree>::eval((int)k, t, knots);

                    pt += weight * ctrlPts[k - 1];
                }

                splineData.push_back(pt);
            }
        }
    }


}

#endif