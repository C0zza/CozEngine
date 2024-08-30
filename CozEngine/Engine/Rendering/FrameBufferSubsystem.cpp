#include "stdafx.h"

#include "FrameBufferSubsystem.h"

#include "Rendering/FrameBuffer.h"

namespace
{
    float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };
}

void LFrameBufferSubsystem::Initialize()
{
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    ScreenTextureShader = LResourceManager::GetResource<LShader>("Engine/Content/Shaders/ScreenTextureShader.casset");
}

void LFrameBufferSubsystem::Deinitialize()
{
    glDeleteVertexArrays(1, &quadVAO);
    glDeleteBuffers(1, &quadVBO);
}

void LFrameBufferSubsystem::DrawFrameBuffer(const LFrameBuffer* FrameBuffer)
{
    ScreenTextureShader->Use();
    glBindVertexArray(quadVAO);
    glDisable(GL_DEPTH_TEST);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, FrameBuffer->GetTexture());
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
