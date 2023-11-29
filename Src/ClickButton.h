#ifndef ClickButton_H
#define ClickButton_H

// vars for Button
struct tagButton {
    unsigned char _activeHigh:1; // Assume active-low button
    unsigned char _btnState:1; // initial button state in active-high logic
    unsigned char _lastState:1;
    unsigned char depressed:1;
    signed char clicks;
    signed char numClicks;
    unsigned char _clickCount;
    unsigned long _lastBounceTime;
    unsigned long debounceTime; // Debounce timer in ms
    unsigned long multiclickTime; // Time limit for multi clicks
    unsigned long longClickTime; // time until long clicks register
};

typedef struct tagButton Button;

unsigned long _millis(void);
extern void updateBTN(Button *btn, unsigned char btnState);
void triplePressHandler(Button *btn);
void doublePressHandler(Button *btn);
void singlePressHandler(Button *btn);
void singleLongPressHandler(Button *btn);
void doubleLongPressHandler(Button *btn);
void tripleLongPressHandler(Button *btn);


#endif /* ClickButton_H */