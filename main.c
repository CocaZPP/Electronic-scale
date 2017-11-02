#include<reg52.h>
#include<intrins.h>
#define uchar unsigned char 
#define uint unsigned int 
#define ulong unsigned long
#define LCD1602_DATAPINS P0 
uchar code table[]="welcome to use!";
uchar code table1[]="shiwu=   .    g"; 
long HX711_Buffer,HX711_Buffer0,Weight_Maopi, Weight_Shiwu;
//1602IO口定义
sbit LCD1602_E=P2^7; //1602使能信号
sbit LCD1602_RW=P2^5;//1602读写选择端
sbit LCD1602_RS=P2^6;//1602数据 命令选择端

//HX711  
sbit DT=P3^7;
sbit SCK=P3^6;	               
void Get_Maopi();
void Get_Weight();
//延时函数
void Lcd1602_Delay1ms(uint ms) 
{  
uchar a ;
        while(ms--)
        {
            for(a=0;a<120;a++);
        } 
}
//1602写命令  

void LcdWriteCom(uchar com)	  //写入命令
{
	LCD1602_E = 0;     //使能
	LCD1602_RS = 0;	   //选择发送命令
	LCD1602_RW = 0;	   //选择写入
	
	LCD1602_DATAPINS = com;     //放入命令
	Lcd1602_Delay1ms(1);		//等待数据稳定

	LCD1602_E = 1;	          //写入时序
	Lcd1602_Delay1ms(5);	  //保持时间
	LCD1602_E = 0;
}
//1602写数据
void LcdWriteData(uchar dat)			//写入数据
{
	LCD1602_E = 0;	//使能清零
	LCD1602_RS = 1;	//选择输入数据
	LCD1602_RW = 0;	//选择写入

	LCD1602_DATAPINS = dat; //写入数据
	Lcd1602_Delay1ms(1);

	LCD1602_E = 1;   //写入时序
	Lcd1602_Delay1ms(5);   //保持时间
	LCD1602_E = 0;
}
//1602初始化
void init_1602()//初始化
{
uchar y,z;   
	LCD1602_E = 0; 
	LCD1602_RW = 0;
    LcdWriteCom(0x38);  //开显示
	LcdWriteCom(0x0c);  //开显示不显示光标
	LcdWriteCom(0x06);  //写一个指针加1
	LcdWriteCom(0x01);  //清屏
    LcdWriteCom(0x80);  //设置数据指针起点
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



//HX711函数 
void Delayus()
{  
_nop_();
_nop_();
}

//通道A增益为128的函数
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
//HX711处理函数
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
//主函数
        void main()
        {
        init_1602();//初始化
        getdate();
        Get_Maopi();
                while(1)
                {
                getdate();
                Get_Weight(); 
                gsbq();
                }
        }