/* Main.c file generated by "Proteus 8 Pro" New Project wizard
 *
 * Created:   Tue Nov 7 2023
 * Processor: PIC12F683
 * Compiler:  MPLAB XC8
 */

#define _XTAL_FREQ 8000000UL // Required for __delay_us(),__delay_ms(), Timer1(millis) and Timer2(PWM) (specify clock 8MHz)

// GLOBAL INCLUDES
#include <xc.h>
#include "config_xc8.h"
#include "EEPROM.h"
#include "ClickButton.h"
//====================

// DEFINITIONS
#if defined _12F1822
#define BTN1 RA1               //pin 6: Push Button1
#define BTN2 RA0               //pin 7: Welding leads as a Button2
#define LED RA4                //pin 3: Led
#define KEY RA5                //pin 2: Key to trigger transistors
#elif defined _12F683
#define BTN1 GP1               //pin 6: Push Button1
#define BTN2 GP0               //pin 7: Welding leads as a Button2
#define LED GP4                //pin 3: Led
#define KEY GP5                //pin 2: Key to trigger transistors
#endif

#define SHORT_BEEP_MS 50                 //Short Beep time (in ms)
#define LONG_BEEP_MS 200                 //Long Beep time (in ms)
#define BTN_ACTIVE_HIGH 1                //Set to 1 if active-high or set to 0 if active-low
#define BTN_DELAY_BW_MULTI_CLICKS_MS 250 //Time (in ms) to detect multi clicks of button
#define BTN_LONG_CLICK_MS 1000           //Time (in ms) to detect Long click handler
#define MAX_INCR_GLOBAL_MODES 0          //Max increment a value of global modes
#define MAX_INCR_SIXTH_MODE 10           //Max increment a value of sixth mode (step in 3ms)

//#define BEEP_CHANGE_MODE                 //Uncomment this line if we want to beep buzzer(number of times) when change mode
//#define SAVE_LAST_MODE                   //Uncomment this line if we want to save last used mode when power-off
//====================

// VARIABLES
typedef __bit boolean;
volatile unsigned long millis = 0;
unsigned char countFlashLed = 0;
boolean modeEdit = 0;
boolean ledDelay = 0;
boolean ledFlash = 0;
unsigned char mode = 0;
const unsigned char maxModes = 6; //Total modes is 6.
const unsigned char keyDelayArray[maxModes+1] = { 5, 8, 11, 14, 17, 20, 0 };
unsigned char keyIncrSixthMode = 0;

Button btn1 = { BTN_ACTIVE_HIGH, \
               !BTN_ACTIVE_HIGH, \
               !BTN_ACTIVE_HIGH, \
               0, \
               0, \
               0, \
               0, \
               0, \
               20, \
               BTN_DELAY_BW_MULTI_CLICKS_MS, \
               BTN_LONG_CLICK_MS };
//we use same definitions as on btn1 or use your own
Button btn2 = { BTN_ACTIVE_HIGH, \
               !BTN_ACTIVE_HIGH, \
               !BTN_ACTIVE_HIGH, \
               0, \
               0, \
               0, \
               0, \
               0, \
               20, \
               BTN_DELAY_BW_MULTI_CLICKS_MS, \
               BTN_LONG_CLICK_MS };
//====================

void setup(void);	       
	       
void __interrupt() isr(void) {
    if (TMR1IF) {
        TMR1IF = 0;             //Clear interrupt Timer1 flag
#if defined _12F1822
        unsigned short tpr = _XTAL_FREQ/1000; //Description in setup();
#elif defined _12F683
        unsigned short tpr = _XTAL_FREQ/4/1000; //Description in setup();
#endif
        tpr = 0xFFFF - tpr;
        TMR1H = tpr >> 8;       //Set Initial Value of Timer1
        TMR1L = tpr & 0xFF;     //*
    millis++;
    }
}

unsigned long _millis(void) {
    return millis;
}

void _delay_us(unsigned long microseconds) {
    while(microseconds > 0)
    {
         __delay_us(1);  //for this internal function we need constant value and defined _XTAL_FREQ
         microseconds--;
    }
}

void _delay_ms(unsigned long milliseconds) {
    while(milliseconds > 0)
    {
         __delay_ms(1);  //for this internal function we need constant value and defined _XTAL_FREQ
         milliseconds--;
    }
}

void beep(unsigned long value) {
    CCPR1L = PR2/2;     //Set PWM duty cycle to 50% (or turn On Buzzer)
    _delay_ms(value);
    CCPR1L = 0;         //Set PWM duty cycle to 0% (or turn Off Buzzer)
}

void sleepMCU(void) {
    ADON = 0;          // Disable ADC (disable because it work when MCU is sleep)
//    commented because WDT alreadey fully disabled in CONFIG1:
//    SWDTEN = 0;      // Disable Watchdog (disable because it draws a current 300nA(datasheet) when MCU is sleep)
    TMR1ON = 0;        // Disable TMR1 while sleep (disable because it draws a current 650nA(datasheet) when MCU is sleep)
    PEIE = 0;          // Disable Peripheral Interrupt
    GIE = 0;           // Disable Global Interrupt
#if defined _12F1822
    IOCAP1 = 1;        // Enable Interrupt On Change on RA1(Button 1) as positive edge register
    IOCAF1 = 0;        // Clear interrupt flag on RA1(Button 1) in Interrupt On Change register
    IOCIE = 1;         // Enable Interrupt On Change
#elif defined _12F683
    CMCON0 = 0b111;    //Turning off the comparator for Sleep
    ULPWUE = 1;        //Enable Ultra Low-Power Wake-up
    INTEDG = 1;        //Interrupt Edge Select: Interrupt on rising edge of INT pin
    IOC = 0b00000010;  //Enable Interrupt On Change only on on GP1(Button 1)
    GPIF = 0;          //Clear GPIO Change Interrupt Flag
    GPIE = 1;          //Enables the GPIO change interrupt
#endif
    NOP();
    SLEEP();           // Sleep the processor
    NOP();
    while(BTN1);      // Wait while the button will be released
//  RESET();        // Software reset has not implemented on PIC12F683
#if defined _12F1822
    IOCAP1 = 0;        // Disable Interrupt On Change on RA1(Button 1) as positive edge register ofter Sleep
    IOCIE = 0;         // Disable Interrupt On Change ofter Sleep
#elif defined _12F683
    GPIE = 0;          //Disable the GPIO change interrupt ofter Sleep
    GPIF = 0;          //Clear GPIO Change Interrupt Flag after Sleep
#endif
    ADON = 1;          // Enable ADC
    TMR1ON = 1;        // Turn on TMR1
    PEIE = 1;          // Enable Peripheral Interrupt
    GIE = 1;           // Enable Global Interrupt
}

// led blink function where argument "count" is number of blinks
// function use "ledDelay" and "ledFlash" as bool value, from millis() with modulo division operator
// this allows us use delay without function _delay() that hungs MCU while
void ledBlink(unsigned char count) {
    if (ledDelay) {
        if (ledFlash && (count+1) - countFlashLed > 0) {
            if (LED) LED=0;       //LED on
        } else {
            if (!LED) {
                LED=1;            //LED off
                countFlashLed++;
            }
        }
    } else {
        if (!LED) LED=1;          //LED off
        countFlashLed = 0;
    }
}

// single click button
void singlePressHandler(Button *btn) {
    if (&btn1 == btn) {
        if (mode == maxModes-1 && modeEdit) {    //Switch Sixth mode
            beep(SHORT_BEEP_MS);
            if (keyIncrSixthMode < MAX_INCR_SIXTH_MODE)
                keyIncrSixthMode++;
            else
                keyIncrSixthMode = 0;
            EEPROM_WriteByte(1,keyIncrSixthMode);
        } else {                                     //Switch Global mode
            if (mode < maxModes)
                mode++;
            else
                mode = 0;
#ifdef SAVE_LAST_MODE
            EEPROM_WriteByte(0,mode);
#endif
            countFlashLed = 0;    
            if (mode == maxModes) { //max mode (end of array keyDelayArray) is sleep idle mode
                beep(LONG_BEEP_MS);
                LED=1;              //LED off before sleep
                KEY=1;              //set KEY to HIGH(MOSFETs is closed) state before sleep
                sleepMCU();         //Sleep CPU
	        setup();            //Reinit pins, timers and peripheral after sleep
	        mode = 0;           //Change mode to first one after sleep 
	        beep(LONG_BEEP_MS); //Beep after sleep (like first power-up)
            } else {
#ifdef BEEP_CHANGE_MODE
                for(int i = 0; i <= mode; i++) {
                beep(10);
                _delay_ms(10);
            }
#else
                beep(SHORT_BEEP_MS);
#endif
            }
        }
    }
}

void doublePressHandler(Button *btn) {
}

void triplePressHandler(Button *btn) {
}

// long click button
void singleLongPressHandler(Button *btn) {
    if (&btn1 == btn) {
        if (mode == maxModes-1 && modeEdit) {
            beep(LONG_BEEP_MS);
            modeEdit = 0;
        } else if (mode == maxModes-1 && !modeEdit) {
            beep(LONG_BEEP_MS);
            keyIncrSixthMode = 0;
            EEPROM_WriteByte(1,0);
            modeEdit = 1;
        }
    }
    if (&btn2 == btn) {  //btn2 as Spot Welding Electrodes
        if (!modeEdit) {
            KEY = 0; //pull-down trigger gate of transistor
            //if we use sixth mode, then we use delay in milliseconds, for example var keyDelayArray[6] in sixth mode =20ms
            //value keyIncrSixthMode is reads from EEPROM in main() function
            //in sub-mode of sixth mode 1...10 we get 20ms+(mode*3ms) i.e. from
	    // 20ms+3ms up to 20ms+30ms, current keyIncrSixthMode is saved in EEPROM by default
            if (mode == maxModes-1)
                _delay_ms((keyDelayArray[mode])+(keyIncrSixthMode*3));
            else
            //when we switch mode 1...5 we get actual value from array keyDelayArray[]
                _delay_ms(keyDelayArray[mode]);
            KEY = 1;//pull-up trigger gate of transistor
            beep(SHORT_BEEP_MS);
        }
        
    }
}

void doubleLongPressHandler(Button *btn) {
}

void tripleLongPressHandler(Button *btn) {
}

void setup(void) {
#if defined _12F1822
    OSCCON = 0b01110010;    //Set clock to 8MHZ, select internal oscillator
    ANSELA = 0;             //Init(clear) bits for Analog select
    TRISA = 0b00000011;     //Set port RA0 and RA1 as input
//    LATA = 0b00000000;
#elif defined _12F683
    OSCCON = 0b01110001;     //Set clock to 8MHZ, select internal oscillator
    CMCON0 = 0b111;          //Turning off the comparator
    ANSEL = 0;               //Init(clear) bits for Analog select
    TRISIO = 0b00000011;     //Set port GP0 and GP1 as input
//    nGPPU = 0;
//    WPU = 0b11111100;
#endif
    KEY=1;                   //set KEY to HIGH(MOSFET is closed) state when power-up
    LED=0;

#if defined _12F1822
    // Set up TIMER1 for PIC12F1822 to tick at 1ms intervals.
    // The oscillator of Timer1 ticks at FOSC(without prescaler).
    // That is 8MHz or 1/8000000 seconds per tick.
    // or 0.000000125s, or 0.000125ms, so 1 ms is 8000 ticks.
    TMR1CS1 = 0;           //System Clock (FOSC) for Timer1
    TMR1CS0 = 1;           //*
    unsigned short tpr = _XTAL_FREQ/1000;
#elif defined _12F683
    // Set up TIMER1 for PIC12F683 to tick at 1ms intervals.
    // The oscillator of Timer1 ticks at FOSC(without prescaler).
    // That is 2MHz(8MHz/4) or 1/2000000 seconds per tick.
    // or 0.0000005s, or 0.0005ms, so 1 ms is 2000 ticks.
    TMR1CS = 0;            //Internal clock (FOSC/4)
    unsigned short tpr = _XTAL_FREQ/4/1000;
#endif
    tpr = 0xFFFF - tpr;
    TMR1H = tpr >> 8;      //Set Initial Value of Timer1
    TMR1L = tpr & 0xFF;    //*
    TMR1IF = 0;            //Clear interrupt Timer1 flag
    TMR1IE = 1;            //Enable Timer1 Overflow Interrupt
    TMR1ON = 1;            //Turn on TMR1

    // PWM Setup on RA2 (on 12F1822) or GP2 (on 12F683)
    // The TIMER2 used for CCP1 clock.
    // The oscillator of Timer2 ticks at FOSC/4 (8MHz/4).
    // We chose prescaler value for the Timer2 is 4.
    // Regular Buzzer rezonant frequncy is 2kHz,
    // so FOSC/4/4/2000 = 250 or PR2 value is 250, also PR2/2
    // will be 50% duty cycle of value CCPR1L, i.e. 250/2 = 125
#if defined _12F1822
    CCP1SEL = 0;               //Pin 5 (RA2) as pin CCP1
#endif
    CCP1CON = 0b00001100;      //Use PWM function (single)
    T2CON = 0b00000001;        //Set TMR2 prescaler value to 4, i.e. (FOSC/4)/4
    CCPR1L = 0;                //Init with 0 duty cycle
    CCPR1H = 0;                //*
    TMR2 = 0;                  //Init Timer 2
    unsigned char tpr2 = _XTAL_FREQ/4/4/2000;
    PR2 = tpr2;                //Set PWM cycle (to 2000Hz for Buzzer)
    TMR2ON = 1;                //Turn on TMR2 (PWM)
    CCPR1L = 0;                //Set PWM duty cycle to 0=0% (i.e. turn Off Buzzer)

    // Enable the interrupt system
    PEIE = 1;          // Enable Peripheral Interrupt
    GIE = 1;           // Enable Global Interrupt
}

void main(void)
{
    setup();
    
    //If Button already is pressed at start, then reset settings (rewrite EEPROM to default values)
    if (BTN1) {
#ifdef SAVE_LAST_MODE
        EEPROM_WriteByte(0,0);                //Reset the mode to: 0
#endif
        EEPROM_WriteByte(1,keyIncrSixthMode); //Reset the Sixth mode to: 20ms
        EEPROM_WriteByte(2,0);                //Reset increment a value of global modes to: 0 
    }
#ifdef SAVE_LAST_MODE
    mode = EEPROM_ReadByte(0);
    if (mode > maxModes || mode == maxModes) {
        EEPROM_WriteByte(0,0);
        mode = 0;
    }
#else
    mode = 0;
#endif
    keyIncrSixthMode = EEPROM_ReadByte(1);
    if (keyIncrSixthMode > MAX_INCR_SIXTH_MODE) {
        EEPROM_WriteByte(1,0);
        keyIncrSixthMode = 0;
    }

#ifdef BEEP_CHANGE_MODE
    for(int i = 0; i <= mode; i++) {  //Beep number of times(mode) when power-up or reset after sleep
        beep(SHORT_BEEP_MS);
        _delay_ms(SHORT_BEEP_MS);
    }
#else
    beep(LONG_BEEP_MS); //Beep when power-up
#endif

    while(1) {
        updateBTN(&btn1, BTN1); //Button1 for switch modes (single click)
        updateBTN(&btn2, BTN2); //Short-circuit of welding leads as a Button2 (long click)
        
        if (!modeEdit) {
            ledDelay = (_millis()/1000)%2;               //using no delay method to blink leds
            ledFlash = (_millis()/50)%2;                 //*
            ledBlink(mode);                              //Blink in Global modes "mode" times
        } else {
            ledDelay = (_millis()/1000)%2;               //*
            ledFlash = (_millis()/50)%2;                 //*
            ledBlink(keyIncrSixthMode);                  //Blink in Sixth mode incr times
        }
    }
}