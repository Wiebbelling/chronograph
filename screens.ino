void drawCalibratingScreen(int percent)
{ 
  display.clearBuffer();
  
  display.setFont(u8g2_font_ncenR08_tr);  
  display.setFontDirection(0);

  display.setCursor(0,12);
  display.print("Calibrando Sistema");

  display.setCursor(0,30);
  display.print("Aguarde...");

  display.drawBox(5,45, percent*80/100 ,5);

  display.setCursor(95,50);
  display.print(percent);
  display.print(" %");
  
  display.sendBuffer();
}

void drawClearScreen()
{

  display.setFont(u8g2_font_crox3h_tf);  
  display.setFontDirection(0);
  
  display.drawBox(0,10,5,3);
  display.drawBox(10,10,5,3);
  display.drawBox(20,10,5,3);
  display.drawBox(30,10,5,3);
   
  display.setCursor(42, 12);
  display.print("J | ");
  display.print(bbGram,2);
  display.print("g 6mm");
  
  display.setFont(u8g2_font_ncenR18_tr);

  display.drawBox(15,40,8,5);
  display.drawBox(30,40,8,5);
  display.drawBox(45,40,8,5);
  
  display.setCursor(60, 50);
  display.print("FPS");

  display.setFont(u8g2_font_5x7_tr);

  display.setCursor(0,64);
  display.print(bat);
  display.print("%");
}


void drawFpsScreen(int fps, long joule)
{
  if(shotCount > 99)
    shotCount = 1;

  display.setFont(u8g2_font_crox3h_tf);  
  display.setFontDirection(0);
  int jouleP = joule/1000;
  int jouleL = joule%1000;
  char auxBuf[10];
  sprintf(auxBuf, "%1d.%03d",jouleP, jouleL);
  
  display.setCursor(0,12);
  display.print(auxBuf);
   
  display.setCursor(42, 12);
  display.print("J | ");
  display.print(bbGram,2);
  display.print("g 6mm");

  
  display.setFont(u8g2_font_ncenR18_tr);

  display.setCursor(15,50);
  display.print(fps);
  
  display.setCursor(60, 50);
  display.print("FPS");
  
  display.setFont(u8g2_font_crox3h_tf);  
  display.setFontDirection(0);
  
  display.setCursor(110, 64);
  display.print(shotCount);

  display.setFont(u8g2_font_5x7_tr);

  display.setCursor(0,64);
  display.print(bat);
  display.print("%");
}

//Debug codes

//void drawText(char *value)
//{
//  display.clearBuffer();
//  display.setFont(u8g2_font_crox3h_tf);  
//  display.setFontDirection(0);
//  display.setCursor(0,12);
//  display.print(value);
//  display.sendBuffer();
//}
//
//void drawChar(char value)
//{
//  display.clearBuffer();
//  display.setFont(u8g2_font_crox3h_tf);  
//  display.setFontDirection(0);
//  display.setCursor(0,40);
//  display.print("Char lido:");
//  display.print(value);
//  display.sendBuffer();
//}
