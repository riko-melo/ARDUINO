


#include "LedControl.h"
#include "SPI.h"
#include "MaxMatrix.h"
#include "avr/pgmspace.h"
#include<EEPROM.h>
#include<SoftwareSerial.h>

SoftwareSerial BT(11, 10);
String readdata = "";
short block_number;
int high_score;
int delay_start = 700;
int coming_block[16][8];
int arranged_blocks[16][8];
long int timePassed = 0;
long delays = 0;
int delay_ = 700;
long bdelay = 0;
PROGMEM const short buttondelay = 150;
PROGMEM const short btdowndelay = 30;
PROGMEM const short btsidedelay = 80;
int next = 0;
///DISPLAY SETTINGS
PROGMEM const short DATA_PIN = 7;
PROGMEM const short CLK_PIN  = 6;
PROGMEM const short CS_PIN   = 5;
PROGMEM const short DISPLAYS = 4;
short screen_intensity = 3;



//pins alloted to various movements.
PROGMEM const short buttonRotate = A3; //Rotate in either direction.
PROGMEM const short buttonRight = A2; //translate right
PROGMEM const short buttonLeft = A0; //translate left
PROGMEM const short buttonDown = A1; //translate down
PROGMEM const short buttonPlay = A4;
PROGMEM const short buttonPause = A5;
PROGMEM const short buttonReset = A6;

LedControl lc = LedControl(DATA_PIN, CLK_PIN, CS_PIN, DISPLAYS);

void LEDRefresh();
void block_predictor(int next);

short readBut()        
{
  readdata = "";
  while (BT.available())
  {
    delay(1);
    char c = BT.read();
    readdata += c;
  }
  if (analogRead(buttonLeft) < 500 || readdata == "left")
  {
    //left
    bdelay = millis() + btsidedelay;
    return 2;
  }

  if (analogRead(buttonDown) < 500 || readdata == "down")
  {
    //down
    bdelay = millis() + btdowndelay;
    return 4;
  }
  if (analogRead(buttonRight) < 500 || readdata == "right")
  {
    //right
    bdelay = millis() + btsidedelay;
    return 3;
  }
  if (analogRead(buttonRotate) < 500 || readdata == "rotate")
  {
    //rotate
    bdelay = millis() + buttondelay;
    return 1;
  }
  if (analogRead(buttonPlay) < 500 || readdata == "play")
  {
    //rotate
    bdelay = millis() + buttondelay;
    return 5;
  }
  if (analogRead(buttonPause) < 500 || readdata == "pause")
  {
    //rotate
    bdelay = millis() + buttondelay;
    return 6;
  }
  if (analogRead(buttonReset) < 500 || readdata == "reset")
  {
    //rotate
    bdelay = millis() + buttondelay;
    return 7;
  }

  return 0;
}




void set_arrays_zero(int given_array[20][8], int n, int m)
{
  for (int i = 0; i < n; i++)
  {
    for (int j = 0; j < m; j++)
    {
      given_array[i][j] = 0;
    }
  }
  return;
}

void set_arrays_zero12(int given_array[20][12], int n, int m)
{
  for (int i = 0; i < n; i++)
  {
    for (int j = 0; j < m; j++)
    {
      given_array[i][j] = 0;
    }
  }
  return;
}

bool check_space(int temporary_block[20][12])
{
  for (int i = 0; i < 2; i++)
  {
    for (int j = 0; j < 12; j++)
    {
      if (temporary_block[i][j] != 0)
      {
        return false;
      }
    }
  }
  for (int i = 18; i < 20; i++)
  {
    for (int j = 0; j < 12; j++)
    {
      if (temporary_block[i][j] != 0)
      {
        return false;
      }
    }
  }
  for (int i = 0; i < 20; i++)
  {
    for (int j = 0; j < 2; j++)
    {
      if (temporary_block[i][j] != 0)
      {
        return false;
      }
    }
  }
  for (int i = 0; i < 20; i++)
  {
    for (int j = 10; j < 12; j++)
    {
      if (temporary_block[i][j] != 0)
      {
        return false;
      }
    }
  }
  for (int i = 2; i < 18; i++)
  {
    for (int j = 2; j < 10; j++)
    {
      if (arranged_blocks[i - 2][j - 2] != 0 && temporary_block[i][j] != 0)
      {
        return false;
      }
    }
  }
  return true;
}




bool play = 1;
bool pause = 0;
int score = 0;
int block_blueprint[7][5][5] = {0};


void rotate()
{
  Serial.println(F("In Rotate Function"));
  if (block_number == 3)
  {
    return;
  }
  int temp[5][5] = {0};
  bool flag = 0;
  short x, y = 0; //used to find i and j where coming_block[i][j]==3 (center)
  for (int i = 0; i < 16; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      if (coming_block[i][j] == 3)
      {
        x = i;
        y = j;
        flag = 1;
        break;
      }
    }
    if (flag == 1)
    {
      break;
    }
  }

  //to put the matrix in a 5X5 temp matrix
  for (int i = x - 2; i < x + 3; i++)
  {
    for (int j = y - 2; j < y + 3; j++)
    {
      temp[i - x + 2][j - y + 2] = coming_block[i][j];
    }
  }
  for (int i = 0; i <= 2; i++)
  {
    for (int j = i; j < 4 - i; j++)
    {
      int mat = temp[i][j];
      temp[i][j] = temp[j][4 - i];
      temp[j][4 - i] = temp[4 - i][4 - j];
      temp[4 - i][4 - j] = temp[4 - j][i];
      temp[4 - j][i] = mat;
    }
  }
  //new thing
  int temporary_block[20][12] = {0};
  for (int i = x - 2; i < x + 3; i++)
  {
    for (int j = y - 2; j < y + 3; j++)
    {
      temporary_block[i + 2][j + 2] = temp[i - x + 2][j - y + 2];
    }
  }

  bool check = check_space(temporary_block);

  if (check == true)
  {
    for (int i = 0; i < 16; i++)
    {
      for (int j = 0; j < 8; j++)
      {
        coming_block[i][j] = temporary_block[i + 2][j + 2];
      }
    }
  }
  updating_arranged_blocks();
  LEDRefresh();
}

void translate_right()
{
  Serial.println(F("In Translate Right Function"));
  int temporary_block[20][12] = {0};
  for (int i = 0; i < 16; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      temporary_block[i + 2][j + 3] = coming_block[i][j];
    }
  }

  bool check = check_space(temporary_block);

  if (check == true)
  {
    set_arrays_zero(coming_block, 16, 8);
    for (int i = 0; i < 16; i++)
    {
      for (int j = 0; j < 8; j++)
      {
        coming_block[i][j] = temporary_block[i + 2][j + 2];
      }
    }
  }
  updating_arranged_blocks();
  LEDRefresh();
}

void translate_down()
{
  Serial.println(F("In Translate Down Function"));
  int temporary_block[20][12] = {0};
  for (int i = 0; i < 16; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      temporary_block[i + 3][j + 2] = coming_block[i][j];
    }
  }

  bool check = check_space(temporary_block);

  if (check == true)
  {
    set_arrays_zero(coming_block, 16, 8);
    for (int i = 0; i < 16; i++)
    {
      for (int j = 0; j < 8; j++)
      {
        coming_block[i][j] = temporary_block[i + 2][j + 2];
      }
    }
  }
  else
  {
    //Merging arranged blocks with coming blocks because the block can't go further down.
    for (int i = 0; i < 16; i++)
    {
      for (int j = 0; j < 8; j++)
      {
        if (arranged_blocks[i][j] != 0 || coming_block[i][j] != 0)
        {
          arranged_blocks[i][j] = 1;
        }
      }
    }
    set_arrays_zero(coming_block, 16, 8);
    newBlock();
  }
  updating_arranged_blocks();
  LEDRefresh();
}

void translate_left()
{
  Serial.println(F("In Translate Left Function"));
  int temporary_block[20][12] = {0};
  for (int i = 0; i < 16; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      temporary_block[i + 2][j + 1] = coming_block[i][j];
    }
  }

  bool check = check_space(temporary_block);

  if (check == true)
  {
    set_arrays_zero(coming_block, 16, 8);
    for (int i = 0; i < 16; i++)
    {
      for (int j = 0; j < 8; j++)
      {
        coming_block[i][j] = temporary_block[i + 2][j + 2];
      }
    }
  }
  updating_arranged_blocks();
  LEDRefresh();
}

//Handling the new block that will be made available .
void newBlock()
{
  Serial.println(F("New Block"));
  delay(100);
  bool finish_game = is_the_game_over();
  if (finish_game == true)
  {
    play = 1;
    pause = 1;
    return;
  }

  block_number = next;
  next = random(7);
  Serial.println(F("Next:"));
  Serial.println(next);
  block_predictor(next);


  set_arrays_zero(coming_block, 16, 8);
  for (int i = 0; i < 5; i++)
  {
    for (int j = 0; j < 5; j++)
    {
      coming_block[i][j + 2] = block_blueprint[block_number][i][j];
    }
  }
  return;
}

void updating_arranged_blocks()
{
  /*int i;
    int j;
    int number_of_lines_cleared = 0;
    for (i = 15; i >= 0; i--)
    {
    int count = 0;
    for (j = 0; j < 8; j++)
    {
      if (arranged_blocks[i][j]!=0)
      {
        count++;
      }
    }
    if (count == 8)
    {
      num_of_lines_cleared++;
      score++;
      if (score % 5 == 0)
      {
        delay_ = delay_ * 0.9;
      }
      for (j = 0; j < 8; j++)
      {
        arranged_blocks[i][j] = 0;
      }
      delay(50);
      int k;
      for (k = i; k >=0; k--)
      {
        for (j = 0; j < 8; j++)
        {
          arranged_blocks[i][j] = arranged_blocks[i - 1][j];
        }
      }
      for (j = 0; j < 8; j++)
      {
        arranged_blocks[k][j] = 0;
      }
      delay(50);
      i++;
    }
    }*/
  Serial.println(F("In updating arranged blocks function"));
  delay(100);
  int i;
  int j;
  int number_of_lines_cleared = 0;
  for (i = 15; i >= 0; i--)
  {
    int count = 0;
    for (j = 0; j < 8; j++)
    {
      if (arranged_blocks[i][j] != 0)
      {
        count++;
      }
    }

    if (count == 8)
    {
      //clear the row
      number_of_lines_cleared++;
      for (j = 0; j < 4; j++)
      {
        arranged_blocks[i][j] = 0;
        arranged_blocks[i][7 - j] = 0;
        LEDRefresh();
      }
      buzzer1();
      int k;
      for (k = i; k > 0; k--)
      {
        for (j = 0; j < 8; j++)
        {
          arranged_blocks[k][j] = arranged_blocks[k - 1][j];
        }
      }
      for (j = 0; j < 8; j++)
      {
        arranged_blocks[0][j] = 0;
      }
      i++;
      LEDRefresh();
    }
  }
  score = score + number_of_lines_cleared;
  //bonus
  if (number_of_lines_cleared == 2)
  {
    score = score + 1;
  }
  else if (number_of_lines_cleared == 3)
  {
    score = score + 2;
  }
  else if (number_of_lines_cleared == 4)
  {
    score = score + 3;
  }


  if (number_of_lines_cleared)
  {
    if (score > high_score)
    {
      int new_score = score;
      int score1 = new_score / 1000;
      new_score = new_score % 1000;
      int score2 = new_score / 100;
      new_score = new_score % 100;
      int score3 = new_score / 10;
      new_score = new_score % 10;
      int score4 = new_score;
      score1 = score1 * 10 + score2;
      score3 = score3 * 10 + score4;
      EEPROM.write(0, score1);
      EEPROM.write(1, score3);
      high_score = score;
    }
    update_delay();
  }
}


void update_delay()
{
  if (score <= 5)
  {
    delay_ = delay_start;
  }
  else if (score > 5 && score <= 10)
  {
    delay_ = (int) delay_start * 0.9;
  }
  else if (score > 10 && score <= 15)
  {
    delay_ = (int) delay_start * 0.9 * 0.9;
  }
  else if (score > 15 && score <= 20)
  {
    delay_ = (int) delay_start * 0.9 * 0.9 * 0.9;
  }
  else if (score > 20 && score <= 25)
  {
    delay_ = (int) delay_start * 0.9 * 0.9 * 0.9 * 0.9;
  }
  else if (score > 25 && score <= 30)
  {
    delay_ = (int) delay_start * 0.9 * 0.9 * 0.9 * 0.9 * 0.9;
  }
  else if (score > 30 && score <= 35)
  {
    delay_ = (int) delay_start * 0.9 * 0.9 * 0.9 * 0.9 * 0.9 * 0.9;
  }
  else if (score > 35 && score <= 40)
  {
    delay_ = (int) delay_start * 0.9 * 0.9 * 0.9 * 0.9 * 0.9 * 0.9 * 0.9;
  }
  else if (score > 40 && score <= 45)
  {
    delay_ = (int) delay_start * 0.9 * 0.9 * 0.9 * 0.9 * 0.9 * 0.9 * 0.9 * 0.9;
  }

}

bool is_the_game_over()
{
  Serial.println(F("In Is_the_game_over function"));
  updating_arranged_blocks();
  for (int i = 0; i < 8; i++)
  {
    if (arranged_blocks[1][i])
    {
      Serial.println(F("GAME OVER"));
      return true;
    }
  }
  return false;
}


void display_design_on_pause()
{
  /////01234567
  //0//........
  //1//.11..11.
  //2//.11..11.
  //3//........
  //4//........
  //5//1......1
  //6//.1....1.
  //7//..1111..

  dis(3, 1, 1);
  dis(3, 1, 2);
  dis(3, 1, 5);
  dis(3, 1, 6);
  dis(3, 2, 1);
  dis(3, 2, 2);
  dis(3, 2, 5);
  dis(3, 2, 6);
  dis(3, 5, 0);
  dis(3, 5, 7);
  dis(3, 6, 1);
  dis(3, 6, 6);
  dis(3, 7, 2);
  dis(3, 7, 3);
  dis(3, 7, 4);
  dis(3, 7, 5);

}

void dis(int screen, int i, int j)
{
  int k = 7 - i + 8 * j;
  int x = k / 8;
  int y = k % 8;
  lc.setLed(screen, 7 - x, 7 - y, true);
}

void display_on_pause2(int screen , int score)
{
  int score1 = score / 100;
  score = score % 100;
  int score2 = score / 10;
  score = score % 10;
  int score3 = score;

  //first_digit
  for (int i = 0; i < score1; i++)
  {
    dis(screen, 7, i);
  }

  int k = 1;
  int l = 0;
  if (score2 == 0)
  {
    //dis(screen,x,y);
    dis(screen, 0 + k, 0 + l);
    dis(screen, 0 + k, 1 + l);
    dis(screen, 0 + k, 2 + l);
    dis(screen, 1 + k, 0 + l);
    dis(screen, 1 + k, 2 + l);
    dis(screen, 2 + k, 0 + l);
    dis(screen, 2 + k, 2 + l);
    dis(screen, 3 + k, 0 + l);
    dis(screen, 3 + k, 2 + l);
    dis(screen, 4 + k, 0 + l);
    dis(screen, 4 + k, 1 + l);
    dis(screen, 4 + k, 2 + l);
  }
  else if (score2 == 1)
  {
    dis(screen, 0 + k, 2 + l);
    dis(screen, 1 + k, 2 + l);
    dis(screen, 2 + k, 2 + l);
    dis(screen, 3 + k, 2 + l);
    dis(screen, 4 + k, 2 + l);
  }
  else if (score2 == 2)
  {
    dis(screen, 0 + k, 0 + l);
    dis(screen, 0 + k, 1 + l);
    dis(screen, 0 + k, 2 + l);
    dis(screen, 1 + k, 2 + l);
    dis(screen, 2 + k, 0 + l);
    dis(screen, 2 + k, 1 + l);
    dis(screen, 2 + k, 2 + l);
    dis(screen, 3 + k, 0 + l);
    dis(screen, 4 + k, 0 + l);
    dis(screen, 4 + k, 1 + l);
    dis(screen, 4 + k, 2 + l);
  }
  else if (score2 == 3)
  {
    dis(screen, 0 + k, 0 + l);
    dis(screen, 0 + k, 1 + l);
    dis(screen, 0 + k, 2 + l);
    dis(screen, 1 + k, 2 + l);
    dis(screen, 2 + k, 0 + l);
    dis(screen, 2 + k, 1 + l);
    dis(screen, 2 + k, 2 + l);
    dis(screen, 3 + k, 2 + l);
    dis(screen, 4 + k, 0 + l);
    dis(screen, 4 + k, 1 + l);
    dis(screen, 4 + k, 2 + l);
  }
  else if (score2 == 4)
  {
    dis(screen, 0 + k, 0 + l);
    dis(screen, 0 + k, 2 + l);
    dis(screen, 1 + k, 0 + l);
    dis(screen, 1 + k, 2 + l);
    dis(screen, 2 + k, 0 + l);
    dis(screen, 2 + k, 1 + l);
    dis(screen, 2 + k, 2 + l);
    dis(screen, 3 + k, 2 + l);
    dis(screen, 4 + k, 2 + l);
  }
  else if (score2 == 5)
  {
    dis(screen, 0 + k, 0 + l);
    dis(screen, 0 + k, 1 + l);
    dis(screen, 0 + k, 2 + l);
    dis(screen, 1 + k, 0 + l);
    dis(screen, 2 + k, 0 + l);
    dis(screen, 2 + k, 1 + l);
    dis(screen, 2 + k, 2 + l);
    dis(screen, 3 + k, 2 + l);
    dis(screen, 4 + k, 0 + l);
    dis(screen, 4 + k, 1 + l);
    dis(screen, 4 + k, 2 + l);
  }
  else if (score2 == 6)
  {
    dis(screen, 0 + k, 0 + l);
    dis(screen, 0 + k, 0 + l);
    dis(screen, 0 + k, 0 + l);
    dis(screen, 1 + k, 0 + l);
    dis(screen, 2 + k, 0 + l);
    dis(screen, 2 + k, 1 + l);
    dis(screen, 2 + k, 2 + l);
    dis(screen, 3 + k, 0 + l);
    dis(screen, 3 + k, 2 + l);
    dis(screen, 4 + k, 0 + l);
    dis(screen, 4 + k, 1 + l);
    dis(screen, 4 + k, 2 + l);
  }
  else if (score2 == 7)
  {
    dis(screen, 0 + k, 0 + l);
    dis(screen, 0 + k, 0 + l);
    dis(screen, 0 + k, 1 + l);
    dis(screen, 0 + k, 2 + l);
    dis(screen, 1 + k, 2 + l);
    dis(screen, 2 + k, 2 + l);
    dis(screen, 3 + k, 2 + l);
    dis(screen, 4 + k, 2 + l);
  }
  else if (score2 == 8)
  {
    dis(screen, 0 + k, 0 + l);
    dis(screen, 0 + k, 1 + l);
    dis(screen, 0 + k, 2 + l);
    dis(screen, 1 + k, 0 + l);
    dis(screen, 1 + k, 2 + l);
    dis(screen, 2 + k, 0 + l);
    dis(screen, 2 + k, 1 + l);
    dis(screen, 2 + k, 2 + l);
    dis(screen, 3 + k, 0 + l);
    dis(screen, 3 + k, 2 + l);
    dis(screen, 4 + k, 0 + l);
    dis(screen, 4 + k, 1 + l);
    dis(screen, 4 + k, 2 + l);
  }
  else if (score2 == 9)
  {
    dis(screen, 0 + k, 0 + l);
    dis(screen, 0 + k, 1 + l);
    dis(screen, 0 + k, 2 + l);
    dis(screen, 1 + k, 0 + l);
    dis(screen, 1 + k, 2 + l);
    dis(screen, 2 + k, 0 + l);
    dis(screen, 2 + k, 1 + l);
    dis(screen, 2 + k, 2 + l);
    dis(screen, 3 + k, 2 + l);
    dis(screen, 4 + k, 2 + l);
  }

  l = 5;
  if (score3 == 0)
  {
    //dis(screen,x,y);
    dis(screen, 0 + k, 0 + l);
    dis(screen, 0 + k, 1 + l);
    dis(screen, 0 + k, 2 + l);
    dis(screen, 1 + k, 0 + l);
    dis(screen, 1 + k, 2 + l);
    dis(screen, 2 + k, 0 + l);
    dis(screen, 2 + k, 2 + l);
    dis(screen, 3 + k, 0 + l);
    dis(screen, 3 + k, 2 + l);
    dis(screen, 4 + k, 0 + l);
    dis(screen, 4 + k, 1 + l);
    dis(screen, 4 + k, 2 + l);
  }
  else if (score3 == 1)
  {
    dis(screen, 0 + k, 2 + l);
    dis(screen, 1 + k, 2 + l);
    dis(screen, 2 + k, 2 + l);
    dis(screen, 3 + k, 2 + l);
    dis(screen, 4 + k, 2 + l);
  }
  else if (score3 == 2)
  {
    dis(screen, 0 + k, 0 + l);
    dis(screen, 0 + k, 1 + l);
    dis(screen, 0 + k, 2 + l);
    dis(screen, 1 + k, 2 + l);
    dis(screen, 2 + k, 0 + l);
    dis(screen, 2 + k, 1 + l);
    dis(screen, 2 + k, 2 + l);
    dis(screen, 3 + k, 0 + l);
    dis(screen, 4 + k, 0 + l);
    dis(screen, 4 + k, 1 + l);
    dis(screen, 4 + k, 2 + l);
  }
  else if (score3 == 3)
  {
    dis(screen, 0 + k, 0 + l);
    dis(screen, 0 + k, 1 + l);
    dis(screen, 0 + k, 2 + l);
    dis(screen, 1 + k, 2 + l);
    dis(screen, 2 + k, 0 + l);
    dis(screen, 2 + k, 1 + l);
    dis(screen, 2 + k, 2 + l);
    dis(screen, 3 + k, 2 + l);
    dis(screen, 4 + k, 0 + l);
    dis(screen, 4 + k, 1 + l);
    dis(screen, 4 + k, 2 + l);
  }
  else if (score3 == 4)
  {
    dis(screen, 0 + k, 0 + l);
    dis(screen, 0 + k, 2 + l);
    dis(screen, 1 + k, 0 + l);
    dis(screen, 1 + k, 2 + l);
    dis(screen, 2 + k, 0 + l);
    dis(screen, 2 + k, 1 + l);
    dis(screen, 2 + k, 2 + l);
    dis(screen, 3 + k, 2 + l);
    dis(screen, 4 + k, 2 + l);
  }
  else if (score3 == 5)
  {
    dis(screen, 0 + k, 0 + l);
    dis(screen, 0 + k, 1 + l);
    dis(screen, 0 + k, 2 + l);
    dis(screen, 1 + k, 0 + l);
    dis(screen, 2 + k, 0 + l);
    dis(screen, 2 + k, 1 + l);
    dis(screen, 2 + k, 2 + l);
    dis(screen, 3 + k, 2 + l);
    dis(screen, 4 + k, 0 + l);
    dis(screen, 4 + k, 1 + l);
    dis(screen, 4 + k, 2 + l);
  }
  else if (score3 == 6)
  {
    dis(screen, 0 + k, 0 + l);
    dis(screen, 0 + k, 0 + l);
    dis(screen, 0 + k, 0 + l);
    dis(screen, 1 + k, 0 + l);
    dis(screen, 2 + k, 0 + l);
    dis(screen, 2 + k, 1 + l);
    dis(screen, 2 + k, 2 + l);
    dis(screen, 3 + k, 0 + l);
    dis(screen, 3 + k, 2 + l);
    dis(screen, 4 + k, 0 + l);
    dis(screen, 4 + k, 1 + l);
    dis(screen, 4 + k, 2 + l);
  }
  else if (score3 == 7)
  {
    dis(screen, 0 + k, 0 + l);
    dis(screen, 0 + k, 0 + l);
    dis(screen, 0 + k, 1 + l);
    dis(screen, 0 + k, 2 + l);
    dis(screen, 1 + k, 2 + l);
    dis(screen, 2 + k, 2 + l);
    dis(screen, 3 + k, 2 + l);
    dis(screen, 4 + k, 2 + l);
  }
  else if (score3 == 8)
  {
    dis(screen, 0 + k, 0 + l);
    dis(screen, 0 + k, 1 + l);
    dis(screen, 0 + k, 2 + l);
    dis(screen, 1 + k, 0 + l);
    dis(screen, 1 + k, 2 + l);
    dis(screen, 2 + k, 0 + l);
    dis(screen, 2 + k, 1 + l);
    dis(screen, 2 + k, 2 + l);
    dis(screen, 3 + k, 0 + l);
    dis(screen, 3 + k, 2 + l);
    dis(screen, 4 + k, 0 + l);
    dis(screen, 4 + k, 1 + l);
    dis(screen, 4 + k, 2 + l);
  }
  else if (score3 == 9)
  {
    dis(screen, 0 + k, 0 + l);
    dis(screen, 0 + k, 1 + l);
    dis(screen, 0 + k, 2 + l);
    dis(screen, 1 + k, 0 + l);
    dis(screen, 1 + k, 2 + l);
    dis(screen, 2 + k, 0 + l);
    dis(screen, 2 + k, 1 + l);
    dis(screen, 2 + k, 2 + l);
    dis(screen, 3 + k, 2 + l);
    dis(screen, 4 + k, 2 + l);
  }
}


void display_pause_symbol(int screen)
{
  dis(screen, 1, 1);
  dis(screen, 2, 1);
  dis(screen, 3, 1);
  dis(screen, 4, 1);
  dis(screen, 5, 1);
  dis(screen, 6, 1);
  dis(screen, 1, 2);
  dis(screen, 2, 2);
  dis(screen, 3, 2);
  dis(screen, 4, 2);
  dis(screen, 5, 2);
  dis(screen, 6, 2);
  dis(screen, 1, 5);
  dis(screen, 2, 5);
  dis(screen, 3, 5);
  dis(screen, 4, 5);
  dis(screen, 5, 5);
  dis(screen, 6, 5);
  dis(screen, 1, 6);
  dis(screen, 2, 6);
  dis(screen, 3, 6);
  dis(screen, 4, 6);
  dis(screen, 5, 6);
  dis(screen, 6, 6);
}

void display_reset_symbol(int screen)
{
  dis(screen, 1, 1);
  dis(screen, 2, 2);
  dis(screen, 3, 3);
  dis(screen, 4, 4);
  dis(screen, 5, 5);
  dis(screen, 6, 6);
  dis(screen, 1, 6);
  dis(screen, 2, 5);
  dis(screen, 3, 4);
  dis(screen, 4, 3);
  dis(screen, 5, 2);
  dis(screen, 6, 1);
}

void display_play_symbol(int screen)
{
  dis(screen, 1, 2);
  dis(screen, 2, 2);
  dis(screen, 3, 2);
  dis(screen, 4, 2);
  dis(screen, 5, 2);
  dis(screen, 6, 2);
  dis(screen, 7, 2);
  dis(screen, 2, 3);
  dis(screen, 3, 3);
  dis(screen, 4, 3);
  dis(screen, 5, 3);
  dis(screen, 6, 3);
  dis(screen, 3, 4);
  dis(screen, 4, 4);
  dis(screen, 5, 4);
  dis(screen, 4, 5);
}

void ppr(int n = 0)
{
  if (n == 0)
  {
    Serial.println(F("In starting mode"));
    set_arrays_zero(arranged_blocks, 16, 8);
    //set_arrays_zero(coming_block, 16, 8);
    char string[] = "~Tetris~ <press start to play>";
    lc.clearDisplay(0);
    lc.clearDisplay(1);
    lc.clearDisplay(2);
    lc.clearDisplay(3);

    // 0 - Displaying Tetris and press start to play
    // 1 - Blank
    // 2 - High Score
    // 3 - A tetris design

    //3 - Displaying a block
    display_design_on_pause();

    //2 - Displaying High Score
    display_on_pause2(2, high_score);

    // 1 - Displaying Current Score
    //display_on_pause2(1, score);

    //0 - Scrolling Text
    //5 = Play button
    //6 = Pause button
    //7 = Reset Button
    display_play_symbol(0);
    while (true)
    {
      int x = readBut();
      display_design_on_pause();
      delay(20);
      if (x == 5)
      {
        //Play
        play = 0;
        lc.clearDisplay(0);
        lc.clearDisplay(1);
        lc.clearDisplay(2);
        lc.clearDisplay(3);
        block_predictor(next);
        return;
      }
      else if (x == 2)
      {
        //set intensity
        screen_intensity--;
        screen_intensity = max(screen_intensity, 1);
        lc.setIntensity(0, screen_intensity);
        lc.setIntensity(1, screen_intensity);
        lc.setIntensity(2, screen_intensity);
        lc.setIntensity(3, screen_intensity);
      }
      else if (x == 3)
      {
        //set intensity
        screen_intensity++;
        screen_intensity = min(screen_intensity, 15);
        lc.setIntensity(0, screen_intensity);
        lc.setIntensity(1, screen_intensity);
        lc.setIntensity(2, screen_intensity);
        lc.setIntensity(3, screen_intensity);
      }

      //      delay(100);
      //      m.shiftLeft(false, true);
      //      printStringWithShift(string, 100);
    }
  }
  else
  {
    Serial.println(F("PAUSE MODE"));
    char string[] = "~Tetris~ <press start to continue and reset to play again>";
    lc.clearDisplay(0);
    lc.clearDisplay(1);
    lc.clearDisplay(2);
    lc.clearDisplay(3);

    // 0 - Displaying Tetris and press start to play
    // 1 - Blank
    // 2 - High Score
    // 3 - A tetris Block

    //3 - Displaying a block
    display_design_on_pause();

    //2 - Displaying High Score
    display_on_pause2(2, high_score);

    // 1 - Displaying Current Score
    display_on_pause2(1, score);



    //0 - Scrolling Text

    //0 - Scrolling Text
    //5 = Play button
    //6 = Pause button
    //7 = Reset Button
    bool finish_game = is_the_game_over();
    if (finish_game == 1)
    {
      display_reset_symbol(0);
    }
    else
    {
      display_pause_symbol(0);
    }
    while (true)
    {
      int button = readBut();
      display_design_on_pause();
      if (finish_game == 1 && (button == 5 || button == 6 || button == 7))
      {
        //Reset
        set_arrays_zero(arranged_blocks, 16, 8);
        set_arrays_zero(coming_block, 16, 8);
        play = 0;
        pause = 0;
        return;
      }
      if (finish_game == 0 && button == 5)
      {
        //Play

        play = 0;
        lc.clearDisplay(0);
        lc.clearDisplay(1);
        lc.clearDisplay(2);
        lc.clearDisplay(3);
        block_predictor(next);
        return;
      }
      if (finish_game == 0 && button == 7)
      {
        //Reset
        play = 1;
        pause = 0;
        lc.clearDisplay(0);
        lc.clearDisplay(1);
        lc.clearDisplay(2);
        lc.clearDisplay(3);
        block_predictor(next);
        return;
      }
      else if (button == 2)
      {
        //set intensity
        screen_intensity--;
        screen_intensity = max(screen_intensity, 1);
        lc.setIntensity(0, screen_intensity);
        lc.setIntensity(1, screen_intensity);
        lc.setIntensity(2, screen_intensity);
        lc.setIntensity(3, screen_intensity);
      }
      else if (button == 3)
      {
        //set intensity
        screen_intensity++;
        screen_intensity = min(screen_intensity, 15);
        lc.setIntensity(0, screen_intensity);
        lc.setIntensity(1, screen_intensity);
        lc.setIntensity(2, screen_intensity);
        lc.setIntensity(3, screen_intensity);
      }

      //      delay(100);
      //      m.shiftLeft(false, true);
      //      printStringWithShift(string, 100);
    }

  }
}


void game()
{
  Serial.println(F("In game function"));
  timePassed = millis();
  while (true)
  {
    if (play == 1)
    {
      break;
    }
    //sw = analogRead(A0);
    delay(30);
    //Serial.println(sw);
    if (millis() - timePassed > delay_)
    {
      buzzer();
      Serial.println(F("Translating down"));
      translate_down();
      timePassed = millis();
    }

    //buttun actions
    int button = readBut();

    if (button == 1) //up=rotate
    {
      Serial.println(F("-Rotate function is called"));
      rotate();
    }
    if (button == 2)//right=moveright
    {
      Serial.println(F("> Translate right function is called"));
      translate_right();
    }

    if (button == 3) //left=moveleft
    {
      Serial.println(F("< translate left function is called"));
      translate_left();
    }

    if (button == 4) //down=movedown
    {
      Serial.println(F("| translate down function is called"));
      translate_down();
    }

    if (button == 6)
    {
      Serial.println(F("Pause was pressed"));
      play = 1;
      pause = 1;
      return;
    }
    if (button == 7)
    {
      Serial.println(F("Reset was pressed"));
      play = 1;
      pause = 0;
      return;
    }
  }
}




void LEDRefresh()
{

  lc.clearDisplay(0);
  lc.clearDisplay(1);
  Serial.println(F("LedRefresh"));
  //setLed(Addr of display, row, column, true/false)
  for (int i = 0 ; i < 8; i++)
  {
    for (int j = 0 ; j < 8; j++)
    {
      int k = 7 - i + 8 * j;
      int x = k / 8;
      int y = k % 8;
      if (coming_block[i][j] != 0 ||  arranged_blocks[i][j] != 0)
      {
        lc.setLed(1, x, 7 - y, true);
      }
    }
  }

  for (int i = 8; i < 16; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      int k = 7 - (i - 8) + 8 * j;
      int x = k / 8;
      int y = k % 8;
      if (coming_block[i][j] != 0  ||  arranged_blocks[i][j] != 0)
      {

        lc.setLed(0, x, 7 - y, true);
      }
    }
  }
}

void block_predictor(int next)
{
  lc.clearDisplay(3);
  for (int i = 0; i < 5; i++)
  {
    for (int j = 0; j < 5; j++)
    {
      int k = 7 - i + 8 * j;
      int x = k / 8;
      int y = k % 8;
      if (block_blueprint[next][i][j] != 0)
      {
        lc.setLed(3, x, 7 - y, true);
      }
    }
  }
}





//Time elapsed section.
unsigned long startTime;
unsigned long elapsedTime;

//After_basic_complete_start




////After_basic_complete_end
int melody[] =
{
  262, 193, 193, 220, 196, 0, 247, 264
};
int noteDurations[] =
{
  4, 8, 8, 4, 4, 4, 4, 4
};
/*
  ISR(TIMER1_COMPA_vect)
  {
  //change the 0 to 1 for timer1 and 2 for timer2
  buzzer();
  }
*/
void buzzer()
{
  for (int thisNote = 2; thisNote < 5; thisNote++)
  {

    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 200 / noteDurations[thisNote];
    tone(12, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(8);
  }
}

void buzzer1()
{
  for (int thisNote = 0; thisNote < 8; thisNote++)
  {

    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 100 / noteDurations[thisNote];
    tone(12, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(8);
  }
}

void setup() {

  pinMode(buttonRotate, INPUT);
  pinMode(buttonLeft, INPUT);
  pinMode(buttonRight, INPUT);
  pinMode(buttonDown, INPUT);
  pinMode(buttonPlay, INPUT);
  pinMode(buttonPause, INPUT);
  pinMode(buttonReset, INPUT);

  Serial.begin(9600);
  Serial.println(F("TEAM : TETROMINOES (HERE GOES OUR FUNCTIONING)"));

  int seed =
    (analogRead(0) + 1) *
    (analogRead(1) + 1) *
    (analogRead(2) + 1) *
    (analogRead(3) + 1);
  randomSeed(seed);
  random(10, 9610806);
  seed = seed * random(3336, 15679912) + analogRead(random(4)) ;
  randomSeed(seed);
  random(10, 98046);

  //block_blueprint 0
  //..2..
  //..2..
  //..3..
  //..2..
  //.....
  block_blueprint[0][0][2] = 2;
  block_blueprint[0][1][2] = 2;
  block_blueprint[0][2][2] = 3;
  block_blueprint[0][3][2] = 2;

  //block_blueprint 1
  //.....
  //..2..
  //..32.
  //..2..
  //.....
  block_blueprint[1][1][2] = 2;
  block_blueprint[1][2][2] = 3;
  block_blueprint[1][2][3] = 2;
  block_blueprint[1][3][2] = 2;

  //block 2
  //.....
  //.2...
  //.23..
  //..2..
  //.....
  block_blueprint[2][1][1] = 2;
  block_blueprint[2][2][1] = 2;
  block_blueprint[2][2][2] = 3;
  block_blueprint[2][3][2] = 2;

  //block_blueprint 3
  //.....
  //.22..
  //.23..
  //.....
  //.....
  block_blueprint[3][1][1] = 2;
  block_blueprint[3][1][2] = 2;
  block_blueprint[3][2][1] = 2;
  block_blueprint[3][2][2] = 3;

  //block_blueprint 4
  //.....
  //..2..
  //.23..
  //.2...
  //.....
  block_blueprint[4][1][2] = 2;
  block_blueprint[4][2][1] = 2;
  block_blueprint[4][2][2] = 3;
  block_blueprint[4][3][1] = 2;

  //block_blueprint 5
  //.....
  //.22..
  //..3..
  //..2..
  //.....
  block_blueprint[5][1][1] = 2;
  block_blueprint[5][1][2] = 2;
  block_blueprint[5][2][2] = 3;
  block_blueprint[5][3][2] = 2;

  //block_blueprint 6
  //.....
  //..22.
  //..3..
  //..2..
  //.....
  block_blueprint[6][1][2] = 2;
  block_blueprint[6][1][3] = 2;
  block_blueprint[6][2][2] = 3;
  block_blueprint[6][3][2] = 2;


  //Assigning INPUT_PULLUP to all the pins, they will help us to determine whether a particular switch is pressed or not.(https://www.arduino.cc/en/Tutorial/InputPullupSerial)
  //pinMode(buttonRotate, INPUT_PULLUP); // Rotate
  //pinMode(buttonRight, INPUT_PULLUP);  // Right
  //pinMode(buttonLeft, INPUT_PULLUP);   // Left
  //pinMode(buttonDown, INPUT_PULLUP);   // Down
  //pinMode(play_switch, INPUT_PULLUP); // Play
  //pinMode(pause_switch, INPUT_PULLUP); //Pause
  //pinMode(reset_switch, INPUT_PULLUP); //Reset
  //pinMode(sw, INPUT_PULLUP);
  lc.shutdown(0, false);
  lc.shutdown(1, false);
  lc.shutdown(2, false);
  lc.shutdown(3, false);
  lc.setIntensity(0, screen_intensity);
  lc.setIntensity(1, screen_intensity);
  lc.setIntensity(2, screen_intensity);
  lc.setIntensity(3, screen_intensity);
  lc.clearDisplay(0);
  lc.clearDisplay(1);
  lc.clearDisplay(2);
  lc.clearDisplay(3);
  //
  //  EEPROM.write(0,0);
  //  EEPROM.write(1,1);
  // LED Intensity 0-15


  int num1 = EEPROM.read(0);
  int num2 = EEPROM.read(1);
  num1 = num1 * 100 + num2;
  high_score = num1;

  BT.begin(9600);

  next = random(7);
  Serial.println(F("Next:"));
  Serial.println(next);
  newBlock();
}

// Play = 0 -> Play Game
// Play = 1 -> Pause Game
// Pause = 0 -> On play screen in the starting
// Pause = 1 -> On the play screen in while the game is running
// Intital Play -> 1 || Pause -> 0
void loop()
{
  Serial.println(F("In loop function right now"));
  if (play == 0)
  {
    Serial.println(F("Game function is called"));
    delay(1);
    game();

  }
  else
  {
    if (pause == 0)
    {
      Serial.println(F("Initial ppr is called"));
      newBlock();
      ppr();
    }
    else
    {
      Serial.println(F("Pause ppr is called"));
      ppr(1);
    }
  }
}

