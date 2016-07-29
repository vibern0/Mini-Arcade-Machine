
#define JOYSTICK_X          A1
#define JOYSTICK_Y          A3
#define JOYSTICK_BUTTON     3

void loadInput()
{
    //load joystick
    pinMode(JOYSTICK_X, INPUT);
    pinMode(JOYSTICK_Y, INPUT);
    pinMode(JOYSTICK_BUTTON, INPUT_PULLUP);
}
void getAxis(int *x, int *y)
{
    int xPosition = analogRead(JOYSTICK_X);
    int yPosition = analogRead(JOYSTICK_Y);

    if(xPosition > 600)
        *x = 1;
    else if(xPosition < 400)
        *x = -1;
    else
        *x = 0;

    if(yPosition > 600)
        *y = 1;
    else if(yPosition < 400)
        *y = -1;
    else
        *y = 0;
}
bool getButton()
{
    int dgr = digitalRead(JOYSTICK_BUTTON);
    return ((dgr == 0) ? (true) : (false));
}