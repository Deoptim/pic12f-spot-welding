#if defined _12F1822
// PIC12F1822 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTOSC  //Oscillator Selection: INTOSC oscillator: I/O function on CLKIN pin
#pragma config WDTE = OFF     //Watchdog Timer Enable: WDT disabled
#pragma config PWRTE = ON     //Power-up Timer Enable: PWRT enabled
#pragma config MCLRE = OFF    //MCLR Pin Function Select: MCLR/VPP pin function is digital input
#pragma config CP = OFF       //Flash Program Memory Code Protection: Program memory code protection is disabled
#pragma config CPD = OFF      //Data Memory Code Protection: Data memory code protection is disabled
#pragma config BOREN = ON     //Brown-out Reset Enable: Brown-out Reset enabled
#pragma config CLKOUTEN = OFF //Clock Out Enable: CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin
#pragma config IESO = OFF     //Internal/External Switchover: Internal/External Switchover mode is disabled
#pragma config FCMEN = OFF    //Fail-Safe Clock Monitor Enable: Fail-Safe Clock Monitor is disabled

// CONFIG2
#pragma config WRT = OFF      //Flash Memory Self-Write Protection: Write protection off
#pragma config PLLEN = OFF    //PLL Enable: 4x PLL disabled
#pragma config STVREN = ON    //Stack Overflow/Underflow Reset Enable: Stack Overflow or Underflow will cause a Reset
#pragma config BORV = HI      //Brown-out Reset Voltage Selection: Brown-out Reset Voltage (Vbor), high trip point selected.
#pragma config LVP = OFF      //Low-Voltage Programming Enable: High-voltage on MCLR/VPP must be used for programming

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.


#elif defined _12F683

// PIC12F683 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTOSCIO  // Oscillator Selection bits (INTOSCIO oscillator: I/O function on GP4/OSC2/CLKOUT pin, I/O function on GP5/OSC1/CLKIN)
#pragma config WDTE = OFF        // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON      // Power-up Timer Enable bit (PWRT enabled)
#pragma config MCLRE = OFF      // MCLR Pin Function Select bit (MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Detect (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
#endif