#include <xc.h> // include processor files - each processor file is guarded.
#include "EEPROM.h"
//-----------------------------------------------------
//void EEPROM_WriteByte(unsigned char addr, unsigned char dt);
//unsigned char EEPROM_ReadByte(unsigned char addr);
//-----------------------------------------------------
void EEPROM_WriteByte(unsigned char addr, unsigned char dt)
{
    unsigned char status;
    while(WR);
#if defined _12F1822
    EEADRL = addr;
    EEDATL = dt;
    CFGS = 0;           //Deselect Config space
    EEPGD = 0;          //Point to DATA memory
#elif defined _12F683
    EEADR = addr;
    EEDAT = dt;
#endif
    WREN=1;
    status = GIE;
    GIE = 0;
    EECON2 = 0x55;
    EECON2 = 0xAA;
    WR=1;
    GIE = status;
    WREN=0;
}
//-----------------------------------------------------
unsigned char EEPROM_ReadByte(unsigned char addr)
{
    while(RD || WR);
#if defined _12F1822
    EEADRL=addr;
    CFGS = 0;           //Deselect Config space
    EEPGD = 0;          //Point to DATA memory
    RD = 1;
    return EEDATL;
#elif defined _12F683
    EEADR=addr;
    RD = 1;
    return EEDAT;
#endif
}
/*
//-----------------------------------------------------

void EEPROM_WriteWord(unsigned char addr, unsigned int ucData)
{
    EEPROM_WriteByte(addr, (unsigned char) ucData);
    unsigned char dt = ucData>>8;
    EEPROM_WriteByte(addr+1, dt);
}
//-----------------------------------------------------
unsigned int EEPROM_ReadWord(unsigned char addr)
{
    unsigned int dt = EEPROM_ReadByte(addr+1)*256;
    dt += EEPROM_ReadByte(addr);
    return dt;
}
//-----------------------------------------------------
void EEPROM_WriteDword(unsigned char addr, unsigned long ucData)
{
    EEPROM_WriteWord(addr, (unsigned int) ucData);
    unsigned int dt = ucData>>16;
    EEPROM_WriteWord(addr+2, dt);
}
//-----------------------------------------------------
unsigned long EEPROM_ReadDword(unsigned char addr)
{
    unsigned long dt = EEPROM_ReadWord(addr+2)*65536;
    dt += EEPROM_ReadWord(addr);
    return dt;
}
//-----------------------------------------------------
*/