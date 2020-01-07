#include <eciggpro.h>



#INT_CCP1
void  CCP1_isr(void) 
{

}
#define LCD_ENABLE_PIN PIN_E2
#define LCD_RS_PIN PIN_E1
#define LCD_RW_PIN PIN_E0
#define LCD_DATA4 PIN_D4
#define LCD_DATA5 PIN_D5
#define LCD_DATA6 PIN_D6
#define LCD_DATA7 PIN_D7

#include <lcd.c>
float coils[7]={0.10,0.15,0.18,0.20,0.25,0.30,0.50};
unsigned int16 battery;
float batterp;
int kont;
unsigned int16 i;
int col=0;
int eep=0;
int ecol;
float voltage;
int set=0;
#INT_EXT
void  EXT_isr(void) 
{ 
  if(battery>168)
  {
  set_pwm1_duty(i);
  output_high(pin_b2);
  while(!input(pin_b0))
  {}
  set_pwm1_duty(0);
  output_low(pin_b2);
  }
  else
  { 
   printf(lcd_putc,"\fBataryaAz");
   delay_ms(1000);
  }
}

void art()
{ if(i<255)
  i++;
}
void azalt()
{ if(i>0)
   i--;
}

void kurulum()
{
  while(input(pin_d2))
  {if(!input(pin_d0))
  {
  col++;
  if(col==7)
  col=0;
  delay_ms(100);
  }
  if(!input(pin_d1))
  {
  col--;
  if(col==-1)
  col=6;
  delay_ms(100);
  }
  printf(lcd_putc,"\fCoil:%fohms",coils[col]);
  delay_ms(100);
  }
  printf(lcd_putc,"\fKaydedildi...");
  delay_ms(250);
  write_eeprom(5,1);

}
void main()
{ 
  
   set_pwm1_duty(0);
   setup_adc_ports(AN0);
   setup_adc(adc_clock_div_32);
   setup_timer_2(T2_DIV_BY_16,255,1);      //2,0 ms overflow, 2,0 ms interrupt
   ext_int_edge(H_to_L);
   setup_ccp1(CCP_PWM|CCP_SHUTDOWN_AC_L|CCP_SHUTDOWN_BD_L);
   i=128;
   i=read_eeprom(6);
   enable_interrupts(INT_EXT);
   enable_interrupts(INT_CCP1);
   enable_interrupts(GLOBAL);
   lcd_init();
    printf(lcd_putc,"\fCK ELECTRONICS");
    delay_ms(300);
   eep=read_eeprom(5);
   if(eep==0)
   {    printf(lcd_putc,"\fKurulum...");
        delay_ms(400);
            kurulum();
   }
   set_adc_channel(0);
   while(TRUE)
   {  battery=read_adc();
      kont=battery-168;
      batterp=(kont*2.173913);
      printf(lcd_putc,"\fKalan:%f",batterp);
      voltage=(i*0.0164705882);
      voltage = voltage * voltage;
      voltage= voltage/coils[col];
      printf(lcd_putc,"\nGuc:%fwatts",voltage);
      if(!input(pin_d0))
      {
        art();
      }
     if(!input(pin_d1))
      {
        azalt();
      }
    ecol=i;
    write_eeprom(6,ecol);
    while(!input(pin_d2))
      {
        set++;
        delay_ms(1000);
       if(set==3)
       {
         write_eeprom(5,0);
         set=0;
         printf(lcd_putc,"\fYenidenBaslatýn...");
       }
      
      }
    delay_ms(150);
   }

}
