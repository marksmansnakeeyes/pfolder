#include <iostream>
#include <windows.h>
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>

int main() {

    bool play_mode_defined = false;
    std::string play_mode;

    std::cout << "Choose a play mode - n for normal, s for shuffle" << std::endl;
    
    while (!play_mode_defined) {

        std::cout << "Play mode: ";
        std::cin >> play_mode;

        if (play_mode == "n") {
            play_mode_defined = true;
        } else if (play_mode == "s") {
            play_mode_defined = true;
        } else {
            std::cout << "Invalid option!" << std::endl;
        }
    }

    // mciSendString("open \"song_name.mp3\" type mpegvideo alias song_alias", NULL, 0, NULL);
    //
    // NOTE THE USE OF song_alias AS A PLACEHOLDER!!!! DUMBASS
    //
    // mciSendString("play song_alias wait", NULL, 0, NULL);

    WIN32_FIND_DATA file;
    HANDLE search_handle = FindFirstFile("*.mp3", &file);
    // std::cout << GetLastError() << std::endl;

    int items_count = 0;
    std::vector <std::string> items;

    if (GetLastError() == 2) {
        std::cout << "No .mp3 files found!" << std::endl;
    } else {
        items.push_back(file.cFileName);
        items_count += 1;

        while(FindNextFile(search_handle, &file)) {
            items.push_back(file.cFileName);
            items_count += 1;
        }
    }

    FindClose(search_handle);

    if (play_mode == "s") {
        std::shuffle(items.begin(), items.end(), std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count()));
    }


    // Troubleshooting code - used for checking if vector dynamic list worked
    // 
    // std::cout << "Items: " << items_count << std::endl;
    // std::cout << "Items in vector list: " << items.size() << std::endl;
    // for (int i = 0; i < items_count; i++) {
    //     std::cout << items[i] << std::endl;
    // }

    std::string request_string_part1 = "open \"";
    std::string request_string_part2 = "\" type mpegvideo alias ";
    std::string play_string_part1 = "play ";
    std::string play_string_part2 = " wait";

    for (int index=0; index<items_count; index++) {

       // std::string index_string = std::to_string(index);

       std::string current_song = items.at(index);

       std::string request_string = request_string_part1 + current_song + request_string_part2 + std::to_string(index);
       // std::cout << "request_string: " << request_string << std::endl;

       std::string play_string = play_string_part1 + std::to_string(index) + play_string_part2;
       // std::cout << "play_string: " << play_string << std::endl;

       if (mciSendString(request_string.c_str(), NULL, 0, NULL) == 0) {
           std::cout << "Now playing: " << current_song << std::endl;

           if (mciSendString(play_string.c_str(), NULL, 0, NULL) == 0) {
               std::cout << current_song << " finished\n" << std::endl;

           } else {
               std::cout << "An error occured while playing " << current_song << std::endl;
           }

       } else {
           std::cout << "An error occured while opening " << current_song << std::endl;
       }
    }

    return 0;
}