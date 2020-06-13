#include "Triangle.hpp"
#include "AssignmentPaths.hpp"
#include <iostream>
#include <math.h>

namespace gl=atlas::gl;
namespace math=atlas::math;

namespace assignment1{
    Triangle::Triangle(float mass,atlas::math::Vector position, atlas::math::Vector velocity, float angle,
                       atlas::math::Vector pt1, atlas::math::Vector pt2, atlas::math::Vector pt3,
                       atlas::math::Vector L1, atlas::math::Vector L2, atlas::math::Vector L3):
    mPosition(position),
    mVelocity(velocity),
    mMass(mass),
    mRotate(angle),
    mBuff(GL_ARRAY_BUFFER)

    {
        std::vector<math::Point> tPositions {
            pt1, pt2, pt3
        };

        mVAO.bindVertexArray();
        mBuff.bindBuffer();
        mBuff.bufferData(gl::size<math::Point>(tPositions.size()),
            tPositions.data(), GL_STATIC_DRAW);
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

    void Triangle::renderGeometry(atlas::math::Matrix4 projection, atlas::math::Matrix4 view){
        mShaders[0].hotReloadShaders();

        if (!mShaders[0].shaderProgramValid())
        {
            return;
        }

        mShaders[0].enableShaders();

        mModel = math::Matrix4(1);
        mModel = glm::translate(mModel, mPosition);
        mModel = glm::rotate(mModel, mRotate, math::Vector(0,0,1));

        glUniformMatrix4fv(mUniforms["model"], 1, GL_FALSE, &mModel[0][0]);
        glUniformMatrix4fv(mUniforms["view"], 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(mUniforms["projection"], 1, GL_FALSE, &projection[0][0]);

        // Control points are red.
        glUniform4f(mUniforms["colour"], 1, 0, 0, 1);

        mVAO.bindVertexArray();
        glDrawArrays(GL_TRIANGLES, 0, 3);
        mVAO.unBindVertexArray();
        mShaders[0].disableShaders();
    }

    void Triangle::updateGeometry(atlas::core::Time<> const& time,
                                  atlas::math::Vector L1, atlas::math::Vector L2, atlas::math::Vector L3,
                                  float r, float ang){

        atlas::math::Vector fg = atlas::math::Vector(0, -0.1, 0);//gravity
        atlas::math::Vector norm; //normal
        atlas::math::Vector fs; //spring force
        atlas::math::Vector fd; //damping force
        atlas::math::Vector tf; //total force

        float h;
        float al;

        float ks=0.2;//spring constant
        float kd=0.05;//damping constant
        float dT=time.deltaTime;//delta time

        //find angle between perpL1, L2
        //cos(theta)=(L1dotL2)/(magnitude L1* magnitude L2)
        float angleT; //angle between L1, L2
        float top=glm::dot(L1,L2);//dot product
        float mag1=glm::length(L1);//magnitude of L1
        float mag2=glm::length(L2);//magnitude of L2
        float bottom=mag1*mag2;//dot product of magnitudes

        angleT=acos(top/bottom); //angle
        //std::cout<< "angle is: "<<angleT<<std::endl;

        //ks(angleT-initial angle)-(kd(aVel(t)-initial vel)
        float angVel;
        angVel=(glm::length(mVelocity)*sin(ang))/r;

        tf=ks*(angleT-ang)-(kd*(angVel)-mVelocity);

        //find L3
        /*atlas::math::Vector L3New;
        L3New=L2-L1;
        float mag3=glm::length(L2-L1);//magnitude of L3

        h=mag2*cos(angleT);
        al=mag1+mag2*sin(angleT);
        float alpha=atan(h/al);
        float beta=(3.14/2)-(angleT+alpha);

        float ftang=ks*(mag3-glm::length(L3))*sin(beta);
        tf=ftang*L2; //torque

        //compute spring force fs
        //spring constant
        fs=ks*(-norm)*(mag3-mInitDis);

        //compute damping force fd
        //fd=vr(relative velocity)-N(normalization of position*(vr.n)
        fd=-kd*mVelocity;


        tf=fs+fd;*/
        mPosition+=dT*mVelocity;
        mVelocity+=((dT*tf)/mMass);
    }

    void Triangle::setInitDis(float distance){
        mInitDis=distance;
    }

    atlas::math::Vector Triangle::getPosition(){
        return mPosition;
    }
}
