#define DIRECTION_NULL      0
#define DIRECTION_NORTH     1
#define DIRECTION_SOUTH     2
#define DIRECTION_EAST      3
#define DIRECTION_WEAST     4

#define INITIAL_SNAKE_BODY_SIZE     3
#define INITIAL_SNAKE_X             5
#define INITIAL_SNAKE_Y             10

struct s_snake_move
{
    byte order;
    byte dir;
    struct s_snake_move *next;
};

struct s_body_snake
{
    byte x;
    byte y;
    byte direc;
    byte next_direc;
    boolean change_direc;
    struct s_snake_move *moves;
    struct s_body_snake *next;
};

class Snake
{
private:
    struct s_body_snake *head;
    struct s_body_snake *tail;
    //
    struct s_body_snake* check_in_boundaries(struct s_body_snake* body_slice)
    {
        if(body_slice->x == 255) body_slice->x = LCD_X - 1;
        else if(body_slice->x == LCD_X - 1) body_slice->x = -1;
        else if(body_slice->y == 255) body_slice->y = LCD_Y - 1;
        else if(body_slice->y == LCD_Y - 1) body_slice->y = -1;
        return body_slice;
    }
public:
    Snake(byte x, byte y, byte body_length)
    {
        tail = (struct s_body_snake*)malloc(sizeof(struct s_body_snake));
        tail->x = x;
        tail->y = y + body_length - 1;
        tail->direc = DIRECTION_NORTH;
        tail->next_direc = DIRECTION_NULL;
        tail->moves = NULL;
        tail->next = NULL;
        //
        struct s_body_snake* last_body_slice = tail;
        for(byte l = body_length - 2; l > 0; l --)
        {
            struct s_body_snake* body = (struct s_body_snake*)malloc(sizeof(struct s_body_snake));
            body->x = x;
            body->y = y + l;
            body->direc = DIRECTION_NORTH;
            body->next_direc = DIRECTION_NULL;
            body->moves = NULL;
            body->next = last_body_slice;
            last_body_slice = body;
        }
        //
        head = (struct s_body_snake*)malloc(sizeof(struct s_body_snake));
        head->x = x;
        head->y = y;
        head->direc = DIRECTION_NORTH;
        head->next_direc = DIRECTION_NULL;
        head->moves = NULL;
        head->next = last_body_slice;
    }

    struct s_body_snake* get_head()
    {
        return head;
    }

    struct s_body_snake* get_tail()
    {
        return tail;
    }

    int set_direction(byte direc)
    {
        head->direc = direc;
        head->next->next_direc = direc;
        head->next->change_direc = true;
    }

    void moving()
    {
        byte xxx = 0;
        struct s_body_snake *body = head;
        while(body != NULL)
        {
            switch(body->direc)
            {
                case DIRECTION_NORTH:
                  body->y --;
                  break;
                case DIRECTION_SOUTH:
                  body->y ++;
                  break;
                case DIRECTION_EAST:
                  body->x ++;
                  break;
                case DIRECTION_WEAST:
                  body->x --;
                  break;
            }

            Serial.print(xxx); Serial.print(" "); Serial.print(body->x); Serial.print(" "); Serial.println(body->y);
            
            if(body->next_direc != DIRECTION_NULL)
            {
                if(body->change_direc)
                {
                    body->direc = body->next_direc;
                    Serial.print("direc "); Serial.println(body->direc);
                    if(body->next != NULL)
                    {
                        body->next->next_direc = body->direc;
                        body->next->change_direc = false;
                        Serial.print("next direc "); Serial.println(body->next->next_direc);
                    }
                    body->next_direc = DIRECTION_NULL;
                }
                else
                {
                    body->change_direc = true;
                }
            }
            body = check_in_boundaries(body);
            //
            body = body->next;
            xxx++;
        }
    }  
};

class SnakeGame : public Game
{
private:
    Snake *snake;
public:
    SnakeGame(LCD *lcd, byte x, byte y) : Game(lcd)
    {
        snake = new Snake(x, y, 4);
        lcd->erase();
    }

    Snake* get_snake()
    {
        return snake;
    }

    void changed_direction(byte direc)
    {
        //
    }
    
    void moving()
    {
        struct s_body_snake* head;
        byte tail_x = snake->get_tail()->x;
        byte tail_y = snake->get_tail()->y;
        snake->moving();
        head = snake->get_head();
        getLCD()->setPixel(head->x, head->y, 1);
        //Serial.print(head->x); Serial.print(" "); Serial.println(head->y);
        getLCD()->setPixel(tail_x, tail_y, 0);
        //Serial.print(tail_x); Serial.print(" "); Serial.println(tail_y);
        //getLCD()->updatePixel(head->x, head->y);
        //getLCD()->updatePixel(tail_x, tail_y);
        getLCD()->drawMap();
    }
};

