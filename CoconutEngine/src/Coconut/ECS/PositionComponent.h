#pragma once

#include <Coconut/ECS/ECS.h>
#include <Coconut/ECS/Compoent.h>


namespace Coconut {

	class PositionComponent : public Coconut::Component {
	private:
		int xpos;
		int ypos;


	public:
		PositionComponent() : xpos(0), ypos(0) {
			
		};
		PositionComponent(int x, int y) : xpos(x), ypos(y) {};
		inline int getXpos() {
			return xpos;
		}
		inline int getYpos() {
			return ypos;
		}

		void init() override {
			xpos = 0;
			ypos = 0;
		}

		void update() override {
			xpos++;
			ypos++;
		}

		void setPosition(int x, int y) {
			xpos = x;
			ypos = y;
		}
	};

}