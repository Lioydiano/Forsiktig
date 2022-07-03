#include "game.hpp"
// g++ forsiktig.cpp -o forsiktig.exe


int main(int argc, char* argv[]) {
    while (!_kbhit()) {
        std::cout << "Press any key to start the game..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::cout << "- 'q' to quit\n";
    std::cout << "- 'p' to pause\n";
    std::cout << "- 'r' to resume\n";
    std::cout << "- 'w | a | s | d' to step\n";
    std::cout << "- 'f' to fire a bullet\n";

    mainloop();

    return 0;
}