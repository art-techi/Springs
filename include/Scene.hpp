/* Header file for basic scene.
 * No parameters passed in
 * Private variable for node and line lists set.
 */

#ifndef SCENE_H
#define SCENE_H

#include <atlas/tools/ModellingScene.hpp>
#include <memory>
#include "Node.hpp"
#include "line.hpp"

namespace assignment1 {
    class Scene : public atlas::tools::ModellingScene{
    public:
        Scene();

        void updateScene(double time) override;
        void renderScene() override;

    private:
        std::vector<std::shared_ptr<Node>> mNodes;
        std::vector<std::shared_ptr<Line>> mLines;
    };
}

#endif // SCENE_H
