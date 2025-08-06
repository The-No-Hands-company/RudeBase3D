#include "Vertex.h"
#include <cmath>
#include <algorithm>

// Default constructor
Vertex::Vertex() 
    : position(0.0f, 0.0f, 0.0f)
    , normal(0.0f, 1.0f, 0.0f)
    , texCoord(0.0f, 0.0f)
{
}

// GLM constructors
Vertex::Vertex(const glm::vec3& pos)
    : position(pos)
    , normal(0.0f, 1.0f, 0.0f)
    , texCoord(0.0f, 0.0f)
{
}

Vertex::Vertex(const glm::vec3& pos, const glm::vec3& norm)
    : position(pos)
    , normal(norm)
    , texCoord(0.0f, 0.0f)
{
}

Vertex::Vertex(const glm::vec3& pos, const glm::vec3& norm, const glm::vec2& tex)
    : position(pos)
    , normal(norm)
    , texCoord(tex)
{
}

// Copy constructor from rude::Vertex
Vertex::Vertex(const rude::Vertex& rudeVertex)
    : position(rudeVertex.position)
    , normal(rudeVertex.normal)
    , texCoord(rudeVertex.texCoord)
{
}

// Assignment from rude::Vertex
Vertex& Vertex::operator=(const rude::Vertex& rudeVertex)
{
    position = rudeVertex.position;
    normal = rudeVertex.normal;
    texCoord = rudeVertex.texCoord;
    return *this;
}

// GLM-style getters
glm::vec3 Vertex::getPositionGLM() const
{
    return position;
}

glm::vec3 Vertex::getNormalGLM() const
{
    return normal;
}

glm::vec2 Vertex::getTexCoordGLM() const
{
    return texCoord;
}

// GLM-style setters
void Vertex::setPosition(const glm::vec3& pos)
{
    position = pos;
}

void Vertex::setNormal(const glm::vec3& norm)
{
    normal = norm;
}

void Vertex::setTexCoord(const glm::vec2& tex)
{
    texCoord = tex;
}

// Conversion to rude::Vertex
rude::Vertex Vertex::toRudeVertex() const
{
    rude::Vertex rudeVert(position);
    rudeVert.normal = normal;
    rudeVert.texCoord = texCoord;
    return rudeVert;
}

std::shared_ptr<rude::Vertex> Vertex::toRudeVertexPtr() const
{
    auto rudeVert = std::make_shared<rude::Vertex>(position);
    rudeVert->normal = normal;
    rudeVert->texCoord = texCoord;
    return rudeVert;
}

// Static factory methods
Vertex Vertex::fromGLM(const glm::vec3& pos, const glm::vec3& norm, const glm::vec2& tex)
{
    return Vertex(pos, norm, tex);
}



// Utility methods
void Vertex::normalize()
{
    normal = glm::normalize(normal);
}

float Vertex::distanceTo(const Vertex& other) const
{
    return (position - other.position).length();
}

Vertex Vertex::lerp(const Vertex& other, float t) const
{
    t = std::clamp(t, 0.0f, 1.0f);
    return Vertex(
        position * (1.0f - t) + other.position * t,
        normal * (1.0f - t) + other.normal * t,
        texCoord * (1.0f - t) + other.texCoord * t
    );
}

// Operators
bool Vertex::operator==(const Vertex& other) const
{
    const float epsilon = 1e-6f;
    return (position - other.position).length() < epsilon &&
           (normal - other.normal).length() < epsilon &&
           (texCoord - other.texCoord).length() < epsilon;
}

bool Vertex::operator!=(const Vertex& other) const
{
    return !(*this == other);
}

Vertex Vertex::operator+(const Vertex& other) const
{
    return Vertex(
        position + other.position,
        normal + other.normal,
        texCoord + other.texCoord
    );
}

Vertex Vertex::operator-(const Vertex& other) const
{
    return Vertex(
        position - other.position,
        normal - other.normal,
        texCoord - other.texCoord
    );
}

Vertex Vertex::operator*(float scalar) const
{
    return Vertex(
        position * scalar,
        normal * scalar,
        texCoord * scalar
    );
}

// Helper conversion functions
// All Qt conversion helpers removed. All logic is now GLM-based.
