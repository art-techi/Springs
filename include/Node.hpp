/* Header file for basic node.
 * Mass, position, velocity, force passed in.
 * Private variables for mass, position, velocity,
 *  force, neighbour, vertex array object, buffer, and
 *  initial distance set
 */

#ifndef MODELLING_ASSIGNMENTS_ASSIGNMENT1_INCLUDE_B_SPLINE_HPP
#define MODELLING_ASSIGNMENTS_ASSIGNMENT1_INCLUDE_B_SPLINE_HPP

#pragma once

#include <atlas/utils/Geometry.hpp>
#include <atlas/utils/Mesh.hpp>
#include <atlas/gl/Buffer.hpp>
#include <atlas/gl/VertexArrayObject.hpp>

namespace assignment1
{
    class Node : public atlas::utils::Geometry
    {
    public:
        Node(float mass, atlas::math::Vector position,
             atlas::math::Vector velocity, atlas::math::Vector force);

        ~Node() = default;

        void renderGeometry(atlas::math::Matrix4 projection, atlas::math::Matrix4 view);
        void updateGeometry(atlas::core::Time<> const& time) override;
        void setNeighbour(std::shared_ptr<Node> neighbour);
        atlas::math::Vector getPosition();
        void setInitDis(float distance);

    private:
        float mMass;
        atlas::math::Vector mPosition;
        atlas::math::Vector mVelocity;
        atlas::math::Vector mForce;
        std::shared_ptr<Node> mNeighbour;
        atlas::gl::VertexArrayObject mVAO;
        atlas::gl::Buffer mBuff;
        float mInitDis;
    };
}


#endif
