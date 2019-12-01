/* 
 * File:   lemac_lab6_lcd.h
 * Author: Alex
 *
 * Created on November 13, 2019, 2:44 AM
 */

#ifndef LEMAC_LAB6_LCD_H
#define	LEMAC_LAB6_LCD_H

#ifdef	__cplusplus
extern "C" {
#endif


    void lem_wait_1ms(void);//for delay function
    //WEEK 1- LAB5
    void lcd_cmd(char command);
    void lcd_init(void);
    void lcd_setCursor(char x, char y);
    void lcd_printChar(char myChar);
    
      //WEEK 2- LAB5
    void lcd_printStr(const char *s);

#ifdef	__cplusplus
}
#endif

#endif	/* LEMAC_LAB6_LCD_H */

