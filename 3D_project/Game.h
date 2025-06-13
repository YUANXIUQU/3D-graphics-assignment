#pragma once
#include <iostream>
#include <cstdlib>  
#include <ctime> 
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ShaderProgram.h"
#include <stb_image.h>
#include <fstream>

#include "Window.h"
#include "Texture.h"
#include "Model.h"
#include "Skybox.h"
#include "Track.h"
#include "Player.h"
#include "Barrier.h"
#include "BarrierPool.h"
#include "RenderTexture.h"
#include "Gas.h"

struct Game
{
public:
	Game();
	~Game();
	void run();
	void menu();
	void resetGame();
	void loadHighScore();
	void saveHighScore();
	Texture carTexture;
	Texture barrierTexture;
	Texture roadTexture;
	Texture gasTexture;
	Model gasModel;
	Model carModel;
	Model barrierModel;
	Player player;

	const char* getSkyboxVert()
	{
		return "attribute vec3 a_Position;"\
			"uniform mat4 u_Projection;" \
			"uniform mat4 u_Model;" \
			"uniform mat4 u_View; "\
			"varying vec3 v_TexCoord;               " \
			"void main()                            " \
			"{                                      " \
			" v_TexCoord = a_Position;              " \
			" gl_Position = u_Projection * u_View * u_Model * vec4(a_Position,1.0); " \
			"}                                      ";
	}
	const char* getSkyboxFrag()
	{
		return "varying vec3 v_TexCoord;               " \
			"uniform samplerCube skybox;             "\
			"void main()                            " \
			"{                                      " \
			" gl_FragColor = texture(skybox, v_TexCoord); " \
			"}                                      ";
	}

	// basic vertex shader for 2D textured rendering
	const char* getDefaultVert()
	{
		return "uniform mat4 u_Projection;" \
			"uniform mat4 u_Model;" \
			"attribute vec3 a_Position;            " \
			"attribute vec2 a_TexCoord;             " \
			"varying vec2 v_TexCoord;               " \
			"void main()                            " \
			"{                                      " \
			" vec3 pos = a_Position;                " \
			" gl_Position = u_Projection * u_Model * vec4(pos,1.0); " \
			" v_TexCoord = a_TexCoord;              " \
			"}                                      ";
	}

	// vertex shader for animated text
	const char* getTextVert()
	{
		return "uniform mat4 u_Projection;" \
			"uniform float u_Time; "\
			"uniform mat4 u_Model;" \
			"attribute vec3 a_Position;            " \
			"attribute vec2 a_TexCoord;             " \
			"varying vec2 v_TexCoord;               " \
			"void main()                            " \
			"{                                      " \
			" vec3 pos = a_Position;                " \
			//dynamic geometry
			" float wave = sin(u_Time * 1.3 + pos.y * 10.0) * 10.1;" \
			" float waveZ = sin(u_Time * 10.0 + pos.x * 5.0) * 5;" \
			" pos.x += wave;      " \
			" pos.y += waveZ;" \
			" gl_Position = u_Projection * u_Model * vec4(pos,1.0); " \
			" v_TexCoord = a_TexCoord;              " \
			"}                                      ";
	}

	// fragment shader for 2D texture rendering with transparency
	const char* getDefaultFrag()
	{
		return 
			"uniform sampler2D u_Texture;" \
			"varying vec2 v_TexCoord; " \
			"void main()               " \
			"{                         " \
			" vec4 tex = texture2D(u_Texture, v_TexCoord); "\
			" gl_FragColor = tex*vec4(1,1,1,0.6); " \
			"}                         " \
			"                          ";
	}

	// vertex shader for 3D models with lighting
	const char* getObjVert()
	{
		return  "uniform mat4 u_Projection;" \
			"uniform mat4 u_Model;" \
			"uniform mat4 u_View; "\
			"attribute vec3 a_Position;            " \
			"attribute vec2 a_TexCoord;            " \
			"attribute vec3 a_Normal;             " \
			"varying vec2 v_TexCoord;               " \
			"varying vec3 v_Normal;                 " \
			"varying vec3 v_FragPos;                " \
			"                                       " \
			"void main()                            " \
			"{                                      " \
			" gl_Position = u_Projection * u_View * u_Model * vec4(a_Position,1.0); " \
			" v_TexCoord = a_TexCoord;" \
			" v_Normal = mat3(u_Model) * a_Normal;      " \
			" v_FragPos = vec3(u_Model * vec4(a_Position, 1.0));       " \
			"}                                      " \
			"                                       ";
	}

	// fragment shader with multiple light sources, ambient, diffuse, specular lighting, and spotlights
	const char* getObjFrag()
	{
		return
			"uniform sampler2D u_Texture;    " \
			"uniform vec3 u_ViewPos;                      " \
			"uniform vec3 lightPos[2];    " \
			"varying vec2 v_TexCoord;        " \
			"varying vec3 v_Normal;               " \
			"varying vec3 v_FragPos;                 " \
			"void main()               " \
			"{                         " \
			"vec4 tex = texture(u_Texture, v_TexCoord); " \
			/*setting directional light*/
			"vec3 dirLightPos = vec3(45,1000,0); "
			"vec3 diffuseColor = vec3(1,1,0.5); " \
			"vec3 lighting = vec3(0.0);" \
			"vec3 N = normalize(v_Normal);    " \

			"vec3 lightDir = normalize(dirLightPos - v_FragPos);   " \
			"float diff = max(dot(N, lightDir),0.3);   " \
			"vec3 diffuse = diffuseColor * diff;    " \

			"vec3 specularColor = vec3(1, 1, 1);    " \
			"vec3 viewDir = normalize(u_ViewPos - v_FragPos);    " \
			"vec3 halfwayDir = normalize(lightDir + viewDir);    " \
			"float spec = pow(max(dot(N, halfwayDir), 0.0), 32.0);    " \
			"vec3 specular = spec * specularColor;    " \

			"vec3 spotlightDir = normalize(vec3(1,0,0));   " \
			"float cutOff = cos(radians(5.5));                     "\
			"float outerCutOff = cos(radians(12.5));               "\
			"vec3 spotlightcolor = vec3(1,1,1);"\
			/*setting headlights (spotlight)*/
			"for(int i =0; i < 2; i++)         " \
			"{                                 " \

			"     vec3 lightDir = normalize(lightPos[i] - v_FragPos);       "\
			"     float theta = dot(lightDir, -spotlightDir);            "\
			"     float epsilon = cutOff - outerCutOff;                 "\
			"     float intensity = clamp((theta - outerCutOff) / epsilon, 0.0, 1.0); "\
			"     float spotdiff = max(dot(N, lightDir),0.0);               "\
			"     vec3 spotdiffuse = spotlightcolor * spotdiff * intensity;    " \

			"     vec3 halfwayDir = normalize(lightDir + viewDir);        " \
			"     float spec = pow(max(dot(N, halfwayDir), 0.0), 32.0);   " \
			"     vec3 spotspecular = spotlightcolor * spec * intensity;        "\
			"                                              "\
			"     lighting += spotdiffuse + spotspecular;" \
			"}                                      " \
			"vec3 ambient = vec3(0.3,0.3,0.15) *tex.rgb;"\
			"lighting += ambient + (diffuse + specular);         " \
			"gl_FragColor = vec4(lighting, 1) * tex;              " \
			"}                         " \
			"                          ";
	}

	// post-processing: Vertex shader for fullscreen color grading
	const char* getColorGradingVert()
	{
		return
			"attribute vec3 a_pos;"\
			"attribute vec2 a_TexCoord;"\
			"varying vec2 Tex;"\
			"void main()     "\
			"{               "\
			"    gl_Position = vec4(a_pos,1.0);"\
			"    Tex = a_TexCoord;             "\
			"}                                 ";
	}

	// fragment shader for post-processing color grading: exposure, contrast, saturation, gamma correction
	const char* getColorGradingFrag()
	{
		return
			"varying vec2 Tex;  "\
			"uniform sampler2D screenTexture;"\
			"uniform float exposure = 0.9;"\
			"uniform float contrast = 1.15;"\
			"uniform float saturation = 1.5;"\
			"uniform float gamma = 1.5;"\
			"void main()             "\
			"{                       "\
			"    vec3 color = texture2D(screenTexture,Tex).rgb;"\
			"    color *= exposure; "\
			"    color = (color - 0.5) * contrast + 0.5; "\
			"    float gray = dot(color, vec3(0.299, 0.587, 0.114));  "\
			"    color = mix(vec3(gray), color, saturation);  "\
			"    color = pow(color, vec3(1.0 / gamma));  "\
			"    gl_FragColor = vec4(color,1.0);  "\
			"}                                    ";
	}
private:
	int m_gameState;
	Window m_win;
	
	//menu text setting
	VBO m_playPosition;
	VBO m_titlePosition;
	VBO m_highScorePosition;

	VAO m_playTextVao; 
	VAO m_titleTextVao;
	VAO m_highScoreTextVao;

	//play text setting
	VBO m_scoreTextPosition;
	VBO m_textTexCoords;
	VAO m_scoreTextVao;
	TTF_Font* m_font;
	SDL_Color m_textColor;
	Texture m_textTexture;

	//miniMap 
	RenderTexture m_miniMap;
	RenderTexture m_colorGrading;
	VBO m_miniMapPosition;
	VBO m_miniMapTexCoords;
	VAO m_miniMapVao;
	
	//full screen quad
	VBO m_quadPosition;
	VBO m_quadTexCoords;
	VAO m_quadVao;
	//skybox
	Skybox m_skybox;

	//Gameobjects in game
	Track m_track1;
	Track m_track2;
	BarrierPool m_barrierPool;
	std::vector<Barrier*> m_activeBarrierObj;
	Gas m_gas;

	// score 
	int m_currentScore;
	int m_highScore;

	//Shader
	ShaderProgram m_skyboxShader;
	ShaderProgram m_textShader;
	ShaderProgram m_shader;
	ShaderProgram m_miniMapShader;
	ShaderProgram m_colorGradingShader;

	glm::vec3 m_campos1; //main cam
	glm::vec3 m_campos2; //minimap cam
	glm::mat4 m_projection;
	glm::mat4 m_orthoProj;
	glm::mat4 m_view1;
	glm::mat4 m_view2;
	glm::mat4 m_view2D;
	glm::mat4 m_viewSkyBox;
};

inline void Game::loadHighScore()
{
	std::ifstream file;
	file.open("highscore.txt"); // try to open the file containing the high score
	if (file.is_open())
	{
		file >> m_highScore;
		file.close();
	}
	else
	{
		std::ofstream file("highscore.txt");  // if the file doesn't exist, create it and set high score to 0
		m_highScore = 0;
		file << m_highScore;
		file.close();
	}
}

inline void Game::saveHighScore()
{
	// if current score is higher than the stored high score, update it
	if (m_highScore < m_currentScore)
	{
		m_highScore = m_currentScore;
	}
	std::ofstream file;
	file.open("highscore.txt");  // open the file for writing
	if (file.is_open())
	{
		file << m_highScore;
		file.close();
	}
	
}

inline Game::~Game()
{
	if (m_font) {
		TTF_CloseFont(m_font);
		m_font = nullptr;
	}

	TTF_Quit();
}