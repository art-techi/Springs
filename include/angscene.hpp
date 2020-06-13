/* Header file for angular scene.
 * No parameters passed through
 * Private variables for triangle list, node list,
 *  and line list set.
 */

#ifndef ANGSCENE_H
#define ANGSCENE_H

#include <atlas/tools/ModellingScene.hpp>
#include <memory>
#include "angnode.hpp"
#include "AngLine.hpp"

namespace assignment1 {
    class AngScene : public atlas::tools::ModellingScene{
    public:
        AngScene();
        void updateScene(double time) override;
        void renderScene() override;
        void setInitDis(float distance);

    private:
        std::vector<std::shared_ptr<AngNode>> mNodes;
        std::vector<std::shared_ptr<AngLine>> mLines;
    };
}

#endif // ANGSCENE_H
