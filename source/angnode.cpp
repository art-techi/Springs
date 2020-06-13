/* Holds constructor for angular node shared pointer object.
 * Extended vesion of node source file "node.cpp"
 * Mass, position, velocity, force, and the initial rest angle
 * are passed in.
 *
 * renderGeometry: uploads the shaders to the system.
 * updateGeometry: uses the content from the textbook to calculate
 *      all the mathematical components needed for position and
 *      velocity updates.
 * getPosition: returns the current node's position
 * getVelocity: returns the current node's velocity
 * setHinge: sets the hinge vector which can be used in the
 *      calculations in updateGeometry
 */

#include "AngNode.hpp"
#include "AssignmentPaths.hpp"
#include <math.h>

#include <atlas/core/ImGUI.hpp>
#include <iostream>

namespace gl=atlas::gl;
namespace math=atlas::math;

namespace assignment1{
    AngNode::AngNode(float mass, atlas::math::Vector position, atlas::math::Vector velocity,
               atlas::math::Vector force, atlas::math::Vector restAng) :
        mMass(mass),
        mPosition(position),
        mVelocity(velocity),
        mForce(force),
        mRest(restAng),
        mBuff(GL_ARRAY_BUFFER)
    {
        mAngularVel = 0;

        mVAO.bindVertexArray();
        mBuff.bindBuffer();
        mBuff.bufferData(gl::size<math::Point>(1),
            &(math::Vector(0)[0]), GL_STATIC_DRAW);
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

    void AngNode::renderGeometry(atlas::math::Matrix4 projection, atlas::math::Matrix4 view)
    {
        mShaders[0].hotReloadShaders();
        if (!mShaders[0].shaderProgramValid())
        {
            return;
        }

        mShaders[0].enableShaders();

        mModel = math::Matrix4(1);
        mModel = glm::translate(mModel, mPosition);

        glUniformMatrix4fv(mUniforms["model"], 1, GL_FALSE, &mModel[0][0]);
        glUniformMatrix4fv(mUniforms["view"], 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(mUniforms["projection"], 1, GL_FALSE, &projection[0][0]);

        // Control points are red.
        glUniform4f(mUniforms["colour"], 1, 0, 0, 1);

        mVAO.bindVertexArray();
        glPointSize(10.0f);
        glDrawArrays(GL_POINTS, 0, 1);
        glPointSize(1.0f);
        mVAO.unBindVertexArray();
        mShaders[0].disableShaders();
    }

    void AngNode::updateGeometry(atlas::core::Time<> const& time){
        atlas::math::Vector zAxis=atlas::math::Vector(0,0,1);

        float torque; //total torque
        float inertia;//inertia

        float fs; //spring force
        float fd; //damping force

        float ks=0.5;    //spring constant
        float kd=0.2; //damping constant
        float dT=time.deltaTime;//change in time

        //find angle between vector and xz plane
        atlas::math::Vector nr=glm::normalize(mPosition);
        float theta=atan2(glm::dot(glm::cross(nr, mRest),zAxis),(glm::dot(nr,mRest)));

        //calculate spring force
        fs=ks*theta;

        //calculate damper force
        fd=-kd*(mAngularVel);

        //calculate total torque
        torque=fs+fd;

        //calculate inertia
        inertia=mMass*glm::length2(mPosition);

        //update angular velocity
        mAngularVel += ((torque/inertia)*dT);

        //calculate change in theta
        float deltaTheta=dT*(mAngularVel);

        //update position
        math::Matrix4 mat(1.0f);//identity
        mat=glm::rotate(mat, deltaTheta, zAxis);//rotate position
        math::Vector nPosition=math::Vector(mat*math::Vector4(mPosition, 1));//apply roation to position
        mPosition =nPosition;//set position

    }

    atlas::math::Vector AngNode::getPosition(){
        return mPosition;
    }

    //set the hinge vector
    void AngNode::setHinge(atlas::math::Vector hinge){
        mHinge=hinge;
    }

}
