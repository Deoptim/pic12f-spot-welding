#ifndef _EEPROM_H_H
#define _EEPROM_H_H
//-----------------
extern void EEPROM_WriteByte(unsigned char addr, unsigned char dt);
extern unsigned char EEPROM_ReadByte(unsigned char addr);
/*
extern void EEPROM_WriteWord(unsigned char addr, unsigned int ucData);
extern unsigned int EEPROM_ReadWord(unsigned char addr);
extern void EEPROM_WriteDword(unsigned char addr, unsigned long ucData);
extern unsigned long EEPROM_ReadDword(unsigned char addr);
*/
//-----------------------------------------------------
#endif /* _EEPROM_H */