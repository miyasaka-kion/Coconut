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
	using ComponentID_t = std::size_t;

	// this function shouldn't be called externally,
	// this should be called for getComponentTypeID<blah>() only!
	// maybeencapsulate needed in later version
	inline ComponentID_t getComponentTypeID() {
		static ComponentID_t lastID = 0;
		return lastID++;
	}
	
	// get component ID for a specific type
	// each type has it's own, separate type counter
	// should only call this function
	template <typename T> 
	inline ComponentID_t getComponentTypeID() noexcept {
		static ComponentID_t typeID = getComponentTypeID();
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
		bool m_active = true;
		std::vector<std::unique_ptr<Component> > m_components;

		ComponentArray m_componentArray;
		ComponentBitset m_componentBitset;
		
	public:
		void update() {
			for (auto& c : m_components) c->update();
		}
		void draw() {
			for (auto& c : m_components) c->draw();
		}

		bool isActive() const { return m_active;  }
		void destroy() { m_active = false;  }


		template <typename T>
		bool hasComponent() const {
			return m_componentBitset[getComponentTypeID<T>()];
		}

		template <typename T, typename... TArgs>
		T& addComponent(TArgs&&... mArgs) {
			T* c(new T(std::forward<TArgs>(mArgs)...));
			c->entity = this;
			std::unique_ptr<Component> uPtr{ c };
			m_components.emplace_back(std::move(uPtr));

			m_componentArray[getComponentTypeID<T>()] = c;
			m_componentBitset[getComponentTypeID <T>()] = true;

			c->init();
			return *c;
		}

		template<typename T>
		T& getComponent() const {
			auto ptr(m_componentArray[getComponentTypeID<T>()]);
			return *static_cast<T*> (ptr);
		}
	};

	
	// Manager manage all entities
	class Manager {
	private:
		std::vector<std::unique_ptr<Entity> > m_entities;

	public:
		void update() {
			for (auto& e : m_entities) e->update();
		}

		void draw() {
			for (auto& e : m_entities) e->draw();
		}

		void refresh() {
			// remove entity that is not active
			m_entities.erase(
				std::remove_if(
					std::begin(m_entities),
					std::end(m_entities),
					[](const std::unique_ptr<Entity>& mEntity) {
						return !mEntity->isActive();
					}
				),
				std::end(m_entities)
			);
		}

		// may be some uncleared stuff here...
		Entity& addEntity() {
			std::unique_ptr<Entity> uPtr = std::make_unique<Entity>();
			Entity& e = *uPtr;
			m_entities.emplace_back(std::move(uPtr));
			return e;
		}

	};
}