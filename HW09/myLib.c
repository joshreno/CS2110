#include <string.h>
#include <stdlib.h>
#include "myLib.h"
#include "text.h"

void waitForVblank() {
  while(*SCANLINECOUNTER >= 160);
  while(*SCANLINECOUNTER < 160);
}

void drawImage3(int x, int y, int width, int height, const unsigned short *image) { 
  for(int i = 0; i < height; i++) {
    DMA[3].src = image + width*i;
    //DMA[3].dst = &videoBuffer[OFFSET(x + i, y)];
    DMA[3].dst = videoBuffer + OFFSET(x +i, y, 240);
    DMA[3].cnt = width | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT | DMA_ON;
  }
}

void drawCopyImage(int x, int y, int width, int height, const unsigned short *image) {
  for (int i = 0; i < height; i++) {
    //DMA[3].src = image + width*i;
    DMA[3].src = image + OFFSET(x + i, y, 240);
    DMA[3].dst = videoBuffer + OFFSET(x + i, y, 240);
    DMA[3].cnt = width | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT | DMA_ON;
  }
}

void drawRectangle(int row, int col, int width, int height, unsigned short color)
{
  for(int l = 0; l < height; l++) {
    for(int i = 0; i < width; i++) {
      setPixel(row + l, col + i, color);
    }
  }
}

void setPixel(int row, int col, unsigned short color)
{
  videoBuffer[OFFSET(row, col, 240)] = color;
}

void delay(int n) {
  volatile int x = 0;
  for(int i = 0; i < n*500; i++) {
    x++;
  }
}

void updateHearts(int player, int hp, const unsigned short *image) {
  // player = 1, Thor
  // player = 2, Loki
  // player = 3, Hela or Hulk

  //drawImage3(0, 0, 240, 160, image);
  image = image + 1;
  unsigned short color;
  int ic;
  int ir;
  for (int h = 0; h < hp; h++) {
  	if (player == 1 || player == 4) {
  		ic = 190 + 9*h;
  	} else {
  		ic = 40;
  	}
    ir = 2;
    if (h <= hp) {
    	if (player == 1)
    	{
    		color = BLUE;
    	} else if (player == 2) {
    		color = YELLOW;
    	} else if (player == 3){
    		color = GREEN;
    	}
    } else {
      color = WHITE;
    }
    // set outside of heart
    for (int i = 0; i < 3; i++) {
      setPixel(ir + i, ic, color);
      setPixel(ir + i, ic + 4, color);
    }
    // set middle of heart
    for (int i = 0; i < 4; i++) {
      setPixel(ir + i, ic + 1, color);
      setPixel(ir + i + 1, ic + 2, color);
      setPixel(ir + i, ic + 3, color);
    }
  }
}


void fillScreen3(volatile unsigned short color) {
  DMA[3].src = &color;
  DMA[3].dst = videoBuffer;
  DMA[3].cnt = (240*160) | DMA_SOURCE_FIXED | DMA_DESTINATION_INCREMENT | DMA_ON;
}
