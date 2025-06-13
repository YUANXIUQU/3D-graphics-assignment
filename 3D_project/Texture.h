#pragma once
#include <string>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <SDL2/SDL_ttf.h>

struct Texture
{
	Texture();
	Texture(const std::string& _path);
	Texture(std::vector<std::string> faces);
	Texture(const std::string& _text, TTF_Font* font, SDL_Color color);
	~Texture();
	//Texture(glm::ivec2 _size);
	//Texture(const Texture& _copy);
	//Texture& operator=(const Texture& _assign);
	//~Texture();

	//void size(glm::ivec2 _size);
	glm::ivec2 size() const;

	void load(const std::string& _path);
	void load(std::vector<std::string> faces);
	void load(const std::string& _text, TTF_Font* font, SDL_Color color);
	//void pixel(glm::ivec2 _position, const glm::vec4& _color);
	//glm::vec4 pixel(glm::ivec2 _position) const;

	GLuint id();
	GLuint cubemap_id();

private:
	bool m_dirty;
	std::vector<unsigned char> m_data;
	glm::ivec2 m_size;
	GLuint m_id;

};
