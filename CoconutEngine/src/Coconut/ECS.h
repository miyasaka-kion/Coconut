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
	// each type has it's own, seperate type counter
	template <typename T> 
	inline ComponentID getComponentTypeID() noexcept {
		static ComponentID typeID = getComponentTypeID();
		return typeID;
	}
	
	constexpr std::size_t maxComponents = 32;
	
	using ComponentBitset = std::bitset<maxComponents>;
	using ComponentArray = std::array<Component*, maxComponents>;

	class Component {
	public:
		Entity* entity;
		
		virtual void init() {}
		virtual void update() {}
		virtual void draw() {}
		
		virtual ~Component() {}

	};




	class Entity {
	private:
		// ? when is this initiated?
		bool active = true;
		std::vector<std::unique_ptr<Component> > components;

		ComponentArray componentArray;
		ComponentBitset componentBitset;
		
	public:
		void update() {
			for (auto& c : components) c->update();
			for (auto& c : components) c->draw();
		}
		void draw() {}
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
			componentBitset[getComponentTypeID <T> ()] = true;

			c->init();
			return *c;
		}

		template<typename T>
		T& getComponent() const {
			auto ptr(componentArray[getComponentTypeID<T>()]);
			return *static_cast<T*> (ptr);
		}
	};

	

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

		void refersh() {
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

		Entity& addEntity() {
			Entity* e = new Entity();
			std::unique_ptr<Entity> uPtr{ e };
			// modified version:
			//std::unique_ptr<Entity> uPtr = std::make_unique<Entity>();
			entities.emplace_back(std::move(uPtr));
			return *e;
		}
	};
}