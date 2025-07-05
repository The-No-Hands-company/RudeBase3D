#include <glm/glm.hpp>
#include "HalfEdgeMesh.h"
#include <algorithm>
#include <unordered_set>
#include <queue>
#include <QDebug>

// HalfEdgeVertex Implementation
HalfEdgeVertex::HalfEdgeVertex(const glm::vec3& position, unsigned int id)
    : m_position(position)
    , m_id(id)
    , m_normal(0.0f, 1.0f, 0.0f)
    , m_texCoord(0.0f, 0.0f)
{
}

std::vector<HalfEdgeEdgePtr> HalfEdgeVertex::getIncomingEdges() const {
    std::vector<HalfEdgeEdgePtr> edges;
    auto outgoing = getOutgoingEdge();
    if (!outgoing) return edges;
    
    auto current = outgoing;
    do {
        if (auto twin = current->getTwin()) {
            edges.push_back(twin);
            current = twin->getNext();
        } else {
            break;
        }
    } while (current && current != outgoing);
    
    return edges;
}

std::vector<HalfEdgeEdgePtr> HalfEdgeVertex::getOutgoingEdges() const {
    std::vector<HalfEdgeEdgePtr> edges;
    auto outgoing = getOutgoingEdge();
    if (!outgoing) return edges;
    
    auto current = outgoing;
    do {
        edges.push_back(current);
        if (auto twin = current->getTwin()) {
            current = twin->getNext();
        } else {
            break;
        }
    } while (current && current != outgoing);
    
    return edges;
}

std::vector<HalfEdgeVertexPtr> HalfEdgeVertex::getAdjacentVertices() const {
    std::vector<HalfEdgeVertexPtr> vertices;
    auto outgoingEdges = getOutgoingEdges();
    
    for (const auto& edge : outgoingEdges) {
        if (auto target = edge->getTargetVertex()) {
            vertices.push_back(target);
        }
    }
    
    return vertices;
}

std::vector<HalfEdgeFacePtr> HalfEdgeVertex::getAdjacentFaces() const {
    std::vector<HalfEdgeFacePtr> faces;
    auto outgoingEdges = getOutgoingEdges();
    
    for (const auto& edge : outgoingEdges) {
        if (auto face = edge->getFace()) {
            faces.push_back(face);
        }
    }
    
    return faces;
}

int HalfEdgeVertex::getValence() const {
    return static_cast<int>(getOutgoingEdges().size());
}

bool HalfEdgeVertex::isBoundary() const {
    auto outgoingEdges = getOutgoingEdges();
    for (const auto& edge : outgoingEdges) {
        if (edge->isBoundary()) {
            return true;
        }
    }
    return false;
}

// HalfEdgeEdge Implementation
HalfEdgeEdge::HalfEdgeEdge(unsigned int id)
    : m_id(id)
{
}

bool HalfEdgeEdge::isBoundary() const {
    return !getFace() || !getTwin() || !getTwin()->getFace();
}

glm::vec3 HalfEdgeEdge::getVector() const {
    auto origin = getOriginVertex();
    auto target = getTargetVertex();
    if (origin && target) {
        return target->getPosition() - origin->getPosition();
    }
    return glm::vec3();
}

float HalfEdgeEdge::getLength() const {
    return glm::length(getVector());
}

glm::vec3 HalfEdgeEdge::getMidpoint() const {
    auto origin = getOriginVertex();
    auto target = getTargetVertex();
    if (origin && target) {
        return (origin->getPosition() + target->getPosition()) * 0.5f;
    }
    return glm::vec3();
}

// HalfEdgeFace Implementation
HalfEdgeFace::HalfEdgeFace(unsigned int id)
    : m_id(id)
    , m_normal(0.0f, 1.0f, 0.0f)
{
}

std::vector<HalfEdgeEdgePtr> HalfEdgeFace::getEdges() const {
    std::vector<HalfEdgeEdgePtr> edges;
    auto startEdge = getOuterEdge();
    if (!startEdge) return edges;
    
    auto current = startEdge;
    do {
        edges.push_back(current);
        current = current->getNext();
    } while (current && current != startEdge);
    
    return edges;
}

std::vector<HalfEdgeVertexPtr> HalfEdgeFace::getVertices() const {
    std::vector<HalfEdgeVertexPtr> vertices;
    auto edges = getEdges();
    
    for (const auto& edge : edges) {
        if (auto vertex = edge->getOriginVertex()) {
            vertices.push_back(vertex);
        }
    }
    
    return vertices;
}

std::vector<HalfEdgeFacePtr> HalfEdgeFace::getAdjacentFaces() const {
    std::vector<HalfEdgeFacePtr> faces;
    auto edges = getEdges();
    
    for (const auto& edge : edges) {
        if (auto twin = edge->getTwin()) {
            if (auto face = twin->getFace()) {
                faces.push_back(face);
            }
        }
    }
    
    return faces;
}

int HalfEdgeFace::getVertexCount() const {
    return static_cast<int>(getEdges().size());
}

glm::vec3 HalfEdgeFace::computeNormal() const {
    auto vertices = getVertices();
    if (vertices.size() < 3) return glm::vec3(0, 1, 0);
    
    // Use Newell's method for robust normal calculation
    glm::vec3 normal(0, 0, 0);
    for (size_t i = 0; i < vertices.size(); ++i) {
        auto v1 = vertices[i]->getPosition();
        auto v2 = vertices[(i + 1) % vertices.size()]->getPosition();
        
        normal.x += (v1.y - v2.y) * (v1.z + v2.z);
        normal.y += (v1.z - v2.z) * (v1.x + v2.x);
        normal.z += (v1.x - v2.x) * (v1.y + v2.y);
    }
    
    return glm::normalize(normal);
}

glm::vec3 HalfEdgeFace::getCentroid() const {
    auto vertices = getVertices();
    if (vertices.empty()) return glm::vec3();
    
    glm::vec3 centroid(0, 0, 0);
    for (const auto& vertex : vertices) {
        centroid += vertex->getPosition();
    }
    
    return centroid / static_cast<float>(vertices.size());
}

float HalfEdgeFace::getArea() const {
    auto vertices = getVertices();
    if (vertices.size() < 3) return 0.0f;
    
    // For triangles, use cross product
    if (vertices.size() == 3) {
        auto v0 = vertices[0]->getPosition();
        auto v1 = vertices[1]->getPosition();
        auto v2 = vertices[2]->getPosition();
        
        auto edge1 = v1 - v0;
        auto edge2 = v2 - v0;
        
        return glm::length(glm::cross(edge1, edge2)) * 0.5f;
    }
    
    // For polygons, triangulate and sum areas
    float totalArea = 0.0f;
    auto centroid = getCentroid();
    
    for (size_t i = 0; i < vertices.size(); ++i) {
        auto v1 = vertices[i]->getPosition();
        auto v2 = vertices[(i + 1) % vertices.size()]->getPosition();
        
        auto edge1 = v1 - centroid;
        auto edge2 = v2 - centroid;
        
        totalArea += glm::length(glm::cross(edge1, edge2)) * 0.5f;
    }
    
    return totalArea;
}

// HalfEdgeMesh Implementation
HalfEdgeMesh::HalfEdgeMesh() {
}

void HalfEdgeMesh::clear() {
    m_vertices.clear();
    m_edges.clear();
    m_faces.clear();
    m_vertexMap.clear();
    m_edgeMap.clear();
    m_faceMap.clear();
    m_nextVertexId = 1;
    m_nextEdgeId = 1;
    m_nextFaceId = 1;
}

bool HalfEdgeMesh::isEmpty() const {
    return m_vertices.empty() && m_edges.empty() && m_faces.empty();
}

HalfEdgeVertexPtr HalfEdgeMesh::addVertex(const glm::vec3& position) {
    auto vertex = std::make_shared<HalfEdgeVertex>(position, m_nextVertexId++);
    m_vertices.push_back(vertex);
    m_vertexMap[vertex->getId()] = vertex;
    return vertex;
}

HalfEdgeEdgePtr HalfEdgeMesh::addEdge(HalfEdgeVertexPtr origin, HalfEdgeVertexPtr target) {
    if (!origin || !target || origin == target) {
        return nullptr;
    }
    
    // Check if edge already exists
    auto existingEdge = findEdge(origin, target);
    if (existingEdge) {
        return existingEdge;
    }
    
    // Create the half-edge
    auto edge = std::make_shared<HalfEdgeEdge>(m_nextEdgeId++);
    edge->setOriginVertex(origin);
    edge->setTargetVertex(target);
    
    // Try to find the twin edge
    auto twin = findEdge(target, origin);
    if (twin) {
        edge->setTwin(twin);
        twin->setTwin(edge);
    }
    
    // Set as outgoing edge for origin vertex if it doesn't have one
    if (!origin->getOutgoingEdge()) {
        origin->setOutgoingEdge(edge);
    }
    
    m_edges.push_back(edge);
    m_edgeMap[edge->getId()] = edge;
    
    return edge;
}

HalfEdgeFacePtr HalfEdgeMesh::addFace(const std::vector<HalfEdgeVertexPtr>& vertices) {
    if (vertices.size() < 3) {
        qWarning() << "Cannot create face with less than 3 vertices";
        return nullptr;
    }
    
    // Create face
    auto face = std::make_shared<HalfEdgeFace>(m_nextFaceId++);
    
    // Create or find edges for the face
    std::vector<HalfEdgeEdgePtr> faceEdges;
    for (size_t i = 0; i < vertices.size(); ++i) {
        auto origin = vertices[i];
        auto target = vertices[(i + 1) % vertices.size()];
        
        auto edge = addEdge(origin, target);
        if (!edge) {
            qWarning() << "Failed to create edge for face";
            return nullptr;
        }
        
        edge->setFace(face);
        faceEdges.push_back(edge);
    }
    
    // Connect edges in a loop
    for (size_t i = 0; i < faceEdges.size(); ++i) {
        auto current = faceEdges[i];
        auto next = faceEdges[(i + 1) % faceEdges.size()];
        auto prev = faceEdges[(i + faceEdges.size() - 1) % faceEdges.size()];
        
        current->setNext(next);
        current->setPrev(prev);
    }
    
    // Set one edge as the outer edge
    if (!faceEdges.empty()) {
        face->setOuterEdge(faceEdges[0]);
    }
    
    // Compute and set face normal
    face->setNormal(face->computeNormal());
    
    m_faces.push_back(face);
    m_faceMap[face->getId()] = face;
    
    return face;
}

bool HalfEdgeMesh::removeVertex(HalfEdgeVertexPtr vertex) {
    if (!vertex) return false;
    
    // Remove all adjacent edges and faces
    auto outgoingEdges = vertex->getOutgoingEdges();
    for (const auto& edge : outgoingEdges) {
        if (auto face = edge->getFace()) {
            removeFace(face);
        }
        removeEdge(edge);
    }
    
    // Remove vertex from collections
    auto it = std::find(m_vertices.begin(), m_vertices.end(), vertex);
    if (it != m_vertices.end()) {
        m_vertices.erase(it);
    }
    m_vertexMap.erase(vertex->getId());
    
    return true;
}

bool HalfEdgeMesh::removeEdge(HalfEdgeEdgePtr edge) {
    if (!edge) return false;
    
    // Remove associated face if it exists
    if (auto face = edge->getFace()) {
        removeFace(face);
    }
    
    // Handle twin edge
    if (auto twin = edge->getTwin()) {
        twin->setTwin(nullptr);
        if (auto twinFace = twin->getFace()) {
            removeFace(twinFace);
        }
    }
    
    // Update vertex outgoing edge if necessary
    if (auto origin = edge->getOriginVertex()) {
        if (origin->getOutgoingEdge() == edge) {
            // Find another outgoing edge
            auto outgoingEdges = origin->getOutgoingEdges();
            HalfEdgeEdgePtr newOutgoing = nullptr;
            for (const auto& e : outgoingEdges) {
                if (e != edge) {
                    newOutgoing = e;
                    break;
                }
            }
            origin->setOutgoingEdge(newOutgoing);
        }
    }
    
    // Remove edge from collections
    auto it = std::find(m_edges.begin(), m_edges.end(), edge);
    if (it != m_edges.end()) {
        m_edges.erase(it);
    }
    m_edgeMap.erase(edge->getId());
    
    return true;
}

bool HalfEdgeMesh::removeFace(HalfEdgeFacePtr face) {
    if (!face) return false;
    
    // Clear face reference from edges
    auto edges = face->getEdges();
    for (const auto& edge : edges) {
        edge->setFace(nullptr);
    }
    
    // Remove face from collections
    auto it = std::find(m_faces.begin(), m_faces.end(), face);
    if (it != m_faces.end()) {
        m_faces.erase(it);
    }
    m_faceMap.erase(face->getId());
    
    return true;
}

HalfEdgeVertexPtr HalfEdgeMesh::getVertex(unsigned int id) const {
    auto it = m_vertexMap.find(id);
    return (it != m_vertexMap.end()) ? it->second : nullptr;
}

HalfEdgeEdgePtr HalfEdgeMesh::getEdge(unsigned int id) const {
    auto it = m_edgeMap.find(id);
    return (it != m_edgeMap.end()) ? it->second : nullptr;
}

HalfEdgeFacePtr HalfEdgeMesh::getFace(unsigned int id) const {
    auto it = m_faceMap.find(id);
    return (it != m_faceMap.end()) ? it->second : nullptr;
}

std::vector<HalfEdgeVertexPtr> HalfEdgeMesh::getSelectedVertices() const {
    std::vector<HalfEdgeVertexPtr> selected;
    for (const auto& vertex : m_vertices) {
        if (vertex->isSelected()) {
            selected.push_back(vertex);
        }
    }
    return selected;
}

std::vector<HalfEdgeEdgePtr> HalfEdgeMesh::getSelectedEdges() const {
    std::vector<HalfEdgeEdgePtr> selected;
    for (const auto& edge : m_edges) {
        if (edge->isSelected()) {
            selected.push_back(edge);
        }
    }
    return selected;
}

std::vector<HalfEdgeFacePtr> HalfEdgeMesh::getSelectedFaces() const {
    std::vector<HalfEdgeFacePtr> selected;
    for (const auto& face : m_faces) {
        if (face->isSelected()) {
            selected.push_back(face);
        }
    }
    return selected;
}

void HalfEdgeMesh::clearSelection() {
    for (auto& vertex : m_vertices) {
        vertex->setSelected(false);
    }
    for (auto& edge : m_edges) {
        edge->setSelected(false);
    }
    for (auto& face : m_faces) {
        face->setSelected(false);
    }
}

void HalfEdgeMesh::selectAll() {
    for (auto& vertex : m_vertices) {
        vertex->setSelected(true);
    }
    for (auto& edge : m_edges) {
        edge->setSelected(true);
    }
    for (auto& face : m_faces) {
        face->setSelected(true);
    }
}

void HalfEdgeMesh::invertSelection() {
    for (auto& vertex : m_vertices) {
        vertex->setSelected(!vertex->isSelected());
    }
    for (auto& edge : m_edges) {
        edge->setSelected(!edge->isSelected());
    }
    for (auto& face : m_faces) {
        face->setSelected(!face->isSelected());
    }
}

bool HalfEdgeMesh::validate() const {
    // Check vertex-edge consistency
    for (const auto& vertex : m_vertices) {
        auto outgoing = vertex->getOutgoingEdge();
        if (outgoing && outgoing->getOriginVertex() != vertex) {
            qWarning() << "Vertex" << vertex->getId() << "has inconsistent outgoing edge";
            return false;
        }
    }
    
    // Check edge consistency
    for (const auto& edge : m_edges) {
        // Check twin consistency
        if (auto twin = edge->getTwin()) {
            if (twin->getTwin() != edge) {
                qWarning() << "Edge" << edge->getId() << "has inconsistent twin relationship";
                return false;
            }
            if (edge->getOriginVertex() != twin->getTargetVertex() ||
                edge->getTargetVertex() != twin->getOriginVertex()) {
                qWarning() << "Edge" << edge->getId() << "has inconsistent vertex-twin relationship";
                return false;
            }
        }
        
        // Check next/prev consistency
        if (auto next = edge->getNext()) {
            if (next->getPrev() != edge) {
                qWarning() << "Edge" << edge->getId() << "has inconsistent next/prev relationship";
                return false;
            }
        }
    }
    
    // Check face consistency
    for (const auto& face : m_faces) {
        auto outerEdge = face->getOuterEdge();
        if (outerEdge && outerEdge->getFace() != face) {
            qWarning() << "Face" << face->getId() << "has inconsistent outer edge";
            return false;
        }
    }
    
    return true;
}

bool HalfEdgeMesh::repair() {
    // Basic repair operations
    updateConnectivity();
    updateNormals();
    return validate();
}

glm::vec3 HalfEdgeMesh::getBoundingBoxMin() const {
    if (m_vertices.empty()) return glm::vec3();
    
    glm::vec3 min = m_vertices[0]->getPosition();
    for (const auto& vertex : m_vertices) {
        const auto& pos = vertex->getPosition();
        min.x = std::min(min.x, pos.x);
        min.y = std::min(min.y, pos.y);
        min.z = std::min(min.z, pos.z);
    }
    return min;
}

glm::vec3 HalfEdgeMesh::getBoundingBoxMax() const {
    if (m_vertices.empty()) return glm::vec3();
    
    glm::vec3 max = m_vertices[0]->getPosition();
    for (const auto& vertex : m_vertices) {
        const auto& pos = vertex->getPosition();
        max.x = std::max(max.x, pos.x);
        max.y = std::max(max.y, pos.y);
        max.z = std::max(max.z, pos.z);
    }
    return max;
}

glm::vec3 HalfEdgeMesh::getCentroid() const {
    if (m_vertices.empty()) return glm::vec3();
    
    glm::vec3 centroid(0, 0, 0);
    for (const auto& vertex : m_vertices) {
        centroid += vertex->getPosition();
    }
    return centroid / static_cast<float>(m_vertices.size());
}

void HalfEdgeMesh::updateNormals() {
    // Update face normals
    for (auto& face : m_faces) {
        face->setNormal(face->computeNormal());
    }
    
    // Update vertex normals (area-weighted average of adjacent face normals)
    for (auto& vertex : m_vertices) {
        auto adjacentFaces = vertex->getAdjacentFaces();
        if (adjacentFaces.empty()) continue;
        
        glm::vec3 normal(0, 0, 0);
        for (const auto& face : adjacentFaces) {
            normal += face->getNormal() * face->getArea();
        }
        
        vertex->setNormal(glm::normalize(normal));
    }
}

void HalfEdgeMesh::updateTextureCoordinates() {
    // Simple planar projection for now
    auto min = getBoundingBoxMin();
    auto max = getBoundingBoxMax();
    auto size = max - min;
    
    for (auto& vertex : m_vertices) {
        auto pos = vertex->getPosition();
        float u = (pos.x - min.x) / size.x;
        float v = (pos.z - min.z) / size.z;
        vertex->setTexCoord(glm::vec2(u, v));
    }
}

bool HalfEdgeMesh::isManifold() const {
    // Check if every edge has at most 2 adjacent faces
    for (const auto& edge : m_edges) {
        int faceCount = 0;
        if (edge->getFace()) faceCount++;
        if (auto twin = edge->getTwin()) {
            if (twin->getFace()) faceCount++;
        }
        if (faceCount > 2) {
            return false;
        }
    }
    
    // Check if every vertex has a consistent fan of faces
    for (const auto& vertex : m_vertices) {
        auto outgoingEdges = vertex->getOutgoingEdges();
        std::unordered_set<HalfEdgeFacePtr> adjacentFaces;
        
        for (const auto& edge : outgoingEdges) {
            if (auto face = edge->getFace()) {
                adjacentFaces.insert(face);
            }
        }
        
        // For manifold vertices, the number of faces should match edges (for interior vertices)
        // or edges - 1 (for boundary vertices)
        if (vertex->isBoundary()) {
            if (adjacentFaces.size() != outgoingEdges.size() - 1) {
                return false;
            }
        } else {
            if (adjacentFaces.size() != outgoingEdges.size()) {
                return false;
            }
        }
    }
    
    return true;
}

bool HalfEdgeMesh::isClosed() const {
    // A mesh is closed if it has no boundary edges
    for (const auto& edge : m_edges) {
        if (edge->isBoundary()) {
            return false;
        }
    }
    return true;
}

int HalfEdgeMesh::getEulerCharacteristic() const {
    return static_cast<int>(m_vertices.size()) - 
           static_cast<int>(m_edges.size() / 2) + // Each edge pair counts as one
           static_cast<int>(m_faces.size());
}

std::vector<HalfEdgeVertexPtr> HalfEdgeMesh::getBoundaryVertices() const {
    std::vector<HalfEdgeVertexPtr> boundaryVertices;
    for (const auto& vertex : m_vertices) {
        if (vertex->isBoundary()) {
            boundaryVertices.push_back(vertex);
        }
    }
    return boundaryVertices;
}

std::vector<HalfEdgeEdgePtr> HalfEdgeMesh::getBoundaryEdges() const {
    std::vector<HalfEdgeEdgePtr> boundaryEdges;
    for (const auto& edge : m_edges) {
        if (edge->isBoundary()) {
            boundaryEdges.push_back(edge);
        }
    }
    return boundaryEdges;
}

HalfEdgeEdgePtr HalfEdgeMesh::findEdge(HalfEdgeVertexPtr origin, HalfEdgeVertexPtr target) const {
    if (!origin || !target) return nullptr;
    
    auto outgoingEdges = origin->getOutgoingEdges();
    for (const auto& edge : outgoingEdges) {
        if (edge->getTargetVertex() == target) {
            return edge;
        }
    }
    
    return nullptr;
}

void HalfEdgeMesh::updateConnectivity() {
    // Update target vertices for all edges (derived from next edge's origin)
    for (auto& edge : m_edges) {
        if (auto next = edge->getNext()) {
            edge->setTargetVertex(next->getOriginVertex());
        }
    }
}

void HalfEdgeMesh::connectEdges(HalfEdgeEdgePtr edge1, HalfEdgeEdgePtr edge2) {
    if (!edge1 || !edge2) return;
    
    edge1->setNext(edge2);
    edge2->setPrev(edge1);
}
