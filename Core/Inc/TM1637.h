
#ifndef INC_TM1637_H_
#define INC_TM1637_H_

void tm1637_Init(void);
void tm1637_DisplayDecimal(int v, int displaySeparator);
void tm1637_DisplayClock(int h, int m, int displaySeparator);
void tm1637_DisplayTempDegC(int t);
void tm1637_SetBrightness(char brightness);


#endif /* INC_TM1637_H_ */
