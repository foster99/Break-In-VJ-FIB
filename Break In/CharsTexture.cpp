#include "CharsTexture.h"
#include <iostream>
#include <glm\gtc\matrix_transform.hpp>

CharsTexture::CharsTexture() {
	texQuads = vector<TexturedQuad*>(1,NULL);
	tex.loadFromFile("images/chars.png", TEXTURE_PIXEL_FORMAT_RGBA);
}

void CharsTexture::render()
{
	glm::mat4 modelview = glm::mat4(1.0f);

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);

	for(TexturedQuad* texQuad : texQuads) {
		texQuad->render(tex);
	}
}


void CharsTexture::print(glm::vec2 coords, char c) {
	
	constexpr float stride = 1 / 39;
	constexpr float numDispl = (27.0 + 2.0) * stride;

	float s = 0, t = 0;
	if ('0' <= c && c <= '9')
		s = numDispl + (stride * (c - '0'));

	else if ('A' <= c && c <= 'Z')
		s = stride * (c - 'A');

	else if ('a' <= c && c <= 'z')
		s = stride * (c - 'a');

	else if (c == '.')
		s = stride * 27.0;

	else if (c == ':')
		s = stride * 28.0;

	else return;


	glm::vec2 geom[2] = { glm::vec2(8.f*coords.x, 8.f * coords.y) ,glm::vec2(8.f * coords.x + 8.f, 8.f * coords.y + 8.f) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f,0.f), glm::vec2(1.f,1.f) };

	TexturedQuad* texQuad = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);

	projection = glm::ortho(0.f, float(Game::SCREEN_WIDTH - 1), float(Game::SCREEN_HEIGHT - 1), 0.f);
}


void CharsTexture::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
}
