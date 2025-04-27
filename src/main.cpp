#include "cherry_pink.hpp"
#include "component/mesh_system.hpp"
#include "event/event.hpp"
#include "rendering/mesh.hpp"
#include <event/key_event.hpp>
#include <iostream>

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

    cherrypink::Scene &scene = cherrypink::CurrentScene();
    auto entity = cherrypink::CurrentScene().SpawnEntity();

    cherrypink::Systems().RegisterSystem<cherrypink::TransformComponent>(std::make_shared<RotateSystem>());

    cherrypink::Events().RegisterListener(
            cherrypink::EventType::KEY_EVENT, [](cherrypink::Event &e) {
                cherrypink::KeyEvent keyEvent =
                    static_cast<cherrypink::KeyEvent &>(e);

                std::cout << "Key " << keyEvent.key << " action: " << keyEvent.action << '\n';
                return true;
            });

    cherrypink::CurrentScene().camera.position.z = 1;

    cherrypink::Events().RegisterListener(
            cherrypink::EventType::RENDER_EVENT, [](cherrypink::Event &e) {
                cherrypink::CurrentScene().camera.position.z = 1;
                return true;
            });

    cherrypink::Mesh triangle = cherrypink::GetRenderer().Context()->CreateMesh({
            -1, -1, 0,
             0, -1, 0,
             0,  0, 0,
        }, {0, 1, 2});

    cherrypink::CurrentScene().componentRegistry.AttachComponent<cherrypink::TransformComponent>(entity.Uuid);
    cherrypink::CurrentScene().componentRegistry.AttachComponent<cherrypink::MeshComponent>(entity.Uuid, cherrypink::MeshComponent { triangle });

    cherrypink::Run();
}
