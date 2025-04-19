#include "component/component.hpp"
#include "event/event.hpp"
#include "event/render_event.hpp"
#include "rendering/mesh.hpp"
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
#include <unistd.h>

struct TestComponent {
    int x;
    bool funny;
};

class TestSystem : public tiny_cherno::System<TestComponent> {
    void ProcessComponent(const tiny_cherno::UUID &entityUuid, TestComponent &component) override {
        std::cout << "Component value: " << component.x++ << '\n';
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

    tiny_cherno::Scene *scene = tiny_cherno::CurrentScene();
    auto entity = std::make_shared<tiny_cherno::Entity>();
    scene->SpawnEntity(entity);
    scene->componentRegistry.AttachComponent<TestComponent>(entity->Uuid);
    tiny_cherno::Systems().RegisterSystem<TestComponent>(std::make_shared<TestSystem>());

    tiny_cherno::Events().RegisterListener(
            tiny_cherno::EventType::KEY_EVENT, [](tiny_cherno::Event &e) {
                tiny_cherno::KeyEvent keyEvent =
                    static_cast<tiny_cherno::KeyEvent &>(e);

                std::cout << "Key " << keyEvent.key << " action: " << keyEvent.action << '\n';
                return true;
            });

    tiny_cherno::Events().RegisterListener(
            tiny_cherno::EventType::RENDER_EVENT, [](tiny_cherno::Event &e) {
                tiny_cherno::RenderEvent renderEvent = static_cast<tiny_cherno::RenderEvent &>(e);
                std::cout << "delta time: " << renderEvent.deltaTime << '\n';
                return true;
            });

    tiny_cherno::Mesh triangle = tiny_cherno::GetWindow()->Context()->CreateMesh({
            -1, -1, 0,
             0, -1, 0,
             0,  0, 0,
        }, {0, 1, 2});

    tiny_cherno::Events().RegisterListener(tiny_cherno::RENDER_EVENT, [&triangle](tiny_cherno::Event &e) {
                tiny_cherno::GetWindow()->Context()->DrawMesh(triangle);
                return true;
            });

    tiny_cherno::Run();
}
