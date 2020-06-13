#include "BSpline.hpp"
#include "AssignmentPaths.hpp"
#include "DeBoor.hpp"

namespace assignment1
{
    BSpline::BSpline() :
        mSplineBuffer(GL_ARRAY_BUFFER),
        mControlBuffer(GL_ARRAY_BUFFER),
        mDegree(0),
        mDrawCtrlPts(true),
        mDrawCtrlPoly(false),
        mDrawParametric(false)
    {
        namespace math = atlas::math;
        namespace gl = atlas::gl;

        mControlPts =
        {
            {0, 2, 0},
            {1, 3, 0},
            {2, 1, 0},
            {3, 4, 0},
            {4, 0.5f, 0},
            {5, 2.5f, 0}
        };

        mControlVao.bindVertexArray();
        mControlBuffer.bindBuffer();
        mControlBuffer.bufferData(gl::size<math::Point>(mControlPts.size()),
            mControlPts.data(), GL_STATIC_DRAW);
        mControlBuffer.vertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0,
            gl::bufferOffset<float>(0));
        mControlVao.enableVertexAttribArray(0);
        mControlBuffer.unBindBuffer();
        mControlVao.unBindVertexArray();

        mModel = math::Matrix4(1.0f);

        std::vector<gl::ShaderUnit> shaders
        {
            { getShaderDirectory() + "BSpline.vs.glsl", GL_VERTEX_SHADER },
            { getShaderDirectory() + "BSpline.fs.glsl", GL_FRAGMENT_SHADER }
        };

        mShaders.push_back(gl::Shader(shaders));
        mShaders[0].setShaderIncludeDir(getShaderDirectory());
        mShaders[0].compileShaders();
        mShaders[0].linkShaders();

        mSplineVao.bindVertexArray();
        mSplineBuffer.bindBuffer();
        mSplineBuffer.bufferData(gl::size<math::Vector>(0),
            nullptr, GL_DYNAMIC_DRAW);
        mSplineBuffer.vertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0,
            gl::bufferOffset<float>(0));
        mSplineVao.enableVertexAttribArray(0);

        auto var = mShaders[0].getUniformVariable("model");
        mUniforms.insert(UniformKey("model", var));

        var = mShaders[0].getUniformVariable("colour");
        mUniforms.insert(UniformKey("colour", var));

        mSplineBuffer.unBindBuffer();
        mSplineVao.unBindVertexArray();
        mShaders[0].disableShaders();

        generateSplines();
        updateSpline();
    }

    void BSpline::setDegree(int degree)
    {
        if (mDegree != degree)
        {
            mDegree = degree;
            updateSpline();
        }
    }

    void BSpline::drawControlPoints(bool draw)
    {
        mDrawCtrlPts = draw;
    }

    void BSpline::drawControlPolygon(bool draw)
    {
        mDrawCtrlPoly = draw;
    }

    void BSpline::drawParametricSpace(bool draw)
    {
        mDrawParametric = draw;
        updateSpline();
    }

    void BSpline::renderGeometry()
    {
        mShaders[0].hotReloadShaders();
        if (!mShaders[0].shaderProgramValid())
        {
            return;
        }

        mShaders[0].enableShaders();

        glUniformMatrix4fv(mUniforms["model"], 1, GL_FALSE, &mModel[0][0]);

        // Control points are red.
        glUniform4f(mUniforms["colour"], 1, 0, 0, 1);

        mControlVao.bindVertexArray();
        if (mDrawCtrlPts)
        {
            glPointSize(10.0f);
            glDrawArrays(GL_POINTS, 0, (GLsizei)mControlPts.size());
            glPointSize(1.0f);
        }

        if (mDrawCtrlPoly)
        {
            glDrawArrays(GL_LINE_STRIP, 0, (GLsizei)mControlPts.size());
        }

        mControlVao.unBindVertexArray();

        mSplineVao.bindVertexArray();

        // The spline is dark blue.
        glUniform4f(mUniforms["colour"], 9.0f / 255.0f, 3.0f / 255.0f,
            47 / 255.0f, 1.0f);

        if (mDrawParametric)
        {
            for (std::size_t i = 0; i < mControlPts.size(); ++i)
            {
                glDrawArrays(GL_LINE_STRIP, (GLint)i * 100, 100);
            }
        }
        else
        {
            if (mDegree > 0)
            {
                glDrawArrays(GL_LINE_STRIP, 0, (GLsizei)mNumPts);
            }
        }

        mSplineVao.unBindVertexArray();
        mShaders[0].disableShaders();
    }

    void BSpline::generateSplines()
    {
        namespace math = atlas::math;
        using atlas::core::iotaStride;

        for (int i = 0; i < 6; ++i)
        {
            std::vector<float> knotVector(i + 1 + 6);
            iotaStride(std::begin(knotVector), std::end(knotVector), 0.0f, 1.0f);
            mKnotVectors.push_back(knotVector);
        }

        // Modify knot vectors here (if needed).

        std::vector<math::Point> splineData;

        createSpline<1>(mControlPts, mKnotVectors[0], splineData, 100);
        mSplines.push_back(splineData);

        createSpline<1>(mControlPts, mKnotVectors[0], splineData, 100, true);
        mParametrics.push_back(splineData);

        createSpline<2>(mControlPts, mKnotVectors[1], splineData, 100);
        mSplines.push_back(splineData);

        createSpline<2>(mControlPts, mKnotVectors[1], splineData, 100, true);
        mParametrics.push_back(splineData);

        createSpline<3>(mControlPts, mKnotVectors[2], splineData, 100);
        mSplines.push_back(splineData);

        createSpline<3>(mControlPts, mKnotVectors[2], splineData, 100, true);
        mParametrics.push_back(splineData);

        createSpline<4>(mControlPts, mKnotVectors[3], splineData, 100);
        mSplines.push_back(splineData);

        createSpline<4>(mControlPts, mKnotVectors[3], splineData, 100, true);
        mParametrics.push_back(splineData);

        createSpline<5>(mControlPts, mKnotVectors[4], splineData, 100);
        mSplines.push_back(splineData);

        createSpline<5>(mControlPts, mKnotVectors[4], splineData, 100, true);
        mParametrics.push_back(splineData);

        createSpline<6>(mControlPts, mKnotVectors[5], splineData, 100);
        mSplines.push_back(splineData);

        createSpline<6>(mControlPts, mKnotVectors[5], splineData, 100, true);
        mParametrics.push_back(splineData);
    }

    void BSpline::updateSpline()
    {
        namespace math = atlas::math;
        namespace gl = atlas::gl;

        auto& splineData = (mDrawParametric) ? mParametrics[mDegree] :
            mSplines[mDegree];

        mSplineBuffer.bindBuffer();
        mSplineBuffer.bufferData(gl::size<math::Vector>(splineData.size()),
            splineData.data(), GL_DYNAMIC_DRAW);
        mSplineBuffer.unBindBuffer();
        mNumPts = splineData.size();
    }
}