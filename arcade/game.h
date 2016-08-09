
class Game
{
public:
    virtual void updateLCD(LCD *lcd) { }
    virtual void changeDirection(byte direction) { }
    virtual void move() { }
};
