#include "Game.h"

Game::Game() :
    carTexture("Fishing.jpg"),
    barrierTexture("ba.png"),
    roadTexture("grassDirt.jpg"),
    gasTexture("fuel_can.png"),
    gasModel("fuel_can.obj"),
    carModel("Fishing.obj"),
    barrierModel("ba.obj"),
    m_miniMap(300, 200),
    m_colorGrading(1200, 800),
    m_gameState(0),
    m_skybox(glm::vec3(0, 0, 0), glm::vec3(0, 50, 0), glm::vec3(1.0)),
    player(this, glm::vec3(-10, 0, 0), glm::vec3(0, 90, 0), glm::vec3(1.0)),
    m_track1(this, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1.0)),
    m_track2(this, glm::vec3(50, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1.0)),
    m_barrierPool(this, 10),
    m_gas(this, glm::vec3(20, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0.05)),
    m_currentScore(0),
    m_skyboxShader(getSkyboxVert(), getSkyboxFrag()),
    m_textShader(getTextVert(), getDefaultFrag()),
    m_shader(getObjVert(), getObjFrag()),
    m_miniMapShader(getDefaultVert(), getDefaultFrag()),
    m_colorGradingShader(getColorGradingVert(), getColorGradingFrag()),
    m_campos1(0, 15, 25),
    m_campos2(0, 10, -5),
    // set up camera positions and projection/view matrices
    m_projection(glm::perspective(glm::radians(45.0f), (float)1200 / (float)800, 0.1f, 1000.0f)),
    m_orthoProj(glm::ortho(0.0f, (float)1200, 0.0f, (float)800, -1.0f, 1000.0f)),
    m_view1(glm::lookAt(m_campos1, glm::vec3(0, 10, 10), glm::vec3(0, 1, 0))),
    m_view2(glm::lookAt(m_campos2, glm::vec3(0,0,0), glm::vec3(0, 1, 0))),
    m_view2D(glm::mat4(1.0f)),
    m_viewSkyBox(glm::mat4(glm::mat3(m_view1)))
{
    // Load high score from previous session
    loadHighScore();
    // Initialize render textures
    m_miniMap.DefaultMap();
    m_colorGrading.DefaultMap();
    //MENU setting
    //play instruction text quad
    m_playPosition.add(glm::vec2(350, 350));
    m_playPosition.add(glm::vec2(850, 350));
    m_playPosition.add(glm::vec2(850, 450));
    m_playPosition.add(glm::vec2(350, 450));

    //Title name text quad
    m_titlePosition.add(glm::vec2(400, 450));
    m_titlePosition.add(glm::vec2(800, 450));
    m_titlePosition.add(glm::vec2(800, 780));
    m_titlePosition.add(glm::vec2(400, 700));

    //high score text quad
    m_highScorePosition.add(glm::vec2(400, 300));
    m_highScorePosition.add(glm::vec2(700, 300));
    m_highScorePosition.add(glm::vec2(700, 350));
    m_highScorePosition.add(glm::vec2(400, 350));

    // shared text texture coordinates
    m_textTexCoords.add(glm::vec2(0.0f, 1.0f));
    m_textTexCoords.add(glm::vec2(1.0f, 1.0f));
    m_textTexCoords.add(glm::vec2(1.0f, 0.0f));
    m_textTexCoords.add(glm::vec2(0.0f, 0.0f));

    // bind vertex arrays for text UI
    m_playTextVao.setBuffer(&m_playPosition);
    m_playTextVao.setBuffer(&m_textTexCoords);

    m_titleTextVao.setBuffer(&m_titlePosition);
    m_titleTextVao.setBuffer(&m_textTexCoords);

    m_highScoreTextVao.setBuffer(&m_highScorePosition);
    m_highScoreTextVao.setBuffer(&m_textTexCoords);

    // main game scene setting-------------------------------------------------------------------
    // in game score UI
    m_scoreTextPosition.add(glm::vec3(10.0f, 800 - 60.0f, 0.0f));
    m_scoreTextPosition.add(glm::vec3(210.0f, 800 - 60.0f, 0.0f));
    m_scoreTextPosition.add(glm::vec3(210.0f, 800 - 10.0f, 0.0f));
    m_scoreTextPosition.add(glm::vec3(10.0f, 800 - 10.0f, 0.0));

    m_scoreTextVao.setBuffer(&m_scoreTextPosition);
    m_scoreTextVao.setBuffer(&m_textTexCoords);

    // miniMap quad on screen
    m_miniMapPosition.add(glm::vec3(20.0f, 20.0f, 0.0f));
    m_miniMapPosition.add(glm::vec3(320.0f, 20.0f, 0.0f));
    m_miniMapPosition.add(glm::vec3(320.0f, 220.0f, 0.0f));
    m_miniMapPosition.add(glm::vec3(20.0f, 220.0f, 0.0f));

    m_miniMapTexCoords.add(glm::vec2(0.0f, 0.0f));
    m_miniMapTexCoords.add(glm::vec2(1.0f, 0.0f));
    m_miniMapTexCoords.add(glm::vec2(1.0f, 1.0f));
    m_miniMapTexCoords.add(glm::vec2(0.0f, 1.0f));

    m_miniMapVao.setBuffer(&m_miniMapPosition);
    m_miniMapVao.setBuffer(&m_miniMapTexCoords);

    // fullscreen quad for post-processing (color grading)
    m_quadPosition.add(glm::vec2(-1.0f, 1.0f));
    m_quadPosition.add(glm::vec2(1.0f, 1.0f));
    m_quadPosition.add(glm::vec2(1.0f, -1.0f));
    m_quadPosition.add(glm::vec2(-1.0f, -1.0f));

    m_quadTexCoords.add(glm::vec2(0.0f, 1.0f));
    m_quadTexCoords.add(glm::vec2(1.0f, 1.0f));
    m_quadTexCoords.add(glm::vec2(1.0f, 0.0f));
    m_quadTexCoords.add(glm::vec2(0.0f, 0.0f));

    m_quadVao.setBuffer(&m_quadPosition);
    m_quadVao.setBuffer(&m_quadTexCoords);
    // initialize font setting ----------------------------------------------------------------------
    if (TTF_Init() < 0) {
        std::cerr << "TTF_Init failed: " << TTF_GetError() << std::endl;

    }
    m_font = TTF_OpenFont("Bullpen3D.ttf", 72);
    if (!m_font)
    {
        throw std::runtime_error("failed to load font");
    }
   
}
void Game::run()
{
    player.initSize(); // Initialize player size and collision box

    // TEXT
    float spawnObsTime = 0.0f;
    bool quit = false;
    SDL_Event event = { 0 };
    while (!quit)
    {
        glEnable(GL_DEPTH_TEST);
        float start = SDL_GetTicks();

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                quit = true;
                saveHighScore();
                std::cout << "quit";
            }
        }

        // clear screen and enable alpha blending
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
       
        if (m_gameState == 0) // menu state
        {
            menu();
        }
        else if(m_gameState == 1) // game running
        {
            glm::mat4 model(1.0f);

            //update
            // Update all game objects
            player.update();
            m_track1.update();
            m_track2.update();
            m_gas.update(m_activeBarrierObj);
            
            
            
            // minimap rendering//////////////////////////////////////////////
            m_miniMap.bind();
            glViewport(0, 0, 300, 200);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            m_view2 = glm::lookAt(m_campos2,player.getPosition(), glm::vec3(0, 1, 0)),
            m_track1.draw(m_shader, m_view2, m_projection, m_campos1);
            m_track2.draw(m_shader, m_view2, m_projection, m_campos1);
            player.draw(m_shader, m_view2, m_projection, m_campos1);
            if (m_gas.getState()) m_gas.draw(m_shader, m_view2, m_projection, m_campos1);
            m_miniMap.unbind();


            // main scene rendering////////////////////////////////////////////////////////////
            // bind the color grading framebuffer to prepare for rendering to the screen
            m_colorGrading.bind();
            glViewport(0, 0, 1200, 800);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            // Set lighting based on player position
            glUseProgram(m_shader.getId());
            m_shader.setUniform("lightPos[0]", player.getPosition() + glm::vec3(-1, 1, 1));
            m_shader.setUniform("lightPos[1]", player.getPosition() + glm::vec3(-1, 1, -1));
            // handle fuel pickup collision and score update
            if (m_gas.getState() && m_gas.isColliding(m_gas.getPosition(), m_gas.getSize(), player.getPosition(), player.getSize()))
            {
                m_gas.setActive(false);
                m_currentScore++;
                std::string pointStr = "Score: " + std::to_string(m_currentScore);
                m_textTexture.load(pointStr, m_font, m_textColor);
            }
            // draw skybox
            m_skybox.draw(m_skyboxShader, m_viewSkyBox, m_projection, m_campos1);
            // barrier update and draw()
            spawnObsTime += 1.0f / 60.0f;

            if (spawnObsTime > 2.0f) // spawn obstacles
            {
                float Zpos[] = { -6.7,0,6.7 };
                int obstaclesNum = rand() % 2 + 1; // 1 or 2 obstacles
                int z = rand() % 3; // random lane 
                std::cout << "!!!!!!!!!!!!!!!\n";
                for (int c = 0; c < obstaclesNum; c++)
                {
                    Barrier* b = m_barrierPool.getObject(); // get object from pool
                    if (b)
                    {
                        b->setPosition(glm::vec3(30, 0, Zpos[z])); 
                        m_activeBarrierObj.push_back(b);
                        b->setActive(true);
                    }
                    z = (z + 1) % 3;
                }
                spawnObsTime = 0; // reset obstacle spawn timer
            }
            for (auto it = m_activeBarrierObj.begin(); it != m_activeBarrierObj.end(); ++it)
            {
                // update all active barriers
                (*it)->update();
                // Collision test
                if ((*it)->isColliding((*it)->getPosition(), (*it)->getSize(), player.getPosition(), player.getSize()))
                {
                    std::cout << "collide!\n";
                    glm::vec3 temp_pos = (*it)->getPosition();
                    (*it)->aabb_response(temp_pos, (*it)->getSize(), player.getPosition(), player.getSize());
                    (*it)->setPosition(temp_pos);
                    m_gameState = 2;  //state game over
                }
                if (!(*it)->getState())
                {
                    it = m_activeBarrierObj.erase(it); //update activeobjects list
                }
                else
                {
                    // draw all active barriers
                    (*it)->draw(m_shader, m_view1, m_projection, m_campos1);
                }
            }
           
            // draw game objects, minimap on the screen
            m_track1.draw(m_shader, m_view1, m_projection, m_campos1);
            m_track2.draw(m_shader, m_view1, m_projection, m_campos1);
            player.draw(m_shader, m_view1, m_projection, m_campos1);
            if (m_gas.getState()) m_gas.draw(m_shader, m_view1, m_projection, m_campos1);
            glUseProgram(m_textShader.getId());
            // use time to update sine function in shader
            float wavetime = SDL_GetTicks() / 1000.0f; //transfer to seconds
            m_textShader.setUniform("u_Time", wavetime); 
            m_textShader.draw(&m_scoreTextVao, m_textTexture.id(), model, m_orthoProj);
            m_miniMapShader.draw(&m_miniMapVao, m_miniMap.getTexture(), model, m_orthoProj);

            m_colorGrading.unbind();
            // Use the color grading shader to draw the full-screen quad
            m_colorGradingShader.draw(&m_quadVao, m_colorGrading.getTexture(), model, m_orthoProj);

        }
        else if(m_gameState == 2) //game over reset gameObject
        {
            SDL_Delay(1200); //stop game for 1.2s
            m_gameState = 0; //back to menu
            saveHighScore();
            resetGame();
        }
     
        SDL_GL_SwapWindow(m_win.getWin());

        float elapsedms = SDL_GetTicks() - start; // calculate how long the frame has taken
        if (elapsedms < 16.667) // if the frame took less time than 16.667ms ( 60 FPS)
        {
            SDL_Delay(16.667 - elapsedms); // wait for the remainder of the frame time
        }
    }

    SDL_DestroyWindow(m_win.getWin());
    SDL_Quit();
}


void Game::menu()
{
    glm::vec3 camerapos(0, 15, 25);
    glm::mat4 view = glm::lookAt(camerapos, glm::vec3(20, 15, 10), glm::vec3(0, 1, 0));
    glm::mat4 viewSkyBox = glm::mat4(glm::mat3(view));
   
    m_skybox.draw(m_skyboxShader, viewSkyBox, m_projection, m_campos1);
    glm::mat4 model = glm::mat4(1.0f);
    // Title
    // Wave effect // dynamic geometry
    glUseProgram(m_textShader.getId());
    // use time to update sine function in shader
    float wavetime = SDL_GetTicks() / 1000.0f;
    m_textShader.setUniform("u_Time", wavetime);
    m_textColor = { 0,0,0,255 };
    m_textTexture.load("Sunset", m_font, m_textColor); //update texture
    m_textShader.draw(&m_titleTextVao, m_textTexture.id(), model, m_orthoProj);

    // Play option
    m_textColor = { 20,220,230,255 };
    m_textTexture.load("Press Space to Start", m_font, m_textColor);
    m_textShader.draw(&m_playTextVao, m_textTexture.id(), model, m_orthoProj);

    // high score
    m_textColor = { 190,190,100,255 };
    std::string highscoreString = "Highest Score: " + std::to_string(m_highScore);
    m_textTexture.load(highscoreString, m_font, m_textColor);
    m_textShader.draw(&m_highScoreTextVao, m_textTexture.id(), model, m_orthoProj);

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            std::cout << "quit";
        }
        else if (event.key.keysym.sym == SDLK_SPACE)
        {
            m_gameState = 1; // game start
            m_textColor = { 0,220,23,255 };
            m_textTexture.load("Score: 0", m_font, m_textColor); // update to score texture
        }
    }
}


void Game::resetGame() //reset gameobjects position
{
    m_currentScore = 0;
    player.setPosition(glm::vec3(-10, 0, 0));
    m_barrierPool.reset();
    m_track1.setPosition(glm::vec3(0, 0, 0));
    m_track2.setPosition(glm::vec3(50, 0, 0));
    m_activeBarrierObj.clear();
}