/* Set up scene with nodes and grid.
 * Create nodes, pass them to be rendered
 * and updated.
 */

#include "Scene.hpp"
#include <atlas/gl/GL.hpp>
#include <atlas/utils/GUI.hpp>

#include "Node.hpp"
#include <iostream>
#include "line.hpp"

namespace assignment1
{
    Scene::Scene() {
        //create two nodes, one anchor and one that will move
        std::shared_ptr<Node> node1(new Node(0.1, atlas::math::Vector(0, 0, 0),
            atlas::math::Vector(0,0,0), atlas::math::Vector(0,0,0)));
        std::shared_ptr<Node> node2(new Node(0.1, atlas::math::Vector(5, 0, 0),
            atlas::math::Vector(0,0,0), atlas::math::Vector(0,0,0)));
        //draw a line between them (for better visual representation
        std::shared_ptr<Line> line(new Line(node1, node2));

        //set node's neighbours
        node1->setNeighbour(node2);
        node2->setNeighbour(node1);
        float length;

        atlas::math::Vector pos1=node1->getPosition();
        atlas::math::Vector pos2=node2->getPosition();

        length=3; //glm::length(pos2-pos1);

        //set initial distance
        node1->setInitDis(length);
        node2->setInitDis(length);

        //push nodes into node list
        mNodes.push_back(node1);
        mNodes.push_back(node2);
        //push line into line list
        mLines.push_back(line);
    }

    void Scene::renderScene(){
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

        for (auto& node : mNodes) {
            node->renderGeometry(mProjection, mView);
        }

        for(auto& line : mLines){
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

    void Scene::updateScene(double time){
        ModellingScene::updateScene(time);
        //run through each node and update the geometry
        //for(auto& node : mNodes){
            mNodes[1]->updateGeometry(mTime);
        //}
    }
}
