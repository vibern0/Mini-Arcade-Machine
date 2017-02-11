
class Game
{
private:
    LCD* lcd;
public:
    Game(LCD* lcd) { this->lcd = lcd; }
    LCD* getLCD() { return lcd; }
    virtual void changed_direction(byte direc) { }
    virtual void moving() { }
};
