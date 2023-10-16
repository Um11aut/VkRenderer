#pragma once

#include <vector>
#include <memory>
#include "layer.hpp"

class LayerFactory {
public:
    using LayerPtr = std::shared_ptr<Layer>;

    static LayerFactory& getInstance() {
        static LayerFactory instance;
        return instance;
    }

    template <typename T, typename... Args>
    void registerLayer(Args&&... args) {
        layers.push_back(std::make_shared<T>(std::forward<Args>(args)...));
    }

    const std::vector<LayerPtr>& getLayers() const {
        return layers;
    }

private:
    std::vector<LayerPtr> layers;
};