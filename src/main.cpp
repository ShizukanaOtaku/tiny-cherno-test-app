#include <cherry_pink.hpp>
#include <cmath>
#include <event/key_event.hpp>
#include <iostream>
#include <memory>
#include <rendering/mesh.hpp>

#include "assets/resource_manager.hpp"
#include "component/system.hpp"
#include "component/transform_component.hpp"
#include "glm/ext/vector_float3.hpp"
#include "rendering/material.hpp"

class RotateSystem : public cherrypink::UpdateSystem<cherrypink::Transform> {
    void ProcessComponent(const cherrypink::UUID &entityUuid,
            cherrypink::Transform &transform,
            double deltaTime) override {
        transform.rotation.y += 5.0f;
        transform.rotation.z += 2.0f;
    }
};

class ScaleSystem : public cherrypink::UpdateSystem<cherrypink::Transform> {
    void ProcessComponent(const cherrypink::UUID &entityUuid,
            cherrypink::Transform &transform,
            double deltaTime) override {
        float scale = std::sin(cherrypink::Frames() / 50.0);
        transform.scale = glm::vec3(scale);
    }
};

class RainbowSystem
: public cherrypink::RenderSystem<cherrypink::ShaderMaterial> {
    void ProcessComponent(const cherrypink::UUID &entityUuid,
            cherrypink::ShaderMaterial &material,
            double partialTicks) override {
        material.color.SetRed((std::sin(cherrypink::Frames() / 20.0f) + 1.0) /
                2.0f);
        material.color.SetBlue((std::sin(cherrypink::Frames() / 15.0f) + 1.0) /
                2.0f);
        material.color.SetGreen((std::sin(cherrypink::Frames() / 10.0f) + 1.0) /
                2.0f);
    }
};

int main() {
    cherrypink::InitializationError error =
        cherrypink::Init({"Test CherryPink App", 800, 600, true});

    if (error != cherrypink::InitializationError::NONE) {
        std::cerr << "Initializing the runtime failed with code " << error << '\n';
        exit(EXIT_FAILURE);
    }

    auto entity = cherrypink::CurrentScene().SpawnEntity();

    cherrypink::Systems().RegisterUpdateSystem<cherrypink::Transform>(
            std::make_shared<RotateSystem>());
    cherrypink::Systems().RegisterUpdateSystem<cherrypink::Transform>(
            std::make_shared<ScaleSystem>());
    cherrypink::Systems().RegisterRenderSystem<cherrypink::ShaderMaterial>(
            std::make_shared<RainbowSystem>());

    cherrypink::Events().RegisterListener<cherrypink::KeyEvent>(
            [](const cherrypink::KeyEvent &keyEvent) {
            std::cout << "Key " << keyEvent.key << " action: " << keyEvent.action
            << '\n';
            });

    cherrypink::CurrentScene().camera.position.z = 8;

    cherrypink::Mesh *mesh =
        *cherrypink::GetResourceManager().LoadFromFile("teapot.obj");

    cherrypink::CurrentScene()
        .componentRegistry.AttachComponent<cherrypink::Transform>(entity.Uuid);
    cherrypink::CurrentScene()
        .componentRegistry.AttachComponent<cherrypink::Mesh>(entity.Uuid, *mesh);
    cherrypink::CurrentScene()
        .componentRegistry.AttachComponent<cherrypink::ShaderMaterial>(
                entity.Uuid,
                cherrypink::ShaderMaterial{cherrypink::RGBAColor(1, 0, 1)});

    cherrypink::Run();
}
