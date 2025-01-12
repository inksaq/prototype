//
// Created by admin on 11/01/2025.
//

#include <algorithm>
#include "core/layer/layer_stack.h"

namespace Core::Engine {
    LayerStack::~LayerStack() {
        for (auto& layer : layers) {
            layer->OnDetach();
        }
        layers.clear();
    }

    void LayerStack::PushLayer(std::shared_ptr<Layer> layer) {
        layers.emplace(layers.begin() + layerInsertIndex, layer);
        layerInsertIndex++;
        layer->OnAttach();
    }

    void LayerStack::PushOverlay(std::shared_ptr<Layer> overlay) {
        layers.emplace_back(overlay);
        overlay->OnAttach();
    }

    void LayerStack::PopLayer(std::shared_ptr<Layer> layer) {
        auto it = std::find(layers.begin(), layers.begin() + layerInsertIndex, layer);
        if (it != layers.begin() + layerInsertIndex) {
            layer->OnDetach();
            layers.erase(it);
            layerInsertIndex--;
        }
    }

    void LayerStack::PopOverlay(std::shared_ptr<Layer> overlay) {
        auto it = std::find(layers.begin() + layerInsertIndex, layers.end(), overlay);
        if (it != layers.end()) {
            overlay->OnDetach();
            layers.erase(it);
        }
    }
}