#ifndef MODELLING_ASSIGNMENTS_ASSIGNMENT1_INCLUDE_TENSOR_PATCH_HPP
#define MODELLING_ASSIGNMENTS_ASSIGNMENT1_INCLUDE_TENSOR_PATCH_HPP

#pragma once

#include <atlas/utils/Geometry.hpp>
#include <atlas/gl/VertexArrayObject.hpp>
#include <atlas/gl/Buffer.hpp>

namespace assignment1
{
    class TensorPatch : public atlas::utils::Geometry
    {
    public:
        TensorPatch();
        ~TensorPatch() = default;

        void updateGeometry(atlas::core::Time<> const& t) override;
        void renderGeometry() override;

        void drawControlCage(bool draw);
        void drawControlPoints(bool draw);

    private:
        atlas::gl::VertexArrayObject mSurfaceVao;
        atlas::gl::Buffer mSurfaceBuffer;
        atlas::gl::Buffer mIndexBuffer;
        std::vector<atlas::math::Point> mSurfaceData;
        bool mDrawControlPoints;
        bool mDrawControlCage;
    };
}

#endif