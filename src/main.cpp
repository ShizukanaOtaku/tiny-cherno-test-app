#include "cherry_pink.hpp"
#include "component/mesh_system.hpp"
#include "rendering/mesh.hpp"
#include <event/key_event.hpp>
#include <iostream>
#include <memory>

class RotateSystem : public cherrypink::System<cherrypink::TransformComponent> {
    void ProcessComponent(const cherrypink::UUID &entityUuid, cherrypink::TransformComponent &transform) override {
        transform.rotation.y += 5.0f;
        transform.rotation.z += 2.0f;
    }
};

int main() {
    cherrypink::InitializationError error = cherrypink::Init({ "Test CherryPink App", 800, 600, true });

    if (error != cherrypink::InitializationError::NONE) {
        std::cerr << "Initializing the runtime failed with code " << error << '\n';
        exit(EXIT_FAILURE);
    }

    auto entity = cherrypink::CurrentScene().SpawnEntity();

    cherrypink::Systems().RegisterSystem<cherrypink::TransformComponent>(std::make_shared<RotateSystem>());

    cherrypink::Events().RegisterListener<cherrypink::KeyEvent>([](const cherrypink::KeyEvent &keyEvent) {
                std::cout << "Key " << keyEvent.key << " action: " << keyEvent.action << '\n';
            });

    cherrypink::CurrentScene().camera.position.z = 3;

    cherrypink::Mesh quad = cherrypink::GetRenderer().Context()->CreateMesh({
            -0.5, -0.5, 0,
            -0.5,  0.5, 0,
             0.5,  0.5, 0,
             0.5, -0.5, 0,
        }, {0, 1, 2, 3, 0, 2});

    cherrypink::CurrentScene().componentRegistry.AttachComponent<cherrypink::TransformComponent>(entity.Uuid);
    cherrypink::CurrentScene().componentRegistry.AttachComponent<cherrypink::MeshComponent>(entity.Uuid, cherrypink::MeshComponent { quad });

    cherrypink::Run();
}
