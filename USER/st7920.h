#ifndef ST7290_H
#define ST7290_H

#include <stm32f10x.h>

void lcd_ch_busy(void);                                 // LCDæ��⺯��
void lcd_wr_cmd(unsigned char cmd);                     // LCDд�����
void lcd_wr_dat(unsigned char dat);                     // LCDд���ݺ���
unsigned char receive_byte(void);		        // ����һ�ֽ�����
unsigned char lcd_read(void);                           // ��LCD�ڲ�RAM�ж�����
void lcd_set_xy(unsigned char x, unsigned char y);      //������ʾλ��
void GUI_LCD_Init(void);                                // LCD��ʼ������
void SID_OUT(void);                                     //�������ݿ�Ϊ���
void SID_IN(void);                                      //������ݿ�Ϊ����
void GUI_LCD_FAST_CLR(void);                           //ͨ��д�ո�ʵ�ֿ������������ǶԷ�ɫ��ʾ��Ч
void GUI_LCD_CLR(unsigned char CLR_DAT);                //��ȫ��������
void GUI_LCD_NEGA(unsigned char x, unsigned char y, unsigned char num, unsigned char flag);//��ɫ��ʾ
void GUI_DISP_HZ(unsigned char x, unsigned char y, unsigned char *str);                    //��ʾ�ַ���
void GUI_DISP_INT(unsigned char x, unsigned char y, int num, unsigned char flag); //��ʾ��Χ0-65535
void GUI_DISP_FLOAT(unsigned char x, unsigned char y, float num, unsigned char bit_count, unsigned char flag);//������Χ600���ڣ�С�������λ
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
void GUI_DISP_PERCENT( unsigned char x, unsigned char y, unsigned int num);                      //��ʾ�ٷ���
#endif
