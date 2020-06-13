/* Header file for angular line.
 * Two nodes passed through.
 * Private variables for each node, vertex array object,
 *  and buffer set.
 */

#ifndef ANGLINE_HPP
#define ANGLINE_HPP

#include "AngNode.hpp"

namespace assignment1
{
    class AngLine : public atlas::utils::Geometry
    {
    public:
        AngLine(std::shared_ptr<AngNode> node1, std::shared_ptr<AngNode> node2);


        ~AngLine() = default;

        void renderGeometry(atlas::math::Matrix4 projection, atlas::math::Matrix4 view);

    private:
        std::shared_ptr<AngNode> mNode1;
        std::shared_ptr<AngNode> mNode2;
        atlas::gl::VertexArrayObject mVAO;
        atlas::gl::Buffer mBuff;
    };
}

#endif // ANGLINE_HPP
