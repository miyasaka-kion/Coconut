#include <Coconut.h>
#include "EntryPoint.h"

class TestGame : public Coconut::Application {
public:
	TestGame() {

	}
	~TestGame() {

	}


};

Coconut::Application* Coconut::CreateApplication() {
	return new TestGame();
}