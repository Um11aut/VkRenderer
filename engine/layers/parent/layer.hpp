#pragma once

#include <vulkan/vulkan.h>

class Layer {
public:
    virtual ~Layer() = default;

    virtual void onStart() {
        // Default implementation for onStart
    }

    virtual void onUpdate() {
        // Default implementation for onUpdate
    }

    virtual void onDestroy() {
        // Default implementation for onDestroy
    }
};