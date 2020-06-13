/* Holds constructor for node shared pointer object.
 * Mass, position, velocity, and force are passed in.
 *
 * renderGeometry: uploads the shaders to the system.
 * updateGeometry: uses the content from lab to update
 *      the position and velocity of the nodes for each
 *      iterative step
 * setNeighbour: sets the node's neighbour
 * getPosition: returns the node's position
 * setInitDis: sets the initial distance between two nodes
 */

#include "Node.hpp"
#include "AssignmentPaths.hpp"


#include <atlas/core/ImGUI.hpp>
#include <iostream>

namespace gl=atlas::gl;
namespace math=atlas::math;

namespace assignment1{
    Node::Node(float mass, atlas::math::Vector position, atlas::math::Vector velocity,
               atlas::math::Vector force) :
        mMass(mass),
        mPosition(position),
        mVelocity(velocity),
        mForce(force),
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

    void Node::renderGeometry(atlas::math::Matrix4 projection, atlas::math::Matrix4 view)
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

    //content from lab used in this function
    void Node::updateGeometry(atlas::core::Time<> const& time){
        atlas::math::Vector fg = atlas::math::Vector(0, -9.81*mMass, 0); //gravity
        atlas::math::Vector norm; //normal
        atlas::math::Vector fs; //spring force
        atlas::math::Vector fd; //damping force
        atlas::math::Vector tf; //total force
        atlas::math::Vector pos;//position
        atlas::math::Vector vel;//velocity

        float lt;   //length
        float ks=0.2; //spring constant
        float kd=0.05;//damping constant
        float dT=time.deltaTime;//change in time

        //compute spring length
        lt=glm::length(mPosition-mNeighbour->mPosition);

        //compute spring force fs
        norm=(mPosition - mNeighbour->mPosition)/lt;
        fs=ks*(-norm)*(lt-mInitDis);

        //compute damping force fd
        fd=-kd*mVelocity;

        //compute total force tf
        tf=fs+fd+fg;

        //update velocity
        mVelocity += ((dT*tf)/mMass);

        //update position
        mPosition += dT * mVelocity;

    }

    //set current node's neighbour to neighbour passed in
    void Node::setNeighbour(std::shared_ptr<Node> neighbour){
        mNeighbour=neighbour;
    }

    //return the position of the current node
    atlas::math::Vector Node::getPosition(){
        return mPosition;
    }

    //set the initial distance between current node and
    //neighbour (passed in)
    void Node::setInitDis(float distance){
        mInitDis=distance;
    }   
}
