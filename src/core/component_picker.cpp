#define GLM_ENABLE_EXPERIMENTAL
#include "core/component_picker.hpp"
#include "core/entity.hpp"
#include "core/scene.hpp"
#include "core/mesh.hpp"
#include "core/qt_glm_utils.hpp"
#include "scene/Camera.h"
#include "core/math/Transform.h"
#include "core/half_edge_mesh.hpp"
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <algorithm>
#include <limits>
#include <map>

namespace rude {

ComponentPicker::ComponentPicker() {
    m_pickingTolerance = 5.0f; // 5 pixels default tolerance
}

SelectionData ComponentPicker::pickComponent(float mouseX, float mouseY, int viewportWidth, int viewportHeight,
                                           ComponentType targetType, ::Entity* targetEntity) {
    if (!m_camera) return SelectionData{};
    
    glm::vec3 rayOrigin = getRayOrigin(mouseX, mouseY, viewportWidth, viewportHeight);
    glm::vec3 rayDirection = getRayDirection(mouseX, mouseY, viewportWidth, viewportHeight);
    
    SelectionData bestSelection;
    float bestDistance = std::numeric_limits<float>::max();
    
    // If no target entity specified, we'd need to iterate through all entities in the scene
    // For now, assuming we have a target entity
    if (targetEntity) {
        SelectionData candidate;
        
        switch (targetType) {
            case ComponentType::Vertex:
                candidate = pickVertex(rayOrigin, rayDirection, targetEntity);
                break;
            case ComponentType::Edge:
                candidate = pickEdge(rayOrigin, rayDirection, targetEntity);
                break;
            case ComponentType::Face:
                candidate = pickFace(rayOrigin, rayDirection, targetEntity);
                break;
            case ComponentType::Entity:
                // Use existing entity picking logic
                candidate.type = ComponentType::Entity;
                candidate.entity = targetEntity;
                break;
            default:
                break;
        }
        
        if (candidate.isValid()) {
            bestSelection = candidate;
        }
    }
    
    return bestSelection;
}

std::vector<SelectionData> ComponentPicker::pickComponentsInRegion(float x1, float y1, float x2, float y2,
                                                                  int viewportWidth, int viewportHeight,
                                                                  ComponentType targetType, ::Entity* targetEntity) {
    std::vector<SelectionData> selections;
    
    if (!targetEntity) return selections;
    
    // Get entity's mesh
    auto mesh = targetEntity->getMesh();
    if (!mesh) return selections;
    
    auto halfEdgeMesh = mesh->toHalfEdgeMesh();
    if (!halfEdgeMesh) return selections;
    Transform& transform = targetEntity->getTransform();
    glm::mat4 modelMatrix = transform.getModelMatrix();

    // Ensure region coordinates are properly ordered
    float minX = std::min(x1, x2);
    float maxX = std::max(x1, x2);
    float minY = std::min(y1, y2);
    float maxY = std::max(y1, y2);

    switch (targetType) {
        case ComponentType::Vertex: {
            for (auto it = halfEdgeMesh->vertices_begin(); it != halfEdgeMesh->vertices_end(); ++it) {
                auto vertex = *it;
                glm::vec3 worldPos = modelMatrix * glm::vec4(vertex->getPosition(), 1.0f);
                glm::vec3 screenPos = worldToScreen(worldPos, viewportWidth, viewportHeight);

                if (screenPos.x >= minX && screenPos.x <= maxX &&
                    screenPos.y >= minY && screenPos.y <= maxY) {
                    SelectionData selection;
                    selection.type = ComponentType::Vertex;
                    selection.vertex = vertex;
                    selections.push_back(selection);
                }
            }
            break;
        }

        case ComponentType::Edge: {
            for (auto it = halfEdgeMesh->edges_begin(); it != halfEdgeMesh->edges_end(); ++it) {
                auto edge = *it;
                if (!edge->getHalfEdge()) continue;

                auto he1 = edge->getHalfEdge();
                auto he2 = he1->getTwin();

                glm::vec3 worldPos1 = modelMatrix * glm::vec4(he1->getVertex()->getPosition(), 1.0f);
                glm::vec3 worldPos2 = glm::vec3(0.0f);
                if (he2 && he2->getVertex()) {
                    worldPos2 = modelMatrix * glm::vec4(he2->getVertex()->getPosition(), 1.0f);
                }

                glm::vec3 screenPos1 = worldToScreen(worldPos1, viewportWidth, viewportHeight);
                glm::vec3 screenPos2 = worldToScreen(worldPos2, viewportWidth, viewportHeight);

                // Check if edge intersects with selection region
                if ((screenPos1.x >= minX && screenPos1.x <= maxX && screenPos1.y >= minY && screenPos1.y <= maxY) ||
                    (screenPos2.x >= minX && screenPos2.x <= maxX && screenPos2.y >= minY && screenPos2.y <= maxY)) {
                    SelectionData selection;
                    selection.type = ComponentType::Edge;
                    selection.edge = edge;
                    selections.push_back(selection);
                }
            }
            break;
        }

        case ComponentType::Face: {
            for (auto it = halfEdgeMesh->faces_begin(); it != halfEdgeMesh->faces_end(); ++it) {
                auto face = *it;
                if (!face->getHalfEdge()) continue;

                // Check if face center is in selection region
                auto he = face->getHalfEdge();
                glm::vec3 center(0.0f);
                int vertexCount = 0;

                auto current = he;
                do {
                    center += current->getVertex()->getPosition();
                    vertexCount++;
                    current = current->getNext();
                } while (current != he);

                if (vertexCount > 0) {
                    center /= static_cast<float>(vertexCount);
                    glm::vec3 worldPos = modelMatrix * glm::vec4(center, 1.0f);
                    glm::vec3 screenPos = worldToScreen(worldPos, viewportWidth, viewportHeight);

                    if (screenPos.x >= minX && screenPos.x <= maxX &&
                        screenPos.y >= minY && screenPos.y <= maxY) {
                        SelectionData selection;
                        selection.type = ComponentType::Face;
                        selection.face = face;
                        selections.push_back(selection);
                    }
                }
            }
            break;
        }

        default:
            break;
    }

    return selections;
}

glm::vec3 ComponentPicker::getRayOrigin(float mouseX, float mouseY, int viewportWidth, int viewportHeight) const {
    if (!m_camera) return glm::vec3(0.0f);
    return m_camera->getPosition();
}

glm::vec3 ComponentPicker::getRayDirection(float mouseX, float mouseY, int viewportWidth, int viewportHeight) const {
    if (!m_camera) return glm::vec3(0.0f, 0.0f, -1.0f);
    
    // Convert mouse coordinates to normalized device coordinates
    float x = (2.0f * mouseX) / viewportWidth - 1.0f;
    float y = 1.0f - (2.0f * mouseY) / viewportHeight;
    
    // Get the inverse projection and view matrices
    glm::mat4 invProjection = glm::inverse(m_camera->getProjectionMatrix());
    glm::mat4 invView = glm::inverse(m_camera->getViewMatrix());
    
    // Convert to world space
    glm::vec4 rayClip = glm::vec4(x, y, -1.0f, 1.0f);
    glm::vec4 rayEye = invProjection * rayClip;
    rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);
    
    glm::vec3 rayWorld = glm::vec3(invView * rayEye);
    return glm::normalize(rayWorld);
}

SelectionData ComponentPicker::pickVertex(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, ::Entity* entity) {
    SelectionData result;
    
    auto mesh = entity->getMesh();
    if (!mesh) return result;
    auto halfEdgeMesh = mesh->toHalfEdgeMesh();
    if (!halfEdgeMesh) return result;
    Transform& transform = entity->getTransform();
    glm::mat4 modelMatrix = transform.getModelMatrix();
    
    float closestDistance = std::numeric_limits<float>::max();
    VertexPtr closestVertex = nullptr;
    
    for (auto it = halfEdgeMesh->vertices_begin(); it != halfEdgeMesh->vertices_end(); ++it) {
        auto vertex = *it;
        glm::vec3 worldPos = modelMatrix * glm::vec4(vertex->getPosition(), 1.0f);

        float distance = pointToRayDistance(worldPos, rayOrigin, rayDirection);
        if (distance < closestDistance && distance < m_pickingTolerance / 100.0f) { // Convert pixel tolerance to world units approximately
            closestDistance = distance;
            closestVertex = vertex;
        }
    }
    
    if (closestVertex) {
        result.type = ComponentType::Vertex;
        result.vertex = closestVertex;
    }
    
    return result;
}

SelectionData ComponentPicker::pickEdge(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, ::Entity* entity) {
    SelectionData result;
    
    auto mesh = entity->getMesh();
    if (!mesh) return result;
    auto halfEdgeMesh = mesh->toHalfEdgeMesh();
    if (!halfEdgeMesh) return result;
    Transform& transform = entity->getTransform();
    glm::mat4 modelMatrix = transform.getModelMatrix();
    
    float closestDistance = std::numeric_limits<float>::max();
    EdgePtr closestEdge = nullptr;
    
    for (auto it = halfEdgeMesh->edges_begin(); it != halfEdgeMesh->edges_end(); ++it) {
        auto edge = *it;
        if (!edge->getHalfEdge()) continue;

        auto he1 = edge->getHalfEdge();
        auto he2 = he1->getTwin();

        glm::vec3 worldPos1 = modelMatrix * glm::vec4(he1->getVertex()->getPosition(), 1.0f);
        glm::vec3 worldPos2 = glm::vec3(0.0f);
        if (he2 && he2->getVertex()) {
            worldPos2 = modelMatrix * glm::vec4(he2->getVertex()->getPosition(), 1.0f);
        }

        float distance = lineSegmentToRayDistance(worldPos1, worldPos2, rayOrigin, rayDirection);
        if (distance < closestDistance && distance < m_pickingTolerance / 100.0f) {
            closestDistance = distance;
            closestEdge = edge;
        }
    }
    
    if (closestEdge) {
        result.type = ComponentType::Edge;
        result.edge = closestEdge;
    }
    
    return result;
}

SelectionData ComponentPicker::pickFace(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, ::Entity* entity) {
    SelectionData result;
    
    auto mesh = entity->getMesh();
    if (!mesh) return result;
    auto halfEdgeMesh = mesh->toHalfEdgeMesh();
    if (!halfEdgeMesh) return result;
    Transform& transform = entity->getTransform();
    glm::mat4 modelMatrix = transform.getModelMatrix();
    
    float closestT = std::numeric_limits<float>::max();
    rude::FacePtr closestFace = nullptr;
    
    for (auto it = halfEdgeMesh->faces_begin(); it != halfEdgeMesh->faces_end(); ++it) {
        auto face = *it;
        if (!face->getHalfEdge()) continue;

        // Get face vertices
        std::vector<glm::vec3> vertices;
        auto he = face->getHalfEdge();
        auto current = he;
        do {
            glm::vec3 worldPos = modelMatrix * glm::vec4(current->getVertex()->getPosition(), 1.0f);
            vertices.push_back(worldPos);
            current = current->getNext();
        } while (current != he);

        // For triangular faces, do direct ray-triangle intersection
        if (vertices.size() == 3) {
            float t;
            glm::vec3 intersection;
            if (rayTriangleIntersect(rayOrigin, rayDirection, vertices[0], vertices[1], vertices[2], t, intersection)) {
                if (t < closestT && t > 0.0f) {
                    closestT = t;
                    closestFace = face;
                }
            }
        } else if (vertices.size() > 3) {
            // For polygons, triangulate and test each triangle
            for (size_t i = 1; i < vertices.size() - 1; ++i) {
                float t;
                glm::vec3 intersection;
                if (rayTriangleIntersect(rayOrigin, rayDirection, vertices[0], vertices[i], vertices[i + 1], t, intersection)) {
                    if (t < closestT && t > 0.0f) {
                        closestT = t;
                        closestFace = face;
                    }
                }
            }
        }
    }
    
    if (closestFace) {
        result.type = ComponentType::Face;
        result.face = closestFace;
    }
    
    return result;
}

float ComponentPicker::pointToRayDistance(const glm::vec3& point, const glm::vec3& rayOrigin, const glm::vec3& rayDirection) const {
    glm::vec3 toPoint = point - rayOrigin;
    float projectionLength = glm::dot(toPoint, rayDirection);
    glm::vec3 projection = rayOrigin + projectionLength * rayDirection;
    return glm::length(point - projection);
}

float ComponentPicker::lineSegmentToRayDistance(const glm::vec3& p1, const glm::vec3& p2,
                                               const glm::vec3& rayOrigin, const glm::vec3& rayDirection) const {
    // Find closest points between line segment and ray
    glm::vec3 segmentDir = p2 - p1;
    glm::vec3 w = rayOrigin - p1;
    
    float a = glm::dot(segmentDir, segmentDir);
    float b = glm::dot(segmentDir, rayDirection);
    float c = glm::dot(rayDirection, rayDirection);
    float d = glm::dot(segmentDir, w);
    float e = glm::dot(rayDirection, w);
    
    float denom = a * c - b * b;
    if (denom < 1e-6f) {
        // Lines are parallel, return distance from ray to either endpoint
        return std::min(pointToRayDistance(p1, rayOrigin, rayDirection),
                       pointToRayDistance(p2, rayOrigin, rayDirection));
    }
    
    float s = (b * e - c * d) / denom;
    float t = (a * e - b * d) / denom;
    
    // Clamp s to [0, 1] for line segment
    s = glm::clamp(s, 0.0f, 1.0f);
    
    glm::vec3 closestOnSegment = p1 + s * segmentDir;
    glm::vec3 closestOnRay = rayOrigin + t * rayDirection;
    
    return glm::length(closestOnSegment - closestOnRay);
}

bool ComponentPicker::rayTriangleIntersect(const glm::vec3& rayOrigin, const glm::vec3& rayDirection,
                                          const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2,
                                          float& t, glm::vec3& intersection) const {
    // Möller-Trumbore ray-triangle intersection algorithm
    const float EPSILON = 1e-6f;
    
    glm::vec3 edge1 = v1 - v0;
    glm::vec3 edge2 = v2 - v0;
    glm::vec3 h = glm::cross(rayDirection, edge2);
    float a = glm::dot(edge1, h);
    
    if (a > -EPSILON && a < EPSILON) {
        return false; // Ray is parallel to triangle
    }
    
    float f = 1.0f / a;
    glm::vec3 s = rayOrigin - v0;
    float u = f * glm::dot(s, h);
    
    if (u < 0.0f || u > 1.0f) {
        return false;
    }
    
    glm::vec3 q = glm::cross(s, edge1);
    float v = f * glm::dot(rayDirection, q);
    
    if (v < 0.0f || u + v > 1.0f) {
        return false;
    }
    
    t = f * glm::dot(edge2, q);
    
    if (t > EPSILON) {
        intersection = rayOrigin + t * rayDirection;
        return true;
    }
    
    return false;
}

glm::vec3 ComponentPicker::worldToScreen(const glm::vec3& worldPos, int viewportWidth, int viewportHeight) const {
    if (!m_camera) return glm::vec3(0.0f);
    
    glm::mat4 view = m_camera->getViewMatrix();
    glm::mat4 proj = m_camera->getProjectionMatrix();
    
    glm::vec4 clipSpace = proj * view * glm::vec4(worldPos, 1.0f);
    if (clipSpace.w == 0.0f) return glm::vec3(0.0f);
    
    glm::vec3 ndc = glm::vec3(clipSpace) / clipSpace.w;
    
    float x = (ndc.x + 1.0f) * 0.5f * viewportWidth;
    float y = (1.0f - ndc.y) * 0.5f * viewportHeight;
    
    return glm::vec3(x, y, ndc.z);
}

float ComponentPicker::screenSpaceDistance(const glm::vec3& worldPos, float mouseX, float mouseY,
                                          int viewportWidth, int viewportHeight) const {
    glm::vec3 screenPos = worldToScreen(worldPos, viewportWidth, viewportHeight);
    float dx = screenPos.x - mouseX;
    float dy = screenPos.y - mouseY;
    return std::sqrt(dx * dx + dy * dy);
}

} // namespace rude
