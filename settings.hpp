#include <fstream>
#include <iostream>
#include <string>
#include <tuple> // Should use struct instead of std::tuple, will change it later
#include <vector>

#define SETTINGS_PATH "C://Forsiktig/settings.txt"
#define DEBUG 0

#define DEFAULT_FRAME_DURATION 100
#define DEFAULT_STARTING_ENEMIES 4
#define DEFAULT_STARTING_AMMUNITIONS 100


unsigned int countLines(std::string file_) {
    std::ifstream file(file_, std::ios::in);
    unsigned int lines = 0;
    std::string line;
    while (std::getline(file, line))
        lines++;
    return lines;
}

// frame duration, starting enemies, starting ammunitions
std::tuple<unsigned int, unsigned int, unsigned int> loadSettings() {
    std::ifstream file(SETTINGS_PATH, std::ios::in);
    if (!file) {
        std::cout << "You don't have saved settings." << std::endl;
        std::cout << "You will play with default settings." << std::endl;
        system("pause > nul");
        return std::make_tuple(DEFAULT_FRAME_DURATION, DEFAULT_STARTING_ENEMIES, DEFAULT_STARTING_AMMUNITIONS);
    }
    unsigned int lines = countLines(SETTINGS_PATH);
    // frame duration, starting enemies, starting ammunitions
    std::vector<std::tuple<unsigned int, unsigned int, unsigned int>> settings(lines);

    for (int i=0; i<lines; i++) {
        // Read
        file >> std::get<0>(settings[i]) >> std::get<1>(settings[i]) >> std::get<2>(settings[i]);

        // Print
        std::cout << i+1 << ")" << std::endl;
        std::cout << "Frame duration: " << std::get<0>(settings[i]) << std::endl;
        std::cout << "Starting ammunitions: " << std::get<2>(settings[i]) << std::endl;
        std::cout << "Starting enemies: " << std::get<1>(settings[i]) << std::endl << std::endl;
    }

    unsigned int choice;
    do {
        std::cout << "Choose a setting (1-" << lines << "): ";
        std::cin >> choice;
    } while (choice > lines);

    return settings[choice-1];
}


void saveSettings(std::tuple<unsigned int, unsigned int, unsigned int> settings_) {
    std::ifstream in_file(SETTINGS_PATH, std::ios::in);
    unsigned int lines = countLines(SETTINGS_PATH);
    unsigned int frame_duration, starting_enemies, starting_ammunitions;
    for (int i=0; i<lines; i++) {
        in_file >> frame_duration >> starting_enemies >> starting_ammunitions;
        if (std::get<0>(settings_) == frame_duration && std::get<1>(settings_) == starting_enemies && std::get<2>(settings_) == starting_ammunitions) {
            std::cout << "Settings already saved!" << std::endl;
            return;
        }
    }
    std::ofstream file(SETTINGS_PATH, std::ios::app);
    file << std::get<0>(settings_) << ' ' << std::get<1>(settings_) << ' ' << std::get<2>(settings_) << std::endl;
    std::cout << "Settings saved!" << std::endl;
}