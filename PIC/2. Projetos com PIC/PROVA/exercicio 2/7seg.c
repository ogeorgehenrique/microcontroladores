#include <7seg.h>

void main()
{

   while(TRUE)
   {  
      if(input(pin_a1)==0){
         //dezena
         output_b(0b11101110);// numero 0
         //unidade
         delay_ms(100);
         output_b(0b01110111);// numero 5
         delay_ms(100);
      }
      //faz o 15
      if(input(pin_a2)==0){
         //dezena
         output_b(0b00101000);// numero 1
         //unidade
         delay_ms(100);
         output_b(0b01110111);// numero 5
         delay_ms(100);
      }
      //faz o 35
      if(input(pin_a3)==0){
         //dezena
         output_b(0b01101101);// numero 3
         //unidade
         delay_ms(100);
         output_b(0b01110111);// numero 5
         delay_ms(100);
      }
      else{
         //liga os zeros 
         output_b(0b11111110);// numero 0
         delay_ms(10);
         output_b(0b11101110);// numero 0
         delay_ms(10);
      }
   
   
   }

}
