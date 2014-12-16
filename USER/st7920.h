#ifndef ST7290_H
#define ST7290_H

#include <stm32f10x.h>

void lcd_ch_busy(void);                                 // LCD忙检测函数
void lcd_wr_cmd(unsigned char cmd);                     // LCD写命令函数
void lcd_wr_dat(unsigned char dat);                     // LCD写数据函数
unsigned char receive_byte(void);		        // 接收一字节数据
unsigned char lcd_read(void);                           // 从LCD内部RAM中读数据
void lcd_set_xy(unsigned char x, unsigned char y);      //设置显示位置
void GUI_LCD_Init(void);                                // LCD初始化函数
void SID_OUT(void);                                     //设置数据口为输出
void SID_IN(void);                                      //设计数据口为输入
void GUI_LCD_FAST_CLR(void);                           //通过写空格实现快速清屏，但是对反色显示无效
void GUI_LCD_CLR(unsigned char CLR_DAT);                //完全清屏函数
void GUI_LCD_NEGA(unsigned char x, unsigned char y, unsigned char num, unsigned char flag);//反色显示
void GUI_DISP_HZ(unsigned char x, unsigned char y, unsigned char *str);                    //显示字符串
void GUI_DISP_INT(unsigned char x, unsigned char y, int num, unsigned char flag); //显示范围0-65535
void GUI_DISP_FLOAT(unsigned char x, unsigned char y, float num, unsigned char bit_count, unsigned char flag);//整数范围600以内，小数点后两位
//void GUI_DISP_HZEX(unsigned char x, unsigned char y, const rom unsigned char *str, unsigned char num, unsigned char flag);
void GUI_DISP_ITEM(unsigned char x, unsigned char y, const unsigned char *str, unsigned char flag);
void GUI_DISP_DEC(unsigned char x, unsigned char y, unsigned int num, unsigned char bits, unsigned char flag);
void GUI_DISP_CHAR(unsigned char x, unsigned char y, unsigned char num, unsigned char nage, unsigned char flag);
void GUI_DISP_LONG(unsigned char x, unsigned char y, unsigned int num_h, unsigned int num_l);
//void GUI_DISP_ROM_HZ(unsigned char x, unsigned char y,  const rom unsigned char  *str);
void GUI_DISP_888_NUN(unsigned char x, unsigned char y, unsigned int num, unsigned char flag);
void GUI_DISP_8888_NUN(unsigned char x, unsigned char y, unsigned int num, unsigned char flag);
void GUI_DISP_888888_NUN(unsigned char x, unsigned char y, unsigned long num);
void GUI_DISP_88_NUN(unsigned char x, unsigned char y, unsigned int num, unsigned char flag, unsigned char flag_P);
void GUI_DISP_88_NUN_NEW(unsigned char x, unsigned char y, unsigned int num);
void GUI_DISP_PERCENT( unsigned char x, unsigned char y, unsigned int num);                      //显示百分数
#endif
