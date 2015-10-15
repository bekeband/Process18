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

/*#define MAIN_MENU_ITEMS 4
#define INPUT_MENU_ITEMS 3
#define DISPLAY_MENU_ITEMS 2
#define SYSTEM_MENU_ITEMS 2*/

typedef struct s_menu_item* p_menu_item;

struct s_menu_item {
  p_menu_item next_menu;
  p_menu_item prev_menu;
  p_menu_item sub_menu;
  char * title;
};


#define WALKING_MENU        1
#define SELECT_MENU         2
#define BACK_THE_MENU       3

//#define NO_MENU_SELECT    (int8_t)-1
//#define BACK_MENU_LEVEL   (int8_t)-2

extern struct s_menu_item main_menus[];

p_menu_item MenuProcess(uint8_t but, p_menu_item menuptr, s_status* PROGRAM_STATUS);

#ifdef	__cplusplus
}
#endif

#endif	/* MENU_H */

