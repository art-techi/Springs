/*Student Name: Margaret (Meg) Coleman
 * Student ID: 
 * Last Updated: Sept. 24, 2017
 * CSC 473 A1
 *
 * Included files for linear spring:
 *      Line.cpp        Line.hpp
 *      Node.cpp        Node.hpp
 *      Scene.cpp       Scene.hpp
 *
 * Included files for angular spring:
 *      AngLine.cpp     AngLine.hpp
 *      AngNode.cpp     AngNode.hpp
 *      AngScene.cpp    AngScene.hpp
 *
 * Descriptions of each with functionality and relevant variables
 * described in each file.
 *
 * Uncomment line 51 for linear spring functionality.
 * Uncomment line 53 for angular spring functionality.
 *
 * Note: When I was working on this assignment with another student,
 *      we actually implemented the Torsion spring instead of the
 *      angular spring, thinking that they were the same thing.
 *      Therefore, as an extension component, please consider the
 *      implementation of torsionscene.
 */

#include <atlas/utils/Application.hpp>
#include <atlas/utils/WindowSettings.hpp>
#include <atlas/gl/ErrorCheck.hpp>

#include <atlas/tools/ModellingScene.hpp>
#include "Scene.hpp"
#include "angscene.hpp"
#include "torsionscene.hpp"

int main()
{
    using atlas::utils::WindowSettings;
    using atlas::utils::ContextVersion;
    using atlas::utils::Application;
    using atlas::utils::ScenePointer;
    using atlas::tools::ModellingScene;

    namespace a1 = assignment1;

    atlas::gl::setGLErrorSeverity(ATLAS_GL_ERROR_SEVERITY_HIGH | 
        ATLAS_GL_ERROR_SEVERITY_MEDIUM);

    WindowSettings settings;
    settings.contextVersion = ContextVersion(4, 1);
    settings.isForwardCompat = true;
    settings.isMaximized = true;
    settings.title = "Animation Assignment 1";

    Application::getInstance().createWindow(settings);
    //uncomment line below for linear spring
    Application::getInstance().addScene(ScenePointer(new a1::Scene));
    //uncomment line below for angular spring
    //Application::getInstance().addScene(ScenePointer(new a1::AngScene));
    //uncomment line below for torsion spring
    //Application::getInstance().addScene(ScenePointer(new a1::TorsionScene));
    Application::getInstance().runApplication();

    return 0;
}
