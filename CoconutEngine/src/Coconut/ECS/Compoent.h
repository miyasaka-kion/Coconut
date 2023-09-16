#pragma once

#include <Coconut/ECS/ECS.h>

namespace Coconut {

	class PositionComponent :public Coconut::Component {
	private:
		int xpos = 0;
		int ypos = 0;


	public:
		inline int x() {
			return xpos;
		}
		inline int y() {
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