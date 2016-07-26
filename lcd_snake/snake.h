#define DIRECTION_NORTH     1
#define DIRECTION_SOUTH     2
#define DIRECTION_EAST      3
#define DIRECTION_WEST      4
//
#define INITIAL_SNAKE_BODY_SIZE     8
#define INITIAL_SNAKE_X             5
#define INITIAL_SNAKE_Y             10
//
struct body_move_
{
    byte order;
    byte direction;
    struct body_move_ *next;
};
typedef struct body_move_ BodyMove;

struct body_snake_
{
    byte x;
    byte y;
    byte direction;
    struct body_snake_ *next;
    BodyMove *next_moves;
};
typedef struct body_snake_ BodySnake;

class Snake
{
private:
    BodySnake *head;
    BodySnake *tail;
    BodySnake *body_snake;
public:
    Snake() { }
    Snake(byte x, byte y)
    {
        body_snake = (BodySnake*)malloc(sizeof(BodySnake));
        body_snake->x = x;
        body_snake->y = y;
        body_snake->direction = DIRECTION_SOUTH;
        body_snake->next = NULL;
        body_snake->next_moves = NULL;

        head = body_snake;
        tail = body_snake;
    }
    void growth()
    {
        BodySnake *new_body_part;
        BodyMove *new_body_move;
        BodyMove *tail_moves;
        byte x, y;
        x = y = 0;

        switch(tail->direction)
        {
            case DIRECTION_NORTH:
                y ++;
                break;
            case DIRECTION_SOUTH:
                y --;
                break;
            case DIRECTION_EAST:
                x --;
                break;
            case DIRECTION_WEST:
                x ++;
                break;
        }

        new_body_part = (BodySnake*)malloc(sizeof(BodySnake));
        new_body_part->x = tail->x + x;
        new_body_part->y = tail->y + y;
        new_body_part->direction = tail->direction;
        new_body_part->next = NULL;
        new_body_part->next_moves = NULL;

        tail_moves = tail->next_moves;
        while(tail_moves != NULL)
        {
            new_body_move = (BodyMove*)malloc(sizeof(BodyMove));
            new_body_move->order = tail_moves->order + 1;
            new_body_move->direction = tail_moves->direction;

            new_body_move->next = new_body_part->next_moves;
            new_body_part->next_moves = new_body_move;

            tail_moves = tail_moves->next;
        }

        tail->next = new_body_part;
        tail = new_body_part;
    }
    BodySnake* getHead()
    {
        return head;
    }
    BodySnake* getTail()
    {
        return tail;
    }
    void setDirection(byte direction)
    {
        BodyMove *new_next_move;
        BodySnake *body_part;
        byte order = 1;

        body_part = head;
        while(body_part != NULL)
        {
            new_next_move = (BodyMove*)malloc(sizeof(BodyMove));
            new_next_move->order = order;
            new_next_move->direction = direction;

            new_next_move->next = body_part->next_moves;
            body_part->next_moves = new_next_move;

            order ++;
            body_part = body_part->next;
        }
    }
    void updateDirections()
    {
        BodyMove *move, *prev_move;
        BodySnake *body_part;

        body_part = head;
        while(body_part != NULL)
        {
            move = body_part->next_moves;
            prev_move = move;
            while(move != NULL)
            {
                move->order --;
                if(move->order == 0)
                {
                    int direction = move->direction;
                    body_part->direction = direction;

                    if(prev_move == body_part->next_moves)
                        body_part->next_moves = NULL;
                    else
                        prev_move->next = NULL;

                    free(move);
                    break;
                }
                else
                {
                    prev_move = move;
                    move = move->next;
                }
            }

            body_part = body_part->next;
        }
    }
    void move()
    {
        BodySnake *snake_part = body_snake;
        while(snake_part != NULL)
        {
            switch(snake_part->direction)
            {
                case DIRECTION_NORTH:
                    if(snake_part->y == 0)
                        snake_part->y = LCD_Y;
                    snake_part->y --;
                    break;
                case DIRECTION_SOUTH:
                    snake_part->y ++;
                    if(snake_part->y == LCD_Y)
                        snake_part->y = 0;
                    break;
                case DIRECTION_EAST:
                    snake_part->x ++;
                    if(snake_part->x == LCD_X)
                        snake_part->x = 0;
                    break;
                case DIRECTION_WEST:
                    if(snake_part->x == 0)
                        snake_part->x = LCD_X;
                    snake_part->x --;
                    break;
            }
            //
            snake_part = snake_part->next;
                
        }
    }
};

class SnakeGame : public Game
{
private:
    Snake snake;
    byte snake_food[2];
public:
    SnakeGame(byte x, byte y, byte length) 
    {
        byte i = 0;
        snake = Snake(x, y);
        while(i++ < length)
            snake.growth();

        generateSeed();
    }
    void updateLCD(LCD *lcd)
    {
        BodySnake *body_snake = snake.getHead();
        
        lcd->clear();
        while(body_snake != NULL)
        {
            lcd->setPixel(body_snake->x, body_snake->y, 1);
            body_snake = body_snake->next;
        }
        lcd->setPixel(snake_food[0], snake_food[1], 1);
        //
        lcd->drawMap();
    }
    void updateBodyDirections()
    {
        snake.updateDirections();
    }
    void move()
    {
        checkCatchSeed();
        updateBodyDirections();
        //
        snake.move();
    }
    void changeDirection(byte direction)
    {
        snake.setDirection(direction);
    }
    void growthSnake()
    {
        snake.growth();
    }
    void eatSeed()
    {
        growthSnake();
    }
    void checkCatchSeed()
    {
        BodySnake *body_snake = snake.getHead();
        if( body_snake->x == snake_food[0] &&
            body_snake->y == snake_food[1])
        {
            eatSeed();
            generateSeed();
        }
    }
    void generateSeed()
    {
        static int x = 0;
        snake_food[0] = 5;//rand() % LCD_X;
        snake_food[1] = 15 + x;//rand() % LCD_Y;
        x += 5;
    }
};