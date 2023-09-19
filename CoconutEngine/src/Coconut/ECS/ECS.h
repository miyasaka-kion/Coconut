#pragma once

#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>

#include "Coconut/Log.h"

namespace Coconut {
	class Component;
	class Entity;

	using ComponentID = std::size_t;
	inline ComponentID getComponentTypeID() {
		static ComponentID lastID = 0;
		return lastID++;
	}
	
	// get component ID for a specific type
	// each type has it's own, separate type counter
	template <typename T> 
	inline ComponentID getComponentTypeID() noexcept {
		static ComponentID typeID = getComponentTypeID();
		return typeID;
	}
	
	constexpr std::size_t maxComponents = 32;
	
	using ComponentBitset = std::bitset<maxComponents>;
	using ComponentArray = std::array<Component*, maxComponents>;


	// Component
	class Component {
	public:
		// points to the entity it belongs to
		Entity* entity;
		
		virtual void init() {}
		virtual void update() {}
		virtual void draw() {}
		
		virtual ~Component() {}

	};



	// Entity has many components
	class Entity {
	private:
		bool active = true;
		std::vector<std::unique_ptr<Component> > components;

		ComponentArray componentArray;
		ComponentBitset componentBitset;
		
	public:
		void update() {
			for (auto& c : components) c->update();
		}
		void draw() {
			for (auto& c : components) c->draw();
		}

		bool isActive() const { return active;  }
		void destroy() { active = false;  }


		template <typename T>
		bool hasComponent() const {
			return ComponentBitset[getComponentID<T>()];
		}

		template <typename T, typename... TArgs>
		T& addComponent(TArgs&&... mArgs) {
			T* c(new T(std::forward<TArgs>(mArgs)...));
			c->entity = this;
			std::unique_ptr<Component> uPtr{ c };
			components.emplace_back(std::move(uPtr));

			componentArray[getComponentTypeID<T>()] = c;
			componentBitset[getComponentTypeID <T>()] = true;

			c->init();
			return *c;
		}

		template<typename T>
		T& getComponent() const {
			auto ptr(componentArray[getComponentTypeID<T>()]);
			return *static_cast<T*> (ptr);
		}
	};

	
	// Manager manage all entities
	class Manager {
	private:
		std::vector<std::unique_ptr<Entity> > entities;

	public:
		void update() {
			for (auto& e : entities) e->update();
		}

		void draw() {
			for (auto& e : entities) e->draw();
		}

		void refresh() {
			// remove entity that is not active
			entities.erase(
				std::remove_if(
					std::begin(entities),
					std::end(entities),
					[](const std::unique_ptr<Entity>& mEntity) {
						return !mEntity->isActive();
					}
				),
				std::end(entities)
			);
		}

		// may be some uncleared stuff here...
		Entity& addEntity() {
			std::unique_ptr<Entity> uPtr = std::make_unique<Entity>();
			Entity& e = *uPtr;
			entities.emplace_back(std::move(uPtr));
			return e;
		}

	};
}