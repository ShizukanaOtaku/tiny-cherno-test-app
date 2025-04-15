#include "component/component.hpp"
#include "rendering/window.hpp"
#include "entity/entity.hpp"
#include "scene/scene.hpp"
#include "util/uuid.hpp"
#include <cstdio>
#include <cstdlib>
#include <event/key_event.hpp>
#include <iostream>
#include <memory>
#include <tiny_cherno.hpp>

struct TestComponent {
    int x;
    bool funny;
};

class TestSystem : public tiny_cherno::System<TestComponent> {
    void ProcessComponent(const tiny_cherno::UUID &entityUuid, TestComponent &component) override {
        std::cout << "Processing entity " << entityUuid.Value() << ", data: " << component.funny << ", " << component.x << '\n';
        component.x++;
    }
};

int main() {
    tiny_cherno::WindowParameters params =
        tiny_cherno::WindowParameters{"Test TinyCherno App", 800, 600, false};

    tiny_cherno::InitializationError error = tiny_cherno::Init(params);
    if (error != tiny_cherno::InitializationError::NONE) {
        std::cerr << "Initializing the TinyCherno runtime failed with code " << error << '\n';
        exit(EXIT_FAILURE);
    }

    tiny_cherno::Scene &scene = tiny_cherno::CurrentScene();
    auto entity = std::make_shared<tiny_cherno::Entity>();
    scene.SpawnEntity(entity);
    scene.componentRegistry.AttachComponent<TestComponent>(entity->Uuid);
    tiny_cherno::Systems().RegisterSystem<TestComponent>(std::make_shared<TestSystem>());

    tiny_cherno::Events().RegisterListener(
            tiny_cherno::EventType::KeyEvent, [](tiny_cherno::Event &e) {
                class tiny_cherno::KeyEvent keyEvent =
                    static_cast<class tiny_cherno::KeyEvent &>(e);
                return true;
            });

    tiny_cherno::Run();
}
