#include "TensorPatch.hpp"
#include "AssignmentPaths.hpp"

namespace assignment1
{
    TensorPatch::TensorPatch() :
        mSurfaceBuffer(GL_ARRAY_BUFFER),
        mIndexBuffer(GL_ELEMENT_ARRAY_BUFFER),
        mDrawControlPoints(true),
        mDrawControlCage(false)
    {
        using atlas::math::Point;
        using atlas::math::Matrix4;
        namespace gl = atlas::gl;

        mModel = glm::scale(Matrix4(1.0f), Point(10.0f)) * 
            glm::translate(Matrix4(1.0f), Point(2.0f, 0, 0));

        // Construct the initial mesh of control points.
        float step = 2 / 3.0f;
        for (int i = 0; i < 4; ++i)
        {
            for (int k = 0; k < 4; ++k)
            {
                mSurfaceData.push_back(Point(
                    -1.0f - k * step,
                    -1.0f + i * step,
                    0.0f
                ));
            }
        }

        const GLushort indices[] = 
        {
            0, 1, 1, 2, 2, 3,
            4, 5, 5, 6, 6, 7,
            8, 9, 9, 10, 10, 11,
            12, 13, 13, 14, 14, 15,

            0, 4, 4, 8, 8, 12,
            1, 5, 5, 9, 9, 13,
            2, 6, 6, 10, 10, 14,
            3, 7, 7, 11, 11, 15
        };

        mSurfaceVao.bindVertexArray();
        mSurfaceBuffer.bindBuffer();
        mSurfaceBuffer.bufferData(gl::size<Point>(mSurfaceData.size()),
            mSurfaceData.data(), GL_DYNAMIC_DRAW);
        mSurfaceBuffer.vertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0,
            gl::bufferOffset<float>(0));
        mSurfaceVao.enableVertexAttribArray(0);

        mIndexBuffer.bindBuffer();
        mIndexBuffer.bufferData(gl::size<GLushort>(sizeof(indices)),
            indices, GL_STATIC_DRAW);

        std::vector<gl::ShaderUnit> patchShaders
        {
            { getShaderDirectory() + "TensorPatch.vs.glsl", GL_VERTEX_SHADER },
            { getShaderDirectory() + "TensorPatch.tcs.glsl", 
            GL_TESS_CONTROL_SHADER },
            { getShaderDirectory() + "TensorPatch.tes.glsl", 
            GL_TESS_EVALUATION_SHADER },
            { getShaderDirectory() + "TensorPatch.fs.glsl", GL_FRAGMENT_SHADER }
        };

        mShaders.push_back(gl::Shader(patchShaders));
        mShaders[0].setShaderIncludeDir(getShaderDirectory());
        mShaders[0].compileShaders();
        mShaders[0].linkShaders();

        auto var = mShaders[0].getUniformVariable("model");
        mUniforms.insert(UniformKey("modelS", var));

        std::vector<gl::ShaderUnit> controlShaders
        {
            { getShaderDirectory() + "TensorControl.vs.glsl", GL_VERTEX_SHADER },
            { getShaderDirectory() + "TensorControl.fs.glsl", GL_FRAGMENT_SHADER }
        };

        mShaders.push_back(gl::Shader(controlShaders));
        mShaders[1].setShaderIncludeDir(getShaderDirectory());
        mShaders[1].compileShaders();
        mShaders[1].linkShaders();

        var = mShaders[1].getUniformVariable("model");
        mUniforms.insert(UniformKey("modelC", var));

        var = mShaders[1].getUniformVariable("colour");
        mUniforms.insert(UniformKey("colour", var));

        mIndexBuffer.unBindBuffer();
        mSurfaceBuffer.unBindBuffer();
        mSurfaceVao.unBindVertexArray();
        mShaders[0].disableShaders();
        mShaders[1].disableShaders();
    }

    void TensorPatch::updateGeometry(atlas::core::Time<float> const& t)
    {
        using atlas::math::Point;
        namespace gl = atlas::gl;

        for (std::size_t i = 0; i < mSurfaceData.size(); ++i)
        {
            float fi = (float)i / 16.0f;
            mSurfaceData[i].z = glm::sin(t.totalTime * (0.2f + fi * 0.3f));
        }

        mSurfaceBuffer.bindBuffer();
        mSurfaceBuffer.bufferData(gl::size<Point>(mSurfaceData.size()),
            mSurfaceData.data(), GL_DYNAMIC_DRAW);
        mSurfaceBuffer.unBindBuffer();
    }

    void TensorPatch::renderGeometry()
    {
        using atlas::math::Point4;

        mShaders[0].hotReloadShaders();
        if (!mShaders[0].shaderProgramValid())
        {
            return;
        }

        mShaders[1].hotReloadShaders();
        if (!mShaders[1].shaderProgramValid())
        {
            return;
        }

        mShaders[0].enableShaders();
        glUniformMatrix4fv(mUniforms["modelS"], 1, GL_FALSE, &mModel[0][0]);

        mSurfaceVao.bindVertexArray();
        glPatchParameteri(GL_PATCH_VERTICES, 16);
        glDrawArrays(GL_PATCHES, 0, 16);

        mShaders[0].disableShaders();
        mShaders[1].enableShaders();

        glUniformMatrix4fv(mUniforms["modelC"], 1, GL_FALSE, &mModel[0][0]);


        mIndexBuffer.bindBuffer();
        Point4 colour(1, 0, 0, 1);
        if (mDrawControlPoints)
        {
            glPointSize(10.0f);
            glUniform4fv(mUniforms["colour"], 1, &colour[0]);
            glDrawArrays(GL_POINTS, 0, 16);
            glPointSize(1.0f);
        }

        if (mDrawControlCage)
        {
            glUniform4fv(mUniforms["colour"], 1, &colour[0]);
            glDrawElements(GL_LINES, 48, GL_UNSIGNED_SHORT, nullptr);
        }
        mIndexBuffer.unBindBuffer();

        mShaders[1].disableShaders();
        mSurfaceVao.unBindVertexArray();
    }

    void TensorPatch::drawControlCage(bool draw)
    {
        mDrawControlCage = draw;
    }

    void TensorPatch::drawControlPoints(bool draw)
    {
        mDrawControlPoints = draw;
    }
}
