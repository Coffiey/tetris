
#include <iostream>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>


int playMusic(sf::Music& music);
void stopMusic(sf::Music& music);
void drawScreen();
void gamePlayLoop(int state[], int SIZE);
void controller(int state[], int SIZE,int* PLAY, bool* LeftPressed, bool* RightPressed, bool* UpPressed, bool* DownPressed, int* pos);
void gameAI(int state[], int SIZE, sf::Time point, double* drop);

std::string drawCell();

std::string MAIN_THEME = "assets/main_theme.ogg";

int main() {
    std::cout << "\033[2J"; // clear the entire screen
    std::cout << "\033[H"; // go to start of text block

    sf::Music music;
    // int res = playMusic(music);
    // if (res == -1) return -1;
    int SIZE = 200;
    int state[200];
    std::fill(state, state + SIZE, 0);

    gamePlayLoop(state, SIZE);


    std::cout << "\033[2J"; // clear the entire screen

    return 0;
}

// music Player
int playMusic(sf::Music& music) {
    if (!music.openFromFile(MAIN_THEME)) return -1;

    music.play();
    return 0;
}

void stopMusic(sf::Music& music) {
    music.stop();
}

// rendering functions
std::string drawCell(int cell_state) {
    std::string result;
    switch(cell_state) {
        case 0:
            result = "[ ]";
            break;
        case 1:
            result = "\033[31m[#]\033[0m";
            break;
        case 2:
            result = "\033[32m[#]\033[0m";
            break;
        case 3:
            result = "\033[093m[#]\033[0m";
            break;
        case 4:
            result = "\033[94m[#]\033[0m";
            break;
        case 5:
            result = "\033[95m[#]\033[0m";
            break;
        case 6:
            result = "\033[90m[#]\033[0m";
            break;
        case 7:
            result = "\033[96m[#]\033[0m";
            break;
        default:
            result = "[ ]";
            break;
    }

    return result;
}

void drawScreen(int state[], int SIZE) {
    std::cout << "\x1b[?25l"; // hide curser
    std::cout << "\033[H"; // go to start of text block

    int width = SIZE / 20;
    int height = SIZE / 10;

    for (int y = 0; y < height; y++) {
        std::string row;

        for (int x = 0; x < width; x++) {
            row += drawCell(state[y * 10 + x]);
        }

        row += '\n';
        std::cout << row;
        row = "";
    }
}




// gameplay functions
void controller(int state[], int SIZE, bool* PLAY, bool* LeftPressed, bool* RightPressed, bool* UpPressed, bool* DownPressed, int* pos) {
    bool Left = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left);
    bool Right = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right);
    bool Up = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up);
    bool Down = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down);

    if (*LeftPressed && !Left) {
        state[*pos] = 0;
        *pos -= 1;
        if (state[*pos] != 1) state[*pos] = 1;
    }

    if (*RightPressed && !Right) {
        state[*pos] = 0;
        *pos += 1;
        if (state[*pos] != 1) state[*pos] = 1;
    }
        
    if (*UpPressed && !Up) {
        state[*pos] = 0;
        *pos -= 10;
        if (state[*pos] != 1) state[*pos] = 1;
    }
        
    if (*DownPressed && !Down) {
        state[*pos] = 0;
        *pos += 10;
        if (state[*pos] != 1) state[*pos] = 1;
    }

    *LeftPressed = Left;
    *RightPressed = Right;
    *UpPressed = Up;
    *DownPressed = Down;

    // exit game
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
        *PLAY = false;
    }

}


void gameAI(int state[], int SIZE, sf::Time time_stamp, int* pos, double* drop) {
    // this needs to do multiple things
    // it will fire only every tick
    // therefore its task needs to simply be for now ticking to the bottom
    // bit it need to do all the things I think
    
    // for now check if position is not in the bottom of the task
    state[*pos] = 0;
    *pos += 10;
    if (state[*pos] != 1) state[*pos] = 1;

    // if (*pos > 190) {
    //     // complete the reset step
    // }
}


void gamePlayLoop(int state[], int SIZE) {
    sf::Clock clock;
    bool PLAY = true;
    
    // For now other stuff
    double drop = 0.0;
    double level_multiplier = 1.0;

    // controler constants 
    bool LeftPressed = false;
    bool RightPressed = false;
    bool UpPressed = false;
    bool DownPressed = false;
    
    int pos = 5;
    
    while(PLAY) {
        sf::Time time_stamp = clock.getElapsedTime();
        drawScreen(state, SIZE);
        controller(state, SIZE, &PLAY, &LeftPressed, &RightPressed, &UpPressed, &DownPressed, &pos);
        std::cout << time_stamp.asSeconds();
        if (time_stamp.asSeconds() > level_multiplier) {
            gameAI(state, SIZE, time_stamp, &pos, &drop);
            clock.restart();
        }
    }
}