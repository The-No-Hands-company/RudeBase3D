#pragma once

#include "Common.h"
#include "core/mesh_elements.hpp"
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <memory>

/**
 * @brief Global Vertex class that serves as a bridge between Qt and GLM types
 * 
 * This class provides a convenient interface for working with vertices in the
 * geometry generation and processing pipeline. It handles conversions between
 * Qt types (used in UI and some rendering) and GLM types (used in the rude:: namespace).
 */
class Vertex {
public:
    // Constructors
    Vertex();
    Vertex(const glm::vec3& pos);
    Vertex(const glm::vec3& pos, const glm::vec3& norm);
    Vertex(const glm::vec3& pos, const glm::vec3& norm, const glm::vec2& tex);
    
    // Copy constructor and assignment from rude::Vertex
    Vertex(const rude::Vertex& rudeVertex);
    Vertex& operator=(const rude::Vertex& rudeVertex);
    
    // Destructor
    ~Vertex() = default;

    // Qt-style accessors (for compatibility with existing code)
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;

    // GLM-style getters (for performance-critical code)
    glm::vec3 getPositionGLM() const;
    glm::vec3 getNormalGLM() const;
    glm::vec2 getTexCoordGLM() const;

    // GLM-style setters
    void setPosition(const glm::vec3& pos);
    void setNormal(const glm::vec3& norm);
    void setTexCoord(const glm::vec2& tex);

    // Conversion to rude::Vertex
    rude::Vertex toRudeVertex() const;
    std::shared_ptr<rude::Vertex> toRudeVertexPtr() const;

    // Static factory methods
    static Vertex fromGLM(const glm::vec3& pos, const glm::vec3& norm = glm::vec3(0.0f, 1.0f, 0.0f), const glm::vec2& tex = glm::vec2(0.0f, 0.0f));

    // Utility methods
    void normalize();
    float distanceTo(const Vertex& other) const;
    Vertex lerp(const Vertex& other, float t) const;

    // Operators
    bool operator==(const Vertex& other) const;
    bool operator!=(const Vertex& other) const;
    Vertex operator+(const Vertex& other) const;
    Vertex operator-(const Vertex& other) const;
    Vertex operator*(float scalar) const;

private:
    // No Qt conversion functions needed; all logic is now GLM-based.
};

// Type alias for convenience
using VertexPtr = std::shared_ptr<Vertex>;
