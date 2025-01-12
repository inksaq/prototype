//
// Created by admin on 11/01/2025.
//

#ifndef LAYER_H
#define LAYER_H


#include <string>
#include "../event/event.h"

namespace Core::Engine {
    class Layer {
    public:
        Layer(const std::string& debugName = "Layer")
            : debugName(debugName), enabled(true) {}
        virtual ~Layer() = default;

        virtual void OnAttach() {} // Called when layer is added to the stack
        virtual void OnDetach() {} // Called when layer is removed from the stack
        virtual void OnUpdate(float deltaTime) {}
        virtual void OnRender() {}
        virtual void OnImGuiRender() {} // For layers that need ImGui rendering
        virtual void OnEvent(Event& event) {}

        const std::string& GetName() const { return debugName; }
        bool IsEnabled() const { return enabled; }
        void SetEnabled(bool state) { enabled = state; }

    protected:
        std::string debugName;
        bool enabled;
    };
}

#endif //LAYER_H
