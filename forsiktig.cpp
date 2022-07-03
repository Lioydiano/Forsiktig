#include "game.hpp"
// g++ forsiktig.cpp -o forsiktig.exe


int main(int argc, char* argv[]) {
    std::cout << "- 'q' to quit\n";
    std::cout << "- 'p' to pause\n";
    std::cout << "- 'r' to resume\n";
    std::cout << "- 'w | a | s | d' to step\n";
    std::cout << "- 'f' to fire a bullet\n";
    system("pause > nul");

    mainloop();

    return 0;
}