#include <reg52.h>
#include <intrins.h>

#define uchar unsigned char 
#define uint unsigned int
#define GPIO_KEY P1

sbit rs = P2^6;   //����˿� 
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
д�������
------------------------------------------------*/
void LCD_Write_Com(uchar com)//д����	������ʱ��ͼ��com�����16����
{
	rs=0; //ѡ������
	rw=0; //ѡ��д
	P0=com;//ָ���͵�P0��
	DelayMs(1);//�ȴ������ȶ�
	en=1;
	DelayMs(1);
	en=0;
}
/*------------------------------------------------
д�����ݺ���
------------------------------------------------*/
void LCD_Write_Data(uchar dat)//д����	������ʱ��ͼ��
{
	rs=1; //ѡ������
	rw=0; //ѡ��д
	P0=dat;//ָ���͵�P0��
	DelayMs(1);//�ȴ������ȶ�
	en=1;
	DelayMs(1);
	en=0;
}
/*------------------------------------------------
��������
------------------------------------------------*/
void LCD_Clear(void) 
{ 
    LCD_Write_Com(0x01); 
    DelayMs(5);
}
/*------------------------------------------------
д���ַ�������
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
д���ַ�����
------------------------------------------------*/
void LCD_Write_Char(unsigned char x,unsigned char Data) 
{          
    LCD_Write_Com(0xC0 + x);          
    LCD_Write_Data( Data);  
}
/*------------------------------------------------
��ʼ������
------------------------------------------------*/
void LCD_Init(void) 
{
    LCD_Write_Com(0x38);    /*��ʾģʽ����*/ 
    DelayMs(5); 
    LCD_Write_Com(0x38); 
    DelayMs(5); 
    LCD_Write_Com(0x38); 
    DelayMs(5); 
    LCD_Write_Com(0x38);  
    LCD_Write_Com(0x08);    /*��ʾ�ر�*/ 
    LCD_Write_Com(0x01);    /*��ʾ����*/ 
    LCD_Write_Com(0x06);    /*��ʾ����ƶ�����*/ 
    DelayMs(5); 
    LCD_Write_Com(0x0C);    /*��ʾ�����������*/
}

/*------------------------------------------------
����ɨ�躯��������ɨ���ֵ
------------------------------------------------*/
unsigned char KeyScan(void)  //����ɨ�躯����ʹ�����з�תɨ�跨
{
    unsigned char cord_h,cord_l;//����ֵ�м����
    GPIO_KEY=0x0f;            //�������ȫΪ0
    cord_h=GPIO_KEY&0x0f;     //��������ֵ
    if(cord_h!=0x0f)    //�ȼ�����ް�������
    {                                                                                                              
        DelayMs(10);        //ȥ��
        if((GPIO_KEY&0x0f)!=0x0f)
        {
            cord_h=GPIO_KEY&0x0f;  //��������ֵ
            GPIO_KEY=cord_h|0xf0;  //�����ǰ����ֵ
            cord_l=GPIO_KEY&0xf0;  //��������ֵ
            
            while((GPIO_KEY&0xf0)!=0xf0);//�ȴ��ɿ������
            
            return(cord_h+cord_l);//������������ֵ
        }
    }
		return(0xff);     //���ظ�ֵ
}

unsigned char KeyPro(void)
{
    switch(KeyScan())
    {
			case 0x7e:return 3;break; //������Ӧ�ļ���ʾ���Ӧ����ֵ
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
            if(i==0)//�ص���һ���ַ�ʱ����
                LCD_Clear();//����
						LCD_Write_String(0,0,"the keyvalue is!");
            LCD_Write_Char(0+i,a1[num]);//������ʾ�����ַ�
            i++;
            if(i==16)//�����һ����ʾ����ת���ڶ���
            {
                i=0;
            }
        }
    }
}