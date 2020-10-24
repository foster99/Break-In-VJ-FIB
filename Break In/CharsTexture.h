#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "Texture.h"
#include "TexturedQuad.h"
#include "Game.h"

class CharsTexture {
public:

    CharsTexture();
    void render();
    void print(glm::vec2 coords, char c);

private:

    void initShaders();

    Texture tex;
    ShaderProgram texProgram;
    vector<TexturedQuad*> texQuads;
    glm::mat4 projection;
};

