#pragma once
/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <string.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <map.h>
#include <wall.h>

#include <sprite_renderer.h>



#include <iostream>


class SpriteRenderer
{
public:
    // Constructor (inits shaders/shapes)
    SpriteRenderer();
    // Destructor
    ~SpriteRenderer();

    void setShader(Shader* shader);
    // Renders a defined quad textured with given sprite
    void DrawSprite(int texture, glm::vec2 position, glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));
private:
    // Render state
    Shader*       shader;
    unsigned int quadVAO;
    // Initializes and configures the quad's buffer and vertex attributes
    void initRenderData();
};

#endif