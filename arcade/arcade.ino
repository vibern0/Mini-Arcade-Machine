#include "lcd.h"
#include "game.h"
#include "snake2.h"
#include "space_invaders.h"
#include "input.h"

#define LOAD_WINDOW     1
#define CHOOSE_GAME     2
#define LOADING_GAME    3
#define PLAYING_GAME    4
//
#define SECONDS_TO_LOAD 3

LCD *lcd;
byte machine_state;
unsigned long int last_time;
byte option_game = 1;
Game *game;

void setup()
{
    int index;
    lcd = new LCD(7, 6, 5, 16, 15, 2);
    lcd->init();
    lcd->erase();
    //
    loadInput();
    //
    lcd->gotoXY(0, 0);
    for (index = 0; index < LCD_X * LCD_Y / 8; index++)
    {
        lcd->write(LCD_D, arcade_logo_image[index]);
    }
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
        game = new SnakeGame(lcd, INITIAL_SNAKE_X, INITIAL_SNAKE_Y);
        machine_state = PLAYING_GAME;
        last_time = millis() + 1000;
    }
    else if(machine_state == PLAYING_GAME)
    {
        if(millis() > last_time + 1000)
        {
            last_time = millis();
            game->moving();
            Serial.println("move");
        }
    }
}

void loadGamesList()
{
    lcd->erase();
    lcd->gotoXY(0, 0);
    lcd->writeString("Choose one:");
    lcd->gotoXY(0, 2);
    lcd->writeString("Snake");
    lcd->gotoXY(0, 3);
    lcd->writeString(">S.Invaders");
}

