//TESTADO - feito para rodar no na Board PicGenios

#include <16F877A.h>
#device ADC=16

#FUSES NOWDT                    //No Watch Dog Timer
#FUSES NOBROWNOUT               //No brownout reset
#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O

#use delay(crystal=20000000)

#define BOTAO           PIN_B0  // botão pedestre (ativo em 0)


int1 botao_pressionado() {
   if (!input(BOTAO)) {
      delay_ms(20);
      if (!input(BOTAO)) {
         while (!input(BOTAO)) { delay_ms(10); }
         return TRUE;
      }
   }
   return FALSE;
}

void main()
{

   while(TRUE)
   {
       if (botao_pressionado())
       {
         output_low(PIN_B1);
         delay_ms(100);
         output_low(PIN_B2);
         delay_ms(100);
         output_low(PIN_B3);
         delay_ms(100);
         output_low(PIN_B4);
         output_low(PIN_B5);
         delay_ms(100);
         output_low(PIN_B6);
         delay_ms(100);
         output_low(PIN_B7);
         delay_ms(100);
         output_high(PIN_B1);
         delay_ms(3000);
         output_high(PIN_B6);
       }

   }
}
