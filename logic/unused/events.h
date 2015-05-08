#ifndef EVENTS_H_INCLUDED
#define EVENTS_H_INCLUDED

/********************************************************************
STRUCTURES AND ENUMS FOR
EVENT HANDLING FUNCTIONS
********************************************************************/
// the different types of events that exist
typedef enum Events
{
	button1=0,
	button2,
	button3,
	button4,
	button5,
	button6,
	button7,
	button8,
	MAX_EVENTS
} e_Events;

typedef void (*callback) (e_Events eventType);

typedef struct EventHandlers
{
	callback cb;
	struct EventHandlers *next;
} s_EventHandlers;

s_EventHandlers *listeners[MAX_EVENTS];

/********************************************************************
FORWARD DECLARES FOR
EVENT HANDLING FUNCTIONS
********************************************************************/
void InitListeners(s_EventHandlers *handlers[], int size);
void DestroyListeners(s_EventHandlers *handlers[], int size);
int RegisterEvent(e_Events event, callback cb);
int UnregisterEvent(e_Events event, callback cb);
void FireEvent(e_Events eventType);

#endif // EVENTS_H_INCLUDED
