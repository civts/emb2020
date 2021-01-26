#include "msp.h"
#include "stdbool.h"

int main(void) {
  WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; // stop watchdog timer

  bool gameStarted = false;
  bool won = false;

  // Initialize hardware
  // Show menu
  // Game loop (user can pauses)
  bool paused = false;
  // Game end
  if (won) {
    // Show win screen
  } else {
    // Show lost screen
  }

  // Below this line there is the blinking led example
  P1->DIR = ~BIT1; // Set P1.1 input
  P1->OUT = BIT1;  // Set pull-up mode
  P1->REN = BIT1;  // Enable pull-up resistor
  P1->SEL0 = 0;    // Select GPIO
  P1->SEL1 = 0;    // Select GPIO

  P1->DIR |= BIT0; // P1.0 set as output

  while (1) {
    while (P1->IN & BIT1)
      ; // Check button pressed
    while (!(P1->IN & BIT1))
      ; // Check button released

    P1->OUT ^= BIT0; // Toggle P1.0 LED
  }
}
