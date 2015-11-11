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

  typedef union {
  struct {
    unsigned MUST_REDRAW:   1;
    unsigned MUST_SAVE:     1;
  };
  struct {
    uint8_t STATUS_BYTE;
  };
} s_menu_edit_status;


typedef void t_GetTextFunction(char*);

struct s_menu_options {
  uint8_t SEC_LEVEL;
  uint8_t MENU_TYPE;
  enum e_data_type DATA_TYPE;
  uint8_t IS_ROLLING;
};

struct s_menu_item {
  p_menu_item next_menu;
  p_menu_item prev_menu;
  void* param1;
  void* param2;
  struct s_menu_options options;
  const char * title;
};

struct s_enum_data {
  int8_t*  address;
  int8_t    size;
};
typedef struct s_enum_data* p_enum_data;

struct s_text_data {
  const char*     address;
  uint8_t   length;
};
typedef struct s_text_data* p_text_data;

struct s_int_data {
  int*  address;
  int   minvalue;
  int   maxvalue;
};
typedef struct s_int_data* p_int_data;

typedef struct s_menu_options* p_menu_options;

struct s_float_data {
  float*  address;
  float   minvalue;
  float   maxvalue;
};
typedef struct s_float_data* p_float_data;

#define WALKING_MENU        1
#define SELECT_MENU         2
#define BACK_THE_MENU       3

extern struct s_menu_item main_menus[];

int MenuProcess(uint8_t but, s_status* PROGRAM_STATUS);
void InitSettingDatas();

#ifdef	__cplusplus
}
#endif

#endif	/* MENU_H */

