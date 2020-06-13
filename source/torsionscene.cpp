/* Set up scene with nodes and grid
 * Extension of scene source file "scene.cpp"
 * Create angular nodes, render scene with nodes,
 *  lines and grid, update nodes accordingly.
 */

#include "TorsionScene.hpp"
#include <atlas/gl/GL.hpp>
#include <atlas/utils/GUI.hpp>
#include "TorsionNode.hpp"

#include "Triangle.hpp"
#include "TorsionNode.hpp"
#include "TorsionLine.hpp"
#include <iostream>

namespace assignment1
{
    TorsionScene::TorsionScene() {
        //create nodes
        std::shared_ptr<TorsionNode> nodex0(new TorsionNode(1, atlas::math::Vector(0, 0, 5),
            atlas::math::Vector(0,0,0), atlas::math::Vector(0,0,0),-1.74533));
        std::shared_ptr<TorsionNode> nodex1(new TorsionNode(1, atlas::math::Vector(0, 0, -5),
            atlas::math::Vector(0,0,0), atlas::math::Vector(0,0,0),-1.74533));
        std::shared_ptr<TorsionNode> nodex2(new TorsionNode(1, atlas::math::Vector(-0.8681, 4.924,0),
            atlas::math::Vector(0,0,0), atlas::math::Vector(0,0,0),-1.74533));
        std::shared_ptr<TorsionNode> nodex3(new TorsionNode(1, atlas::math::Vector(5, 0, 0),
            atlas::math::Vector(0,0,0), atlas::math::Vector(0,0,0),-1.74533));

        std::shared_ptr<TorsionLine> line01(new TorsionLine(nodex0,nodex1));
        std::shared_ptr<TorsionLine> line02(new TorsionLine(nodex0,nodex2));
        std::shared_ptr<TorsionLine> line03(new TorsionLine(nodex0,nodex3));
        std::shared_ptr<TorsionLine> line12(new TorsionLine(nodex1,nodex2));
        std::shared_ptr<TorsionLine> line13(new TorsionLine(nodex1,nodex3));

        //find hinge and set it as a usable variable for each node
        atlas::math::Vector x01=nodex1->getPosition()-nodex0->getPosition();
        atlas::math::Vector hinge=glm::normalize(x01);
        nodex0->setHinge(hinge);
        nodex1->setHinge(hinge);
        nodex2->setHinge(hinge);
        nodex3->setHinge(hinge);

        //push nodes into list
        mNodes.push_back(nodex3);
        mNodes.push_back(nodex2);
        mNodes.push_back(nodex1);
        mNodes.push_back(nodex0);

        mLines.push_back(line01);
        mLines.push_back(line02);
        mLines.push_back(line03);
        mLines.push_back(line12);
        mLines.push_back(line13);
    }

    void TorsionScene::renderScene(){
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

    void TorsionScene::updateScene(double time){
        ModellingScene::updateScene(time);

        int curr=4;
        for(auto& node : mNodes){
            node->updateGeometry(mTime,curr,mNodes[3]->getPosition(),mNodes[1]->getPosition(),
                    mNodes[0]->getPosition(),mNodes[1]->getVelocity(),
                    mNodes[0]->getVelocity(),mNodes[2]->getPosition()-mNodes[3]->getPosition());
            curr--;
        }
    }
}
