#include <stdlib.h>
#include<stdio.h>
#include<string.h>
#include <st7920.h>

#ifdef  DeBug
#define GPIO_LCD_SID       GPIOB
#define GPIO_Pin_LCD_SID   GPIO_Pin_6

#define GPIO_LCD_CS        GPIOB
#define GPIO_Pin_LCD_CS    GPIO_Pin_7

#define GPIO_LCD_CLK       GPIOB
#define GPIO_Pin_LCD_CLK   GPIO_Pin_5

#define RCC_LCD_GPIO       RCC_APB2Periph_GPIOB 

#else
#define GPIO_LCD_SID       GPIOB
#define GPIO_Pin_LCD_SID   GPIO_Pin_0

#define GPIO_LCD_CS        GPIOB
#define GPIO_Pin_LCD_CS    GPIO_Pin_1

#define GPIO_LCD_CLK       GPIOC
#define GPIO_Pin_LCD_CLK   GPIO_Pin_5

#define RCC_LCD_GPIO           RCC_APB2Periph_GPIOB| RCC_APB2Periph_GPIOC
#endif

#define LCD_SID    GPIO_ReadOutputDataBit(GPIO_LCD_SID,GPIO_Pin_LCD_SID)                 // ��LCD�����Ŀ��ƶ˿�

#define SET_CS()   GPIO_SetBits(GPIO_LCD_CS, GPIO_Pin_LCD_CS)
#define CLR_CS()   GPIO_ResetBits(GPIO_LCD_CS, GPIO_Pin_LCD_CS)

#define SET_SID()  GPIO_SetBits(GPIO_LCD_SID,GPIO_Pin_LCD_SID)
#define CLR_SID()  GPIO_ResetBits(GPIO_LCD_SID,GPIO_Pin_LCD_SID)

#define SET_CLK()  GPIO_SetBits(GPIO_LCD_CLK,GPIO_Pin_LCD_CLK)
#define CLR_CLK()  GPIO_ResetBits(GPIO_LCD_CLK,GPIO_Pin_LCD_CLK)
const unsigned char AC[] =	  //����ָ������Ļ�и����еļĴ�����ַ����ʾ����ʱ��
{
    0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
    0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97,
    0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f,
    0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f,
};
//************************************************************
//������Ȩ: CharlieShao
//��������: void GUI_LCD_Init(void)
//��������: LCD��ʼ������
//�������: ��
//���ز���: ��
//����ʱ��: 2013-02-05
//�޸ļ�¼:
//   ��¼I:
//         �޸�ʱ��:
//         �޸�����:
//************************************************************
void GUI_LCD_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd( RCC_LCD_GPIO, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_LCD_SID;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIO_LCD_SID, &GPIO_InitStructure);
    GPIO_SetBits(GPIO_LCD_SID, GPIO_Pin_LCD_SID);
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_LCD_CLK;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIO_LCD_CLK, &GPIO_InitStructure);
    GPIO_SetBits(GPIO_LCD_CLK, GPIO_Pin_LCD_CLK);
	
   	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_LCD_CS;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIO_LCD_CS, &GPIO_InitStructure);
    GPIO_SetBits(GPIO_LCD_CS, GPIO_Pin_LCD_CS);

    lcd_wr_cmd(0x30);                                // �����趨������ָ��
    lcd_wr_cmd(0x0C);                                // ��ʾ���أ�������ʾon���α�on���α�λon
    lcd_wr_cmd(0x01);                                // ������AC��0
    lcd_wr_cmd(0x04);                                // �����趨�㣬д��ʱ,�α����ƶ�
    lcd_wr_cmd(0x80);
}
void SID_OUT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}
void SID_IN(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

//************************************************************
//������Ȩ: CharlieShao
//��������: void lcd_send_byte(unsigned char dat)
//��������: ��ST7290��д��һ�ֽڵ�����
//�������: uunsigned char dat Ҫд�������
//���ز���: ��
//����ʱ��: 2013-02-05
//�޸ļ�¼:
//   ��¼I:
//         �޸�ʱ��:
//         �޸�����:
//************************************************************
void lcd_send_byte(unsigned char dat)
{
    unsigned char i = 0;
    SID_OUT();
    CLR_CLK();
    SET_CS();
    for(i = 0; i < 8; i++)
    {

        if(dat & 0x80)
        {
            SET_SID();
        }
        else
        {
            CLR_SID();
        }
        __nop();
        __nop();
        __nop();
        SET_CLK();
        __nop();
        __nop();
        __nop();
        dat <<= 1;
        CLR_CLK();//��֤���һ�β�����������������
    }
    CLR_CS();
}
//************************************************************
//������Ȩ: CharlieShao
//��������: unsigned char receive_byte(void)
//��������: ��ȡST7290�ڲ�����
//�������: ��
//���ز���: ��
//����ʱ��: 2013-02-05
//�޸ļ�¼:
//   ��¼I:
//         �޸�ʱ��:
//         �޸�����:
//************************************************************
unsigned char receive_byte(void)//�ɿ�
{
    unsigned char i = 0, temp_h = 0, temp_l = 0;
    SID_IN();
    CLR_CLK();
    SET_CS();
    for(i = 0; i < 8; i++)
    {
        temp_h = temp_h << 1;
        //			  temp=30;
        //				while(temp--){};//300ns, ��С����13.88ns
        SET_CLK();
        //				temp=10;
        //				while(temp--){};//100ns, ��С����13.88ns
        if(LCD_SID)
        {
            temp_h++;
        }
        CLR_CLK();
    }

    for(i = 0; i < 8; i++)
    {
        temp_l = temp_l << 1;
        //		    temp=30;
        //				while(temp--){};//300ns, ��С����13.88ns
        SET_CLK();
        //				temp=10;
        //				while(temp--){};//100ns, ��С����13.88ns
        if(LCD_SID)
        {
            temp_l++;
        }
        CLR_CLK();
    }
    CLR_CS();
    return ((0xf0 & temp_h) + (0x0f & temp_l));
}
//************************************************************
//������Ȩ: CharlieShao
//��������: void lcd_wr_cmd(unsigned char cmd)
//��������: ��ST7290��д���������
//�������: unsigend char cmd Ҫд�������
//���ز���: ��
//����ʱ��: 2013-02-05
//�޸ļ�¼:
//   ��¼I:
//         �޸�ʱ��:
//         �޸�����:
//************************************************************
void lcd_wr_cmd(unsigned char cmd)
{
    //    lcd_ch_busy();
    __nop();
    __nop();
    __nop();
    lcd_send_byte(0xf8);          //11111,RW(0),RS(0),0
    lcd_send_byte(0xf0 & cmd);
    lcd_send_byte(0xf0 & cmd << 4);
}
//************************************************************
//������Ȩ: CharlieShao
//��������: void lcd_wr_dat(unsigned char dat)
//��������: ��ST7290��д������
//�������: unsigend char md Ҫд�������
//���ز���: ��
//����ʱ��: 2013-02-05
//�޸ļ�¼:
//   ��¼I:
//         �޸�ʱ��:
//         �޸�����:
//************************************************************
void lcd_wr_dat(unsigned char dat)
{
    //    lcd_ch_busy();
    __nop();
    __nop();
    __nop();
    lcd_send_byte(0xfa);          //11111,RW(0),RS(1),0
    lcd_send_byte(0xf0 & dat);
    lcd_send_byte(0xf0 & dat << 4);
}
//************************************************************
//������Ȩ: CharlieShao
//��������: void lcd_ch_busy(void)
//��������: ��ȡST7290��״̬
//�������: ��
//���ز���: ��
//����ʱ��: 2013-02-05
//�޸ļ�¼:
//   ��¼I:
//         �޸�ʱ��:
//         �޸�����:
//************************************************************
void lcd_ch_busy(void)
{
    do
    {
        lcd_send_byte(0xfc);     //11111,RW(1),RS(0),0
    }
    while(0x80 & receive_byte());
}

//************************************************************
//������Ȩ: CharlieShao
//��������: unsigned char lcd_read(void)
//��������: ��ȡST7290�ڲ�����
//�������: ��
//���ز���: ��ST7920�ж�ȡ��������
//����ʱ��: 2013-02-05
//�޸ļ�¼:
//   ��¼I:
//         �޸�ʱ��:
//         �޸�����:
//��ע   : �ڴ���ģʽ��ֻ�ܶ���ָ��,���ܶ���SRAM�е�����
//************************************************************
unsigned char lcd_read(void)
{
    unsigned char temp = 0;
    lcd_ch_busy();
    lcd_send_byte(0xfe);
    temp = receive_byte();
    return temp;
}

//************************************************************
//������Ȩ: CharlieShao
//��������: void GUI_LCD_CLR(unsigned char CLR_DAT)
//��������: LCD��������
//�������: unsigned char CLR_DAT  0xFF:LCDȫ�� 0x00:LCDȫ��
//���ز���: ��
//����ʱ��: 2013-02-05
//�޸ļ�¼:
//   ��¼I:
//         �޸�ʱ��:
//************************************************************
void GUI_LCD_CLR(unsigned char CLR_DAT)
{
    unsigned char i = 0;
    unsigned char j = 0;

    lcd_wr_cmd(0x34);					      // ����ͼ��ָ��,ͼ��ģʽ�µ���������
    lcd_wr_cmd(0x36);					      // ����ͼ��ָ��

    for(i = 0; i < 32; i++)
    {
        lcd_wr_cmd(0x80 + i);
        lcd_wr_cmd(0x80);
        for(j = 0; j < 16; j++)
        {
            lcd_wr_dat(CLR_DAT);
        }
    }

    for(i = 0; i < 32; i++)
    {
        lcd_wr_cmd(0x80 + i);
        lcd_wr_cmd(0x88);
        for(j = 0; j < 16; j++)
        {
            lcd_wr_dat(CLR_DAT);
        }
    }

    lcd_wr_cmd(0x30);                         // ���ػ���ָ��,�ı�ģʽ�µ���������
    lcd_wr_cmd(0x80);                         // ���ػ���ָ��
    for(i = 0; i < 64; i++)
    {
        lcd_wr_dat(0x20);
    }
}

//************************************************************
//������Ȩ: CharlieShao
//��������: void GUI_LCD_NEGA(unsigned char x,unsigned char y,unsigned char num,unsigned char flag)
//��������: ����һ���е��ֽڽ��з�ɫ��ʾ
//�������: unsigned char x          ��X��1 - 4
//          unsigned char y         ��Y�� 0 - 15
//          unsigned cahr num       ������0 - 15 ��Yֵ����
//          unsigned char flag     0:ȡ����ɫ��ʾ 1:���з�ɫ��ʾ
//���ز���: ��
//����ʱ��: 2013-02-05
//�޸ļ�¼:
//   ��¼I:
//         �޸�ʱ��:
//         �޸�����:
//************************************************************
void GUI_LCD_NEGA(unsigned char x, unsigned char y, unsigned char num, unsigned char flag)
{
    unsigned char i = 0;	 	  			 	  	  // ѭ�����Ʊ���
    unsigned char j = 0;                              // ��ѭ�����Ʊ���
    unsigned char temp = 0;					 		  // �м����
    unsigned char b_odd = 0;						  // �Ƿ�Ϊ����
    unsigned char write_date = 0;					  // Ҫд�������
    unsigned char base_xpos = 0;                      // �趨��׼��ַ
    unsigned char base_ypos = 0;                      // �趨��׼��ַ

    if(flag == 0)									  // ȡ����ɫ��ʾ
    {
        write_date = 0x00;
    }
    else if(flag == 1)								  // ���з�ɫ��ʾ
    {
        write_date = 0xFF;
    }

    b_odd = num % 2;
    num = num / 2;

    if(b_odd == 1)									  // Ϊ����
    {
        num += 1;
    }

    temp = num + x;
    if(temp > 16)
    {
        temp = 17 - x;
        num = temp;
    }

    if(y == 1)                                     	  // ��һ��
    {
        base_xpos = 0x80;                             // ��һ�е���ʼ��ַ
        base_ypos = 0x80;
    }
    else if(y == 2)
    {
        base_xpos = 0x80;							  // �ڶ��е���ʼ��ַ
        base_ypos = 0x90;
    }
    else if((y == 3))
    {
        base_xpos = 0x88;                              // �����е���ʼ��ַ
        base_ypos = 0x80;
    }
    else if(y == 4)
    {
        base_xpos = 0x88;							   // �����е���ʼ��ַ
        base_ypos = 0x90;
    }

    if(x >= 1)
    {
        x = x - 1;
    }

    lcd_wr_cmd(0x34);
    lcd_wr_cmd(0x36);

    for(j = 0; j < num; j++)
    {
        for(i = 0; i < 16; i++)
        {
            lcd_wr_cmd(base_ypos + i);                 // �趨GDRAM Y������
            lcd_wr_cmd(base_xpos + x + j);             // �趨GDRAM X������
            if(((j + 1) == num) && (b_odd == 1))		 // Ϊ����
            {
                lcd_wr_dat(write_date);
            }
            else
            {
                lcd_wr_dat(write_date);
                lcd_wr_dat(write_date);
            }
        }
    }
    lcd_wr_cmd(0x30);
}
//************************************************************
//������Ȩ: CharlieShao
//��������: void set_xy(unsigned char x,unsigned char y)
//��������: �趨��ʾλ��,�ı�ģʽ��
//�������: unsigned char x       ��x��1 - 4
//               unsigned char y       ��y��1 - 8
//���ز���: ��
//����ʱ��: 2013-02-05
//�޸ļ�¼:
//   ��¼I:
//         �޸�ʱ��:
//         �޸�����:
//************************************************************
void lcd_set_xy(unsigned char x, unsigned char y)  //��x��1-8����y��1-4
{
    unsigned char addr = 0;

    addr = 8 * y + x - 9; 		  				 //��x,y����
    lcd_wr_cmd(AC[addr]);					   //���AC��ʵ�ʼĴ�����ַ
}

void lcd_set_xy_16(unsigned char x, unsigned char y)  //��x��1-8����y��1-4
{
    unsigned char addr = 0;

    addr = 8 * y + x - 9; 		  				 //��x,y����
    lcd_wr_cmd(AC[addr]);					   //���AC��ʵ�ʼĴ�����ַ
}
//************************************************************
//������Ȩ: CharlieShao
//��������: void GUI_DISP_HZ(unsigned char x,unsigned char y,unsigned char *str)
//��������: ��ָ��λ����ʾָ���ĺ����ַ���
//�������: unsigned char x       Ҫ��ʾ��X������ 1 - 8
//          unsigned char y       Ҫ��ʾ��X������ 1 - 4
//          unsigned char *str    Ҫ��ʾ�ĺ����ַ���
//���ز���: ��
//����ʱ��: 2013-02-05
//�޸ļ�¼:
//   ��¼I:
//         �޸�ʱ��:
//         �޸�����:
//************************************************************
void GUI_DISP_HZ(unsigned char x, unsigned char y, unsigned char  *str)
{
    unsigned char *p ;   			 		   //����һ��ָ��

    p = str;								   //ָ��ָ���ַ�����M16�ڴ��еĵ�ַ

    lcd_wr_cmd(0x30);                          //���ػ���ָ��
    lcd_set_xy(x, y);						  //��Ҫ����Һ���ļĴ�����ַ
    while(*p != 0)						       //�ж�ָ���Ƿ����ַ�����ĩβ
    {
        lcd_wr_dat(*p);				       	   //д��ָ��ָ����ַ�
        p = ++str;							 //str��ַ��1���ָ��
    }
}
void GUI_DISP_ROM_HZ(unsigned char x, unsigned char y,  const unsigned char  *str)
{
    //unsigned char *p ;   			 		   //����һ��ָ��

    //p = str;								   //ָ��ָ���ַ�����M16�ڴ��еĵ�ַ

    lcd_wr_cmd(0x30);              //���ػ���ָ��
    lcd_set_xy(x, y);						  //��Ҫ����Һ���ļĴ�����ַ
    while(*str != 0)						      //�ж�ָ���Ƿ����ַ�����ĩβ
    {
        lcd_wr_dat(*str);				       	   //д��ָ��ָ����ַ�
        str = ++str;							 //str��ַ��1���ָ��
    }
}
//************************************************************
//������Ȩ: CharlieShao
//��������: void GUI_DISP_HZEX(unsigned char x,unsigned char y,unsigned char *str,unsigned char num,unsigned char flag)
//��������: ��ָ��λ����ʾָ���ĺ����ַ���
//�������: unsigned char x       Ҫ��ʾ��X������ 1 - 8
//          unsigned char y       Ҫ��ʾ��X������ 1 - 4
//          unsigned char num     Ҫ���з�ɫ��ʾ�ĺ�����
//          unsigned char *str    Ҫ��ʾ�ĺ����ַ���
//          unsigned char flag    0 ������ʾ 1 ��ɫ��ʾ
//���ز���: ��
//����ʱ��: 2013-02-05
//�޸ļ�¼:
//   ��¼I:
//         �޸�ʱ��:
//         �޸�����:

void GUI_DISP_HZEX(unsigned char x, unsigned char y, const unsigned char *str, unsigned char num, unsigned char flag)
{
    GUI_DISP_ROM_HZ(x, y, str);
    GUI_LCD_NEGA(x, y, num * 2, flag);
}
//************************************************************
//������Ȩ: CharlieShao
//��������: void GUI_DISP_CHAR(unsigned char x,unsigned char y,unsigned char num,unsigned char nage,unsigned char flag)
//��������: ��ָ��λ����ʾ��λ����
//�������: unsigned char x       Ҫ��ʾ��X������ 1 - 8
//          unsigned char y       Ҫ��ʾ��X������ 1 - 4
//          unsigned int num      Ҫ��ʾ������
//          unsigned char nega    ��ʾ�Ƿ���з�ɫ��ʾ 1����ɫ��ʾ  0��������ʾ
//          unsigned char flag    ��ʾ��λΪ0�Ƿ���ʾ  1��������ʾ  0����������ʾ
//���ز���: ��
//����ʱ��: 2013-02-05
//�޸ļ�¼:
//   ��¼I:
//         �޸�ʱ��:
//         �޸�����:
//************************************************************
void GUI_DISP_CHAR(unsigned char x, unsigned char y, unsigned char num, unsigned char nage, unsigned char flag)
{
    unsigned char str[3]  = {0};			   // Ҫ��ʾ���ַ�����
    unsigned char temp_h  = 0;				   // ʮλ��
    unsigned char temp_l  = 0;				   // ��λ��
    unsigned char index   = 0;				   // ��ŵ�
    unsigned char *temp  = NULL;
    unsigned char *p     = NULL;

    temp = str;
    p    = str;

    temp_h = num / 10;
    temp_l = num % 10;

    if(temp_h == 0)
    {
        if(flag == 1)
        {
            str[index] = 0x30;
            index     += 1;
        }
    }
    else
    {
        str[index] = temp_h + 0x30;
        index     += 1;
    }

    str[index] = temp_l + 0x30;

    //	if(nage == 1)								// ��ʾҪ���з�ɫ��ʾ
    {
        GUI_LCD_NEGA(x, y, 2, nage);
    }

    lcd_wr_cmd(0x30);                           // ���ػ���ָ��
    lcd_set_xy(x, y);						   // ��Ҫ����Һ���ļĴ�����ַ
    while(*temp != 0)						    // �ж�ָ���Ƿ����ַ�����ĩβ
    {
        lcd_wr_dat(*p);				       	    // д��ָ��ָ����ַ�
        p = ++temp;							  // str��ַ��1���ָ��
    }
}

//************************************************************
//������Ȩ: CharlieShao
//��������: void GUI_DISP_INT(unsigned char x,unsigned char y,unsigned int num,unsigned char flag)
//��������: ��ָ��λ����ʾINT�ͱ�����ֵ
//�������: unsigned char x       Ҫ��ʾ��X������ 1 - 8
//          unsigned char y       Ҫ��ʾ��X������ 1 - 4
//          unsigned int num      Ҫ��ʾ������,��ʾ��Χ-32767-+32768
//          unsigned char f       ����Ϊ1������Ϊ0
//          unsigned char flag    ��ʾ�Ƿ���з�ɫ��ʾ 1����ɫ��ʾ  0��������ʾ
//���ز���: ��
//����ʱ��: 2013-02-05
//�޸ļ�¼:
//   ��¼I:
//         �޸�ʱ��:
//         �޸�����:
//************************************************************
void GUI_DISP_INT(unsigned char x, unsigned char y, int num, unsigned char flag)
{
    unsigned char index = 4;		            // ���ڶ�����������
    unsigned char f;                                 //����Ϊ1������Ϊ0
    unsigned char bit_num = 0;			     // int�ͱ�����λ��
    unsigned char str[6] = {0};
    unsigned char *temp = NULL;			     // ����һ��ָ��

    if(num < 0)
    {
        f = 1;
        num = -num;
    }
    while(num / 10 != 0)
    {
        str[index] = num % 10 + 0x30;
        if(index >= 1)							// �����ݴ���
        {
            index--;
        }
        num = num / 10;
        bit_num += 1;
    }
    str[index] = num % 10 + 0x30;
    bit_num += 1;
    temp = &str[index];
    if(f == 1)
    {
        temp = temp - 1;
        *temp = '-';
    }
    GUI_LCD_NEGA(x, y, bit_num + f, flag );						// ��ʾҪ���з�ɫ��ʾ

    lcd_wr_cmd(0x30);                           // ���ػ���ָ��
    lcd_set_xy(x, y);						   // ��Ҫ����Һ���ļĴ�����ַ
    while(*temp != 0)						    // �ж�ָ���Ƿ����ַ�����ĩβ
    {
        lcd_wr_dat(*(temp++));				       	    // д��ָ��ָ����ַ�							  // str��ַ��1���ָ��
    }
}
//************************************************************
//������Ȩ: CharlieShao
//��������: void GUI_DISP_FLOAT(unsigned char x, unsigned char y, float num, unsigned char bitnum, unsigned char flag);//��������ʾ��֧��С�������λ
//��������: ��ָ��λ����ʾINT�ͱ�����ֵ
//�������: unsigned char x       Ҫ��ʾ��X������ 1 - 8
//          unsigned char y       Ҫ��ʾ��X������ 1 - 4
//           float                Ҫ��ʾ������
//           bitnum               ��ʾ��С�����λ�������3λ
//           unsigned char flag    ��ʾ�Ƿ���з�ɫ��ʾ 1����ɫ��ʾ  0��������ʾ
//���ز���: ��
//����ʱ��: 2013-02-05
//�޸ļ�¼:
//   ��¼I:
//         �޸�ʱ��:
//         �޸�����:
//************************************************************
void GUI_DISP_FLOAT(unsigned char x, unsigned char y, float num, unsigned char bit_count, unsigned char flag)//������Χ600���ڣ�С�������λ
{
    unsigned char index = 0;		      // ���ڶ�����������
    unsigned char negative_flag;             //floatΪ����ʱ��1
    unsigned int  int_value;	             // ��������
    unsigned int  bit_value ;	             // С������
    unsigned char int_count = 0;		    // int������λ��
    unsigned char str[9] = {0};              //5λint+С����+2λС��
    unsigned char *temp = NULL;			     // ����һ��ָ��

    if(num < 0)
    {
        num = -num;
        negative_flag = 1;
    }

    int_value = num ;	             // ��������
    bit_value =  (int)(num * 100) % 100;	        // С������


    str[8] = bit_value % 10 + 0x30;
    str[7] = bit_value / 10 % 10 + 0x30;
    str[6] = '.';

    index = 5;
    while(int_value / 10 != 0)
    {
        str[index] = int_value % 10 + 0x30;
        if(index >= 2)							// �����ݴ���
        {
            index--;
        }
        int_value = int_value / 10;
        int_count += 1;
    }

    str[index] = int_value % 10 + 0x30;
    int_count += 1;

    if(negative_flag == 1)
    {
        str[index - 1] = '-';
        temp = &str[index - 1];
    }
    else temp = &str[index];

    GUI_LCD_NEGA(x, y, int_count + 1 + bit_count + negative_flag, flag );						// ��ʾҪ���з�ɫ��ʾ

    lcd_wr_cmd(0x30);                           // ���ػ���ָ��
    lcd_set_xy(x, y);						   // ��Ҫ����Һ���ļĴ�����ַ
    while(*temp != '.')						   // �ж�ָ���Ƿ���������ĩβ
    {
        lcd_wr_dat(*(temp++));				       	    // д��ָ��ָ����ַ�							  // str��ַ��1���ָ��
    }
    lcd_wr_dat('.');
    for(; bit_count > 0; bit_count--)
    {
        lcd_wr_dat(*(++temp));
    }
}
//************************************************************
//������Ȩ: CharlieShao
//��������: void GUI_DISP_888_NUN(unsigned char x,unsigned char y,unsigned int num,unsigned char flag)
//��������: ��ָ��λ����ʾ��λ��
//�������: unsigned char x       Ҫ��ʾ��X������ 1 - 8
//          unsigned char y       Ҫ��ʾ��X������ 1 - 4
//          unsigned int num      Ҫ��ʾ������
//          unsigned char flag    ��ʾ�Ƿ���з�ɫ��ʾ 1����ɫ��ʾ  0��������ʾ
//���ز���: ��
//����ʱ��: 2013-02-05
//�޸ļ�¼:
//   ��¼I:
//         �޸�ʱ��:
//         �޸�����:
//************************************************************
void GUI_DISP_888_NUN(unsigned char x, unsigned char y, unsigned int num, unsigned char flag)
{
    unsigned char str[5] = {0};
    unsigned char   i = 0;

    if(num >= 1000)
    {
        num = 990;
    }
    if(num >= 100)
    {
        str[2] = num % 10 + 0x30;
        str[1] = num / 10 % 10 + 0x30;
        str[0] = num / 100 + 0x30;
    }
    else  if(num < 10)
    {
        str[2] = ' ';
        str[1] = ' ';
        str[0] = num % 10 + 0x30;
    }
    else
    {
        str[2] = ' ';
        str[1] = num % 10 + 0x30;
        str[0] = num / 10 % 10 + 0x30;

    }

    str[4] = 0;
    if(flag == 1)								// ��ʾҪ���з�ɫ��ʾ
    {
        GUI_LCD_NEGA(x, y, 3, flag );
    }
    lcd_wr_cmd(0x30);                           // ���ػ���ָ��
    lcd_set_xy(x, y);						   // ��Ҫ����Һ���ļĴ�����ַ
    while(i != 3)						  // �ж�ָ���Ƿ����ַ�����ĩβ
    {
        lcd_wr_dat(str[i++]);				       	    // д��ָ��ָ����ַ�

    }
}
//************************************************************
//������Ȩ: CharlieShao
//��������: void GUI_DISP_8888_NUN(unsigned char x,unsigned char y,unsigned int num,unsigned char flag)
//��������: ��ָ��λ����ʾ��λ��
//�������: unsigned char x       Ҫ��ʾ��X������ 1 - 8
//          unsigned char y       Ҫ��ʾ��X������ 1 - 4
//          unsigned int num      Ҫ��ʾ������
//          unsigned char flag    ��ʾ�Ƿ���з�ɫ��ʾ 1����ɫ��ʾ  0��������ʾ
//���ز���: ��
//����ʱ��: 2013-02-05
//�޸ļ�¼:
//   ��¼I:
//         �޸�ʱ��:
//         �޸�����:
//************************************************************
void GUI_DISP_8888_NUN(unsigned char x, unsigned char y, unsigned int num, unsigned char flag)
{
    unsigned char str[5] = {0};
    unsigned char   i = 0;

    if(num >= 10000)
    {
        num = 9999;
    }
    if(num >= 1000)
    {
        str[3] = num % 10 + 0x30;      //��
        str[2] = num / 10 % 10 + 0x30; //ʮ
        str[1] = num / 100 % 10 + 0x30; //��
        str[0] = num / 1000 + 0x30;    //ǧ
    }
    else  if((num >= 100) && (num < 1000))
    {
        str[3] = ' ';
        str[2] = num % 10 + 0x30;      //��
        str[1] = num / 10 % 10 + 0x30; //ʮ
        str[0] = num / 100 + 0x30; //��
    }
    else  if((num >= 10) && (num < 100))
    {
        str[3] = ' ';
        str[2] = ' ';
        str[1] = num % 10 + 0x30;
        str[0] = num / 10 + 0x30;
    }
    else  if((num > 0) && (num < 10))
    {
        str[3] = ' ';
        str[2] = ' ';
        str[1] = ' ';
        str[0] = num % 10 + 0x30;

    }
    str[4] = 0;
    //	if(flag == 1)								// ��ʾҪ���з�ɫ��ʾ
    {
        GUI_LCD_NEGA(x, y, 4, flag );
    }
    lcd_wr_cmd(0x30);                           // ���ػ���ָ��
    lcd_set_xy(x, y);						   // ��Ҫ����Һ���ļĴ�����ַ
    while(i != 4)						  // �ж�ָ���Ƿ����ַ�����ĩβ
    {
        lcd_wr_dat(str[i++]);				       	    // д��ָ��ָ����ַ�
    }
}
//************************************************************
//������Ȩ: CharlieShao
//��������: void GUI_DISP_888888_NUN(unsigned char x,unsigned char y,unsigned long num)
//��������: ��ָ��λ����ʾ��λ�޷��ų����εĺ���
//�������: unsigned char x       Ҫ��ʾ��X������ 1 - 8
//          unsigned char y       Ҫ��ʾ��X������ 1 - 4
//          unsigned int num      Ҫ��ʾ������
//���ز���: ��
//����ʱ��: 2013-02-05
//�޸ļ�¼:
//   ��¼I:
//         �޸�ʱ��:
//         �޸�����:
//************************************************************
void GUI_DISP_888888_NUN(unsigned char x, unsigned char y, unsigned long num)
{
    unsigned char str[7] ;
    unsigned char   i = 0;
    unsigned long temp;
    temp = num;
    if(temp >= 1000000)
    {
        temp = 999999;
    }
    for(i = 0; i <= 5; i++)
    {
        str[5 - i] = temp % 10 + 0x30;
        temp = temp / 10;
    }
    lcd_wr_cmd(0x30);                           // ���ػ���ָ��
    lcd_set_xy(x, y);						   // ��Ҫ����Һ���ļĴ�����ַ
    for(i = 0; i <= 5; i++)
    {
        lcd_wr_dat(str[i]);				       	    // д��ָ��ָ����ַ�
    }
}
//************************************************************
//������Ȩ: CharlieShao
//��������: void GUI_DISP_88_NUN(unsigned char x,unsigned char y,unsigned int num,unsigned char flag,unsigned char flag_P)
//��������: ��ָ��λ����ʾ��λ�޷��ų����εĺ���
//�������: unsigned char x       Ҫ��ʾ��X������ 1 - 8
//          unsigned char y       Ҫ��ʾ��Y������ 1 - 4
//          unsigned int num      Ҫ��ʾ������
//          unsigned char flag    ��ʾ�Ƿ���з�ɫ��ʾ 1����ɫ��ʾ  0��������ʾ
//          unsigned char flag_P  ��ʾ�����ķ��ţ���+����������-��������
//���ز���: ��
//����ʱ��: 2013-02-05
//�޸ļ�¼:
//   ��¼I:
//         �޸�ʱ��:
//         �޸�����:
//************************************************************
void GUI_DISP_88_NUN(unsigned char x, unsigned char y, unsigned int num, unsigned char flag, unsigned char flag_P)
{


    unsigned char str[6] = {0};
    unsigned char   i = 0;

    if(num >= 100)
    {
        num = 99;
    }

    if(num < 10)
    {
        str[3] = ' ';
        str[2] = ' ';
        str[1] = num % 10 + 0x30;
        str[0] = flag_P;

    }
    else
    {

        str[3] = ' ';
        str[2] = num % 10 + 0x30;
        str[1] = num / 10 % 10 + 0x30;
        str[0] = flag_P;
    }

    str[4] = 0;

    //	if(flag == 1)								// ��ʾҪ���з�ɫ��ʾ
    {
        GUI_LCD_NEGA(x, y, 3, flag );
    }


    lcd_wr_cmd(0x30);                           // ���ػ���ָ��
    lcd_set_xy(x, y);						   // ��Ҫ����Һ���ļĴ�����ַ
    while(i != 3)						  // �ж�ָ���Ƿ����ַ�����ĩβ
    {
        lcd_wr_dat(str[i++]);				       	    // д��ָ��ָ����ַ�

    }
}
void GUI_DISP_88_NUN_NEW(unsigned char x, unsigned char y, unsigned int num)
{
    unsigned char str[2] = {0};
    unsigned char   i = 0;

    if(num >= 100)
    {
        num = 99;
    }
    if(num < 10)
    {
        str[1] = ' ';
        str[0] = num % 10 + 0x30;
    }
    else
    {
        str[1] = num % 10 + 0x30;
        str[0] = num / 10 % 10 + 0x30;
    }
    lcd_wr_cmd(0x30);                           // ���ػ���ָ��
    lcd_set_xy(x, y);						   // ��Ҫ����Һ���ļĴ�����ַ
    while(i != 2)						  // �ж�ָ���Ƿ����ַ�����ĩβ
    {
        lcd_wr_dat(str[i++]);				       	    // д��ָ��ָ����ַ�

    }
}
//************************************************************
//������Ȩ: CharlieShao
//��������: void GUI_DISP_LONG(unsigned char x,unsigned char y,unsigned int num_h,unsigned int num_l)
//��������: ��ָ��λ����ʾLONG�ͱ�����ֵ
//�������: unsigned char x       Ҫ��ʾ��X������ 1 - 8
//          unsigned char y       Ҫ��ʾ��X������ 1 - 4
//          unsigned int num_h    Ҫ��ʾ�����ֵĸ�λ
//          unsigned int num_l    Ҫ��ʾ�����ֵĵ�λ
//���ز���: ��
//����ʱ��: 2013-02-05
//�޸ļ�¼:
//   ��¼I:
//         �޸�ʱ��:
//         �޸�����:
//************************************************************
void GUI_DISP_LONG(unsigned char x, unsigned char y, unsigned int num_h, unsigned int num_l)
{
    unsigned char index = 0;				   // ���ڶ�����������
    unsigned char bit_num = 0;				   // int�ͱ�����λ��
    unsigned char str[9] = {0};
    unsigned char *p = NULL ;   			    // ����һ��ָ��
    unsigned char *temp = NULL;
    unsigned int temp_h  = 0;
    unsigned int temp_l  = 0;

    temp_h = num_h;
    temp_l = num_l;
    index = 7;

    if(temp_h == 0x0000)
    {
        while(num_l / 10 != 0)
        {
            str[index] = num_l % 10 + 0x30;
            if(index >= 1)							// �����ݴ���
            {
                index--;
            }
            num_l = num_l / 10;
            bit_num += 1;
        }

        str[index] = num_l % 10 + 0x30;
        bit_num += 1;
        if(temp_h != 0x00)
        {
            index--;
        }
    }
    else
    {
        str[index] = num_l % 10 + 0x30;
        index--;
        num_l = num_l / 10;
        str[index] = num_l % 10 + 0x30;
        index--;
        num_l = num_l / 10;
        str[index] = num_l % 10 + 0x30;
        index--;
        num_l = num_l / 10;
        str[index] = num_l % 10 + 0x30;
        index--;

    }

    while(num_h / 10 != 0)
    {
        str[index] = num_h % 10 + 0x30;
        if(index >= 1)							// �����ݴ���
        {
            index--;
        }
        num_h = num_h / 10;
        bit_num += 1;
    }

    if(temp_h != 0x0000)
    {
        str[index] = num_h % 10 + 0x30;
        bit_num += 1;
    }

    temp = &str[index];
    p = &str[index];

    lcd_wr_cmd(0x30);                           // ���ػ���ָ��
    lcd_set_xy(x, y);						   // ��Ҫ����Һ���ļĴ�����ַ
    while(*temp != 0)						    // �ж�ָ���Ƿ����ַ�����ĩβ
    {
        lcd_wr_dat(*p);				       	    // д��ָ��ָ����ַ�
        p = ++temp;							  // str��ַ��1���ָ��
    }
}
//************************************************************
//������Ȩ: CharlieShao
//��������: void GUI_DISP_DEC(unsigned char x,unsigned char y,unsigned int num,unsigned char bits,unsigned char flag)
//�������: unsigned char x       Ҫ��ʾ��X������ 1 - 8
//          unsigned char y       Ҫ��ʾ��X������ 1 - 4
//          unsigned int num      Ҫ��ʾ������
//          unsigned char bits    Ҫ��ʾ��С����λ�� 1 - 5 λ
//          unsigned char flag    ��ʾ�Ƿ���з�ɫ��ʾ 1����ɫ��ʾ  0��������ʾ
//���ز���: ��
//����ʱ��: 2013-02-05
//�޸ļ�¼:
//   ��¼I:
//         �޸�ʱ��:
//         �޸�����:
//************************************************************
void GUI_DISP_DEC(unsigned char x, unsigned char y, unsigned int num, unsigned char bits, unsigned char flag)
{
    unsigned char str[8]  = {0};			// ��ʼ���ַ�����

    unsigned char i       = 0;				// ѭ�����Ʊ���
    unsigned char index = 6;				// ���ڶ�����������
    unsigned char bit_num = 0;				// ��Ч���ֵ���Ŀ
    unsigned char *temp = NULL;			// ����һ��ָ��

    while((num / 10) != 0)
    {
        str[index] = (num % 10) + 0x30;
        num = num / 10;
        bit_num += 1;
        index 	-= 1;
    }

    str[index] = (num % 10) + 0x30;
    bit_num += 1;
    index 	-= 1;

    if(bit_num <= bits)						// ��Ч������Ŀ��ʵ��Ҫ�����
    {
        for(i = bit_num; i < bits; i++)
        {
            str[index] = 0x30;				// ��� 0
            index     -= 1;
            bit_num   += 1;
        }
        str[index] = 0x2E;					// С����ASCIIֵ
        index     -= 1;
        str[index] = 0x30;
        bit_num   += 2;
        temp = &str[index];
    }
    else
    {
        temp = &str[index];
        for(i = bits; i < bit_num; i++)
        {
            str[index] = str[index + 1];
            index     += 1;
        }
        str[index] = 0x2E;
        bit_num   += 1;
    }

    if(flag == 1)								// ��ʾҪ���з�ɫ��ʾ
    {
        GUI_LCD_NEGA(x, y, bit_num, 1);
    }

    lcd_wr_cmd(0x30);                           // ���ػ���ָ��
    lcd_set_xy(x, y);						   // ��Ҫ����Һ���ļĴ�����ַ
    while(*temp != 0)						    // �ж�ָ���Ƿ����ַ�����ĩβ
    {
        lcd_wr_dat(*temp);				       	// д��ָ��ָ����ַ�
        temp++;							    	// str��ַ��1���ָ��
    }
}
//************************************************************
//������Ȩ: CharlieShao
//��������: void GUI_LCD_FAST_CLR(void)
//��������: ������������
//�������: ��
//���ز���: ��
//����ʱ��: 2013-02-05
//�޸ļ�¼:
//   ��¼I:
//         �޸�ʱ��:
//         �޸�����:
//************************************************************
void GUI_LCD_FAST_CLR(void)
{
    unsigned char DEL[]  = "  ";
    unsigned char j = 8;
    unsigned char i = 4;
    while(j)
    {
        i = 4;
        while(i)
        {
            GUI_DISP_HZ(j, i, DEL) ;
            i--;
        }
        j--;
    }

}

