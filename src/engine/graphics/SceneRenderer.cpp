#include "SceneRenderer.h"
#include "Renderer.h"
#include <algorithm>

namespace voidx {
    std::vector<QuadRenderCommand> SceneRenderer::s_Commands;
    glm::vec4 SceneRenderer::s_CameraBounds = {0.0f, 0.0f, 0.0f, 0.0f};

    void SceneRenderer::BeginScene(const OrthographicCamera& camera) {
        s_Commands.clear();
        s_CameraBounds = camera.GetVisibleBounds();
        Renderer::ResetStats();
        Renderer::BeginScene(camera);
    }

    void SceneRenderer::Submit(const QuadRenderCommand& cmd) {
        float diag = glm::length(cmd.Size) * 0.5f;
        if (cmd.Position.x - diag > s_CameraBounds.z || cmd.Position.x + diag < s_CameraBounds.x ||
            cmd.Position.y - diag > s_CameraBounds.w || cmd.Position.y + diag < s_CameraBounds.y) {
            return;
            }
        s_Commands.push_back(cmd);
    }

    void SceneRenderer::EndScene() {
        std::sort(s_Commands.begin(), s_Commands.end(), [](const QuadRenderCommand& a, const QuadRenderCommand& b) {
            if (a.ZOrder != b.ZOrder) return a.ZOrder < b.ZOrder;
            if (a.IsTransparent != b.IsTransparent) return !a.IsTransparent;
            return a.TextureID < b.TextureID;
        });

        for (const auto& cmd : s_Commands) {
            if (cmd.TextureID == 0) {
                Renderer::DrawColorQuad(cmd.Position, cmd.Size, cmd.Color, cmd.Rotation);
            } else if (cmd.UseUVs) {
                Renderer::DrawQuadUV(cmd.TextureID, cmd.Position, cmd.Size, cmd.UV, cmd.Color, cmd.Pivot, cmd.Rotation, cmd.FlipX, cmd.FlipY);
            } else {
                Renderer::DrawQuad(cmd.TextureID, cmd.Position, cmd.Size, cmd.Rotation, cmd.Color, cmd.Pivot, cmd.FlipX, cmd.FlipY);
            }
        }

        Renderer::EndScene();
        s_Commands.clear();
    }
}