#include "EventHandler.h"

//structure to hold information necessary for callbacks to handlers
typedef struct EventHandlingObject
{
    Rect touchableArea;
    EventHandler evHandler;
    bool needsNoActionNotification;
    void* selfRef;
}EventHandlingObject;

//a "null" event to send to handlers that need to no there was no event
static SDL_Event BUTTONUP_EVENT;
//list of available handlers
static List EventHandlers;
//whether the module is initilialised or not
static bool isInitialised = false;

void EH_init()
{
    if(!isInitialised)
    {
        List_new(&EventHandlers, sizeof(EventHandlingObject), NULL);
        isInitialised = true;
    }
    SDL_Event e;
    e.type=SDL_MOUSEBUTTONUP;
    e.button.x = -1;
    e.button.y = -1;
    BUTTONUP_EVENT = e;
}

void EH_destroy()
{
    if(isInitialised)
    {
        List_destroy(&EventHandlers);
        isInitialised = false;
    }
}

//Register an object as a handler for events
void EH_registerHandler(Rect touchableArea, EventHandler evHan, bool needsNotification, void* selfRef)
{
    if(!isInitialised) EH_init();
    EventHandlingObject obj;
    obj.touchableArea = touchableArea;
    obj.evHandler = evHan;
    obj.needsNoActionNotification = needsNotification;
    obj.selfRef = selfRef;
    List_append(&EventHandlers, &obj);
}

//sends events to handler for the ocurring area, returns handlers result
bool EH_handleEvent(SDL_Event* ev)
{
    if(!isInitialised)EH_init();
    //if no registered handlers, return false (unhandled)
    if(EventHandlers.logicalLength < 1) return false;

    bool handled = false;
    ListNode* current = EventHandlers.head;
    EventHandlingObject* evHanObj;
    while(current)
    {
        evHanObj = current->data;
        if(Rect_containsPoint(evHanObj->touchableArea, ev->button.x, ev->button.y))
        {
            handled = evHanObj->evHandler(evHanObj->selfRef, ev);
        }else{
            if(evHanObj->needsNoActionNotification) evHanObj->evHandler(evHanObj->selfRef, &BUTTONUP_EVENT);
        }
        current = current->next;
    }
    return handled;
}