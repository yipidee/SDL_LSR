//prototype event handling function
static EventHandler _eh(AnalogueController* c, SDL_Event* e)
{
    SDL_Event.type==SDL_EventType.SDL_MOUSEBUTTONDOWN
    SDL_MouseButtonEvent* me = e;
    c->knob.x = me->x;
    return true;
}

//create controller
AnalogueController AnalCont_create()
{
    AnalogueController c;
    c.isPressed = false;
    c.base = {50, 50, 50};
    c.knob = {50, 50, 30};
    c.touchableArea = {0, 0, 100, 100};
    c.mode = ANALOGUE_MODE;
    c.evHand = &_eh;
    return c;
}

//get current input
uint32_t AnalCont_getCurrentInput(AnalogueController* ac);

//set and get
void AnalCont_setPosition(AnalogueController* ac, int x, int y);
void AnalCont_setSize(AnalogueController* ac, int r);
void AnalCont_setKnobSize(AnalogueController* ac, int r);
void AnalCont_setTouchableArea(AnalogueController* ac, Rect rect);
void AnalCont_setTouchHandler(AnalogueController* ac, EventHandler handler);
void AnalCont_setPressed(AnalogueController* ac, bool isPressed);
int AnalCont_getSize(AnalogueController* ac);
int AnalCont_getKnobSize(AnalogueController* ac);
Rect AnalCont_getTouchableArea(AnalogueController* ac);
bool AnalCont_getPressed(AnalogueController* ac);

