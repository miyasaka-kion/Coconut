#pragma once

#include <vector>

class GameContext;

class Layer {
public:
    Layer(GameContext* gameContext) : m_gameContext(gameContext) {}
    virtual void Update() {}
    virtual void Render() {}

    GameContext* m_gameContext = nullptr;
};

class LayerManager {

public:
    template < typename T, typename... Args >
    void AddLayer(Args&&... args) {
        static_assert(std::is_base_of<Layer, T>::value, "T must inherit from Layer");
        
        m_layers.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
    }


    void Update() {
        for(auto& layer : m_layers) {
            layer->Update();
        }
    }
    void Render() {
        for(auto& layer : m_layers) {
            layer->Render();
        }
    }

private:
    std::vector< std::unique_ptr<Layer> > m_layers;
};
