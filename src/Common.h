#pragma once

// Include Qt components with proper paths
#include <QtCore/QString>
#include <QtCore/QObject>
#include <QtOpenGLWidgets/QOpenGLWidget>
#include <QtGui/QMatrix4x4>
#include <QtGui/QVector3D>
#include <QtGui/QVector2D>
#include <QtGui/QVector4D>
#include <QtGui/QQuaternion>
#include <memory>
#include <vector>
#include <string>
#include <string_view>
#include <concepts>
#include <ranges>
#include <expected>
#include <span>

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

class Mesh;
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
using MeshPtr = std::shared_ptr<Mesh>;
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
const QVector3D WORLD_UP(0.0f, 1.0f, 0.0f);
const QVector3D WORLD_FORWARD(0.0f, 0.0f, -1.0f);
const QVector3D WORLD_RIGHT(1.0f, 0.0f, 0.0f);

// C++23 Concepts for type safety
template<typename T>
concept Transformable = requires(T t) {
    { t.getTransform() } -> std::convertible_to<Transform&>;
    { t.setTransform(std::declval<Transform>()) } -> std::same_as<void>;
};

template<typename T>
concept Renderable = requires(T t) {
    { t.getMesh() } -> std::convertible_to<MeshPtr>;
    { t.getMaterial() } -> std::convertible_to<MaterialPtr>;
    { t.isVisible() } -> std::convertible_to<bool>;
};
