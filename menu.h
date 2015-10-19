/* 
 * File:   menu.h
 * Author: user
 *
 * Created on 2015. október 14., 14:17
 */

#ifndef MENU_H
#define	MENU_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "main.h"

typedef struct s_menu_item* p_menu_item;

typedef enum e_data_type {ENUM, TINT, FLOAT, TEXT};
typedef enum e_menu_type {EDITED_VALUE, VIEWED_VALUE, HAS_SUBMENU};

struct s_menu_options {
  uint8_t SEC_LEVEL;
  uint8_t MENU_TYPE;
  uint8_t DATA_TYPE;
};

struct s_menu_item {
  p_menu_item next_menu;
  p_menu_item prev_menu;
  void* param1;
  void* param2;
  struct s_menu_options options;
  char * title;
};


#define WALKING_MENU        1
#define SELECT_MENU         2
#define BACK_THE_MENU       3

//#define NO_MENU_SELECT    (int8_t)-1
//#define BACK_MENU_LEVEL   (int8_t)-2

extern struct s_menu_item main_menus[];

int MenuProcess(uint8_t but, s_status* PROGRAM_STATUS);

#ifdef	__cplusplus
}
#endif

#endif	/* MENU_H */

