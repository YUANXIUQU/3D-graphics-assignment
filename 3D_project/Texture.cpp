#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <SDL2/SDL_ttf.h>
#include <stb_image.h>
#include <iostream>
#include <stdexcept>

Texture::Texture() :
	m_dirty(false),
	m_size(0, 0),
	m_id(0)
{}
Texture::Texture(const std::string& _path) :
	m_dirty(false),
	m_size(0,0),
	m_id(0)
{
	load(_path);
}

// Load cubemap texture from 6 image faces
Texture::Texture(std::vector<std::string> faces) :
	m_dirty(false),
	m_size(0, 0),
	m_id(0)
{
	load(faces);
}
// Load texture from text string using SDL_ttf
Texture::Texture(const std::string& _text, TTF_Font* font, SDL_Color color):
	m_dirty(false),
	m_size(0, 0),
	m_id(0)
{
	load(_text,font,color);
}



Texture::~Texture()
{
	if (m_id)
	{
		glDeleteTextures(1, &m_id);
	}
}

glm::ivec2 Texture::size() const
{
	return m_size;
}

void Texture::load(const std::string& _path)
{
	unsigned char* data = stbi_load(_path.c_str(), &m_size.x, &m_size.y, NULL, 4);

	if (!data)
	{
		throw std::runtime_error("Failed to load texture");
	}

	for (size_t i = 0; i < m_size.x * m_size.y * 4; ++i)
	{
		m_data.push_back(data[i]);
	}

	m_dirty = true;
	free(data);
}


// loads 6 textures for a cubemap
void Texture::load(std::vector<std::string> faces)
{
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &m_size.x, &m_size.y, NULL, 4);
		if (data)
		{
			for (size_t c = 0; c < m_size.x * m_size.y * 4; ++c)
			{
				m_data.push_back(data[c]);
			}
		}
		free(data);
	}

	m_dirty = true;
}

// loads texture from text using SDL_ttf
void Texture::load(const std::string& _text, TTF_Font* font, SDL_Color color)
{
	SDL_Surface* surface = TTF_RenderText_Blended(font, _text.c_str(), color);

	if (!surface) {
		std::cerr << "Failed to render text: " << TTF_GetError() << std::endl;
		SDL_FreeSurface(surface);
	}
	// convert to RGBA format
	SDL_Surface* rgbaSurface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);
	if (!rgbaSurface) {
		std::cerr << "Failed to convert surface format: " << SDL_GetError() << std::endl;
		SDL_FreeSurface(surface);
		return;
	}
	m_size.x = rgbaSurface->w;
	m_size.y = rgbaSurface->h;

	m_data.clear();  // clear previous texture data

	unsigned char* data = static_cast<unsigned char*>(rgbaSurface->pixels);
	for (int i = 0; i < m_size.x * m_size.y * 4; ++i)
	{
		m_data.push_back(data[i]);
	}

	m_dirty = true;
	// clean up SDL surfaces
	SDL_FreeSurface(rgbaSurface);
	SDL_FreeSurface(surface);
}


GLuint Texture::cubemap_id()
{
	if (!m_id)
	{
		glGenTextures(1, &m_id);
	}
	if (m_dirty)
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);
		for (GLuint i = 0; i < 6; ++i)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
		        0, GL_RGBA, m_size.x, m_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE,
				&m_data[i * m_size.x * m_size.y * 4]);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	return m_id;
}


GLuint Texture::id()
{
	if (!m_id)
	{
		glGenTextures(1, &m_id);
	}

	if (m_dirty)
	{
		glBindTexture(GL_TEXTURE_2D, m_id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_size.x, m_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_data.data());//the pointer to the m_data arrat
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		m_dirty = false;
	}

	return m_id;
}