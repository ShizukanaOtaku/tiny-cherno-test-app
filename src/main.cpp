#include "rendering/window.hpp"
#include <cstdio>
#include <event/key_event.hpp>
#include <tiny_cherno.hpp>

int main() {
    tiny_cherno::WindowParameters params =
        tiny_cherno::WindowParameters{"Test TinyCherno App", 800, 600, false};
    tiny_cherno::init(params);
    tiny_cherno::TinyChernoRuntime::GetRuntime()
        ->event_dispatcher.RegisterListener(
            tiny_cherno::EventType::KeyEvent, [](tiny_cherno::Event &e) {
                printf("Received a key event! ");
                class tiny_cherno::KeyEvent keyEvent =
                    static_cast<class tiny_cherno::KeyEvent &>(e);

                printf("%i: %i\n", keyEvent.key, keyEvent.action);
                return true;
            });
    tiny_cherno::run();
}
