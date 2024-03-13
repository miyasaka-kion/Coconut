#pragma once

#include <vector>

class GameContext;

class Layer {
protected:
    // The instance of Layer is created by LayerManager.
    Layer() : m_gameContext(nullptr) {}

public:
    virtual void Update() {}
    virtual void GUIRender() {}

    GameContext* m_gameContext;

    friend class LayerManager;
};

class LayerManager {
public:
    LayerManager() = delete;
    LayerManager(GameContext* gameContext) : m_gameContext(gameContext) {}
    template < typename T, typename... Args >
    void AddLayer(Args&&... args) {
        static_assert(std::is_base_of<Layer, T>::value, "T must inherit from Layer");
        m_layers.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
        m_layers.back()->m_gameContext = m_gameContext;
    }


    void Update() {
        for(auto& layer : m_layers) {
            layer->Update();
        }
    }
    void Render() {
        for(auto& layer : m_layers) {
            layer->GUIRender();
        }
    }

private:
    std::vector< std::unique_ptr<Layer> > m_layers;
    GameContext* m_gameContext = nullptr;
};
