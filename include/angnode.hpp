/* Header file for angular node.
 * Mass, position, velocity, force, and rest angle passed in
 * Private variables for position, velocity, force, hinge, r left,
 * r right, normal left, normal right, force 0, force 1, force 2,
 * force 3, vertex array object, buffer, rest angle, d02, and d03 set
 */

#ifndef ANGNODE_H
#define ANGNODE_H

#include <atlas/utils/Geometry.hpp>
#include <atlas/gl/Buffer.hpp>
#include <atlas/gl/VertexArrayObject.hpp>

namespace assignment1
{
    class AngNode : public atlas::utils::Geometry
    {
    public:
        AngNode(float mass, atlas::math::Vector position,
                atlas::math::Vector velocity, atlas::math::Vector force, atlas::math::Vector restAng);

        ~AngNode() = default;

        void renderGeometry(atlas::math::Matrix4 projection, atlas::math::Matrix4 view);
        void updateGeometry(atlas::core::Time<> const& time);

        atlas::math::Vector getPosition();
        void setHinge(atlas::math::Vector hinge);

    private:
        float mMass;
        atlas::math::Vector mPosition;
        atlas::math::Vector mVelocity;
        atlas::math::Vector mForce;
        atlas::math::Vector mHinge;
        atlas::math::Vector mRest;
        atlas::gl::VertexArrayObject mVAO;
        atlas::gl::Buffer mBuff;

        float mAngularVel;
    };
}

#endif // ANGNODE_H
