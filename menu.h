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

#define MAIN_MENU_ITEMS 4
#define INPUT_MENU_ITEMS 3
#define DISPLAY_MENU_ITEMS 2
#define SYSTEM_MENU_ITEMS 2

struct s_menu_item {
  uint8_t level;
  uint8_t pos;
  uint8_t feature;
  char * title;
};

#ifdef	__cplusplus
}
#endif

#endif	/* MENU_H */

