
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "menu.h"
#include "buttons.h"
#include "lcddriver.h"

struct s_menu_item display_menus[] =
{ {&display_menus[1], &display_menus[1], NULL, NULL, {0,0,0},(char*)"REFR"}  ,
  {&display_menus[0], &display_menus[0], NULL, NULL, {0,0,0}, (char*)"DATAS"}};

struct s_menu_item system_menus[] =
{ {&system_menus[1], &system_menus[2], NULL, NULL, {0,0,0}, (char*)"SERIAL"},
  {&system_menus[2], &system_menus[0], NULL, NULL, {0,VIEWED_VALUE,0}, (char*)"VERSION"},
  {&system_menus[0], &system_menus[1], NULL, NULL, {0,0,0}, (char*)"RESET"}};

struct s_menu_item input_menus[] =
{ {&input_menus[1], &input_menus[2], NULL, NULL, {0,0,0}, (char*)"TYPE"},
  {&input_menus[2], &input_menus[0], NULL, NULL, {0,0,0}, (char*)"RANGE"},
  {&input_menus[0], &input_menus[1], NULL, NULL, {0,0,0}, (char*)"CALIB"}};

struct s_menu_item main_menus[] = 
{ {&main_menus[1], &main_menus[3], &input_menus[0], NULL, {0,HAS_SUBMENU,0},(char*)"INPUT"},
  {&main_menus[2], &main_menus[0], NULL, NULL, {0,0,0},(char*)"OUTPUT"},
  {&main_menus[3], &main_menus[1], &display_menus[0], NULL, {0,HAS_SUBMENU,0},(char*)"DISPLAY"},
  {&main_menus[0], &main_menus[2], &system_menus[0], NULL, {0,HAS_SUBMENU,0},(char*)"SYSTEM"}};

uint8_t menu_stack_ptr = 0;
p_menu_item menu_stack[3] = {&main_menus[0], };

int MenuProcess(uint8_t but, s_status* PROGRAM_STATUS)
{ char DPBUFFER[DISPLAY_WIDTH + 1]; int retval = 0;
  char VALBUFFER[DISPLAY_WIDTH + 1];

  if (PROGRAM_STATUS->MUST_REDRAW)
    {
      LCDSendCmd(CLR_DISP);
      if (menu_stack[menu_stack_ptr]->options.MENU_TYPE == HAS_SUBMENU)
      { /* There are submenu. */
        sprintf(DPBUFFER, "<- %s ->", menu_stack[menu_stack_ptr]->title);
      } else
      { /* No submenu perhaps enumerate, or direct value setting. */
        sprintf(DPBUFFER, "<- %s :", menu_stack[menu_stack_ptr]->title);
        sprintf(VALBUFFER, "DT = %i", menu_stack[menu_stack_ptr]->options.DATA_TYPE);
/*        if (menu_stack[menu_stack_ptr]->options.DATA_TYPE == TEXT)
        {
          sprintf(VALBUFFER, "TEXT DATA");
        } else
        if (menu_stack[menu_stack_ptr]->options.DATA_TYPE == ENUM)
        {
          sprintf(VALBUFFER, "ENUM DATA");
        }else
        if (menu_stack[menu_stack_ptr]->options.DATA_TYPE == FLOAT)
        {
          sprintf(VALBUFFER, "FLOAT DATA");
        }
        if (menu_stack[menu_stack_ptr]->options.DATA_TYPE == TINT)
        {
          sprintf(VALBUFFER, "INT DATA");
        }*/
      }
      LCDSendCmd(DD_RAM_ADDR + ((DISPLAY_WIDTH - strlen(DPBUFFER)) / 2));
      LCDSendStr(DPBUFFER);
      if (menu_stack[menu_stack_ptr]->options.MENU_TYPE != HAS_SUBMENU)
      {
        LCDSendCmd(DD_RAM_ADDR2 + ((DISPLAY_WIDTH - strlen(VALBUFFER)) / 2));
        LCDSendStr(VALBUFFER);
      }
      PROGRAM_STATUS->MUST_REDRAW = 0;
    }

  switch (but)
  {
    case BUT_UP_OFF:
      menu_stack[menu_stack_ptr] = menu_stack[menu_stack_ptr]->next_menu;
      PROGRAM_STATUS->MUST_REDRAW = 1;
    break;
    case BUT_DN_OFF:
      menu_stack[menu_stack_ptr] = menu_stack[menu_stack_ptr]->prev_menu;
      PROGRAM_STATUS->MUST_REDRAW = 1;
    break;
        /* Enter the submenu, if there are.*/
    case BUT_OK_OFF:
      /* We are selected the ACTUAL_MENU submenu... */
      if (menu_stack[menu_stack_ptr]->options.MENU_TYPE == HAS_SUBMENU)
      {
        menu_stack[menu_stack_ptr + 1] = menu_stack[menu_stack_ptr]->param1;
        menu_stack_ptr++;
        PROGRAM_STATUS->MUST_REDRAW = 1;
      }
    break;

    case BUT_ES_OFF:
      /* We are selected the higher menu level...  */
      if (menu_stack_ptr)
      {
        menu_stack_ptr--;
        PROGRAM_STATUS->MUST_REDRAW = 1;
      } else
      {/* Back to the main menu screen. */
        retval = 1;
      }
    break;
  }
  return retval;
}