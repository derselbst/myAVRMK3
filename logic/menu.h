#include <stdint.h>

struct action
{
	const char msg[21+1];
	void (*task)(void);
	const uint8_t button_mask;
};

void printMenu(const struct action a[]);
void getUserInput(const struct action a[], const uint8_t breakButton);
void getUserInput2(const struct action a[], const uint8_t breakButton, void(*alwaysTask)(void));
