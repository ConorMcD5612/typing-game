//
// Created by conor on 12/5/2023.
//

#include "arrow.h"
#include "../framework/color.h"


Arrow::Arrow(Shader & shader, vec2 pos, vec2 size, struct color _color, Direction direction)
        : Shape(shader, pos, size, _color) {
    this->direction = direction;
    initVectors();
    initVAO();
    initVBO();
    initEBO();
}

Arrow::Arrow(Arrow const& other) : Shape(other) {
    initVectors();
    initVAO();
    initVBO();
    initEBO();
}

Arrow::~Arrow() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Arrow::draw() const {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Arrow::initVectors() {

   auto arrowVertices = {
           //rectangle
           -0.5f,  0.5f,  // Top left
           0.5f,  0.5f,  // Top right
           -0.5f, -0.5f,  // Bottom left
               0.5f, -0.5f,  // Bottom right

           // triangle head
           0.5f,  1.0f,
           0.5f,  -1.0f,
           1.0f,  0.0f
   };

    switch(direction) {
        case UP:
            arrowVertices = {
                    -0.5f, -0.5f,
                    0.5f, -0.5f,
                    -0.5f, 0.5f,
                    0.5f, 0.5f,

                    -1.0f, 0.5f,
                    1.0f, 0.5f,
                    0.0f, 1.0f
            };
            break;
        case DOWN:
            arrowVertices = {
                    -0.5f, -0.5f,
                    0.5f, -0.5f,
                    -0.5f, 0.5f,
                    0.5f, 0.5f,

                    -1.0f, -0.5f,
                    1.0f, -0.5f,
                    0.0f, -1.0f
            };
            break;

        case LEFT:
            arrowVertices = {
                    -0.5f,  0.5f,
                    0.5f,  0.5f,
                    -0.5f, -0.5f,
                    0.5f, -0.5f,

                    -0.5f,  1.0f,
                    -0.5f, -1.0f,
                    -1.0f,  0.0f
            };
            break;



        case RIGHT:
            arrowVertices = {
                    -0.5f,  0.5f,
                    0.5f,  0.5f,
                    -0.5f, -0.5f,
                    0.5f, -0.5f,

                    0.5f,  1.0f,
                    0.5f,  -1.0f,
                    1.0f,  0.0f
            };
            break;

    }


        this->vertices.insert(vertices.end(), arrowVertices);
    this->indices.insert(indices.end(), {
            0, 1, 2, //rect
            1, 2, 3, //rect
            4, 5, 6  //head
    });
}
// Overridden Getters from Shape
float Arrow::getLeft() const        { return pos.x - (size.x / 2); }
// TODO: Implement getRight, getTop, and getBottom
float Arrow::getRight() const       { return pos.x + (size.x / 2); /* placeholder for compilation */ }
float Arrow::getTop() const         { return pos.y + (size.y / 2); /* placeholder for compilation */ }
float Arrow::getBottom() const      { return pos.y - (size.y / 2); /* placeholder for compilation */ }

Direction Arrow::getDirection() { return direction;}