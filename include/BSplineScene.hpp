#ifndef MODELLING_ASSIGNMENTS_ASSIGNMENT1_INCLUDE_B_SPLINE_SCENE_HPP
#define MODELLING_ASSIGNMENTS_ASSIGNMENT1_INCLUDE_B_SPLINE_SCENE_HPP

#pragma once

#include "BSpline.hpp"

#include <atlas/tools/ModellingScene.hpp>
#include <atlas/gl/Buffer.hpp>

namespace assignment1
{
    class BSplineScene : public atlas::tools::ModellingScene
    {
    public:
        BSplineScene();

        void updateScene(double time) override;
        void renderScene() override;

    private:
        BSpline mSpline;
        int mSplineDegree;
        bool mShowCtrl;
        bool mShowPoly;
        bool mShowParametric;
    };
}

#endif
