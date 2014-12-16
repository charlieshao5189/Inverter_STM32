#include <stdlib.h>
#include<stdio.h>
#include<string.h>
#include <st7920.h>

#define LCD_SID    GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_0)                 // 与LCD相连的控制端口

#define SET_CS()   GPIO_SetBits(GPIOB,GPIO_Pin_1)
#define CLR_CS()   GPIO_ResetBits(GPIOB,GPIO_Pin_1)

#define SET_SID()  GPIO_SetBits(GPIOB,GPIO_Pin_0)
#define CLR_SID()  GPIO_ResetBits(GPIOB,GPIO_Pin_0)

#define SET_CLK()  GPIO_SetBits(GPIOC,GPIO_Pin_5)
#define CLR_CLK()  GPIO_ResetBits(GPIOC,GPIO_Pin_5)


const unsigned char AC[] =	  //基本指令下屏幕中各行列的寄存器地址（显示汉字时）
{
    0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
    0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97,
    0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f,
    0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f,
};


//************************************************************
//函数版权: 河南万和电气股份有限公司
//函数名称: void lcd_send_byte(unsigned char dat)
//函数功能: 向ST7290中写入一字节的数据
//输入参数: uunsigned char dat 要写入的数据
//返回参数: 无
//创建时间: 2013-02-05
//修改记录:
//   记录I:
//         修改时间:
//         修改描述:
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
        CLR_CLK();//保证最后一次操作数据线总是拉低
    }
    CLR_CS();
}
//************************************************************
//函数版权: 河南万和电气股份有限公司
//函数名称: unsigned char receive_byte(void)
//函数功能: 读取ST7290内部数据
//输入参数: 无
//返回参数: 无
//创建时间: 2013-02-05
//修改记录:
//   记录I:
//         修改时间:
//         修改描述:
//************************************************************
unsigned char receive_byte(void)//可靠
{
    unsigned char i = 0, temp_h = 0, temp_l = 0;
    SID_IN();
    CLR_CLK();
    SET_CS();
    for(i = 0; i < 8; i++)
    {
        temp_h = temp_h << 1;
        //			  temp=30;
        //				while(temp--){};//300ns, 最小周期13.88ns
        SET_CLK();
        //				temp=10;
        //				while(temp--){};//100ns, 最小周期13.88ns
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
        //				while(temp--){};//300ns, 最小周期13.88ns
        SET_CLK();
        //				temp=10;
        //				while(temp--){};//100ns, 最小周期13.88ns
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
//函数版权: 河南万和电气股份有限公司
//函数名称: void lcd_wr_cmd(unsigned char cmd)
//函数功能: 向ST7290中写入控制命令
//输入参数: unsigend char cmd 要写入的命令
//返回参数: 无
//创建时间: 2013-02-05
//修改记录:
//   记录I:
//         修改时间:
//         修改描述:
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
//函数版权: 河南万和电气股份有限公司
//函数名称: void lcd_wr_dat(unsigned char dat)
//函数功能: 向ST7290中写入数据
//输入参数: unsigend char md 要写入的数据
//返回参数: 无
//创建时间: 2013-02-05
//修改记录:
//   记录I:
//         修改时间:
//         修改描述:
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
//函数版权: 河南万和电气股份有限公司
//函数名称: void lcd_ch_busy(void)
//函数功能: 读取ST7290的状态
//输入参数: 无
//返回参数: 无
//创建时间: 2013-02-05
//修改记录:
//   记录I:
//         修改时间:
//         修改描述:
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
//函数版权: 河南万和电气股份有限公司
//函数名称: unsigned char lcd_read(void)
//函数功能: 读取ST7290内部数据
//输入参数: 无
//返回参数: 从ST7920中读取到的数据
//创建时间: 2013-02-05
//修改记录:
//   记录I:
//         修改时间:
//         修改描述:
//备注   : 在串行模式下只能读出指令,不能读出SRAM中的数据
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
//函数版权: 河南万和电气股份有限公司
//函数名称: void GUI_LCD_Init(void)
//函数功能: LCD初始化函数
//输入参数: 无
//返回参数: 无
//创建时间: 2013-02-05
//修改记录:
//   记录I:
//         修改时间:
//         修改描述:
//************************************************************
void GUI_LCD_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_SetBits(GPIOB, GPIO_Pin_0 | GPIO_Pin_1);
	
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_SetBits(GPIOC, GPIO_Pin_5);

    lcd_wr_cmd(0x30);                                // 功能设定，基本指令
    lcd_wr_cmd(0x0C);                                // 显示开关，整体显示on，游标on，游标位on
    lcd_wr_cmd(0x01);                                // 清屏，AC归0
    lcd_wr_cmd(0x04);                                // 进入设定点，写入时,游标右移动
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
//函数版权: 河南万和电气股份有限公司
//函数名称: void GUI_LCD_CLR(unsigned char CLR_DAT)
//函数功能: LCD清屏函数
//输入参数: unsigned char CLR_DAT  0xFF:LCD全黑 0x00:LCD全亮
//返回参数: 无
//创建时间: 2013-02-05
//修改记录:
//   记录I:
//         修改时间:
//************************************************************
void GUI_LCD_CLR(unsigned char CLR_DAT)
{
    unsigned char i = 0;
    unsigned char j = 0;

    lcd_wr_cmd(0x34);					      // 进入图形指令,图形模式下的清屏代码
    lcd_wr_cmd(0x36);					      // 进入图形指令

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

    lcd_wr_cmd(0x30);                         // 返回基本指令,文本模式下的清屏代码
    lcd_wr_cmd(0x80);                         // 返回基本指令
    for(i = 0; i < 64; i++)
    {
        lcd_wr_dat(0x20);
    }
}

//************************************************************
//函数版权: 河南万和电气股份有限公司
//函数名称: void GUI_LCD_NEGA(unsigned char x,unsigned char y,unsigned char num,unsigned char flag)
//函数功能: 控制一行中的字节进行反色显示
//输入参数: unsigned char x          行X从1 - 4
//          unsigned char y         列Y从 0 - 15
//          unsigned cahr num       个数从0 - 15 依Y值而定
//          unsigned char flag     0:取消反色显示 1:进行反色显示
//返回参数: 无
//创建时间: 2013-02-05
//修改记录:
//   记录I:
//         修改时间:
//         修改描述:
//************************************************************
void GUI_LCD_NEGA(unsigned char x, unsigned char y, unsigned char num, unsigned char flag)
{
    unsigned char i = 0;	 	  			 	  	  // 循环控制变量
    unsigned char j = 0;                              // 列循环控制变量
    unsigned char temp = 0;					 		  // 中间参数
    unsigned char b_odd = 0;						  // 是否为奇数
    unsigned char write_date = 0;					  // 要写入的数据
    unsigned char base_xpos = 0;                      // 设定基准地址
    unsigned char base_ypos = 0;                      // 设定基准地址

    if(flag == 0)									  // 取消反色显示
    {
        write_date = 0x00;
    }
    else if(flag == 1)								  // 进行反色显示
    {
        write_date = 0xFF;
    }

    b_odd = num % 2;
    num = num / 2;

    if(b_odd == 1)									  // 为奇数
    {
        num += 1;
    }

    temp = num + x;
    if(temp > 16)
    {
        temp = 17 - x;
        num = temp;
    }

    if(y == 1)                                     	  // 第一行
    {
        base_xpos = 0x80;                             // 第一行的起始地址
        base_ypos = 0x80;
    }
    else if(y == 2)
    {
        base_xpos = 0x80;							  // 第二行的起始地址
        base_ypos = 0x90;
    }
    else if((y == 3))
    {
        base_xpos = 0x88;                              // 第三行的起始地址
        base_ypos = 0x80;
    }
    else if(y == 4)
    {
        base_xpos = 0x88;							   // 第四行的起始地址
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
            lcd_wr_cmd(base_ypos + i);                 // 设定GDRAM Y轴坐标
            lcd_wr_cmd(base_xpos + x + j);             // 设定GDRAM X轴坐标
            if(((j + 1) == num) && (b_odd == 1))		 // 为奇数
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
//函数版权: 河南万和电气股份有限公司
//函数名称: void set_xy(unsigned char x,unsigned char y)
//函数功能: 设定显示位置,文本模式下
//输入参数: unsigned char x       行x从1 - 4
//               unsigned char y       列y从1 - 8
//返回参数: 无
//创建时间: 2013-02-05
//修改记录:
//   记录I:
//         修改时间:
//         修改描述:
//************************************************************
void lcd_set_xy(unsigned char x, unsigned char y)  //行x从1-8，列y从1-4
{
    unsigned char addr = 0;

    addr = 8 * y + x - 9; 		  				 //对x,y调整
    lcd_wr_cmd(AC[addr]);					   //查表AC的实际寄存器地址
}

void lcd_set_xy_16(unsigned char x, unsigned char y)  //行x从1-8，列y从1-4
{
    unsigned char addr = 0;

    addr = 8 * y + x - 9; 		  				 //对x,y调整
    lcd_wr_cmd(AC[addr]);					   //查表AC的实际寄存器地址
}
//************************************************************
//函数版权: 河南万和电气股份有限公司
//函数名称: void GUI_DISP_HZ(unsigned char x,unsigned char y,unsigned char *str)
//函数功能: 在指定位置显示指定的汉字字符串
//输入参数: unsigned char x       要显示的X轴坐标 1 - 8
//          unsigned char y       要显示的X轴坐标 1 - 4
//          unsigned char *str    要显示的汉字字符串
//返回参数: 无
//创建时间: 2013-02-05
//修改记录:
//   记录I:
//         修改时间:
//         修改描述:
//************************************************************
void GUI_DISP_HZ(unsigned char x, unsigned char y, unsigned char  *str)
{
    unsigned char *p ;   			 		   //定义一个指针

    p = str;								   //指针指向字符串在M16内存中的地址

    lcd_wr_cmd(0x30);                          //返回基本指令
    lcd_set_xy(x, y);						  //将要放入液晶的寄存器地址
    while(*p != 0)						       //判断指针是否到了字符串的末尾
    {
        lcd_wr_dat(*p);				       	   //写入指针指向的字符
        p = ++str;							 //str地址加1后给指针
    }
}
void GUI_DISP_ROM_HZ(unsigned char x, unsigned char y,  const unsigned char  *str)
{
    //unsigned char *p ;   			 		   //定义一个指针

    //p = str;								   //指针指向字符串在M16内存中的地址

    lcd_wr_cmd(0x30);              //返回基本指令
    lcd_set_xy(x, y);						  //将要放入液晶的寄存器地址
    while(*str != 0)						      //判断指针是否到了字符串的末尾
    {
        lcd_wr_dat(*str);				       	   //写入指针指向的字符
        str = ++str;							 //str地址加1后给指针
    }
}
//************************************************************
//函数版权: 河南万和电气股份有限公司
//函数名称: void GUI_DISP_HZEX(unsigned char x,unsigned char y,unsigned char *str,unsigned char num,unsigned char flag)
//函数功能: 在指定位置显示指定的汉字字符串
//输入参数: unsigned char x       要显示的X轴坐标 1 - 8
//          unsigned char y       要显示的X轴坐标 1 - 4
//          unsigned char num     要进行反色显示的汉字数
//          unsigned char *str    要显示的汉字字符串
//          unsigned char flag    0 正常显示 1 反色显示
//返回参数: 无
//创建时间: 2013-02-05
//修改记录:
//   记录I:
//         修改时间:
//         修改描述:

void GUI_DISP_HZEX(unsigned char x, unsigned char y, const unsigned char *str, unsigned char num, unsigned char flag)
{
    GUI_DISP_ROM_HZ(x, y, str);
    GUI_LCD_NEGA(x, y, num * 2, flag);
}
//************************************************************
//函数版权: 河南万和电气股份有限公司
//函数名称: void GUI_DISP_CHAR(unsigned char x,unsigned char y,unsigned char num,unsigned char nage,unsigned char flag)
//函数功能: 在指定位置显示两位数字
//输入参数: unsigned char x       要显示的X轴坐标 1 - 8
//          unsigned char y       要显示的X轴坐标 1 - 4
//          unsigned int num      要显示的数字
//          unsigned char nega    表示是否进行反色显示 1：反色显示  0：正常显示
//          unsigned char flag    表示高位为0是否显示  1：进行显示  0：不进行显示
//返回参数: 无
//创建时间: 2013-02-05
//修改记录:
//   记录I:
//         修改时间:
//         修改描述:
//************************************************************
void GUI_DISP_CHAR(unsigned char x, unsigned char y, unsigned char num, unsigned char nage, unsigned char flag)
{
    unsigned char str[3]  = {0};			   // 要显示的字符数组
    unsigned char temp_h  = 0;				   // 十位数
    unsigned char temp_l  = 0;				   // 个位数
    unsigned char index   = 0;				   // 存放的
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

    //	if(nage == 1)								// 表示要进行反色显示
    {
        GUI_LCD_NEGA(x, y, 2, nage);
    }

    lcd_wr_cmd(0x30);                           // 返回基本指令
    lcd_set_xy(x, y);						   // 将要放入液晶的寄存器地址
    while(*temp != 0)						    // 判断指针是否到了字符串的末尾
    {
        lcd_wr_dat(*p);				       	    // 写入指针指向的字符
        p = ++temp;							  // str地址加1后给指针
    }
}

//************************************************************
//函数版权: 河南万和电气股份有限公司
//函数名称: void GUI_DISP_INT(unsigned char x,unsigned char y,unsigned int num,unsigned char flag)
//函数功能: 在指定位置显示INT型变量的值
//输入参数: unsigned char x       要显示的X轴坐标 1 - 8
//          unsigned char y       要显示的X轴坐标 1 - 4
//          unsigned int num      要显示的数字,表示范围-32767-+32768
//          unsigned char f       负数为1，整数为0
//          unsigned char flag    表示是否进行反色显示 1：反色显示  0：正常显示
//返回参数: 无
//创建时间: 2013-02-05
//修改记录:
//   记录I:
//         修改时间:
//         修改描述:
//************************************************************
void GUI_DISP_INT(unsigned char x, unsigned char y, int num, unsigned char flag)
{
    unsigned char index = 4;		            // 用于对数组进行填充
    unsigned char f;                                 //负数为1；整数为0
    unsigned char bit_num = 0;			     // int型变量的位数
    unsigned char str[6] = {0};
    unsigned char *temp = NULL;			     // 定义一个指针

    if(num < 0)
    {
        f = 1;
        num = -num;
    }
    while(num / 10 != 0)
    {
        str[index] = num % 10 + 0x30;
        if(index >= 1)							// 进行容错处理
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
    GUI_LCD_NEGA(x, y, bit_num + f, flag );						// 表示要进行反色显示

    lcd_wr_cmd(0x30);                           // 返回基本指令
    lcd_set_xy(x, y);						   // 将要放入液晶的寄存器地址
    while(*temp != 0)						    // 判断指针是否到了字符串的末尾
    {
        lcd_wr_dat(*(temp++));				       	    // 写入指针指向的字符							  // str地址加1后给指针
    }
}
//************************************************************
//函数版权: 河南万和电气股份有限公司
//函数名称: void GUI_DISP_FLOAT(unsigned char x, unsigned char y, float num, unsigned char bitnum, unsigned char flag);//浮点数显示，支持小数点后三位
//函数功能: 在指定位置显示INT型变量的值
//输入参数: unsigned char x       要显示的X轴坐标 1 - 8
//          unsigned char y       要显示的X轴坐标 1 - 4
//           float                要显示的数字
//           bitnum               显示的小数点的位数，最大3位
//           unsigned char flag    表示是否进行反色显示 1：反色显示  0：正常显示
//返回参数: 无
//创建时间: 2013-02-05
//修改记录:
//   记录I:
//         修改时间:
//         修改描述:
//************************************************************
void GUI_DISP_FLOAT(unsigned char x, unsigned char y, float num, unsigned char bit_count, unsigned char flag)//整数范围600以内，小数点后两位
{
    unsigned char index = 0;		      // 用于对数组进行填充
    unsigned char negative_flag;             //float为负数时置1
    unsigned int  int_value;	             // 整数部分
    unsigned int  bit_value ;	             // 小数部分
    unsigned char int_count = 0;		    // int变量的位数
    unsigned char str[9] = {0};              //5位int+小数点+2位小数
    unsigned char *temp = NULL;			     // 定义一个指针

    if(num < 0)
    {
        num = -num;
        negative_flag = 1;
    }

    int_value = num ;	             // 整数部分
    bit_value =  (int)(num * 100) % 100;	        // 小数部分


    str[8] = bit_value % 10 + 0x30;
    str[7] = bit_value / 10 % 10 + 0x30;
    str[6] = '.';

    index = 5;
    while(int_value / 10 != 0)
    {
        str[index] = int_value % 10 + 0x30;
        if(index >= 2)							// 进行容错处理
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

    GUI_LCD_NEGA(x, y, int_count + 1 + bit_count + negative_flag, flag );						// 表示要进行反色显示

    lcd_wr_cmd(0x30);                           // 返回基本指令
    lcd_set_xy(x, y);						   // 将要放入液晶的寄存器地址
    while(*temp != '.')						   // 判断指针是否到了整数的末尾
    {
        lcd_wr_dat(*(temp++));				       	    // 写入指针指向的字符							  // str地址加1后给指针
    }
    lcd_wr_dat('.');
    for(; bit_count > 0; bit_count--)
    {
        lcd_wr_dat(*(++temp));
    }
}
//************************************************************
//函数版权: 河南万和电气股份有限公司
//函数名称: void GUI_DISP_888_NUN(unsigned char x,unsigned char y,unsigned int num,unsigned char flag)
//函数功能: 在指定位置显示三位数
//输入参数: unsigned char x       要显示的X轴坐标 1 - 8
//          unsigned char y       要显示的X轴坐标 1 - 4
//          unsigned int num      要显示的数字
//          unsigned char flag    表示是否进行反色显示 1：反色显示  0：正常显示
//返回参数: 无
//创建时间: 2013-02-05
//修改记录:
//   记录I:
//         修改时间:
//         修改描述:
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
    if(flag == 1)								// 表示要进行反色显示
    {
        GUI_LCD_NEGA(x, y, 3, flag );
    }
    lcd_wr_cmd(0x30);                           // 返回基本指令
    lcd_set_xy(x, y);						   // 将要放入液晶的寄存器地址
    while(i != 3)						  // 判断指针是否到了字符串的末尾
    {
        lcd_wr_dat(str[i++]);				       	    // 写入指针指向的字符

    }
}
//************************************************************
//函数版权: 河南万和电气股份有限公司
//函数名称: void GUI_DISP_8888_NUN(unsigned char x,unsigned char y,unsigned int num,unsigned char flag)
//函数功能: 在指定位置显示四位数
//输入参数: unsigned char x       要显示的X轴坐标 1 - 8
//          unsigned char y       要显示的X轴坐标 1 - 4
//          unsigned int num      要显示的数字
//          unsigned char flag    表示是否进行反色显示 1：反色显示  0：正常显示
//返回参数: 无
//创建时间: 2013-02-05
//修改记录:
//   记录I:
//         修改时间:
//         修改描述:
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
        str[3] = num % 10 + 0x30;      //个
        str[2] = num / 10 % 10 + 0x30; //十
        str[1] = num / 100 % 10 + 0x30; //百
        str[0] = num / 1000 + 0x30;    //千
    }
    else  if((num >= 100) && (num < 1000))
    {
        str[3] = ' ';
        str[2] = num % 10 + 0x30;      //个
        str[1] = num / 10 % 10 + 0x30; //十
        str[0] = num / 100 + 0x30; //百
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
    //	if(flag == 1)								// 表示要进行反色显示
    {
        GUI_LCD_NEGA(x, y, 4, flag );
    }
    lcd_wr_cmd(0x30);                           // 返回基本指令
    lcd_set_xy(x, y);						   // 将要放入液晶的寄存器地址
    while(i != 4)						  // 判断指针是否到了字符串的末尾
    {
        lcd_wr_dat(str[i++]);				       	    // 写入指针指向的字符
    }
}
//************************************************************
//函数版权: 河南万和电气股份有限公司
//函数名称: void GUI_DISP_888888_NUN(unsigned char x,unsigned char y,unsigned long num)
//函数功能: 在指定位置显示六位无符号长整形的函数
//输入参数: unsigned char x       要显示的X轴坐标 1 - 8
//          unsigned char y       要显示的X轴坐标 1 - 4
//          unsigned int num      要显示的数字
//返回参数: 无
//创建时间: 2013-02-05
//修改记录:
//   记录I:
//         修改时间:
//         修改描述:
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
    lcd_wr_cmd(0x30);                           // 返回基本指令
    lcd_set_xy(x, y);						   // 将要放入液晶的寄存器地址
    for(i = 0; i <= 5; i++)
    {
        lcd_wr_dat(str[i]);				       	    // 写入指针指向的字符
    }
}
//************************************************************
//函数版权: 河南万和电气股份有限公司
//函数名称: void GUI_DISP_88_NUN(unsigned char x,unsigned char y,unsigned int num,unsigned char flag,unsigned char flag_P)
//函数功能: 在指定位置显示六位无符号长整形的函数
//输入参数: unsigned char x       要显示的X轴坐标 1 - 8
//          unsigned char y       要显示的Y轴坐标 1 - 4
//          unsigned int num      要显示的数字
//          unsigned char flag    表示是否进行反色显示 1：反色显示  0：正常显示
//          unsigned char flag_P  表示此数的符号，“+”：正，“-”：负号
//返回参数: 无
//创建时间: 2013-02-05
//修改记录:
//   记录I:
//         修改时间:
//         修改描述:
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

    //	if(flag == 1)								// 表示要进行反色显示
    {
        GUI_LCD_NEGA(x, y, 3, flag );
    }


    lcd_wr_cmd(0x30);                           // 返回基本指令
    lcd_set_xy(x, y);						   // 将要放入液晶的寄存器地址
    while(i != 3)						  // 判断指针是否到了字符串的末尾
    {
        lcd_wr_dat(str[i++]);				       	    // 写入指针指向的字符

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
    lcd_wr_cmd(0x30);                           // 返回基本指令
    lcd_set_xy(x, y);						   // 将要放入液晶的寄存器地址
    while(i != 2)						  // 判断指针是否到了字符串的末尾
    {
        lcd_wr_dat(str[i++]);				       	    // 写入指针指向的字符

    }
}
//************************************************************
//函数版权: 河南万和电气股份有限公司
//函数名称: void GUI_DISP_LONG(unsigned char x,unsigned char y,unsigned int num_h,unsigned int num_l)
//函数功能: 在指定位置显示LONG型变量的值
//输入参数: unsigned char x       要显示的X轴坐标 1 - 8
//          unsigned char y       要显示的X轴坐标 1 - 4
//          unsigned int num_h    要显示的数字的高位
//          unsigned int num_l    要显示的数字的低位
//返回参数: 无
//创建时间: 2013-02-05
//修改记录:
//   记录I:
//         修改时间:
//         修改描述:
//************************************************************
void GUI_DISP_LONG(unsigned char x, unsigned char y, unsigned int num_h, unsigned int num_l)
{
    unsigned char index = 0;				   // 用于对数组进行填充
    unsigned char bit_num = 0;				   // int型变量的位数
    unsigned char str[9] = {0};
    unsigned char *p = NULL ;   			    // 定义一个指针
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
            if(index >= 1)							// 进行容错处理
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
        if(index >= 1)							// 进行容错处理
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

    lcd_wr_cmd(0x30);                           // 返回基本指令
    lcd_set_xy(x, y);						   // 将要放入液晶的寄存器地址
    while(*temp != 0)						    // 判断指针是否到了字符串的末尾
    {
        lcd_wr_dat(*p);				       	    // 写入指针指向的字符
        p = ++temp;							  // str地址加1后给指针
    }
}
//************************************************************
//函数版权: 河南万和电气股份有限公司
//函数名称: void GUI_DISP_DEC(unsigned char x,unsigned char y,unsigned int num,unsigned char bits,unsigned char flag)
//输入参数: unsigned char x       要显示的X轴坐标 1 - 8
//          unsigned char y       要显示的X轴坐标 1 - 4
//          unsigned int num      要显示的数字
//          unsigned char bits    要显示的小数的位数 1 - 5 位
//          unsigned char flag    表示是否进行反色显示 1：反色显示  0：正常显示
//返回参数: 无
//创建时间: 2013-02-05
//修改记录:
//   记录I:
//         修改时间:
//         修改描述:
//************************************************************
void GUI_DISP_DEC(unsigned char x, unsigned char y, unsigned int num, unsigned char bits, unsigned char flag)
{
    unsigned char str[8]  = {0};			// 初始化字符数组

    unsigned char i       = 0;				// 循环控制变量
    unsigned char index = 6;				// 用于对数组进行填充
    unsigned char bit_num = 0;				// 有效数字的数目
    unsigned char *temp = NULL;			// 定义一个指针

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

    if(bit_num <= bits)						// 有效数字数目比实际要求的少
    {
        for(i = bit_num; i < bits; i++)
        {
            str[index] = 0x30;				// 填充 0
            index     -= 1;
            bit_num   += 1;
        }
        str[index] = 0x2E;					// 小数点ASCII值
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

    if(flag == 1)								// 表示要进行反色显示
    {
        GUI_LCD_NEGA(x, y, bit_num, 1);
    }

    lcd_wr_cmd(0x30);                           // 返回基本指令
    lcd_set_xy(x, y);						   // 将要放入液晶的寄存器地址
    while(*temp != 0)						    // 判断指针是否到了字符串的末尾
    {
        lcd_wr_dat(*temp);				       	// 写入指针指向的字符
        temp++;							    	// str地址加1后给指针
    }
}
//************************************************************
//函数版权: 河南万和电气股份有限公司
//函数名称: void GUI_LCD_FAST_CLR(void)
//函数功能: 快速清屏函数
//输入参数: 无
//返回参数: 无
//创建时间: 2013-02-05
//修改记录:
//   记录I:
//         修改时间:
//         修改描述:
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

