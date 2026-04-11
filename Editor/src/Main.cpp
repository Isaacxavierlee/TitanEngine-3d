#include <TitanEngine/Core/Engine.h>
#include <iostream>

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "TitanEngine Editor Starting..." << std::endl;
    std::cout << "========================================" << std::endl;

    Titan::Engine engine;

    if (!engine.Initialize("TitanEngine Editor", 1280, 720)) {
        std::cerr << "Failed to initialize engine!" << std::endl;
        return -1;
    }

    std::cout << "Press ESC to exit..." << std::endl;

    engine.Run();
    engine.Shutdown();

    std::cout << "TitanEngine Editor Shutdown" << std::endl;
    return 0;
}