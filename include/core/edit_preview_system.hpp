//==============================================================================
// RudeBase3D Engine - edit_preview_system.hpp
//==============================================================================
// Professional 3D Modeling & Animation Suite
// https://github.com/The-No-Hands-company/RudeBase3D
//
// Copyright (c) 2025 The-No-Hands-company
// Licensed under the MIT License
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to 
// deal in the Software without restriction, including without limitation the 
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or 
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//==============================================================================

#pragma once

#include <memory>
#include <functional>
#include <unordered_map>
#include <string>
#include <glm/glm.hpp>

// Forward declarations
namespace RudeBase3D {
namespace Core {
    class Scene;
}
}

namespace rude {
namespace ecs {
    class ECSManager;
}
    class Mesh;
    class Scene;
}

class Entity;
using EntityID = unsigned int;
constexpr EntityID INVALID_ENTITY_ID = 0;

namespace RudeBase3D {
namespace Core {

class Scene;

/**
 * @brief Preview state for an edit operation
 */
enum class PreviewState {
    None,       // No preview active
    Active,     // Preview is being shown
    Committed,  // Preview was applied
    Cancelled   // Preview was cancelled
};

/**
 * @brief Parameters for edit preview operations
 */
struct EditPreviewParams {
    // Common parameters
    float intensity = 1.0f;
    bool enabled = true;
    
    // Subdivision parameters
    int subdivisionLevels = 1;
    bool smoothSubdivision = true;
    
    // Extrude parameters
    glm::vec3 extrudeDirection = glm::vec3(0.0f, 0.0f, 1.0f);
    float extrudeDistance = 1.0f;
    
    // Bevel parameters
    float bevelWidth = 0.1f;
    int bevelSegments = 3;
    
    // Scale parameters
    glm::vec3 scaleFactors = glm::vec3(1.0f);
    glm::vec3 scalePivot = glm::vec3(0.0f);
    
    // Boolean parameters
    EntityID booleanTarget = INVALID_ENTITY_ID;
    enum class BooleanOperation { Union, Subtraction, Intersection } booleanOp = BooleanOperation::Union;
};

/**
 * @brief Edit preview operation types
 */
enum class EditOperation {
    Subdivision,
    Extrude,
    Bevel,
    Scale,
    Rotate,
    Boolean,
    Smooth,
    Decimate,
    Custom
};

/**
 * @brief Function signature for custom edit operations
 */
using EditOperationFunction = std::function<std::shared_ptr<rude::Mesh>(
    const std::shared_ptr<rude::Mesh>& originalMesh, 
    const EditPreviewParams& params
)>;

/**
 * @brief System for real-time edit previews before committing changes
 * 
 * This system allows users to see the results of modeling operations
 * in real-time before deciding to apply or cancel them, providing
 * a non-destructive workflow similar to advanced sculpting applications.
 */
class EditPreviewSystem {
public:
    /**
     * @brief Constructor
     * @param ecsManager ECS manager for component operations
     * @param scene Scene for preview entity management
     */
    EditPreviewSystem(rude::ecs::ECSManager* ecsManager, rude::Scene* scene);
    ~EditPreviewSystem();

    // Core preview operations
    bool startPreview(EntityID entityId, EditOperation operation, const EditPreviewParams& params = {});
    void updatePreview(const EditPreviewParams& params);
    void commitPreview();
    void cancelPreview();
    
    // Preview state management
    bool hasActivePreview() const { return m_previewState == PreviewState::Active; }
    PreviewState getPreviewState() const { return m_previewState; }
    EntityID getPreviewEntity() const { return m_previewEntityId; }
    EditOperation getCurrentOperation() const { return m_currentOperation; }
    
    // Parameter access
    const EditPreviewParams& getCurrentParams() const { return m_currentParams; }
    void setCurrentParams(const EditPreviewParams& params);
    
    // Visual settings
    void setPreviewMaterial(const std::string& materialName) { m_previewMaterialName = materialName; }
    void setPreviewOpacity(float opacity) { m_previewOpacity = opacity; }
    void setShowOriginal(bool show) { m_showOriginal = show; }
    void setWireframeOverlay(bool enable) { m_wireframeOverlay = enable; }
    
    // Custom operations
    void registerCustomOperation(const std::string& name, EditOperationFunction function);
    bool startCustomPreview(EntityID entityId, const std::string& operationName, const EditPreviewParams& params = {});
    
    // Batch preview (for multiple objects)
    bool startBatchPreview(const std::vector<EntityID>& entityIds, EditOperation operation, const EditPreviewParams& params = {});
    
    // Preview history
    void enablePreviewHistory(bool enable) { m_historyEnabled = enable; }
    bool canUndoPreview() const;
    bool canRedoPreview() const;
    void undoPreview();
    void redoPreview();
    
    // Events
    std::function<void(EntityID, EditOperation)> onPreviewStarted;
    std::function<void(EntityID, EditOperation)> onPreviewUpdated;
    std::function<void(EntityID, EditOperation, bool)> onPreviewFinished; // bool: committed or cancelled
    
    // Update system
    void update(float deltaTime);
    
private:
    // Core data
    rude::ecs::ECSManager* m_ecsManager;
    rude::Scene* m_scene;
    
    // Preview state
    PreviewState m_previewState = PreviewState::None;
    EntityID m_originalEntityId = INVALID_ENTITY_ID;
    EntityID m_previewEntityId = INVALID_ENTITY_ID;
    EditOperation m_currentOperation = EditOperation::Subdivision;
    EditPreviewParams m_currentParams;
    
    // Original data backup
    std::shared_ptr<rude::Mesh> m_originalMesh;
    glm::mat4 m_originalTransform;
    
    // Visual settings
    std::string m_previewMaterialName = "preview_material";
    float m_previewOpacity = 0.7f;
    bool m_showOriginal = true;
    bool m_wireframeOverlay = true;
    
    // Custom operations
    std::unordered_map<std::string, EditOperationFunction> m_customOperations;
    
    // Batch preview support
    std::vector<EntityID> m_batchEntityIds;
    std::vector<EntityID> m_batchPreviewIds;
    
    // History system
    bool m_historyEnabled = false;
    struct PreviewHistoryEntry {
        EditPreviewParams params;
        std::shared_ptr<rude::Mesh> mesh;
        float timestamp;
    };
    std::vector<PreviewHistoryEntry> m_previewHistory;
    int m_historyIndex = -1;
    
    // Internal methods
    std::shared_ptr<rude::Mesh> applyOperation(const std::shared_ptr<rude::Mesh>& mesh, EditOperation operation, const EditPreviewParams& params);
    std::shared_ptr<rude::Mesh> applySubdivision(const std::shared_ptr<rude::Mesh>& mesh, const EditPreviewParams& params);
    std::shared_ptr<rude::Mesh> applyExtrude(const std::shared_ptr<rude::Mesh>& mesh, const EditPreviewParams& params);
    std::shared_ptr<rude::Mesh> applyBevel(const std::shared_ptr<rude::Mesh>& mesh, const EditPreviewParams& params);
    std::shared_ptr<rude::Mesh> applyScale(const std::shared_ptr<rude::Mesh>& mesh, const EditPreviewParams& params);
    std::shared_ptr<rude::Mesh> applyRotate(const std::shared_ptr<rude::Mesh>& mesh, const EditPreviewParams& params);
    std::shared_ptr<rude::Mesh> applySmooth(const std::shared_ptr<rude::Mesh>& mesh, const EditPreviewParams& params);
    std::shared_ptr<rude::Mesh> applyDecimate(const std::shared_ptr<rude::Mesh>& mesh, const EditPreviewParams& params);
    
    void createPreviewEntity();
    void updatePreviewEntity();
    void destroyPreviewEntity();
    void setupPreviewMaterial();
    void updateOriginalVisibility();
    
    void addToHistory(const EditPreviewParams& params, const std::shared_ptr<rude::Mesh>& mesh);
    void clearHistory();
    
    // Cleanup
    void cleanup();
};

} // namespace Core
} // namespace RudeBase3D
