#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <atlas/utils/Geometry.hpp>
#include <atlas/gl/VertexArrayObject.hpp>
#include <atlas/gl/Buffer.hpp>
#include <memory>

namespace assignment1{
    class Triangle : public atlas::utils::Geometry{
    public:
        Triangle(float mass,atlas::math::Vector position, atlas::math::Vector velocity, float angle,
                  atlas::math::Vector pt1, atlas::math::Vector pt2, atlas::math::Vector pt3,
                 atlas::math::Vector L1, atlas::math::Vector L2, atlas::math::Vector L3);
        ~Triangle()=default;

        void renderGeometry(atlas::math::Matrix4 projection, atlas::math::Matrix4 view);
        void updateGeometry(atlas::core::Time<> const& time,
                            atlas::math::Vector L1, atlas::math::Vector L2, atlas::math::Vector L3,
                            float r, float ang);
        //void updateGeometry(atlas::core::Time<> const& time, float r, float theta);
        void setInitDis(float distance);
        atlas::math::Vector getPosition();

    private:
        atlas::gl::VertexArrayObject mVAO;
        atlas::gl::Buffer mBuff;
        atlas::math::Vector mPosition;
        atlas::math::Vector mVelocity;
        float mMass;
        float mRotate;
        float mInitDis;
    };
}

#endif // TRIANGLE_H
