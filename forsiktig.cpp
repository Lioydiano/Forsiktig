#include "game.hpp"
// g++ forsiktig.cpp -o forsiktig.exe

#define VERSION "1.1.0a1"
#define DATE "12-07-2022"


int main(int argc, char* argv[]) {
    if (argc == 2) {
        if (strcmp(argv[1], "--source") == 0) {
            std::cout << "https://github.com/Lioydiano/Forsiktig" << std::endl;
        } else if (strcmp(argv[1], "--version") == 0) {
            std::cout << "Forsiktig " << VERSION << " (" << DATE << ")" << std::endl;
        } else {
            std::cout << "Forsiktig " << VERSION << " (" << DATE << ")" << std::endl;
            std::cout << "Usage: " << argv[0] << " [--source | --version]" << std::endl;
        }
        return 0;
    }

    CLS;
    std::cout << "\x1b]2;Forsiktig\x07"; // Set window title
    std::cout << "\x1b[31;1mForsiktig v" << VERSION << "\nFLAK-ZOSO " << DATE << "\x1b[0m\n\n";
    std::cout << "Instructions:\n";
    std::cout << "- 'q' to quit\n";
    std::cout << "- 'p' to pause\n";
    std::cout << "- 'r' to resume <-- you also start the game resuming it\n";
    std::cout << "- 'w | a | s | d' to step\n";
    std::cout << "- Use arrows to fire a bullet\n";
    std::cout << "- 'x' to enable/disable auto-strike\n\n";
    std::cout << "- '+' to enable/disable cross-strike\n";
    std::cout << "Digits from 1 to 9 are your enemies, while you're a $\n";
    std::cout << "Avoid bullets fired from enemies, and shoot them to get ammunitions\n";
    getch();

    mainloop();

    return 0;
}