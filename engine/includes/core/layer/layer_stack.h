//
// Created by admin on 11/01/2025.
//

#ifndef STACK_H
#define STACK_H

#include "layer.h"
#include <vector>
#include <memory>


namespace Core::Engine {
    class LayerStack {
    public:
        LayerStack() = default;
        ~LayerStack();

        void PushLayer(std::shared_ptr<Layer> layer);
        void PushOverlay(std::shared_ptr<Layer> overlay);
        void PopLayer(std::shared_ptr<Layer> layer);
        void PopOverlay(std::shared_ptr<Layer> overlay);

        // Template method declaration and implementation
        template<typename T>
        std::shared_ptr<T> GetLayerByName(const std::string& name) {
            auto it = std::find_if(layers.begin(), layers.end(),
                [&name](const std::shared_ptr<Layer>& layer) {
                    return layer->GetName() == name;
                });

            if (it != layers.end()) {
                return std::dynamic_pointer_cast<T>(*it);
            }
            return nullptr;
        }

        std::vector<std::shared_ptr<Layer>>::iterator begin() { return layers.begin(); }
        std::vector<std::shared_ptr<Layer>>::iterator end() { return layers.end(); }
        std::vector<std::shared_ptr<Layer>>::reverse_iterator rbegin() { return layers.rbegin(); }
        std::vector<std::shared_ptr<Layer>>::reverse_iterator rend() { return layers.rend(); }

        std::vector<std::shared_ptr<Layer>>::const_iterator begin() const { return layers.begin(); }
        std::vector<std::shared_ptr<Layer>>::const_iterator end() const { return layers.end(); }
        std::vector<std::shared_ptr<Layer>>::const_reverse_iterator rbegin() const { return layers.rbegin(); }
        std::vector<std::shared_ptr<Layer>>::const_reverse_iterator rend() const { return layers.rend(); }

    private:
        std::vector<std::shared_ptr<Layer>> layers;
        unsigned int layerInsertIndex = 0;
    };
}

#endif //STACK_H
