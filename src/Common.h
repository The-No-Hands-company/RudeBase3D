#pragma once

#ifdef USE_QT
#include <QtCore/QString>
#include <QtCore/QObject>
#include <QtOpenGLWidgets/QOpenGLWidget>
#endif
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <memory>
#include <vector>
#include <string>
#include <string_view>
#include <concepts>
#include <ranges>
#include <expected>
#include <span>
#include <glm/glm.hpp>
#include "core/mesh_forward.hpp"

// Conversion utilities between Qt and GLM types (deprecated, to be removed)
// inline glm::vec3 toGlm(const QVector3D& qvec) {
//     return glm::vec3(qvec.x(), qvec.y(), qvec.z());
// }
// inline glm::vec2 toGlm(const QVector2D& qvec) {
//     return glm::vec2(qvec.x(), qvec.y());
// }
// inline QVector3D toQt(const glm::vec3& gvec) {
//     return QVector3D(gvec.x, gvec.y, gvec.z);
// }
// inline QVector2D toQt(const glm::vec2& gvec) {
//     return QVector2D(gvec.x, gvec.y);
// }

// Forward declarations
class Scene;
class SceneObject;
class Camera;
namespace rude {
    class HalfEdgeMesh;
    class Vertex;
    class Edge;
    class Face;
}

class Material;
class Transform;
class GeometryConverter;
class NURBSSurface;
class SubdivisionMesh;
class VoxelGrid;
class PointCloud;
class ImplicitSurface;
class BVHTree;
class HybridGeometry;
class HybridGeometryManager;
class GeometryProcessingPipeline;

// Forward declaration of PrimitiveType from rude namespace
namespace rude {
    enum class PrimitiveType;
}

// Enum for transform modes
enum class TransformMode {
    Select,
    Translate,
    Rotate,
    Scale
};

// Enum for rendering modes
enum class RenderMode {
    Wireframe,
    Solid,
    SolidWireframe
};

// Enum for edit modes
enum class EditMode {
    Object,      // Transform entire objects
    Edit,        // Vertex/edge/face editing
    Sculpt,      // High-resolution sculpting
    UV,          // UV coordinate editing
    Paint,       // Texture painting
    Animation    // Rigging and animation
};

// Enum for selection types
enum class SelectionType {
    Object,      // Entire objects
    Vertex,      // Individual vertices
    Edge,        // Mesh edges
    Face,        // Mesh faces
    Loop,        // Edge/face loops
    Ring         // Edge/face rings
};

// Common typedefs
using SceneObjectPtr = std::shared_ptr<SceneObject>;
using MaterialPtr = std::shared_ptr<Material>;
using HalfEdgeMeshPtr = std::shared_ptr<rude::HalfEdgeMesh>;
using HalfEdgeVertexPtr = std::shared_ptr<rude::Vertex>;
using HalfEdgeEdgePtr = std::shared_ptr<rude::Edge>;
using HalfEdgeFacePtr = std::shared_ptr<rude::Face>;
using NURBSSurfacePtr = std::shared_ptr<NURBSSurface>;
using SubdivisionMeshPtr = std::shared_ptr<SubdivisionMesh>;
using VoxelGridPtr = std::shared_ptr<VoxelGrid>;
using PointCloudPtr = std::shared_ptr<PointCloud>;
using ImplicitSurfacePtr = std::shared_ptr<ImplicitSurface>;
using BVHTreePtr = std::shared_ptr<BVHTree>;
using HybridGeometryPtr = std::shared_ptr<HybridGeometry>;

// Geometry processing constants
constexpr float PI = 3.14159265359f;
constexpr float EPSILON = 1e-6f;
constexpr float DEG_TO_RAD = PI / 180.0f;
constexpr float RAD_TO_DEG = 180.0f / PI;

// World coordinate system constants
const glm::vec3 WORLD_UP(0.0f, 1.0f, 0.0f);
const glm::vec3 WORLD_FORWARD(0.0f, 0.0f, -1.0f);
const glm::vec3 WORLD_RIGHT(1.0f, 0.0f, 0.0f);

// Conversion functions between Qt and GLM types (deprecated, to be removed)
// inline glm::vec3 qtToGlm(const QVector3D& v) {
//     return glm::vec3(v.x(), v.y(), v.z());
// }
// inline glm::vec2 qtToGlm(const QVector2D& v) {
//     return glm::vec2(v.x(), v.y());
// }
// inline QVector3D glmToQt(const glm::vec3& v) {
//     return QVector3D(v.x, v.y, v.z);
// }
// inline QVector2D glmToQt(const glm::vec2& v) {
//     return QVector2D(v.x, v.y);
// }

// C++23 Concepts for type safety
template<typename T>
concept Transformable = requires(T t) {
    { t.getTransform() } -> std::convertible_to<Transform&>;
    { t.setTransform(std::declval<Transform>()) } -> std::same_as<void>;
};

template<typename T>
concept Renderable = requires(T t) {
    { t.getMesh() } -> std::convertible_to<rude::MeshPtr>;
    { t.getMaterial() } -> std::convertible_to<MaterialPtr>;
    { t.isVisible() } -> std::convertible_to<bool>;
};
