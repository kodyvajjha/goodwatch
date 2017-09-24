#include "lcd.h"

/* Digits look like this, and we index them with 0 being the
   leftmost.
     
     AAAAA
    F     B
    F     B
    F     B
     GGGGG
    E     C
    E     C
    E     C
     DDDDD  dp

 */

//This maps the segments of each digit.
// A,      B,      C,      D,      E,      F,      G,     dp         digit
int map[10][8]={
  {0x0b04, 0x0b40, 0x0b20, 0x0b01, 0x0a10, 0x0a20, 0x0b02, 0x0b10}, //7
  {0x0940, 0x0a04, 0x0a02, 0x0910, 0x0901, 0x0902, 0x0920, 0x0a01}, //6
  {0x0804, 0x0840, 0x0820, 0x0801, 0x0710, 0x0720, 0x0802, 0x0810}, //5
  {0x0640, 0x0704, 0x0702, 0x0610, 0x0601, 0x0602, 0x0620, 0x0701}, //4
  {0x0504, 0x0540, 0x0520, 0x0501, 0x0410, 0x0420, 0x0502, 0x0510}, //3
  {0x0c02, 0x0404, 0x0402, 0x0310, 0x0302, 0x0304, 0x0340, 0x0401}, //2
  {0x0204, 0x0220, 0x0210, 0x0201, 0x0110, 0x0120, 0x0202, 0x0301}, //1
  {0x0040, 0x0104, 0x0102, 0x0010, 0x0001, 0x0002, 0x0020, 0x0201}, //0
};

enum mappos {A=1, B=2, C=4, D=8, E=0x10, F=0x20, G=0x40, DP=0x80};
int numfont[]={
  A|B|C|D|E|F,   //0
  B|C,           //1
  A|B|G|E|D,     //2
  A|B|G|C|D,     //3
  F|G|B|C,       //4
  A|F|G|C|D,     //5
  A|F|G|E|C|D,   //6
  A|B|C,         //7
  A|B|C|D|E|F|G, //8
  A|B|G|F|C|D,   //9
  A|F|B|G|E|C,   //A
  F|E|G|C|D,     //B
  A|F|E|D,       //C
  E|G|C|D|B,     //D
  A|F|E|G|D,     //E
  A|G|F|E        //F
};

#define DRAWPOINT(todraw) lcdm[todraw>>8]|=todraw&0xFF
#define DRAWPOINTB(todraw) lcdbm[todraw>>8]|=todraw&0xFF
void lcd_digit(int pos, int digit){
  int segments=numfont[digit];
  int bit, todraw;
  for(bit=0;bit<8;bit++){
    if(segments&(1<<bit)){
      DRAWPOINT(map[pos][bit]);
      //DRAWPOINTB(map[pos][bit]);
    }
  }
}

//! Draws a decimal number on the screen.
void lcd_number(long num){
  long bcd=0;
  int i;
  for(i=0;i<8;i++){
    bcd|=((num%10)<<(4*i));
    num/=10;
  }
  lcd_hex(bcd);
}

//! Draws hex on the screen.
void lcd_hex(long num){
  /* So in an ideal world, we'd have characters arranged nicely into
     LCDM[] bytes as some development tools do, but in the real world,
     we're pretty much stuck with them as they are physically
     arranged.
     
     This function takes a buffer of eight hex characters and displays
     them on the screen.
   */
  int i;


  for(i=0;i<8;i++)
    lcd_digit(i,(num>>(4*i))&0xf);
  
}


//! Activates the colon.  2 for invert.
int setcolon(int on){
  if(on==2)          //Blink
    lcdm[3]^=0x20;
  else if(on==1)     //On
    lcdm[3]|=0x20;
  else               //Off
    lcdm[3]&=~0x20;  
}

//! Activates the am.  2 for invert.
int setam(int on){
  if(on==2)          //Blink
    lcdm[0]^=0x04;
  else if(on==1)     //On
    lcdm[0]|=0x04;
  else               //Off
    lcdm[0]&=~0x04;  
}

//! Activates the pm.  2 for invert.
int setpm(int on){
  if(on==2)          //Blink
    lcdm[1]^=0x40;
  else if(on==1)     //On
    lcdm[1]|=0x40;
  else               //Off
    lcdm[1]&=~0x40;  
}

//! Activates the mult sign. 2 for invert.
int setmult(int on){
  if(on==2)          //Blink
    lcdm[4]^=0x40;
  else if(on==1)     //On
    lcdm[4]|=0x40;
  else               //Off
    lcdm[4]&=~0x40;  
}

//! Activates the minus sign. 2 for invert.
int setminus(int on){
  if(on==2)          //Blink
    lcdm[6]^=0x04;
  else if(on==1)     //On
    lcdm[6]|=0x04;
  else               //Off
    lcdm[6]&=~0x04;  
}

//! Activates the plus sign. 2 for invert.
int setplus(int on){
  if(on==2)          //Blink
    lcdm[7]^=0x40;
  else if(on==1)     //On
    lcdm[7]|=0x40;
  else               //Off
    lcdm[7]&=~0x40;  
}

//! Activates the divide sign. 2 for invert.
int setdivide(int on){
  if(on==2)          //Blink
    lcdm[0xc]^=0x04;
  else if(on==1)     //On
    lcdm[0xc]|=0x04;
  else               //Off
    lcdm[0xc]&=~0x04;  
}

