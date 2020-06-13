#include "BSplineScene.hpp"

#include <atlas/gl/GL.hpp>
#include <atlas/utils/GUI.hpp>

namespace assignment1
{
    BSplineScene::BSplineScene() :
        mSplineDegree(0),
        mShowCtrl(true),
        mShowPoly(false),
        mShowParametric(false)
    { }

    void BSplineScene::updateScene(double time)
    {
        ModellingScene::updateScene(time);

        mSpline.updateGeometry(atlas::core::Time<float>());
    }

    void BSplineScene::renderScene()
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
        mUniformMatrixBuffer.bindBufferBase(0);
        mUniformMatrixBuffer.unBindBuffer();

        if (mShowGrid)
        {
            mGrid.renderGeometry(mProjection, mView);
        }

        mSpline.renderGeometry();

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

        ImGui::SetNextWindowSize(ImVec2(400, 200), ImGuiSetCond_FirstUseEver);
        ImGui::Begin("Spline Controls");
        const char* degrees[] = { "1", "2", "3", "4", "5", "6" };
        ImGui::Combo("B-Spline degree", &mSplineDegree, degrees,
            ((int)(sizeof(degrees) / sizeof(*degrees))));
        mSpline.setDegree(mSplineDegree);

        ImGui::Checkbox("Show control points", &mShowCtrl);
        mSpline.drawControlPoints(mShowCtrl);

        ImGui::Checkbox("Show control polygon", &mShowPoly);
        mSpline.drawControlPolygon(mShowPoly);

        ImGui::Checkbox("Show parametric space", &mShowParametric);
        mSpline.drawParametricSpace(mShowParametric);
        ImGui::End();

        ImGui::Render();
    }
}
