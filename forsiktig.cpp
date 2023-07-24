#include "game.hpp"
// g++ forsiktig.cpp -o forsiktig

#define VERSION "1.9.4"
#define DATE "24-07-2023"


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
    std::cout << "\t\t\t  \x1b[3mForsiktig - 2D doesn't mean nice\x1b[0m\n\t\t\t\t\x1b[3mNo mercy for numbers\x1b[0m\n\n";
    std::cout << "\t\t  \x1b[31;1mForsiktig v" << VERSION << "\t\tFLAK-ZOSO " << DATE << "\x1b[0m\n\n";

    std::cout << "\t\t\t- '\x1b[35mq\x1b[0m' to \x1b[3mquit\x1b[0m\n";
    std::cout << "\t\t\t- '\x1b[35mp\x1b[0m' to \x1b[3mpause\x1b[0m\n";
    std::cout << "\t\t\t- '\x1b[35mr\x1b[0m' to \x1b[3mresume\x1b[0m <-- \x1b[3mstart\x1b[0m the game\n";
    std::cout << "\t\t\t- '\x1b[35mw\x1b[0m | \x1b[35ma\x1b[0m | \x1b[35ms\x1b[0m | \x1b[35md\x1b[0m' to step\n";
    #if _WIN32
        std::cout << "\t\t\t- '\x1b[35m^\x1b[0m | \x1b[35m<\x1b[0m | \x1b[35mv\x1b[0m | \x1b[35m>\x1b[0m' to fire a bullet\n";
    #elif __linux__
        std::cout << "\t\t\t- '\x1b[35mi\x1b[0m | \x1b[35mj\x1b[0m | \x1b[35mk\x1b[0m | \x1b[35ml\x1b[0m' to fire a bullet\n";
    #endif
    std::cout << "\t\t\t- '\x1b[35mx\x1b[0m' to enable/disable auto-strike\n";
    std::cout << "\t\t\t- '\x1b[35m+\x1b[0m' to enable/disable cross-strike\n";
    std::cout << "\t\t\t- '\x1b[35mb\x1b[0m' to enable/disable build-mode\n";
    std::cout << "\t\t\t- '\x1b[35mm\x1b[0m' to enable/disable mine-mode\n\n";

    std::cout << "\t\t\t- Numbers from \x1b[34m1\x1b[0m to \x1b[34m9\x1b[0m are your enemies\n\t\t\t- \x1b[90m%\x1b[0m is your worst and smartest enemy\n\t\t\t- You're the \x1b[31m$\x1b[0m\n";
    std::cout << "\t\t\t- Avoid \x1b[32mbullets\x1b[0m fired from enemies\n\t\t\t- Shoot them \x1b[95m>\x1b[0m to get ammunitions\n\n\n";

    std::cout << "\t\t\t  \x1b[3mMath is coming for you...\x1b[0m";
    
    getch();

    mainloop();

    return 0;
}