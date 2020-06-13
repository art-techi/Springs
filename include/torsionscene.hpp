#ifndef TORSIONSCENE_HPP
#define TORSIONSCENE_HPP
/* Header file for angular scene.
 * No parameters passed through
 * Private variables for triangle list, node list,
 *  and line list set.
 */

#include <atlas/tools/ModellingScene.hpp>
#include <memory>
#include "TorsionNode.hpp"
#include "TorsionLine.hpp"

namespace assignment1 {
    class TorsionScene : public atlas::tools::ModellingScene{
    public:
        TorsionScene();
        void updateScene(double time) override;
        void renderScene() override;
        void setInitDis(float distance);

    private:
        std::vector<std::shared_ptr<TorsionNode>> mNodes;
        std::vector<std::shared_ptr<TorsionLine>> mLines;
    };
}

#endif // TORSIONSCENE_HPP
