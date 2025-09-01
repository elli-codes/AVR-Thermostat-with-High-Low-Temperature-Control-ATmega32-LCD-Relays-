#include <mega32.h>

#include <delay.h>
#include <stdio.h>
#include <alcd.h>

unsigned int adc_data;
// Voltage Reference: AVCC pin
#define ADC_VREF_TYPE ((0<<REFS1) | (1<<REFS0) | (0<<ADLAR))

// ADC interrupt service routine
interrupt [ADC_INT] void adc_isr(void)
{
// Read the AD conversion result
adc_data=ADCW;
}

// Read the AD conversion result
// with noise canceling
unsigned int read_adc(unsigned char adc_input)
{
ADMUX=adc_input | ADC_VREF_TYPE;
// Delay needed for the stabilization of the ADC input voltage
delay_us(10);
#asm
    in   r30,mcucr
    cbr  r30,__sm_mask
    sbr  r30,__se_bit | __sm_adc_noise_red
    out  mcucr,r30
    sleep
    cbr  r30,__se_bit
    out  mcucr,r30
#endasm
return adc_data;
}





int flag_1 = 0;
int flag_2 = 0;
int flag_eb = 0;
int flag0;
int test;      
int t_1 = 0;
int t_2 = 0;
int t_eb = 0;

enum type {state1 ,
           state2 ,
           state3 ,
           sub1_state3,
           sub2_state3}mode;


char data1[17];
char data2[17];
int H ;
int L;
float v;
char str[17];




void main(void)

{
DDRA=(0<<DDA7) | (0<<DDA6) | (0<<DDA5) | (0<<DDA4) | (0<<DDA3) | (0<<DDA2) | (0<<DDA1) | (0<<DDA0);

// State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=T Bit0=T
PORTA=(0<<PORTA7) | (0<<PORTA6) | (0<<PORTA5) | (0<<PORTA4) | (0<<PORTA3) | (0<<PORTA2) | (0<<PORTA1) | (0<<PORTA0);
 // Analog Comparator initialization
// Analog Comparator: Off
// The Analog Comparator's positive input is
// connected to the AIN0 pin
// The Analog Comparator's negative input is
// connected to the AIN1 pin
ACSR=(1<<ACD) | (0<<ACBG) | (0<<ACO) | (0<<ACI) | (0<<ACIE) | (0<<ACIC) | (0<<ACIS1) | (0<<ACIS0);

// ADC initialization
// ADC Clock frequency: 62.500 kHz
// ADC Voltage Reference: AVCC pin
// ADC Auto Trigger Source: ADC Stopped
ADMUX=ADC_VREF_TYPE;
ADCSRA=(1<<ADEN) | (0<<ADSC) | (0<<ADATE) | (0<<ADIF) | (1<<ADIE) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
SFIOR=(0<<ADTS2) | (0<<ADTS1) | (0<<ADTS0);

// SPI initialization
// SPI disabled
SPCR=(0<<SPIE) | (0<<SPE) | (0<<DORD) | (0<<MSTR) | (0<<CPOL) | (0<<CPHA) | (0<<SPR1) | (0<<SPR0);

// TWI initialization
// TWI disabled
TWCR=(0<<TWEA) | (0<<TWSTA) | (0<<TWSTO) | (0<<TWEN) | (0<<TWIE);  
   
DDRD=0x00;  //bottom
PORTD=0xFF; //pull up
DDRC=0XFF;//RELAY


lcd_init(16);

// Global enable interrupts
#asm("sei")
while (1)
      {  
         
        delay_ms(1);
        t_1++;
        t_2++;
        t_eb++;
       
         switch (mode)
         {

            case state1:
                   
                    lcd_gotoxy(0, 0);
                    lcd_putsf("1_Temp");
                    lcd_gotoxy(0, 1);
                    lcd_putsf("2_Set");
                   
                   
                   
           

                if ( flag_1 == 0) {
                 
                  if (PIND.0 == 0){
                        flag_1 = 1;
                        t_1 = 0;
                       
                    }
                }

                else {
                      if (t_1 >29){
                       if (PIND.0 == 0)
                         t_1 = 0;
                         else{
                            mode=state2;
                            lcd_clear();
                            flag_1=0;
                           
                           
                        }
                     
                      }
               
               
                }  
               
               
                if(flag_2==0){
                    if(PIND.1==0){
                        flag_2=1;
                        t_2=0;
                    }
                }
                else{
                    if(t_2>29){
                        if(PIND.1==0)
                            t_2=0;
                        else{
                            mode=state3;
                            lcd_clear();
                            flag_2=0;
                        }
                    }
                }
               
               
               
               
                break;
               
          ////////////////////////////////
          case state2:

            v=read_adc(0);
            v=v*500/1023;
            sprintf(str,"temp=%0.2f \xdf C",v);
            lcd_gotoxy(0,0);// important !!!!!!!!!!!!!!!!
            lcd_puts(str);
           
           
           
           
            lcd_gotoxy(0,1);
            sprintf(data1,"H=%0.2d ",H);
            lcd_puts(data1);

            lcd_gotoxy(8,1);
            sprintf(data2,"L=%0.2d",L);
            lcd_puts(data2);  
           
             if (H != 0 && v > H) { // important => or >=
            PORTC.0 = 1; // High temp
            PORTC.1 = 0;
            }
            else if (L != 0 && v < L) {
            PORTC.0 = 0;
            PORTC.1 = 1; // Low temp
            }
            else {
            PORTC.0 = 0;
            PORTC.1 = 0; // Normal range
            }    
           
                   

             if(flag_eb==0){
                        if(PIND.2==0){
                            flag_eb=1;
                            t_eb=0;
                        }
                    }
                    else{
                        if(t_eb>5){
                            if(PIND.2==0)
                                t_eb=0;
                            else{
                                mode=state1;
                                lcd_clear();
                                flag_eb=0;
                            }
                        }
                    }
                    break;
            ////////////////////////////////////////////
            case state3:
             
            lcd_gotoxy(0,0);
            sprintf(data1 ,"H=%0.2d    ",H );  
            lcd_puts(data1);
            sprintf(data2 ,"L=%0.2d",L );
            lcd_puts(data2);
           
               if ( flag_1 == 0) {
                     
                      if (PIND.0 == 0){
                            flag_1 = 1;
                            t_1 = 0;
                           
                        }
                    }

                    else {
                          if (t_1 >29){
                           if (PIND.0 == 0)
                             t_1 = 0;
                             else{
                                mode=sub1_state3;
                                lcd_clear();
                                flag_1=0;
                                flag0=0;
                               
                            }
                         
                          }
                   
                   
                    }  
                   
                    if(flag_2==0){
                   
                        if(PIND.1==0){
                            flag_2=1;
                            t_2=0;
                        }
                    }
                    else{
                        if(t_2>29){
                            if(PIND.1==0)
                                t_2=0;
                            else{
                                mode=sub2_state3;
                                lcd_clear();
                                flag_2=0;
                                flag0=0;
                            }
                        }
                    }    
           
                   

             if(flag_eb==0){
                        if(PIND.2==0){
                            flag_eb=1;
                            t_eb=0;
                        }
                    }
                    else{
                        if(t_eb>5){
                            if(PIND.2==0)
                                t_eb=0;
                            else{
                                mode=state1;
                                lcd_clear();
                                flag_eb=0;
                                flag0=0;
                            }
                        }
                    }
                    break;
          //////////////////////////////////////////////     
          case sub1_state3:
           
           lcd_gotoxy(0,0);
            sprintf(data1 ,"H=%0.2d    ",H );
            lcd_puts(data1);        
             if ( flag_1 == 0) {
                     
                      if (PIND.0 == 0){
                            flag_1 = 1;
                            t_1 = 0;
                           
                        }
                    }

                    else {
                          if (t_1 >29){
                           if (PIND.0 == 0)
                             t_1 = 0;
                             else{
                                H++;
                                if (H>99)
                                H=99;
                                flag_1=0;
                               
                               
                            }
                         
                          }
                   
                   
                    }
                     if(flag_2==0){
                        if(PIND.1==0){
                            flag_2=1;
                            t_2=0;
                        }
                    }
                    else{
                        if(t_2>29){
                            if(PIND.1==0)
                                t_2=0;
                            else{
                                H--;
                                if(H<0)
                                H=0;
                                flag_2=0;
                            }
                        }
                    }
                    if(flag_eb==0){
                        if(PIND.2==0){
                            flag_eb=1;
                            t_eb=0;
                        }
                    }
                    else{
                        if(t_eb>5){
                            if(PIND.2==0)
                                t_eb=0;
                            else{
                                mode=state3;
                                lcd_clear();
                                flag_eb=0;
                                flag0=1;
                            }
                        }
                    }          
            break ;      
          /////////////////////////////////////////////////////////////  
          case sub2_state3:
           
            lcd_gotoxy(0,0);
            sprintf(data2 ,"L=%0.2d",L );
            lcd_puts(data2);
           
            if ( flag_1 == 0) {
                     
                      if (PIND.0 == 0){
                            flag_1 = 1;
                            t_1 = 0;
                           
                        }
                    }

                    else {
                          if (t_1 >29){
                           if (PIND.0 == 0)
                             t_1 = 0;
                             else{
                                L++;
                                if (L>99)
                                L=99;
                                flag_1=0;
                               
                               
                            }
                         
                          }
                   
                    }
                     
                     if(flag_2==0){
                        if(PIND.1==0){
                            flag_2=1;
                            t_2=0;
                        }
                    }
                    else{
                        if(t_2>29){
                            if(PIND.1==0)
                                t_2=0;
                            else{
                                L--;
                                if(L<0)
                                L=0;
                                flag_2=0;
                               
                            }
                        }
                    }
                   
                    if(flag_eb==0){
                        if(PIND.2==0){
                            flag_eb=1;
                            t_eb=0;
                        }
                    }
                    else{
                        if(t_eb>5){
                            if(PIND.2==0)
                                t_eb=0;
                            else{
                                mode=state3;
                                lcd_clear();
                                flag_eb=0;  
                                flag0=1;
                            }
                        }
                    }  
           
              break;      
               
          } //switch
       
           
               

         
      }//while
}//main
          
