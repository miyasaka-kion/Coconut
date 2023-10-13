#include <memory>

#include "Application.h"
#include <Windows.h>

int CALLBACK WinMain(
    _In_  HINSTANCE hInstance,
    _In_  HINSTANCE hPrevInstance,
    _In_  LPSTR lpCmdLine,
    _In_  int nCmdShow
);

int main(int argc, char* argv[]) {
    std::unique_ptr<Application> app = std::make_unique<Application>();
    app->run();

    return 0;
}