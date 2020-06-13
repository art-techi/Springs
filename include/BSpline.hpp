#ifndef MODELLING_ASSIGNMENTS_ASSIGNMENT1_INCLUDE_B_SPLINE_HPP
#define MODELLING_ASSIGNMENTS_ASSIGNMENT1_INCLUDE_B_SPLINE_HPP

#pragma once

#include <atlas/utils/Geometry.hpp>
#include <atlas/gl/Buffer.hpp>
#include <atlas/gl/VertexArrayObject.hpp>

namespace assignment1
{
    class BSpline : public atlas::utils::Geometry
    {
    public:
        BSpline();
        ~BSpline() = default;

        void setDegree(int degree);

        void drawControlPoints(bool draw);
        void drawControlPolygon(bool draw);
        void drawParametricSpace(bool draw);

        void renderGeometry() override;

    private:
        void generateSplines();
        void updateSpline();

        atlas::gl::VertexArrayObject mSplineVao;
        atlas::gl::VertexArrayObject mControlVao;
        atlas::gl::Buffer mSplineBuffer;
        atlas::gl::Buffer mControlBuffer;

        std::size_t mDegree;
        bool mDrawCtrlPts;
        bool mDrawCtrlPoly;
        bool mDrawParametric;
        std::size_t mNumPts;
        std::vector<atlas::math::Point> mControlPts;
        std::vector<std::vector<atlas::math::Point>> mSplines;
        std::vector<std::vector<atlas::math::Point>> mParametrics;
        std::vector<std::vector<float>> mKnotVectors;
    };
}


#endif