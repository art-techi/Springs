#ifndef TORSIONLINE_HPP
#define TORSIONLINE_HPP
/* Header file for angular line.
 * Two nodes passed through.
 * Private variables for each node, vertex array object,
 *  and buffer set.
 */

#include "TorsionNode.hpp"

namespace assignment1
{
    class TorsionLine : public atlas::utils::Geometry
    {
    public:
        TorsionLine(std::shared_ptr<TorsionNode> node1, std::shared_ptr<TorsionNode> node2);


        ~TorsionLine() = default;

        void renderGeometry(atlas::math::Matrix4 projection, atlas::math::Matrix4 view);

    private:
        std::shared_ptr<TorsionNode> mNode1;
        std::shared_ptr<TorsionNode> mNode2;
        atlas::gl::VertexArrayObject mVAO;
        atlas::gl::Buffer mBuff;
    };
}

#endif // TORSIONLINE_HPP
