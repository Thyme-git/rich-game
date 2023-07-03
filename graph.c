#include "graph.h"

Pixel_t frame_buf[MAP_WIDTH * MAP_HEIGHT];

void func_clean_frame_buf() {
  for(int i = 0; i < MAP_WIDTH * MAP_HEIGHT; i ++) {
    frame_buf[i].symbol = ' ';
    frame_buf[i].color = COLOR_WHITE; 
  }
}

void func_pixel_set_by_coord(int coord, char symbol, Color_enum color) {
  int x,y;
  if(coord < MAP_WIDTH) { //第一行,0~28号
    y = 0;
    x = coord;
  } else if (coord < MAP_WIDTH + MAP_HEIGHT - 2) {    //右边一列，除去角上两个，29~34
    y = coord - MAP_WIDTH + 1;
    x = MAP_WIDTH - 1;
  } else if (coord < 2 * MAP_WIDTH + MAP_HEIGHT - 2) {    //最下面一行，35~63
    y = MAP_HEIGHT - 1;
    x = MAP_WIDTH - 1 - (coord - MAP_WIDTH - MAP_HEIGHT + 2);
  } else {            //左边一列，除去角上，64~69
    x = 0;
    y =  MAP_HEIGHT - (coord - 2 * MAP_WIDTH - (MAP_HEIGHT - 2)) - 2;
  }

  func_pixel_set_by_2D(x, y, symbol, color);
}

void func_push_frame() {
  for(int i = 0;i < MAP_HEIGHT; i ++) {
    for(int j = 0;j < MAP_WIDTH; j ++) {
      func_push_pixel(frame_buf + (i * MAP_WIDTH) + j);
    }
    printf("\n");
  }
}

void func_pixel_set_by_2D(int x, int y, char symbol, Color_enum color) {
  frame_buf[(y * MAP_WIDTH) + x].symbol = symbol;
  frame_buf[(y * MAP_WIDTH) + x].color = color;
}

void func_push_pixel(Pixel_t *pixel_ptr) {
  switch (pixel_ptr->color) {
    case COLOR_RED:
      printf("\033[0m\033[1;31m%c\033[0m", pixel_ptr->symbol);
      break;
    case COLOR_YELLOW:
      printf("\033[0m\033[1;33m%c\033[0m", pixel_ptr->symbol);
      break;
    case COLOR_BLUE:
      printf("\033[0m\033[1;34m%c\033[0m", pixel_ptr->symbol);
      break;
    case COLOR_GREEN:
      printf("\033[0m\033[1;32m%c\033[0m", pixel_ptr->symbol);
      break;
    default:
      printf("%c", pixel_ptr->symbol);
  }
}

void func_set_printf_color(Color_enum color) {
  switch (color) {
  case COLOR_RED:
      printf("\033[0m\033[1;31m");
      break;
  case COLOR_YELLOW:
      printf("\033[0m\033[1;33m");
      break;
  case COLOR_BLUE:
      printf("\033[0m\033[1;34m");
      break;
  case COLOR_GREEN:
      printf("\033[0m\033[1;32m");
      break;
  default:
      printf("\033[0m");
  }
}

void func_reset_printf_color() {
  printf("\033[0m");
}
