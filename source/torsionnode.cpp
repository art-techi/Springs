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

#include "TorsionNode.hpp"
#include "AssignmentPaths.hpp"


#include <atlas/core/ImGUI.hpp>
#include <iostream>

namespace gl=atlas::gl;
namespace math=atlas::math;

namespace assignment1{
    TorsionNode::TorsionNode(float mass, atlas::math::Vector position, atlas::math::Vector velocity,
               atlas::math::Vector force, float restAng) :
        mMass(mass),
        mPosition(position),
        mVelocity(velocity),
        mForce(force),
        mRest(restAng),
        mBuff(GL_ARRAY_BUFFER)
    {

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

    void TorsionNode::renderGeometry(atlas::math::Matrix4 projection, atlas::math::Matrix4 view)
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

    //function uses the math described in the textbook "Foundations of Physically Based
    // Modelling & Animation" by House and Keyser
    void TorsionNode::updateGeometry(atlas::core::Time<> const& time, int curr,
                                 atlas::math::Vector pos0, atlas::math::Vector pos2, atlas::math::Vector pos3,
                                 atlas::math::Vector vel2, atlas::math::Vector vel3, atlas::math::Vector x01){
        atlas::math::Vector tt; //total torque
        atlas::math::Vector tf; //total force applied
        atlas::math::Vector force0; //force applied to x0
        atlas::math::Vector force1; //force applied to x1
        atlas::math::Vector force2; //force applied to x2
        atlas::math::Vector force3; //force applied to x3

        float ks=10;    //spring constant
        float kd=0.05; //damping constant
        float sL;   //tangential component of velocity on left
        float sR;   //tangential component of velocity on right
        float thetaL;//angle on left
        float thetaR;//angle on right
        float tk;   //torque exerted by spring
        float td;   //torque exerted by damper
        float dT=time.deltaTime;//change in time

        //find x02 and x03
        atlas::math::Vector x02=pos2-pos0;
        atlas::math::Vector x03=pos3-pos0;
        //use x02, x03 to get d02, d03 and set them as a usable variable
        float d02=glm::dot(x02, mHinge);
        float d03=glm::dot(x03, mHinge);
        md02=d02;
        md03=d03;

        //use x02, x03 to get RL and RR and set them as a usable variable
        atlas::math::Vector RL=x02-md02*mHinge;
        atlas::math::Vector RR=x03-md03*mHinge;
        mRL=RL;
        mRR=RR;

        //get normals hinge, r on left, and r on right
        mNL=glm::normalize(glm::cross(mHinge, mRL));
        mNR=glm::normalize(glm::cross(mRR, mHinge));

        //find angle between the two vectors
        //  cos(theta)=dot(nL,nR)
        //  sin(theta)=cross(nL,nR)*h
        //  theta=atan2((cross(nL,nR)*h)/dot(nL,nR)))

        float dotNorm=glm::dot(mNL,mNR);
        atlas::math::Vector crossNorm=glm::cross(mNL,mNR);
        float thetaT=atan2(glm::dot(crossNorm,mHinge),dotNorm);

        //torque exerted on spring
        tk=ks*(thetaT-mRest);

        //find sL, sR
        //  sL=dot(velocity2, nL)
        //  sR=dot(velocity3, nR)

        sL=glm::dot(vel2, mNL);
        sR=glm::dot(vel3, mNR);

        //find rotational speed
        //  thetaL=sL/length(rL)
        //  thetaR=sR/length(rR)

        thetaL=sL/glm::length(mRL);
        thetaR=sR/glm::length(mRR);

        //find td
        //  td=-d(at theta)*(thetaL+thetaR)*mHinge

        td=-kd*(thetaL+thetaR);

        //find total torque
        //  tt=tk+td

        tt=(tk+td)*mHinge;

        //depending on which node we are currently on,
        //calculate the appropriate force and apply it to
        //the corresponding position and velocity for the node
        switch (curr){
            //find force0
            //  -(f1+f2+f3)
            case 1:

                force0=-(mF1+mF2+mF3);
                mF0=force0;
                tf=force0;
                //update velocity
                mVelocity += (dT*tf)/mMass;
                //update position
                mPosition += dT *mVelocity;
                break;

            //find force1
            //  f1=(-d02f2+d03f3)/l01
            case 2:

                force1=-(md02*mF2+md03*mF3/glm::length(x01));
                mF1=force1;
                tf=force1;
                //update velocity
                mVelocity += (dT*tf)/mMass;
                //update position
                mPosition += dT *mVelocity;
                break;

            //find force2
            //  f2=(dot(tt, mHinge)/length(rL))*nL
            case 3:

                force2=(glm::dot(tt,mHinge)/glm::length(mRL))*mNL;
                mF2=force2;
                tf=force2;
                mVelocity+=(dT*tf)/mMass;
                mPosition+=dT*mNL*glm::dot(vel2, mNL);
                break;

            //find force3
            //  f3=(dot(tt,mHinge)/length(rR))*nR
            case 4:

                force3=(glm::dot(tt,mHinge)/glm::length(mRR))*mNR;
                mF3=force3;
                tf=force3;
                mVelocity+=(dT*tf)/mMass;
                mPosition+=dT*mNR*glm::dot(vel3, mNR);
                break;
        }
    }

    //return position of current node
    atlas::math::Vector TorsionNode::getPosition(){
        return mPosition;
    }

    //return velocity of current node
    atlas::math::Vector TorsionNode::getVelocity(){
        return mVelocity;
    }

    //set the hinge vector
    void TorsionNode::setHinge(atlas::math::Vector hinge){
        mHinge=hinge;
    }
}
