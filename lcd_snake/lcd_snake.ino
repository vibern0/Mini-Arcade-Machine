#include "game.h"
#include "lcd.h"
#include "snake.h"

#define LOAD_WINDOW     1
#define CHOOSE_GAME     2
#define PLAYING_GAME    3
//
#define SECONDS_TO_LOAD 2

LCD *lcd;
byte machine_state;
unsigned long int last_time;
Game *game;

void setup()
{
    lcd = new LCD(7, 6, 5, 16, 15, 2);
    lcd->init();
    lcd->clear();
    //
    lcd->gotoXY(0, 0);
    lcd->writeString("Welcome! :)");
    machine_state = LOAD_WINDOW;
    last_time = millis();
}

void loop()
{
    if(machine_state == LOAD_WINDOW)
    {
        if(millis() > last_time + SECONDS_TO_LOAD * 1000)
        {
            loadGamesList();
            last_time = millis();
            machine_state = CHOOSE_GAME;
        }
    }
    else if(machine_state == CHOOSE_GAME)
    {
        byte option = 1;
        //get from input
        if(option == 1)
        {
            game = startSnameGame();
            last_time = millis();
            machine_state = PLAYING_GAME;
        }
    }
    else if(machine_state == PLAYING_GAME)
    {
        //update screen
    }
}

void loadGamesList()
{
    lcd->clear();
    lcd->gotoXY(0, 0);
    lcd->writeString("1 - Snake Game");
}
Game* startSnameGame()
{
    Game *game;
    game = new SnakeGame(INITIAL_SNAKE_X, INITIAL_SNAKE_Y, INITIAL_SNAKE_BODY_SIZE);

    return game;
}