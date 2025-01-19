//
// Created by admin on 11/01/2025.
//

#include <algorithm>
#include <iostream>

#include "core/layer/layer_stack.h"

namespace Core::Engine {
    LayerStack::~LayerStack() {
        std::cout << "LayerStack destructor - Starting cleanup..." << std::endl;
        for (auto& layer : layers) {
            try {
                layer->OnDetach();
            } catch (const std::exception& e) {
                std::cerr << "Error during layer cleanup: " << e.what() << std::endl;
            }
        }
        layers.clear();
        std::cout << "LayerStack destructor - Cleanup complete" << std::endl;
    }

    void LayerStack::PushLayer(std::shared_ptr<Layer> layer) {
        if (!layer) {
            std::cerr << "LayerStack::PushLayer - Attempted to push null layer!" << std::endl;
            return;
        }

        std::cout << "LayerStack::PushLayer - Starting for layer: " << layer->GetName() << std::endl;
        try {
            if (layerInsertIndex > layers.size()) {
                std::cerr << "LayerStack::PushLayer - Invalid insert index, resetting to 0" << std::endl;
                layerInsertIndex = 0;
            }

            layers.emplace(layers.begin() + layerInsertIndex, layer);
            layerInsertIndex++;

            std::cout << "LayerStack::PushLayer - Calling OnAttach for layer: " << layer->GetName() << std::endl;
            try {
                layer->OnAttach();
            } catch (const std::exception& e) {
                std::cerr << "LayerStack::PushLayer - OnAttach failed for layer "
                         << layer->GetName() << " with error: " << e.what() << std::endl;
                layers.erase(layers.begin() + (layerInsertIndex - 1));
                layerInsertIndex--;
                throw;
            }

            std::cout << "LayerStack::PushLayer - Successfully added layer: " << layer->GetName() << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "LayerStack::PushLayer - Failed for layer "
                     << layer->GetName() << " with error: " << e.what() << std::endl;
            throw;
        }
    }

    void LayerStack::PushOverlay(std::shared_ptr<Layer> overlay) {
        if (!overlay) {
            std::cerr << "LayerStack::PushOverlay - Attempted to push null overlay!" << std::endl;
            return;
        }

        std::cout << "LayerStack::PushOverlay - Starting for overlay: " << overlay->GetName() << std::endl;
        try {
            layers.emplace_back(overlay);
            overlay->OnAttach();
            std::cout << "LayerStack::PushOverlay - Successfully added overlay: " << overlay->GetName() << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "LayerStack::PushOverlay - Failed for overlay "
                     << overlay->GetName() << " with error: " << e.what() << std::endl;
            throw;
        }
    }

    void LayerStack::PopLayer(std::shared_ptr<Layer> layer) {
        if (!layer) {
            std::cerr << "LayerStack::PopLayer - Attempted to pop null layer!" << std::endl;
            return;
        }

        auto it = std::find(layers.begin(), layers.begin() + layerInsertIndex, layer);
        if (it != layers.begin() + layerInsertIndex) {
            std::cout << "LayerStack::PopLayer - Removing layer: " << layer->GetName() << std::endl;
            layer->OnDetach();
            layers.erase(it);
            layerInsertIndex--;
        }
    }

    void LayerStack::PopOverlay(std::shared_ptr<Layer> overlay) {
        if (!overlay) {
            std::cerr << "LayerStack::PopOverlay - Attempted to pop null overlay!" << std::endl;
            return;
        }

        auto it = std::find(layers.begin() + layerInsertIndex, layers.end(), overlay);
        if (it != layers.end()) {
            std::cout << "LayerStack::PopOverlay - Removing overlay: " << overlay->GetName() << std::endl;
            overlay->OnDetach();
            layers.erase(it);
        }
    }}