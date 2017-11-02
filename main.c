#include<reg52.h>
#include<intrins.h>
#define uchar unsigned char 
#define uint unsigned int 
#define ulong unsigned long
#define LCD1602_DATAPINS P0 
uchar code table[]="welcome to use!";
uchar code table1[]="shiwu=   .    g"; 
long HX711_Buffer,HX711_Buffer0,Weight_Maopi, Weight_Shiwu;
//1602IO�ڶ���
sbit LCD1602_E=P2^7; //1602ʹ���ź�
sbit LCD1602_RW=P2^5;//1602��дѡ���
sbit LCD1602_RS=P2^6;//1602���� ����ѡ���

//HX711  
sbit DT=P3^7;
sbit SCK=P3^6;	               
void Get_Maopi();
void Get_Weight();
//��ʱ����
void Lcd1602_Delay1ms(uint ms) 
{  
uchar a ;
        while(ms--)
        {
            for(a=0;a<120;a++);
        } 
}
//1602д����  

void LcdWriteCom(uchar com)	  //д������
{
	LCD1602_E = 0;     //ʹ��
	LCD1602_RS = 0;	   //ѡ��������
	LCD1602_RW = 0;	   //ѡ��д��
	
	LCD1602_DATAPINS = com;     //��������
	Lcd1602_Delay1ms(1);		//�ȴ������ȶ�

	LCD1602_E = 1;	          //д��ʱ��
	Lcd1602_Delay1ms(5);	  //����ʱ��
	LCD1602_E = 0;
}
//1602д����
void LcdWriteData(uchar dat)			//д������
{
	LCD1602_E = 0;	//ʹ������
	LCD1602_RS = 1;	//ѡ����������
	LCD1602_RW = 0;	//ѡ��д��

	LCD1602_DATAPINS = dat; //д������
	Lcd1602_Delay1ms(1);

	LCD1602_E = 1;   //д��ʱ��
	Lcd1602_Delay1ms(5);   //����ʱ��
	LCD1602_E = 0;
}
//1602��ʼ��
void init_1602()//��ʼ��
{
uchar y,z;   
	LCD1602_E = 0; 
	LCD1602_RW = 0;
    LcdWriteCom(0x38);  //����ʾ
	LcdWriteCom(0x0c);  //����ʾ����ʾ���
	LcdWriteCom(0x06);  //дһ��ָ���1
	LcdWriteCom(0x01);  //����
    LcdWriteCom(0x80);  //��������ָ�����
         for(y=0;y<15;y++)
         {
         LcdWriteData(table[y]);
         }
  LcdWriteCom(0x80+0x40);
  for(z=0;z<15;z++)
         {
         LcdWriteData(table1[z]);
         }
}



//HX711���� 
void Delayus()
{  
_nop_();
_nop_();
}

//ͨ��A����Ϊ128�ĺ���
ulong getdate()
{
ulong count;
uchar i; 
count=0;
DT=1;
Delayus();
SCK=0;
while(DT); 
        for(i=0;i<24;i++)
        {
                SCK=1;
                count=count<<1;
                SCK=0;
                if(DT)
                count++;
        }
SCK=1;
count=count^0x800000;
SCK=0;
DT=1; 
return(count);
}


void Get_Maopi()
{  
HX711_Buffer0 =getdate();  
} 


void Get_Weight() 
{   
HX711_Buffer =getdate();  
        if(HX711_Buffer >= HX711_Buffer0)  
        {  
        Weight_Shiwu = HX711_Buffer; 
        Weight_Shiwu = Weight_Shiwu - HX711_Buffer0;  
        Weight_Shiwu = (unsigned long)((float)Weight_Shiwu/1530);
        }
   
}
//HX711������
void gsbq()
{
uchar ge,shi,bai,qian;
qian=Weight_Shiwu/1000;
bai=Weight_Shiwu%1000/100; 
shi=Weight_Shiwu%100/10        ;
ge=Weight_Shiwu%10;
 LcdWriteCom(0x80+0x40+7);
LcdWriteData(0x30+qian);
LcdWriteData(0x30+bai);
LcdWriteData('.');
LcdWriteData(0x30+shi);
LcdWriteData(0x30+ge);
}
//������
        void main()
        {
        init_1602();//��ʼ��
        getdate();
        Get_Maopi();
                while(1)
                {
                getdate();
                Get_Weight(); 
                gsbq();
                }
        }