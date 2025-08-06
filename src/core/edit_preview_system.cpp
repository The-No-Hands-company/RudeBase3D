#include "core/edit_preview_system.hpp"
#include "ecs/ECSManager.h"
#include "ecs/components/CoreComponents.h"
#include "core/scene.hpp"
#include "core/mesh.hpp"
#include "core/entity.hpp"
#include <iostream>
#include <algorithm>
#include <chrono>

namespace RudeBase3D {
namespace Core {

EditPreviewSystem::EditPreviewSystem(rude::ecs::ECSManager* ecsManager, rude::Scene* scene)
    : m_ecsManager(ecsManager)
    , m_scene(scene) {
    
    // Register built-in operations
    registerCustomOperation("smooth_laplacian", [this](const std::shared_ptr<rude::Mesh>& mesh, const EditPreviewParams& params) {
        return applySmooth(mesh, params);
    });
    
    registerCustomOperation("decimate_quadric", [this](const std::shared_ptr<rude::Mesh>& mesh, const EditPreviewParams& params) {
        return applyDecimate(mesh, params);
    });
    
    std::cout << "EditPreviewSystem initialized" << std::endl;
}

EditPreviewSystem::~EditPreviewSystem() {
    cleanup();
}

bool EditPreviewSystem::startPreview(EntityID entityId, EditOperation operation, const EditPreviewParams& params) {
    // Cancel any existing preview
    if (hasActivePreview()) {
        cancelPreview();
    }
    
    // Validate entity
    if (!m_ecsManager->getWorld().isEntityValid(entityId)) {
        std::cerr << "EditPreviewSystem: Entity " << entityId << " does not exist" << std::endl;
        return false;
    }
    
    // Get mesh component
    if (!m_ecsManager->getWorld().hasComponent<rude::ecs::MeshComponent>(entityId)) {
        std::cerr << "EditPreviewSystem: Entity " << entityId << " has no MeshComponent" << std::endl;
        return false;
    }
    
    auto& meshComp = m_ecsManager->getWorld().getComponent<rude::ecs::MeshComponent>(entityId);
    if (!meshComp.mesh) {
        std::cerr << "EditPreviewSystem: Entity " << entityId << " has no valid mesh" << std::endl;
        return false;
    }
    
    // Store original data
    m_originalEntityId = entityId;
    m_originalMesh = meshComp.mesh;
    m_currentOperation = operation;
    m_currentParams = params;
    
    // Get original transform
    if (m_ecsManager->getWorld().hasComponent<rude::ecs::TransformComponent>(entityId)) {
        auto& transformComp = m_ecsManager->getWorld().getComponent<rude::ecs::TransformComponent>(entityId);
        m_originalTransform = transformComp.getMatrix();
    } else {
        m_originalTransform = glm::mat4(1.0f);
    }
    
    // Create preview entity
    createPreviewEntity();
    
    // Set preview state
    m_previewState = PreviewState::Active;
    
    // Update preview with current parameters
    updatePreview(params);
    
    // Fire event
    if (onPreviewStarted) {
        onPreviewStarted(entityId, operation);
    }
    
    std::cout << "Started edit preview for entity " << entityId << std::endl;
    return true;
}

void EditPreviewSystem::updatePreview(const EditPreviewParams& params) {
    if (!hasActivePreview()) {
        return;
    }
    
    m_currentParams = params;
    updatePreviewEntity();
    
    // Add to history if enabled
    if (m_historyEnabled) {
        if (m_ecsManager->getWorld().hasComponent<rude::ecs::MeshComponent>(m_previewEntityId)) {
            auto& previewMesh = m_ecsManager->getWorld().getComponent<rude::ecs::MeshComponent>(m_previewEntityId);
            if (previewMesh.mesh) {
                addToHistory(params, previewMesh.mesh);
            }
        }
    }
    
    // Fire event
    if (onPreviewUpdated) {
        onPreviewUpdated(m_originalEntityId, m_currentOperation);
    }
}

void EditPreviewSystem::commitPreview() {
    if (!hasActivePreview()) {
        return;
    }
    
    // Get the preview mesh
    if (!m_ecsManager->getWorld().hasComponent<rude::ecs::MeshComponent>(m_previewEntityId)) {
        std::cerr << "EditPreviewSystem: Preview entity has no mesh component" << std::endl;
        cancelPreview();
        return;
    }
    
    auto& previewMeshComp = m_ecsManager->getWorld().getComponent<rude::ecs::MeshComponent>(m_previewEntityId);
    if (!previewMeshComp.mesh) {
        std::cerr << "EditPreviewSystem: Preview mesh is invalid" << std::endl;
        cancelPreview();
        return;
    }
    
    // Apply the preview mesh to the original entity
    if (m_ecsManager->getWorld().hasComponent<rude::ecs::MeshComponent>(m_originalEntityId)) {
        auto& originalMeshComp = m_ecsManager->getWorld().getComponent<rude::ecs::MeshComponent>(m_originalEntityId);
        if (m_ecsManager->getWorld().hasComponent<rude::ecs::MeshComponent>(m_previewEntityId)) {
            auto& previewMeshComp = m_ecsManager->getWorld().getComponent<rude::ecs::MeshComponent>(m_previewEntityId);
            originalMeshComp.mesh = previewMeshComp.mesh;
            originalMeshComp.isDirty = true;
        }
    }
    
    // Update preview state
    m_previewState = PreviewState::Committed;
    
    // Clean up preview entity
    destroyPreviewEntity();
    
    // Restore original visibility
    updateOriginalVisibility();
    
    // Fire event
    if (onPreviewFinished) {
        onPreviewFinished(m_originalEntityId, m_currentOperation, true);
    }
    
    std::cout << "Committed edit preview for entity " << m_originalEntityId << std::endl;
    
    // Reset state
    cleanup();
}

void EditPreviewSystem::cancelPreview() {
    if (!hasActivePreview()) {
        return;
    }
    
    // Update preview state
    m_previewState = PreviewState::Cancelled;
    
    // Clean up preview entity
    destroyPreviewEntity();
    
    // Restore original visibility
    updateOriginalVisibility();
    
    // Fire event
    if (onPreviewFinished) {
        onPreviewFinished(m_originalEntityId, m_currentOperation, false);
    }
    
    std::cout << "Cancelled edit preview for entity " << m_originalEntityId << std::endl;
    
    // Reset state
    cleanup();
}

void EditPreviewSystem::setCurrentParams(const EditPreviewParams& params) {
    m_currentParams = params;
    if (hasActivePreview()) {
        updatePreview(params);
    }
}

void EditPreviewSystem::registerCustomOperation(const std::string& name, EditOperationFunction function) {
    m_customOperations[name] = function;
    std::cout << "Registered custom edit operation: " << name << std::endl;
}

bool EditPreviewSystem::startCustomPreview(EntityID entityId, const std::string& operationName, const EditPreviewParams& params) {
    auto it = m_customOperations.find(operationName);
    if (it == m_customOperations.end()) {
        std::cerr << "EditPreviewSystem: Custom operation '" << operationName << "' not found" << std::endl;
        return false;
    }
    
    return startPreview(entityId, EditOperation::Custom, params);
}

bool EditPreviewSystem::startBatchPreview(const std::vector<EntityID>& entityIds, EditOperation operation, const EditPreviewParams& params) {
    // For now, just start preview on the first entity
    // TODO: Implement proper batch preview support
    if (!entityIds.empty()) {
        m_batchEntityIds = entityIds;
        return startPreview(entityIds[0], operation, params);
    }
    return false;
}

bool EditPreviewSystem::canUndoPreview() const {
    return m_historyEnabled && m_historyIndex > 0;
}

bool EditPreviewSystem::canRedoPreview() const {
    return m_historyEnabled && m_historyIndex < static_cast<int>(m_previewHistory.size()) - 1;
}

void EditPreviewSystem::undoPreview() {
    if (!canUndoPreview()) {
        return;
    }
    
    m_historyIndex--;
    const auto& entry = m_previewHistory[m_historyIndex];
    m_currentParams = entry.params;
    
    // Update preview mesh directly
    if (m_ecsManager->getWorld().hasComponent<rude::ecs::MeshComponent>(m_previewEntityId)) {
        auto& meshComp = m_ecsManager->getWorld().getComponent<rude::ecs::MeshComponent>(m_previewEntityId);
        meshComp.mesh = entry.mesh;
        meshComp.isDirty = true;
    }
}

void EditPreviewSystem::redoPreview() {
    if (!canRedoPreview()) {
        return;
    }
    
    m_historyIndex++;
    const auto& entry = m_previewHistory[m_historyIndex];
    m_currentParams = entry.params;
    
    // Update preview mesh directly
    if (m_ecsManager->getWorld().hasComponent<rude::ecs::MeshComponent>(m_previewEntityId)) {
        auto& meshComp = m_ecsManager->getWorld().getComponent<rude::ecs::MeshComponent>(m_previewEntityId);
        meshComp.mesh = entry.mesh;
        meshComp.isDirty = true;
    }
}

void EditPreviewSystem::update(float deltaTime) {
    // Update any animated preview parameters or effects
    if (hasActivePreview()) {
        // Could add subtle animation effects here, like pulsing opacity
        // or updating time-based parameters
    }
}

std::shared_ptr<rude::Mesh> EditPreviewSystem::applyOperation(const std::shared_ptr<rude::Mesh>& mesh, EditOperation operation, const EditPreviewParams& params) {
    switch (operation) {
        case EditOperation::Subdivision:
            return applySubdivision(mesh, params);
        case EditOperation::Extrude:
            return applyExtrude(mesh, params);
        case EditOperation::Bevel:
            return applyBevel(mesh, params);
        case EditOperation::Scale:
            return applyScale(mesh, params);
        case EditOperation::Rotate:
            return applyRotate(mesh, params);
        case EditOperation::Smooth:
            return applySmooth(mesh, params);
        case EditOperation::Decimate:
            return applyDecimate(mesh, params);
        case EditOperation::Boolean:
            // TODO: Implement boolean preview
            return mesh;
        case EditOperation::Custom:
            // Custom operations handled separately
            return mesh;
        default:
            return mesh;
    }
}

std::shared_ptr<rude::Mesh> EditPreviewSystem::applySubdivision(const std::shared_ptr<rude::Mesh>& mesh, const EditPreviewParams& params) {
    if (!mesh) return nullptr;
    
    // For now, just return a copy of the original mesh
    // TODO: Implement proper subdivision using the project's mesh processing tools
    return std::make_shared<rude::Mesh>(*mesh);
}

std::shared_ptr<rude::Mesh> EditPreviewSystem::applyExtrude(const std::shared_ptr<rude::Mesh>& mesh, const EditPreviewParams& params) {
    if (!mesh) return nullptr;
    
    // For now, just return a copy of the original mesh
    // TODO: Implement proper extrude using the project's mesh processing tools
    return std::make_shared<rude::Mesh>(*mesh);
}

std::shared_ptr<rude::Mesh> EditPreviewSystem::applyBevel(const std::shared_ptr<rude::Mesh>& mesh, const EditPreviewParams& params) {
    // Placeholder - bevel is complex to implement properly
    return std::make_shared<rude::Mesh>(*mesh);
}

std::shared_ptr<rude::Mesh> EditPreviewSystem::applyScale(const std::shared_ptr<rude::Mesh>& mesh, const EditPreviewParams& params) {
    if (!mesh) return nullptr;
    
    // For now, just return a copy of the original mesh
    // TODO: Implement proper scaling using the project's mesh processing tools
    return std::make_shared<rude::Mesh>(*mesh);
}

std::shared_ptr<rude::Mesh> EditPreviewSystem::applyRotate(const std::shared_ptr<rude::Mesh>& mesh, const EditPreviewParams& params) {
    // Placeholder - rotation needs angle parameters
    return std::make_shared<rude::Mesh>(*mesh);
}

std::shared_ptr<rude::Mesh> EditPreviewSystem::applySmooth(const std::shared_ptr<rude::Mesh>& mesh, const EditPreviewParams& params) {
    if (!mesh) return nullptr;
    
    // For now, just return a copy of the original mesh
    // TODO: Implement proper smoothing using the project's mesh processing tools
    return std::make_shared<rude::Mesh>(*mesh);
}

std::shared_ptr<rude::Mesh> EditPreviewSystem::applyDecimate(const std::shared_ptr<rude::Mesh>& mesh, const EditPreviewParams& params) {
    // Placeholder - decimation is complex to implement properly
    return std::make_shared<rude::Mesh>(*mesh);
}

void EditPreviewSystem::createPreviewEntity() {
    // Create new entity for preview
    m_previewEntityId = m_ecsManager->getWorld().createEntity();
    
    // Copy mesh component
    if (m_ecsManager->getWorld().hasComponent<rude::ecs::MeshComponent>(m_originalEntityId)) {
        auto& originalMeshComp = m_ecsManager->getWorld().getComponent<rude::ecs::MeshComponent>(m_originalEntityId);
        m_ecsManager->getWorld().addComponent<rude::ecs::MeshComponent>(m_previewEntityId, originalMeshComp);
    }
    
    // Copy transform component
    if (m_ecsManager->getWorld().hasComponent<rude::ecs::TransformComponent>(m_originalEntityId)) {
        auto& originalTransformComp = m_ecsManager->getWorld().getComponent<rude::ecs::TransformComponent>(m_originalEntityId);
        m_ecsManager->getWorld().addComponent<rude::ecs::TransformComponent>(m_previewEntityId, originalTransformComp);
    }
    
    // Setup preview material and visibility
    setupPreviewMaterial();
    updateOriginalVisibility();
}

void EditPreviewSystem::updatePreviewEntity() {
    if (m_previewEntityId == INVALID_ENTITY_ID) {
        return;
    }
    
    // Apply operation to create new mesh
    auto newMesh = applyOperation(m_originalMesh, m_currentOperation, m_currentParams);
    
    // Update preview entity's mesh
    if (m_ecsManager->getWorld().hasComponent<rude::ecs::MeshComponent>(m_previewEntityId)) {
        auto& meshComp = m_ecsManager->getWorld().getComponent<rude::ecs::MeshComponent>(m_previewEntityId);
        if (newMesh) {
            meshComp.mesh = newMesh;
        }
        meshComp.isDirty = true;
    }
}

void EditPreviewSystem::destroyPreviewEntity() {
    if (m_previewEntityId != INVALID_ENTITY_ID) {
        m_ecsManager->getWorld().destroyEntity(m_previewEntityId);
        m_previewEntityId = INVALID_ENTITY_ID;
    }
}

void EditPreviewSystem::setupPreviewMaterial() {
    // TODO: Set up special preview material with transparency/wireframe
    // This would integrate with your material system
}

void EditPreviewSystem::updateOriginalVisibility() {
    if (m_ecsManager->getWorld().hasComponent<rude::ecs::RenderableComponent>(m_originalEntityId)) {
        auto& renderComp = m_ecsManager->getWorld().getComponent<rude::ecs::RenderableComponent>(m_originalEntityId);
        renderComp.visible = m_showOriginal && !hasActivePreview();
    }
}

void EditPreviewSystem::addToHistory(const EditPreviewParams& params, const std::shared_ptr<rude::Mesh>& mesh) {
    if (!m_historyEnabled) return;
    
    // Remove any history after current index
    if (m_historyIndex >= 0 && m_historyIndex < static_cast<int>(m_previewHistory.size()) - 1) {
        m_previewHistory.erase(m_previewHistory.begin() + m_historyIndex + 1, m_previewHistory.end());
    }
    
    // Add new entry
    PreviewHistoryEntry entry;
    entry.params = params;
    entry.mesh = mesh;
    entry.timestamp = std::chrono::duration<float>(std::chrono::steady_clock::now().time_since_epoch()).count();
    
    m_previewHistory.push_back(entry);
    m_historyIndex = m_previewHistory.size() - 1;
    
    // Limit history size
    const size_t maxHistorySize = 50;
    if (m_previewHistory.size() > maxHistorySize) {
        m_previewHistory.erase(m_previewHistory.begin());
        m_historyIndex--;
    }
}

void EditPreviewSystem::clearHistory() {
    m_previewHistory.clear();
    m_historyIndex = -1;
}

void EditPreviewSystem::cleanup() {
    m_previewState = PreviewState::None;
    m_originalEntityId = INVALID_ENTITY_ID;
    m_previewEntityId = INVALID_ENTITY_ID;
    m_originalMesh.reset();
    m_batchEntityIds.clear();
    m_batchPreviewIds.clear();
    clearHistory();
}

} // namespace Core
} // namespace RudeBase3D
