#define DIRECTION_NORTH     1
#define DIRECTION_SOUTH     2
#define DIRECTION_EAST      3
#define DIRECTION_WEST      4
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

class SnakeGame
{
    
    byte map[LCD_X * LCD_Y / 8];
    //byte snake_body[MAX_SNAKE_BODY_SIZE][2]; // x | y
    
    //
    BodySnake *snake_body;//[MAX_SNAKE_BODY_SIZE];
    //BodyMove *next_moves;
    //
    
    //byte snake_length;
    byte snake_food[2];
    
    void setPixel(byte x, byte y, byte color)
    {
        if ((x < 0) || (x >= LCD_X) || (y < 0) || (y >= LCD_Y))
            return;

        if (color) 
            map[x+ (y/8)*LCD_X] |= (1 << (y % 8));
        else
            map[x+ (y/8)*LCD_X] &= ~(1 << (y % 8));

    }

public:
    SnakeGame() 
    {
    }
    void init(byte x, byte y, byte length)
    {
        BodySnake *new_body_part;
        snake_body = NULL;
        //
        memset(map, 0, sizeof map);
        
        for(byte w = 0; w < length; w++)
        {
            new_body_part = (BodySnake*)malloc(sizeof(BodySnake));
            new_body_part->x = x;
            new_body_part->y = y + w;
            new_body_part->direction = DIRECTION_SOUTH;
            new_body_part->next_moves = NULL;
            //
            new_body_part->next = snake_body;
            snake_body = new_body_part;
        }
    }
    void updateLCD(LCD *lcd)
    {
        BodySnake *body_part = snake_body;
        //
        memset(map, 0, sizeof map);
        //
        while(body_part != NULL)
        {
            setPixel(body_part->x, body_part->y, 1);
            //
            body_part = body_part->next;
        }
        //
        lcd->gotoXY(0, 0);
        lcd->write(LCD_D, 0x00);
        
        for (word i = 0; i < (LCD_X * LCD_Y / 8); i++)
        {
            lcd->write(LCD_D, map[i]);
        }
        lcd->write(LCD_D, 0x00);
    }
    void updateBodyDirections()
    {
        //calcular tempo restante usando order para mudar de posicao
        
        BodySnake *snake_part = snake_body;
        BodyMove *move;
        while(snake_part != NULL)
        {
            if(snake_part->next_moves != NULL)
            {
                //    
                move = snake_part->next_moves;
                while(move != NULL)
                {
                    move->order --;
                    if(move->order == 0)
                    {
                        snake_part->direction = move->direction;
                        snake_part->next_moves = move->next;
                        free(move);
                    }
                    move = move->next;
                }
            }
            //
            snake_part = snake_part->next;
        }
        
    }
    void move()
    {
        updateBodyDirections();
        //
        BodySnake *body_part = snake_body;
        while(body_part != NULL)
        {
            switch(body_part->direction)
            {
                case DIRECTION_NORTH:
                    if(body_part->y == 0)
                        body_part->y = LCD_Y;
                    body_part->y --;
                    break;
                case DIRECTION_SOUTH:
                    body_part->y ++;
                    if(body_part->y == LCD_Y)
                        body_part->y = 0;
                    break;
                case DIRECTION_EAST:
                    body_part->x ++;
                    if(body_part->x == LCD_X)
                        body_part->x = 0;
                    break;
                case DIRECTION_WEST:
                    if(body_part->x == 0)
                        body_part->x = LCD_X;
                    body_part->x --;
                    break;
            }
            //
            body_part = body_part->next;
                
        }
    }
    void changeDirection(byte d)
    {
        byte order_part = 1;
        BodySnake *snake_part = snake_body;
        BodyMove *move;
        while(snake_part != NULL)
        {
            move = (BodyMove*)malloc(sizeof(BodyMove));
            move->order = order_part;
            move->direction = d;
            //
            move->next = snake_part->next_moves;
            snake_part->next_moves = move;
            //
            snake_part = snake_part->next;
            order_part ++;
        }
    }
};