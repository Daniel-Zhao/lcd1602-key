#include <reg52.h>
#include <intrins.h>

#define uchar unsigned char 
#define uint unsigned int
#define GPIO_KEY P1

sbit rs = P2^6;   //定义端口 
sbit rw = P2^5;
sbit en = P2^7;

uchar code a1[] = "123/456%789@*0#=";
void DelayMs(uint xms)
{
	uint x,y;
	for(x=xms;x>0;x--)
		for(y=110;y>0;y--);
}

/*------------------------------------------------
写入命令函数
------------------------------------------------*/
void LCD_Write_Com(uchar com)//写命令	（根据时序图）com即命令，16进制
{
	rs=0; //选择命令
	rw=0; //选择写
	P0=com;//指令送到P0口
	DelayMs(1);//等待数据稳定
	en=1;
	DelayMs(1);
	en=0;
}
/*------------------------------------------------
写入数据函数
------------------------------------------------*/
void LCD_Write_Data(uchar dat)//写数据	（根据时序图）
{
	rs=1; //选择数据
	rw=0; //选择写
	P0=dat;//指令送到P0口
	DelayMs(1);//等待数据稳定
	en=1;
	DelayMs(1);
	en=0;
}
/*------------------------------------------------
清屏函数
------------------------------------------------*/
void LCD_Clear(void) 
{ 
    LCD_Write_Com(0x01); 
    DelayMs(5);
}
/*------------------------------------------------
写入字符串函数
------------------------------------------------*/
void LCD_Write_String(unsigned char x,unsigned char y,unsigned char *s) 
{     
    if (y == 0) 
    {     
        LCD_Write_Com(0x80 + x);     
    }
    else 
    {     
        LCD_Write_Com(0xC0 + x);     
    }        
    while (*s) 
    {     
        LCD_Write_Data( *s);     
        s++;     
    }
}
/*------------------------------------------------
写入字符函数
------------------------------------------------*/
void LCD_Write_Char(unsigned char x,unsigned char Data) 
{          
    LCD_Write_Com(0xC0 + x);          
    LCD_Write_Data( Data);  
}
/*------------------------------------------------
初始化函数
------------------------------------------------*/
void LCD_Init(void) 
{
    LCD_Write_Com(0x38);    /*显示模式设置*/ 
    DelayMs(5); 
    LCD_Write_Com(0x38); 
    DelayMs(5); 
    LCD_Write_Com(0x38); 
    DelayMs(5); 
    LCD_Write_Com(0x38);  
    LCD_Write_Com(0x08);    /*显示关闭*/ 
    LCD_Write_Com(0x01);    /*显示清屏*/ 
    LCD_Write_Com(0x06);    /*显示光标移动设置*/ 
    DelayMs(5); 
    LCD_Write_Com(0x0C);    /*显示开及光标设置*/
}

/*------------------------------------------------
按键扫描函数，返回扫描键值
------------------------------------------------*/
unsigned char KeyScan(void)  //键盘扫描函数，使用行列反转扫描法
{
    unsigned char cord_h,cord_l;//行列值中间变量
    GPIO_KEY=0x0f;            //行线输出全为0
    cord_h=GPIO_KEY&0x0f;     //读入列线值
    if(cord_h!=0x0f)    //先检测有无按键按下
    {                                                                                                              
        DelayMs(10);        //去抖
        if((GPIO_KEY&0x0f)!=0x0f)
        {
            cord_h=GPIO_KEY&0x0f;  //读入列线值
            GPIO_KEY=cord_h|0xf0;  //输出当前列线值
            cord_l=GPIO_KEY&0xf0;  //读入行线值
            
            while((GPIO_KEY&0xf0)!=0xf0);//等待松开并输出
            
            return(cord_h+cord_l);//键盘最后组合码值
        }
    }
		return(0xff);     //返回该值
}

unsigned char KeyPro(void)
{
    switch(KeyScan())
    {
			case 0x7e:return 3;break; //按下相应的键显示相对应的码值
			case 0x7d:return 2;break;
			case 0x7b:return 1;break;
			case 0x77:return 0;break;
			
			case 0xbe:return 7;break;
			case 0xbd:return 6;break;
			case 0xbb:return 5;break;
			case 0xb7:return 4;break;
			
			case 0xde:return 11;break;
			case 0xdd:return 10;break;
			case 0xdb:return 9;break;
			case 0xd7:return 8;break;
			
			case 0xee:return 15;break;
			case 0xed:return 14;break;
			case 0xeb:return 13;break;
			case 0xe7:return 12;break;
			
			default:return 0xff;break;
    }
}

void main(void) 
{ 
    unsigned char i,num; 
    
    LCD_Init(); 
    
    LCD_Write_String(0,0,"the keyvalue is!");
    
    while (1) 
    {  
        num=KeyPro();
        if(num!=0xff)
        {
            if(i==0)//回到第一个字符时清屏
                LCD_Clear();//清屏
						LCD_Write_String(0,0,"the keyvalue is!");
            LCD_Write_Char(0+i,a1[num]);//依次显示输入字符
            i++;
            if(i==16)//如果第一行显示满，转到第二行
            {
                i=0;
            }
        }
    }
}