#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
// 1 unit  + 0 [e t]
// 2 units + 2 [i a n m]
// 3 units + 4[s u r w d k g o]
// 4 units + 8[h v f ! l @ p j b x c y z q #  ]
char MORSE_TABLE[] = "etianmsurwdkgohvf!l@pjbxcyzq# ";

/* 
E .     0
T -     1

I ..    00
A .-    01
N -.    10
M --    11

S ...   000
U ..-   001
R .-.   010
W .--   011
D -..   100
K -.-   101
G --.   110
O ---   111

H ....  0000
V ...-  0001
F ..-.  0010
! ..--  0011
L .-..  0100
@ .-.-  0101
P .--.  0110
J .---  0111
B -...  1000
X -..-  1001
C -.-.  1010
Y -.--  1011
Z --..  1100
Q --.-  1101
# ---.  1110
  ----  1111
 */

void setup() {
  lcd.begin(16, 2);
  analogWrite(6, 15);
  lcd.print("Morse Translator");
}

void wait_button_settle(void)
{
  long settle_time = 300;
  long count = 0;
  while (count < settle_time)
    {
      delayMicroseconds(1);
      if (digitalRead(13) == HIGH)
         count = 0;
      count++;
    } 
}
long wait_get_button(void)
{
  while (digitalRead(13) == LOW)
    continue;
  long count = 0;
  while (digitalRead(13) == HIGH)
  {
    delayMicroseconds(1);
    count++;
  }
  return count;
}

//returns true if waited full time
bool soft_wait_on_button(long wait_time)
{
    long count = 0;
    while (digitalRead(13) == LOW && count < wait_time)
    {
      delayMicroseconds(1);
      count++;
    }
    if (count >= wait_time)
      return false;
    return true;
}

void loop() {
  int cPos = 0;
  int i = 0;
  lcd.setCursor(0, 1);
  int next_let = 1;
  long dash_time = 30000;
  
 while (next_let == 1)
 {
   
   int index = 0;
   for (i = 0; i < 4; i++)
   {
     index <<= 1;
     index |= wait_get_button() > dash_time ? 1 : 0;
      wait_button_settle();
      bool button_pressed = soft_wait_on_button((long)100000);
      if (button_pressed)
        continue;
      break;
      
   }

   int offset = 0;
   switch(i) 
   {
       case 0: break;
       case 1: offset = 2; break;
       case 2: offset = 2+4; break;
       case 3: offset = 2+4+8; break;
       default: break;
   }
   lcd.print(MORSE_TABLE[offset + index]);
   cPos++;
   lcd.setCursor(cPos, 1);
   
   if (cPos >= 16)
   {
     for(i=0; i < 16; i++)
     {
       lcd.setCursor(i, 1);
       lcd.print(" ");
     }
     cPos = 0;
   }
}
}

