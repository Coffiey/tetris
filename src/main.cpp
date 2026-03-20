
#include <iostream>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>


int playMusic(sf::Music& music);
void stopMusic(sf::Music& music);
void drawScreen();
void gamePlayLoop(int state[], int SIZE);
void controller(int state[], int SIZE,int* PLAY, bool* LeftPressed, bool* RightPressed, bool* UpPressed, bool* DownPressed, int* pos);
void gameAI(int state[], int SIZE, int* pos, int* phase, char* piece);
void drawSquare(int state[], int* pos);
void removeSquare(int state[], int* pos);
bool checkSquare(int state[], int* pos);

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

void removeSquare(int state[], int* pos) {
    // kiss
    // this is a game play piece
    if (*pos % 10 != 0) state[*pos] = 0;
    if (*pos % 10 != 0) state[*pos - 1] = 0;
    if (*pos % 10 != 0 && *pos > 10) state[*pos - 10] = 0;
    if (*pos % 10 != 0 && *pos > 10) state[*pos - 11] = 0;
}

void drawSquare(int state[], int* pos) {
    // this is a game play piece
    if (*pos % 10 != 0) state[*pos] = 1;
    if (*pos % 10 != 0) state[*pos - 1] = 1;
    if (*pos % 10 != 0 && *pos > 10) state[*pos - 10] = 1;
    if (*pos % 10 != 0 && *pos > 10) state[*pos - 11] = 1;
}

bool checkSquare(int state[], int* pos) {
    if (*pos > 190) return true;
    if (state[*pos + 10] || state[*pos + 9]) return true;
    return false;
} 

// gameplay functions
void controller(int state[], int SIZE, bool* PLAY, bool* LeftPressed, bool* RightPressed, bool* UpPressed, bool* DownPressed, int* pos) {
    bool Left = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left);
    bool Right = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right);
    bool Up = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up);
    bool Down = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down);
    if (checkSquare(state, pos)) return;
    
    if (*LeftPressed && !Left) {
        removeSquare(state, pos);
        *pos -= 1;
        drawSquare(state, pos);
    }

    if (*RightPressed && !Right) {
        removeSquare(state, pos);
        *pos += 1;
        drawSquare(state, pos);
    }
        
    if (*UpPressed && !Up) {
        removeSquare(state, pos);
        *pos -= 10;
        drawSquare(state, pos);
    }
        
    if (*DownPressed && !Down) {
        removeSquare(state, pos);
        *pos += 10;
        drawSquare(state, pos);
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


void gameAI(int state[], int SIZE, int* pos, int* phase, char* piece) {
    switch (*phase) {
        case 0:
            *pos = 5;
            drawSquare(state, pos);
            *phase = 1;
            //example of spawining a block
            break;
        case 1:
            // if cannot move
            if (checkSquare(state, pos)) {
                *phase = 2;
                break;
                }
            removeSquare(state, pos);
            *pos += 10;
            drawSquare(state, pos);
            break;
        case 2:
            // 2
            // check that it lands (1 full tick cycle to land)
            // lock piece in place 3
            // check for full tetris 3
            // check for single line tetris 3
            
            *phase = 0;
            *pos = -1;
            break;
        default:
            std::cout << "error";
            break;

    }
}


void gamePlayLoop(int state[], int SIZE) {
    sf::Clock clock;
    bool PLAY = true;
    
    // For now other stuff
    // char piece = 'c';

    double level_multiplier = 0.5;

    char piece = 'o';
    int phase = 0;
    int pos = -1;

    // controler constants 
    bool LeftPressed = false;
    bool RightPressed = false;
    bool UpPressed = false;
    bool DownPressed = false;
    
    
    while(PLAY) {
        sf::Time time_stamp = clock.getElapsedTime();
        drawScreen(state, SIZE);
        controller(state, SIZE, &PLAY, &LeftPressed, &RightPressed, &UpPressed, &DownPressed, &pos);
        std::cout << time_stamp.asSeconds() << '\n';
        std::cout << "phase: " << phase << '\n';
        std::cout << "pos: " << pos;
        if (time_stamp.asSeconds() > level_multiplier) {
            // this is the game play tick only needing to alter the frame every tick of the 
            gameAI(state, SIZE, &pos, &phase, &piece);
            clock.restart();
        }
    }
}