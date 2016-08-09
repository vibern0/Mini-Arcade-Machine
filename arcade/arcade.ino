#include "lcd.h"
#include "game.h"
#include "snake.h"
#include "space_invaders.h"
#include "input.h"

#define LOAD_WINDOW     1
#define CHOOSE_GAME     2
#define LOADING_GAME    3
#define PLAYING_GAME    4
//
#define SECONDS_TO_LOAD 0

LCD *lcd;
byte machine_state;
unsigned long int last_time;
SnakeGame *game;
byte option_game = 1;

void setup()
{
    int index;
    lcd = new LCD(7, 6, 5, 16, 15, 2);
    lcd->init();
    lcd->clear();
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
        if(millis() > last_time + SECONDS_TO_LOAD * 4000)
        {
            loadGamesList();
            last_time = millis();
            machine_state = CHOOSE_GAME;
        }
    }
    else if(machine_state == CHOOSE_GAME)
    {
        game = new SnakeGame(INITIAL_SNAKE_X, INITIAL_SNAKE_Y, INITIAL_SNAKE_BODY_SIZE);
        last_time = millis();
        machine_state = LOADING_GAME;

        /*int index;
        int x, y;
        //get from input
        getAxis(&x, &y);
        if(y > 0 && option_game == 2)
        {
            option_game = 1;
            lcd->gotoXY(25, 1);
            lcd->writeCharacter('<');
            lcd->gotoXY(25, 2);
            lcd->writeCharacter(' ');
        }
        else if(y < 0 && option_game == 1)
        {
            option_game = 2;
            lcd->gotoXY(25, 2);
            lcd->writeCharacter('<');
            lcd->gotoXY(25, 1);
            lcd->writeCharacter(' ');
        }

        if(getButton() == true)
        {
            lcd->clear();
            lcd->gotoXY(0, 0);
            switch(option_game)
            {
                case 1:
                    game = startSnakeGame();
                    for(index = 0; index < 6 * 84; index++)
                        lcd->write(LCD_D, snakegame_load_image[index]);
                    break;
                case 2:
                    game = startSpaceInvadersGame();
                    for(index = 0; index < 6 * 84; index++)
                        lcd->write(LCD_D, spaceinvaders_load_image[index]);
                    break;
            }
            last_time = millis();
            machine_state = LOADING_GAME;
        }*/
    }
    else if(machine_state == LOADING_GAME)
    {
        if(millis() > last_time + SECONDS_TO_LOAD * 1000)
        {
            game->updateLCD(lcd);
            machine_state = PLAYING_GAME;
        }
    }
    else if(machine_state == PLAYING_GAME)
    {
        int x, y;
        //get from input
        getAxis(&x, &y);
        //
        game->move();
        //
        if(x > 0)
            game->changeDirection(DIRECTION_EAST);
        else if(x < 0)
            game->changeDirection(DIRECTION_WEST);
        else if(y > 0)
            game->changeDirection(DIRECTION_NORTH);
        else if(y < 0)
            game->changeDirection(DIRECTION_SOUTH);

        game->updateLCD(lcd);
    }
}

void loadGamesList()
{
    lcd->clear();
    lcd->gotoXY(0, 0);
    lcd->writeString("Choose an option:");
    lcd->gotoXY(0, 1);
    lcd->writeString("1 - Snake Game");
    lcd->gotoXY(0, 2);
    lcd->writeString("2 - Space Invaders");

    lcd->gotoXY(25, 1);
    lcd->writeCharacter('<');
}

/*Game* startSnakeGame()
{
    Game *game;
    game = new SnakeGame(INITIAL_SNAKE_X, INITIAL_SNAKE_Y, INITIAL_SNAKE_BODY_SIZE);

    return game;
}*/

Game* startSpaceInvadersGame()
{
    Game* game;

    return game;
}
