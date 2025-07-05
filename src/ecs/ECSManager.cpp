#include "ecs/ECSManager.h"
#include <iostream>
#include <sstream>

namespace rude::ecs {

ECSManager::ECSManager() {
    m_world = std::make_unique<World>();
}

ECSManager::~ECSManager() {
    if (m_initialized) {
        shutdown();
    }
}

bool ECSManager::initialize() {
    if (m_initialized) {
        return true;
    }

    try {
        // Register all core components
        registerCoreComponents();
        
        // Register and setup all core systems
        registerCoreSystems();
        
        // Setup system signatures
        setupSystemSignatures();
        
        // Initialize all systems
        m_world->initialize();
        
        m_initialized = true;
        
        std::cout << "ECS System initialized successfully" << std::endl;
        std::cout << "Registered components: " << MAX_COMPONENTS << " max" << std::endl;
        std::cout << "Registered systems: " << m_world->getSystemCount() << std::endl;
        
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Failed to initialize ECS System: " << e.what() << std::endl;
        return false;
    }
}

void ECSManager::shutdown() {
    if (!m_initialized) {
        return;
    }

    // Shutdown all systems
    m_world->shutdown();
    
    // Clear system references
    m_renderSystem.reset();
    m_cameraSystem.reset();
    m_selectionSystem.reset();
    m_lightingSystem.reset();
    m_hierarchySystem.reset();
    m_meshSystem.reset();
    
    m_initialized = false;
    
    std::cout << "ECS System shutdown complete" << std::endl;
}

void ECSManager::update(float deltaTime) {
    if (!m_initialized) {
        return;
    }

    // Update all systems
    m_world->update(deltaTime);
}

void ECSManager::registerCoreComponents() {
    // Register all core component types
    m_world->registerComponent<TransformComponent>();
    m_world->registerComponent<MeshComponent>();
    m_world->registerComponent<MaterialComponent>();
    m_world->registerComponent<RenderableComponent>();
    m_world->registerComponent<NameComponent>();
    m_world->registerComponent<HierarchyComponent>();
    m_world->registerComponent<SelectableComponent>();
    m_world->registerComponent<CameraComponent>();
    m_world->registerComponent<LightComponent>();
}

void ECSManager::registerCoreSystems() {
    // Register all core systems
    m_hierarchySystem = m_world->registerSystem<HierarchySystem>();
    m_meshSystem = m_world->registerSystem<MeshSystem>();
    m_renderSystem = m_world->registerSystem<RenderSystem>();
    m_cameraSystem = m_world->registerSystem<CameraSystem>();
    m_selectionSystem = m_world->registerSystem<SelectionSystem>();
    m_lightingSystem = m_world->registerSystem<LightingSystem>();
    
    // Set world reference for systems that need it
    // Note: In a real implementation, you'd need friend access or a setter method
    // For now, this is a conceptual placeholder
}

void ECSManager::setupSystemSignatures() {
    // Hierarchy system: entities with Transform and Hierarchy components
    {
        Signature signature;
        signature.set(m_world->getComponentType<TransformComponent>());
        signature.set(m_world->getComponentType<HierarchyComponent>());
        m_world->setSystemSignature<HierarchySystem>(signature);
    }
    
    // Render system: entities with Transform, Mesh, Material, and Renderable components
    {
        Signature signature;
        signature.set(m_world->getComponentType<TransformComponent>());
        signature.set(m_world->getComponentType<MeshComponent>());
        signature.set(m_world->getComponentType<MaterialComponent>());
        signature.set(m_world->getComponentType<RenderableComponent>());
        m_world->setSystemSignature<RenderSystem>(signature);
    }
    
    // Camera system: entities with Transform and Camera components
    {
        Signature signature;
        signature.set(m_world->getComponentType<TransformComponent>());
        signature.set(m_world->getComponentType<CameraComponent>());
        m_world->setSystemSignature<CameraSystem>(signature);
    }
    
    // Selection system: entities with Selectable components
    {
        Signature signature;
        signature.set(m_world->getComponentType<SelectableComponent>());
        m_world->setSystemSignature<SelectionSystem>(signature);
    }
    
    // Lighting system: entities with Transform and Light components
    {
        Signature signature;
        signature.set(m_world->getComponentType<TransformComponent>());
        signature.set(m_world->getComponentType<LightComponent>());
        m_world->setSystemSignature<LightingSystem>(signature);
    }
    
    // Mesh system: entities with Mesh components
    {
        Signature signature;
        signature.set(m_world->getComponentType<MeshComponent>());
        m_world->setSystemSignature<MeshSystem>(signature);
    }
}

Entity ECSManager::createObject(const std::string& name, const glm::vec3& position) {
    Entity entity = m_world->createEntity();
    
    // Add core components
    TransformComponent transform;
    transform.position = position;
    m_world->addComponent(entity, transform);
    
    m_world->addComponent(entity, NameComponent{name});
    m_world->addComponent(entity, RenderableComponent{});
    m_world->addComponent(entity, SelectableComponent{});
    
    // Add empty mesh and material components (to be filled later)
    m_world->addComponent(entity, MeshComponent{});
    m_world->addComponent(entity, MaterialComponent{});
    
    return entity;
}

Entity ECSManager::createCamera(const std::string& name, const glm::vec3& position, const glm::vec3& lookAt) {
    Entity entity = m_world->createEntity();
    
    // Add transform component
    TransformComponent transform;
    transform.position = position;
    
    // Calculate rotation to look at target
    glm::vec3 forward = glm::normalize(lookAt - position);
    glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));
    glm::vec3 up = glm::cross(right, forward);
    
    // Create look-at matrix and extract rotation
    glm::mat3 rotationMatrix(right, up, -forward);
    transform.rotation = glm::quat_cast(glm::mat4(rotationMatrix));
    
    m_world->addComponent(entity, transform);
    m_world->addComponent(entity, NameComponent{name});
    m_world->addComponent(entity, CameraComponent{});
    m_world->addComponent(entity, SelectableComponent{});
    
    return entity;
}

Entity ECSManager::createLight(const std::string& name,
                              LightComponent::LightType type,
                              const glm::vec3& position,
                              const glm::vec3& color,
                              float intensity) {
    Entity entity = m_world->createEntity();
    
    // Add transform component
    TransformComponent transform;
    transform.position = position;
    m_world->addComponent(entity, transform);
    
    // Add light component
    LightComponent light;
    light.type = type;
    light.color = color;
    light.intensity = intensity;
    m_world->addComponent(entity, light);
    
    m_world->addComponent(entity, NameComponent{name});
    m_world->addComponent(entity, SelectableComponent{});
    
    return entity;
}

std::vector<Entity> ECSManager::getEntitiesByName(const std::string& name) const {
    std::vector<Entity> entities;
    
    // This is a simplified implementation
    // In a real system, you might want to maintain an index for faster lookups
    for (Entity entity = 1; entity < MAX_ENTITIES; ++entity) {
        if (m_world->isEntityValid(entity) && m_world->hasComponent<NameComponent>(entity)) {
            const auto& nameComponent = m_world->getComponent<NameComponent>(entity);
            if (nameComponent.name == name) {
                entities.push_back(entity);
            }
        }
    }
    
    return entities;
}

Entity ECSManager::getEntityByName(const std::string& name) const {
    auto entities = getEntitiesByName(name);
    return entities.empty() ? NULL_ENTITY : entities[0];
}

void ECSManager::setParent(Entity parent, Entity child) {
    if (!m_world->isEntityValid(parent) || !m_world->isEntityValid(child)) {
        return;
    }
    
    // Ensure both entities have hierarchy components
    if (!m_world->hasComponent<HierarchyComponent>(parent)) {
        m_world->addComponent(parent, HierarchyComponent{});
    }
    if (!m_world->hasComponent<HierarchyComponent>(child)) {
        m_world->addComponent(child, HierarchyComponent{});
    }
    
    // Remove child from previous parent if it has one
    auto& childHierarchy = m_world->getComponent<HierarchyComponent>(child);
    if (childHierarchy.parent != NULL_ENTITY) {
        auto& oldParentHierarchy = m_world->getComponent<HierarchyComponent>(childHierarchy.parent);
        oldParentHierarchy.removeChild(child);
    }
    
    // Set new parent-child relationship
    childHierarchy.parent = parent;
    auto& parentHierarchy = m_world->getComponent<HierarchyComponent>(parent);
    parentHierarchy.addChild(child);
}

void ECSManager::removeParent(Entity child) {
    if (!m_world->isEntityValid(child) || !m_world->hasComponent<HierarchyComponent>(child)) {
        return;
    }
    
    auto& childHierarchy = m_world->getComponent<HierarchyComponent>(child);
    if (childHierarchy.parent != NULL_ENTITY) {
        auto& parentHierarchy = m_world->getComponent<HierarchyComponent>(childHierarchy.parent);
        parentHierarchy.removeChild(child);
        childHierarchy.parent = NULL_ENTITY;
    }
}

std::string ECSManager::getStatistics() const {
    std::ostringstream oss;
    oss << "ECS Statistics:\\n";
    oss << "Living Entities: " << m_world->getLivingEntityCount() << "/" << MAX_ENTITIES << "\\n";
    oss << "Registered Systems: " << m_world->getSystemCount() << "\\n";
    oss << "Max Components: " << MAX_COMPONENTS << "\\n";
    
    if (m_renderSystem) {
        oss << "Renderable Entities: " << m_renderSystem->getEntityCount() << "\\n";
    }
    if (m_cameraSystem) {
        oss << "Camera Entities: " << m_cameraSystem->getEntityCount() << "\\n";
    }
    if (m_lightingSystem) {
        oss << "Light Entities: " << m_lightingSystem->getEntityCount() << "\\n";
    }
    
    return oss.str();
}

} // namespace rude::ecs
