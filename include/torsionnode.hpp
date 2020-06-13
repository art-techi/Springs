#ifndef TORSIONNODE_HPP
#define TORSIONNODE_HPP
/* Header file for angular node.
 * Mass, position, velocity, force, and rest angle passed in
 * Private variables for position, velocity, force, hinge, r left,
 * r right, normal left, normal right, force 0, force 1, force 2,
 * force 3, vertex array object, buffer, rest angle, d02, and d03 set
 */

#include <atlas/utils/Geometry.hpp>
#include <atlas/gl/Buffer.hpp>
#include <atlas/gl/VertexArrayObject.hpp>

namespace assignment1
{
    class TorsionNode : public atlas::utils::Geometry
    {
    public:
        TorsionNode(float mass, atlas::math::Vector position,
                atlas::math::Vector velocity, atlas::math::Vector force, float restAng);

        ~TorsionNode() = default;

        void renderGeometry(atlas::math::Matrix4 projection, atlas::math::Matrix4 view);
        void updateGeometry(atlas::core::Time<> const& time, int curr,
                            atlas::math::Vector pos0, atlas::math::Vector pos2, atlas::math::Vector pos3,
                            atlas::math::Vector vel2, atlas::math::Vector vel3, atlas::math::Vector x01);
        void setNeighbours(std::shared_ptr<TorsionNode> neighbour1, std::shared_ptr<TorsionNode> neighbour2,
                           std::shared_ptr<TorsionNode> neighbour3);
        atlas::math::Vector getPosition();
        atlas::math::Vector getVelocity();
        atlas::math::Vector getHinge();
        atlas::math::Vector getRL();
        atlas::math::Vector getRR();
        atlas::math::Vector getNL();
        atlas::math::Vector getNR();

        void setHinge(atlas::math::Vector hinge);

    private:
        float mMass;
        atlas::math::Vector mPosition;
        atlas::math::Vector mVelocity;
        atlas::math::Vector mForce;
        atlas::math::Vector mHinge;
        atlas::math::Vector mRL;
        atlas::math::Vector mRR;
        atlas::math::Vector mNL;
        atlas::math::Vector mNR;
        atlas::math::Vector mF0;
        atlas::math::Vector mF1;
        atlas::math::Vector mF2;
        atlas::math::Vector mF3;
        atlas::gl::VertexArrayObject mVAO;
        atlas::gl::Buffer mBuff;

        float mRest;
        float md02;
        float md03;
    };
}

#endif // TORSIONNODE_HPP
