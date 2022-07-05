#include "game.hpp"
// g++ forsiktig.cpp -o forsiktig.exe


int main(int argc, char* argv[]) {
    if (argc == 2) {
        if (strcmp(argv[1], "--source") == 0) {
            std::cout << "https://github.com/Lioydiano/Forsiktig" << std::endl;
        }
        return 0;
    }

    system("cls");
    std::cout << "Welcome to Forsiktig!\nFLAK-ZOSO 3/7/22\n\n";
    std::cout << "Instructions:\n";
    std::cout << "- 'q' to quit\n";
    std::cout << "- 'p' to pause\n";
    std::cout << "- 'r' to resume <-- you also start the game resuming it\n";
    std::cout << "- 'w | a | s | d' to step\n";
    std::cout << "- Use arrows to fire a bullet\n";
    std::cout << "- 'x' to enable/disable auto-strike\n\n";
    std::cout << "Digits from 1 to 9 are your enemies, while you're a $\n";
    std::cout << "Avoid bullets fired from enemies, and shoot them to get ammunitions\n";
    system("pause > nul");

    mainloop();

    return 0;
}