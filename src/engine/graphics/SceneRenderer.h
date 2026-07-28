#pragma once
#include "RenderCommand.h"
#include "Camera/OrthographicCamera.h"
#include <vector>

namespace voidx {
    class SceneRenderer {
    public:
        static void BeginScene(const OrthographicCamera& camera);
        static void Submit(const QuadRenderCommand& cmd);
        static void EndScene();

    private:
        static void SortAndFlush();

        static std::vector<QuadRenderCommand> s_Commands;
        static glm::vec4 s_CameraBounds;
    };
}