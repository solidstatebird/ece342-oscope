#ifndef __PQ_H
#define __PQ_H

void sinewave(long freq);
void sinewave2(long freq);
int midTrigger();
int vertBoundCheck(int vertin);
void drawIn1(int start);
void drawIn2(int start);
void drawTrigger();
void drawGrid();
double calculateHscale();
void displayHscale(double hdiv);
void displayRunStop(); 
void displayData();
void displayMode();
void updateScreen2();

#endif