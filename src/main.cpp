#include "rendering/window.hpp"
#include <tiny_cherno.hpp>

int main() {
    tiny_cherno::WindowParameters params =
        tiny_cherno::WindowParameters{"Test TinyCherno App", 800, 600, false};
    tiny_cherno::init(params);
}
