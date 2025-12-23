#include <16F628A.h>

#FUSES NOWDT                    //No Watch Dog Timer
#FUSES NOBROWNOUT               //No brownout reset
#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O

#use delay(crystal=20000000)

#define LED PIN_None
#define DELAY 1000



void main()
{

   //Example blinking LED program
   while(true)
   {
     //Ao manter o botâo precionado o led irá acender, se soltar o botao ele apaga
      if(input(pin_a1)==0){
         output_high(pin_a0);
      }
      if(input(pin_a1)==1){
         output_low(pin_a0);
      }
      
      //mesma logica, mas com menos codigo  
      if(input(pin_a2)==0){
         output_high(pin_a0);
      }else{
         output_low(pin_a0);
      }
    
      
      //Ao precionar o botão o LED irá ficar asceso por 1000ms
      if(input(pin_a3)==0){
         output_high(pin_a0);
         delay_ms(1000);
         output_low(pin_a0);
      }
      
      
   }

}
