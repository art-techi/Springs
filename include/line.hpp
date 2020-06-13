/* Header file for basic line.
 * Two nodes passed in
 * Private variables for two nodes, vertex array
 *  object, and buffer set
 */

#ifndef LINE_HPP
#define LINE_HPP
#include "node.hpp"

namespace assignment1
{
    class Line : public atlas::utils::Geometry
    {
    public:
        Line(std::shared_ptr<Node> node1, std::shared_ptr<Node> node2);


        ~Line() = default;

        void renderGeometry(atlas::math::Matrix4 projection, atlas::math::Matrix4 view);

    private:
        std::shared_ptr<Node> mNode1;
        std::shared_ptr<Node> mNode2;
        atlas::gl::VertexArrayObject mVAO;
        atlas::gl::Buffer mBuff;
    };
}

#endif // LINE_HPP
