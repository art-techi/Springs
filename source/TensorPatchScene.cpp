#include "TensorPatchScene.hpp"

#include <atlas/utils/GUI.hpp>
#include <atlas/core/Float.hpp>

namespace assignment1
{
    TensorPatchScene::TensorPatchScene() :
        mTick(1 / 60.0f),
        mLastTime(0.0f),
        mShowControlPoints(true),
        mShowControlCage(false),
        mIsPlaying(false)
    { }

    void TensorPatchScene::keyPressEvent(int key, int scancode, int action,
        int mods)
    {
        ModellingScene::keyPressEvent(key, scancode, action, mods);

        if (action == GLFW_PRESS)
        {
            switch (key)
            {
            case GLFW_KEY_SPACE:
                mIsPlaying = !mIsPlaying;

            default:
                break;
            }
        }
    }

    void TensorPatchScene::updateScene(double time)
    {
        using atlas::core::geq;

        mTime.currentTime = (float)time;

        if (geq(mTime.currentTime - mLastTime, mTick))
        {
            mLastTime += mTick;
            mTime.deltaTime = mTick;
            mTime.totalTime = mLastTime;

            if (mIsPlaying)
            {
                mPatch.updateGeometry(mTime);
            }
        }
    }

    void TensorPatchScene::renderScene()
    {
        atlas::utils::Gui::getInstance().newFrame();
        const float grey = 161.0f / 255.0f;
        glClearColor(grey, grey, grey, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mProjection = glm::perspective(
            glm::radians(mCamera.getCameraFOV()),
            (float)mWidth / mHeight, 1.0f, 1000000.0f);

        mUniformMatrixBuffer.bindBuffer();
        mUniformMatrixBuffer.bufferSubData(0, sizeof(atlas::math::Matrix4),
            &mProjection[0][0]);

        mView = mCamera.getCameraMatrix();
        mUniformMatrixBuffer.bufferSubData(sizeof(atlas::math::Matrix4),
            sizeof(atlas::math::Matrix4), &mView[0][0]);
        mUniformMatrixBuffer.bindBufferBase(1);
        mUniformMatrixBuffer.unBindBuffer();

        if (mShowGrid)
        {
            mGrid.renderGeometry(mProjection, mView);
        }

        mPatch.renderGeometry();

        ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
        ImGui::Begin("Global HUD");
        ImGui::Checkbox("Show grid", &mShowGrid);
        if (ImGui::Button("Reset Camera"))
        {
            mCamera.resetCamera();
        }

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f /
            ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();

        ImGui::SetNextWindowSize(ImVec2(300, 100), ImGuiSetCond_FirstUseEver);
        ImGui::Begin("Patch controls");
        ImGui::Checkbox("Play", &mIsPlaying);
        ImGui::Checkbox("Show control cage", &mShowControlCage);
        mPatch.drawControlCage(mShowControlCage);

        ImGui::Checkbox("Show control points", &mShowControlPoints);
        mPatch.drawControlPoints(mShowControlPoints);

        ImGui::End();
        ImGui::Render();
    }
}