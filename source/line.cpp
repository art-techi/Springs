/* Holds constructor for line shared pointer object.
 * Two nodes are passed in to create the line segment.
 * renderGeometry uploads the shaders to the system.
 */

#include "Node.hpp"
#include "AssignmentPaths.hpp"
#include "line.hpp"

#include <atlas/core/ImGUI.hpp>
#include <iostream>

namespace gl=atlas::gl;
namespace math=atlas::math;

namespace assignment1{
    Line::Line(std::shared_ptr<Node> node1, std::shared_ptr<Node> node2) :
        mNode1(node1),
        mNode2(node2),
        mBuff(GL_ARRAY_BUFFER)
    {
        std::vector<math::Vector> vertices={
            math::Vector(0,0,0),
            math::Vector(1,0,0)
        };

        mVAO.bindVertexArray();
        mBuff.bindBuffer();
        mBuff.bufferData(gl::size<math::Vector>(vertices.size()),
            vertices.data(), GL_STATIC_DRAW);
        mBuff.vertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0,
            gl::bufferOffset<math::Vector>(0));
        mVAO.enableVertexAttribArray(0);
        mVAO.unBindVertexArray();
        mBuff.unBindBuffer();

        std::vector<gl::ShaderUnit> shaders
        {
            { getShaderDirectory() + "Node.vs.glsl", GL_VERTEX_SHADER },
            { getShaderDirectory() + "Node.fs.glsl", GL_FRAGMENT_SHADER }
        };

        mShaders.push_back(gl::Shader(shaders));
        //mShaders[0].setShaderIncludeDir(getShaderDirectory());
        mShaders[0].compileShaders();
        mShaders[0].linkShaders();

        auto var = mShaders[0].getUniformVariable("model");
        mUniforms.insert(UniformKey("model", var));

        var = mShaders[0].getUniformVariable("projection");
        mUniforms.insert(UniformKey("projection", var));

        var = mShaders[0].getUniformVariable("view");
        mUniforms.insert(UniformKey("view", var));

        var = mShaders[0].getUniformVariable("colour");
        mUniforms.insert(UniformKey("colour", var));

        mVAO.unBindVertexArray();
        mShaders[0].disableShaders();
    }

    void Line::renderGeometry(atlas::math::Matrix4 projection, atlas::math::Matrix4 view)
    {
        mShaders[0].hotReloadShaders();

        if (!mShaders[0].shaderProgramValid())
        {
            return;
        }

        mShaders[0].enableShaders();

        math::Vector pos=mNode2->getPosition()-mNode1->getPosition();
        float len=glm::length(pos);
        math::Vector norm=pos/len;

        math::Vector xAxis=math::Vector(1,0,0);
        math::Vector rAxis=glm::normalize(glm::cross(xAxis, pos));
        float ang=glm::acos(glm::dot(xAxis, pos)/len);

        mModel = math::Matrix4(1);
        mModel = glm::translate(mModel, mNode1->getPosition());
        mModel = glm::rotate(mModel, ang, rAxis);
        mModel = glm::scale(mModel, math::Vector(len,1,1));

        glUniformMatrix4fv(mUniforms["model"], 1, GL_FALSE, &mModel[0][0]);
        glUniformMatrix4fv(mUniforms["view"], 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(mUniforms["projection"], 1, GL_FALSE, &projection[0][0]);

        // Control points are red.
        glUniform4f(mUniforms["colour"], 1, 0, 0, 1);

        mVAO.bindVertexArray();
        glDrawArrays(GL_LINES, 0, 3);
        mVAO.unBindVertexArray();
        mShaders[0].disableShaders();
    }
}
