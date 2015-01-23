#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "events.h"

// TODO: GLOBAL: prüfe dass den funktionen nicht maxevents und nullzeiger reingereicht wird


/*********************************************************************
MAIN ENTRY POINT
********************************************************************/
//int main(int argc, char *argv[])
//{
//	s_EventHandlers *handlers = NULL;
//	const char data[] = "this is the data";
//	InitListeners(listeners, MAX_EVENTS);
//
//	if(RegisterEvent(TEST1, Test1Handler1)==-1)
//		printf("duplicate test handler\n");
//	if(RegisterEvent(TEST1, Test1Handler2)==-1)
//		printf("duplicate test handler\n");
//	if(RegisterEvent(TEST1, Test1Handler2)==-1)
//		printf("duplicate test handler\n");
//	if(RegisterEvent(TEST1, Test1Handler4)==-1)
//		printf("duplicate test handler\n");
//	if(RegisterEvent(TEST2, Test2Handler1)==-1)
//		printf("duplicate test handler\n");
//	if(RegisterEvent(TEST3, Test3Handler1)==-1)
//		printf("duplicate test handler\n");
//	if(RegisterEvent(TEST3, Test3Handler2)==-1)
//		printf("duplicate test handler\n");
//
//	// execute functions according to event type
//	// could be moved to a extra function
//// 	handlers = listeners[TEST1];
//// 	for(; handlers != NULL; handlers = handlers->next){
//// 		handlers->cb(data);
//// 	}
//// 	handlers = listeners[TEST2];
//// 	for(; handlers != NULL; handlers = handlers->next){
//// 		handlers->cb(data);
//// 	}
//// 	handlers = listeners[TEST3];
//// 	for(; handlers != NULL; handlers = handlers->next){
//// 		handlers->cb(data);
//// 	}
//	FireEvent(TEST1);
//	FireEvent(TEST2);
//	FireEvent(TEST3);
//	DestroyListeners(listeners, MAX_EVENTS);
//	return 0;
//}

// TODO: unregisterevent(); erstellen hahaha
// todo: erweitere die callback funtion um einen parameter (den ersten) welcher den typ des events empfangt
// todo: sinnvoll eine doppeltverkettete list zu nehmen??

int RegisterEvent(e_Events event, callback cb)
{
	s_EventHandlers *handlers = listeners[event];

	if (handlers == NULL)
	{
		if(!(handlers = malloc(sizeof(s_EventHandlers))))
		{
			return 0; // error returned from malloc
		}

		handlers->cb = cb;
		handlers->next = NULL;
		listeners[event] = handlers;
	}
	else
	{
		while (handlers->next != NULL)
		{
			// handlers already registered for this event
			// check to see if it is a redundant handler for this event
			handlers = handlers->next;

			if (handlers->cb == cb)
			{
				return -1;
			}
		}

		s_EventHandlers *nextHandler;
		if (!(nextHandler = (s_EventHandlers*) malloc(sizeof(s_EventHandlers))))
		{
			return 0; // error returned from malloc
		}
		nextHandler->cb = cb;
		nextHandler->next = NULL;
		handlers->next = nextHandler;
	}

	return 1;
}

int UnregisterEvent(e_Events event, callback cb)
{
	s_EventHandlers *handlers = listeners[event];

	if (handlers == NULL)
	{
		// no event to unregister
		return -1;
	}
	else
	{
		if(handlers->cb == cb)
		{
			s_EventHandlers *nextHandler = handlers->next;
			free(handlers);
			handlers = nextHandler;
		}
		else
		{
			while (handlers->next != NULL)
			{
				if (handlers->next->cb == cb)
				{
					s_EventHandlers *nextHandler = handlers->next->next;
					free(handlers->next);
					handlers->next = nextHandler;
					return -1;
				}
				handlers = handlers->next;
			}
		}
	}

	return 1;
}

void FireEvent(e_Events eventType)
{
	s_EventHandlers * handlers = listeners[eventType];
	for(; handlers != NULL; handlers = handlers->next)
	{
		handlers->cb(eventType);
	}
}

void InitListeners(s_EventHandlers *handlers[], int size)
{
	int i;
	for (i = 0; i < MAX_EVENTS; i++)
	{
		handlers[i] = NULL;
	}
}

void DestroyListeners(s_EventHandlers *handlers[], int size)
{
	int i;
	s_EventHandlers *deleteMe, *next;
	for (i = 0; i < MAX_EVENTS; i++){
		deleteMe = handlers[i];
		while (deleteMe){
			next = deleteMe->next;
			free(deleteMe);
			deleteMe = next;
		}
	}
}
