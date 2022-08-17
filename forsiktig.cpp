#include "game.hpp"
// g++ forsiktig.cpp -o forsiktig.exe

#define VERSION "1.6.0"
#define DATE "17-08-2022"


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
    std::cout << "- '\x1b[35mq\x1b[0m' to quit\n";
    std::cout << "- '\x1b[35mp\x1b[0m' to pause\n";
    std::cout << "- '\x1b[35mr\x1b[0m' to resume <-- you also start the game resuming it\n";
    std::cout << "- '\x1b[35mw\x1b[0m | \x1b[35ma\x1b[0m | \x1b[35ms\x1b[0m | \x1b[35md\x1b[0m' to step\n";
    std::cout << "- Use \x1b[35marrows\x1b[0m to fire a bullet or to build an obstacle\n";
    std::cout << "- '\x1b[35mx\x1b[0m' to enable/disable auto-strike\n";
    std::cout << "- '\x1b[35m+\x1b[0m' to enable/disable cross-strike\n";
    std::cout << "- '\x1b[35mb\x1b[0m' to enable/disable build-mode\n";
    std::cout << "- '\x1b[35mm\x1b[0m' to enable/disable mine-mode\n\n";
    std::cout << "Digits from \x1b[34m1\x1b[0m to \x1b[34m9\x1b[0m are your enemies, while you're a \x1b[31m$\x1b[0m\n";
    std::cout << "Avoid \x1b[32mbullets\x1b[0m fired from enemies, and shoot them to get ammunitions\n";
    getch();

    mainloop();

    return 0;
}