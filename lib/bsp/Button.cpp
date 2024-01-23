/**
 * @file Button.cpp
 * @author Vicky Hung (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-08-01
 *
 * @copyright Wonder Construct (c) 2023
 *
 * @note
 * @todo
 * 1.change button logic from if/if/if to if/else if/else
 * 2.change back logic from back main to back upper level
 * 3.change " case 81:" from if to %
 * 4.think "Cursor < 2 + *(pBLEState + 9)
 *
 * Main OLED and Button distribution :
 *                 ________________
 *   SLED 0 -> o  |                | O  <- Button 1
 *   SLED 1 -> o  |    SSD 1306    |
 * Button 0 -> O  |________________| O  <- Button 2
 *
 * Side OLED and Button distribution :
 *  _________    ___
 * | CH 1115 |  /   \ <- Long Button ( Button 3 ) with Buttton LED
 * |_________|  \___/
 */

#include "Button.h"
void Button::Update() {
  // check
  if (!Press[0] && !Press[1] && !Press[2] && !Press[3]) return;
  if (millis() - LastPress <
      BUTTON_DEAD_TIME)  // Avoide attach interrupt double triggure.
  {
    memset(Press, false, sizeof(Press));
    return;
  }
  LastPress = millis();
}
