//
// Created by admin on 11/01/2025.
//

#ifndef STACK_H
#define STACK_H

#include <vector>
#include <memory>
#include "layer.h"

namespace Core::Engine {
    class LayerStack {
    public:
        LayerStack() = default;
        ~LayerStack();

        void PushLayer(std::shared_ptr<Layer> layer);
        void PushOverlay(std::shared_ptr<Layer> overlay);
        void PopLayer(std::shared_ptr<Layer> layer);
        void PopOverlay(std::shared_ptr<Layer> overlay);

        std::vector<std::shared_ptr<Layer>>::iterator begin() { return layers.begin(); }
        std::vector<std::shared_ptr<Layer>>::iterator end() { return layers.end(); }
        std::vector<std::shared_ptr<Layer>>::reverse_iterator rbegin() { return layers.rbegin(); }
        std::vector<std::shared_ptr<Layer>>::reverse_iterator rend() { return layers.rend(); }

    private:
        std::vector<std::shared_ptr<Layer>> layers;
        unsigned int layerInsertIndex = 0;
    };
}
#endif //STACK_H
