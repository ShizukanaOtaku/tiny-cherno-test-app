#include "cherry_pink.hpp"
#include "component/component.hpp"
#include "event/event.hpp"
#include "rendering/mesh.hpp"
#include "entity/entity.hpp"
#include "scene/scene.hpp"
#include "util/uuid.hpp"
#include <cstdio>
#include <cstdlib>
#include <event/key_event.hpp>
#include <iostream>
#include <memory>
#include <unistd.h>

struct TestComponent {
    int x;
    bool funny;
};

class TestSystem : public cherrypink::System<TestComponent> {
    void ProcessComponent(const cherrypink::UUID &entityUuid, TestComponent &component) override {
        std::cout << "Component value: " << component.x++ << '\n';
    }
};

int main() {
    cherrypink::InitializationError error = cherrypink::Init({ "Test CherryPink App", 800, 600, false });

    if (error != cherrypink::InitializationError::NONE) {
        std::cerr << "Initializing the runtime failed with code " << error << '\n';
        exit(EXIT_FAILURE);
    }

    cherrypink::Scene &scene = cherrypink::CurrentScene();
    auto entity = cherrypink::CurrentScene().SpawnEntity();
    scene.componentRegistry.AttachComponent<TestComponent>(entity.Uuid);
    cherrypink::Systems().RegisterSystem<TestComponent>(std::make_shared<TestSystem>());

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
                cherrypink::CurrentScene().camera.position.z += 0.01;
                return true;
            });

    cherrypink::Mesh triangle = cherrypink::GetRenderer().Context()->CreateMesh({
            -1, -1, 0,
             0, -1, 0,
             0,  0, 0,
        }, {0, 1, 2});

    cherrypink::Events().RegisterListener(cherrypink::RENDER_EVENT, [&triangle](cherrypink::Event &e) {
                cherrypink::GetRenderer().Context()->DrawMesh(triangle);
                return true;
            });

    cherrypink::Run();
}
