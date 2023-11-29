/*
ClickButton library (Adapted Arduino library(CPP) for a PIC(C))

  Short clicks:
    Single click - singlePressHandler(btn);
    Double click - doublePressHandler(btn);
    Triple click - triplePressHandler(btn);
    
  Long clicks (hold button for one second or longer on last click):
    
    Single-click - singleLongPressHandler(btn);
    Double-click - doubleLongPressHandler(btn);
    Triple-click - tripleLongPressHandler(btn);
*/


#include <xc.h>	// include processor files - each processor file is guarded.
#include "ClickButton.h"

void updateBTN(Button *btn, unsigned char btnState) {
    unsigned long now = _millis();          // get current time
    btn->_btnState = btnState;  // current appearant button state

    // Make the button logic active-high in code
    if (!btn->_activeHigh) btn->_btnState = !btn->_btnState;

    // If the switch btnChanged, due to noise or a button press, reset the debounce timer
    if (btn->_btnState != btn->_lastState) btn->_lastBounceTime = now;

    // debounce the button (Check if a stable, btnChanged state has occured)
    if (now - btn->_lastBounceTime > btn->debounceTime && btn->_btnState != btn->depressed)
    {
        btn->depressed = btn->_btnState;
        if (btn->depressed) btn->_clickCount++;
    }

    btn->_lastState = btn->_btnState;

    // If the button released state is stable, report nr of clicks and start new cycle
    if (!btn->depressed && (now - btn->_lastBounceTime) > btn->multiclickTime)
    {
        // positive count for released buttons
        btn->clicks = btn->_clickCount;
        btn->_clickCount = 0;
    }
    // Check for "long click"
    if (btn->depressed && (now - btn->_lastBounceTime > btn->longClickTime))
    {
        // negative count for long clicks
        btn->clicks = 0 - btn->_clickCount;
        btn->_clickCount = 0;
    }
    if (btn->clicks != 0)
        btn->numClicks = btn->clicks;
    else {
        if (btn->numClicks == 3) {
            triplePressHandler(btn);
        }
        if (btn->numClicks == 2) {
            doublePressHandler(btn);
        }
        if (btn->numClicks == 1) {
            singlePressHandler(btn);
        }
        if (btn->numClicks == -1) {
            singleLongPressHandler(btn);
        }
        if (btn->numClicks == -2) {
            doubleLongPressHandler(btn);
        }
        if (btn->numClicks == -3) {
            tripleLongPressHandler(btn);
        }
    btn->numClicks = 0;
    }
}