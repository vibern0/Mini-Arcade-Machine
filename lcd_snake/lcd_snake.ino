#include "lcd.h"
#include "snake.h"

#define INITIAL_SNAKE_BODY_SIZE     8
#define INITIAL_SNAKE_X             5
#define INITIAL_SNAKE_Y             10

LCD *lcd;
SnakeGame *game;

int x;

void setup()
{
    lcd = new LCD(7, 6, 5, 16, 15, 2);
    game = new SnakeGame(INITIAL_SNAKE_X, INITIAL_SNAKE_Y, INITIAL_SNAKE_BODY_SIZE);
    
    lcd->init();
    lcd->clear();
    
    //lcd->gotoXY(0,0);
    //lcd->writeString("ola");
    game->updateLCD(lcd);
    x = 0;
}

void loop()
{
    game->move();
    game->updateLCD(lcd);
    
    delay(1000);

    
    x++;
    
    if(x == 30)
    {
        game->changeDirection(DIRECTION_EAST);
    }
    else if(x == 50)
    {
        game->changeDirection(DIRECTION_NORTH);
    }
}