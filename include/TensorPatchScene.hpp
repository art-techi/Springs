#ifndef MODELLING_ASSIGNMENTS_ASSIGNMENT1_INCLUDE_TENSOR_PATCH_SCENE_HPP
#define MODELLING_ASSIGNMENTS_ASSIGNMENT1_INCLUDE_TENSOR_PATCH_SCENE_HPP

#pragma once

#include "TensorPatch.hpp"

#include <atlas/tools/ModellingScene.hpp>

namespace assignment1
{
    class TensorPatchScene : public atlas::tools::ModellingScene
    {
    public:
        TensorPatchScene();
        ~TensorPatchScene() = default;

        void keyPressEvent(int key, int scancode, int action, int mods) override;

        void updateScene(double time) override;
        void renderScene() override;

    private:
        float mTick;
        float mLastTime;
        bool mShowControlPoints;
        bool mShowControlCage;
        bool mIsPlaying;
        
        TensorPatch mPatch;
    };
}

#endif