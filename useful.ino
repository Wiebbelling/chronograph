void sendReading()
{
    bt.println(buffer);
}

void readBaterry()
{
  bat = analogRead(A0);
  bat = map(bat, 0, 1024, 0, 420);
  bat = map(bat, 330, 420, 0, 100);
}

void readSetup()
{
  
  int count = 0;
  
  while(bt.available() > 0)
  {
    if(count < sizeof(buffer))
    {
      buffer[count] = bt.read(); 
      count++;
    }
    delay(10);
  }

  bbGram = atoi(&buffer[3])/100.0;

  display.clearBuffer();
  drawClearScreen();
  display.sendBuffer();

}

long fps2ms(int fps)
{
  long ms = (fps * 0.3048) ;
  return (ms);
}


long calculateJoule(int fps)
{
  long ms = fps2ms(fps);
  long ms2 = ms*ms;
  long joule =  ( bbGram * ms*ms ) / 2;
  return (joule);
}
