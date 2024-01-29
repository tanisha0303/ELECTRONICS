#include<reg51.h>
//keypad start

//rows
sbit r1=P2^0; 
sbit r2=P2^1;
sbit r3=P2^2;
sbit r4=P2^3;
//columns
sbit c1=P2^4; 
sbit c2=P2^5;
sbit c3=P2^6;
sbit c4=P2^7

char keypad_lut[4][3]={1,2,3,'A',4,5,6,'B',7,8,9,'C','*',0,'#','D'};

bit colscan()
{
return(c1&c2&c3&c4);
}

unsigned char keyscan()
{
unsigned char rowval,colval;
c1=c2=c3=c4=1;
r1=r2=r3=r4=0;
while(colscan());
r1=0;
r2=r3=r4=1;
if(!colscan())
{
rowval=1;
goto colcheck;
}
r2=0;
r1=r3=r4=1;
if(!colscan())
{
rowval=2;
goto colcheck;
}
r3=0;
r1=r2=r4=1;
if(!colscan())
{
rowval=3;
goto colcheck;
}
r4=0;
r1=r2=r3=1;
if(!colscan())
{
rowval=4;
}

colcheck:
if(c1==0)
colval=1;
else if(c2==0)
colval=2;
else if(c3==0)
colval=3;
else if(c4==0)
colval=4;
while(!(c1&&c2&&c3&&c4));
return keypad_lut[rowval][colval];
}

//keypad over

//lcd start

#define lcd_data P1

sbit rs=P3^0;
sbit rw=P3^1;
sbit en=P3^2;

//delay fuction
void delay_ms(unsigned int n)
{
int i,j;
for(i=0;i<n;i++)
for(j=0;j<124;j++);
}

//write lcd
void write_lcd(unsigned char dat)
{
lcd_data=dat;
rw=0;
en=1;
delay_ms(2);
en=0;
}
//command lcd
void cmd_lcd(unsigned char cmd)
{
rs=0;
write_lcd(cmd);
}
//display lcd
void disp_lcd(unsigned char c)
{
rs=1;
write_lcd(c);
}
//iniciate lcd
void init_lcd(void)
{
cmd_lcd(0x02);
cmd_lcd(0x38);
cmd_lcd(0x01);
cmd_lcd(0x0c);
cmd_lcd(0x06);
cmd_lcd(0x80);
//delay_ms(2);
}
//display mirror on lcd
void str_lcd(unsigned char *s)
{
while(*s)
disp_lcd(*s++);
}

//lcd over

//houses set 1
sbit LED1=P0^0;
//houses set 2
sbit LED2=P0^1;


//main fuction
void main()
{

unsigned char i,read_pwd[5],count1,count2,j=0,k=0;
unsigned char load1[5]="11111",load2[5]="22222";
LED1=0;LED2=0;
init_lcd();
str_lcd("PASS CKT BREAKER");
delay_ms(1000);
while(1)
{
cmd_lcd(0x80);
str_lcd(" Enter password ");
cmd_lcd(0xc0);
str_lcd(" ");
count1=count2=0;
cmd_lcd(0xc0);
for(i=0;i<5;i++)
{
read_pwd[i]= 0x30+keyscan();
disp_lcd('*');
//delay_ms(10);
} 
//str_lcd(read_pwd);
for(i=0;i<5;i++)
{
if(read_pwd[i]==load1[i])
count1++;
if(read_pwd[i]==load2[i])
count2++;
}
cmd_lcd(0x80);

if(count1==5)
{
j++;
if(j%2==0)
str_lcd("1st ckt opened");
else
{
str_lcd("1st ckt closed"); 
LED1=~LED1; 
}
}
else if(count2==5)
{
k++;
if(k%2==0)
str_lcd("2nd ckt opened");
else
{
str_lcd("2nd ckt closed"); 
LED2=~LED2;
}
}
else
{
str_lcd("not authorized");
}
delay_ms(5000);

}
}
