#include "rect.h"
#include "../framework/color.h"

Rect::Rect(Shader & shader, vec2 pos, vec2 size, struct color _color)
    : Shape(shader, pos, size, _color) {
    initVectors();
    initVAO();
    initVBO();
    initEBO();
}

Rect::Rect(Rect const& other) : Shape(other) {
    initVectors();
    initVAO();
    initVBO();
    initEBO();
}

Rect::~Rect() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Rect::draw() const {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Rect::initVectors() {
    this->vertices.insert(vertices.end(), {
            //rectangle
            -0.5f,  0.5f,  // Top left
            0.5f,  0.5f,  // Top right
            -0.5f, -0.5f,  // Bottom left
            0.5f, -0.5f,  // Bottom right

            // triangle head
            0.5f,  1.0f,
            0.5f,  -1.0f,
            1.0f,  0.0f
    });

    this->indices.insert(indices.end(), {
            0, 1, 2, //rect
            1, 2, 3, //rect
            4, 5, 6  //head
    });
}
// Overridden Getters from Shape
float Rect::getLeft() const        { return pos.x - (size.x / 2); }
// TODO: Implement getRight, getTop, and getBottom
float Rect::getRight() const       { return pos.x + (size.x / 2); /* placeholder for compilation */ }
float Rect::getTop() const         { return pos.y + (size.y / 2); /* placeholder for compilation */ }
float Rect::getBottom() const      { return pos.y - (size.y / 2); /* placeholder for compilation */ }
