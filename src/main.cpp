#include "rendering/window.hpp"
#include "entity/entity.hpp"
#include "scene/scene.hpp"
#include <cstdio>
#include <cstdlib>
#include <event/key_event.hpp>
#include <iostream>
#include <memory>
#include <tiny_cherno.hpp>
#include <runtime/runtime.hpp>

struct TestComponent {
    int x;
    bool funny;
};

int main() {
    tiny_cherno::WindowParameters params =
        tiny_cherno::WindowParameters{"Test TinyCherno App", 800, 600, false};

    tiny_cherno::InitializationError error = tiny_cherno::init(params);
    if (error != tiny_cherno::InitializationError::NONE) {
        std::cerr << "Initializing the TinyCherno runtime failed with code " << error << '\n';
        exit(EXIT_FAILURE);
    }

    tiny_cherno::Scene scene;
    auto entity = std::make_shared<tiny_cherno::Entity>();
    scene.entities.push_back(entity);
    TestComponent component = {234, true};
    scene.componentRegistry.AttachComponent(*entity, component);
    auto test = scene.componentRegistry.GetComponent<TestComponent>(*entity);
    if (test.has_value()) {
        std::cout << test->get().x << ' ' << test->get().funny << '\n';
    } else {
        std::cout << "Component not found!\n";
    }

    tiny_cherno::TinyChernoRuntime::GetRuntime()
        ->eventDispatcher.RegisterListener(
            tiny_cherno::EventType::KeyEvent, [](tiny_cherno::Event &e) {
                class tiny_cherno::KeyEvent keyEvent =
                    static_cast<class tiny_cherno::KeyEvent &>(e);
                return true;
            });

    tiny_cherno::run();
}
