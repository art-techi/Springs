/* Set up scene with nodes and grid
 * Extension of scene source file "scene.cpp"
 * Create angular nodes, render scene with nodes,
 *  lines and grid, update nodes accordingly.
 */

#include "AngScene.hpp"
#include <atlas/gl/GL.hpp>
#include <atlas/utils/GUI.hpp>
#include "angnode.hpp"

#include "angnode.hpp"
#include "angline.hpp"
#include <iostream>

namespace assignment1
{
    AngScene::AngScene() {
        //create nodes
        std::shared_ptr<AngNode> nodex0(new AngNode(0.1, atlas::math::Vector(0, 0, 0),
            atlas::math::Vector(0,0,0), atlas::math::Vector(0,0,0),atlas::math::Vector (0, 1,0)));
        std::shared_ptr<AngNode> nodex1(new AngNode(0.1, atlas::math::Vector(5,1,0),
            atlas::math::Vector(0,0,0), atlas::math::Vector(0,0,0),atlas::math::Vector (0,1,0)));

        std::shared_ptr<AngLine> line01(new AngLine(nodex0,nodex1));

        //find hinge and set it as a usable variable for each node
        atlas::math::Vector hinge=atlas::math::Vector(0,0,1);

        nodex0->setHinge(hinge);
        nodex1->setHinge(hinge);

        //push nodes into list
        mNodes.push_back(nodex0);
        mNodes.push_back(nodex1);

        mLines.push_back(line01);
    }

    void AngScene::renderScene(){
        atlas::utils::Gui::getInstance().newFrame();
        float col = 100.0f/255.0f;
        glClearColor(col, col, col, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mProjection=glm::perspective(glm::radians(mCamera.getCameraFOV()),
                                       (float) mWidth/mHeight, 1.0f, 1000.0f);

        mUniformMatrixBuffer.bindBuffer();
        mUniformMatrixBuffer.bufferSubData(0, sizeof(atlas::math::Matrix4),
                                           &mProjection[0][0]);
        mView=mCamera.getCameraMatrix();
        mUniformMatrixBuffer.bufferSubData(sizeof(atlas::math::Matrix4),
                                           sizeof(atlas::math::Matrix4), &mView[0][0]);
        mUniformMatrixBuffer.bindBufferBase(0);
        mUniformMatrixBuffer.unBindBuffer();

        if(mShowGrid){
            mGrid.renderGeometry(mProjection, mView);
        }

        /*for (auto& triangle : mTriangles){
            triangle->renderGeometry(mProjection, mView);
        }*/
        for(auto& node: mNodes){
            node->renderGeometry(mProjection, mView);
        }

        for(auto& line: mLines){
            line->renderGeometry(mProjection, mView);
        }

        ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
        ImGui::Begin("Global HUD");
        ImGui::Checkbox("Show grid", &mShowGrid);
        if(ImGui::Button("Reset Camera")){
            mCamera.resetCamera();
        }

        ImGui::Text("%.3f ms/frame", 1000.0f/ImGui::GetIO().Framerate);
        ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
        ImGui::End();

        ImGui::Render();
    }

    void AngScene::updateScene(double time){
        ModellingScene::updateScene(time);

        mNodes[1]->updateGeometry(mTime);
    }
}
