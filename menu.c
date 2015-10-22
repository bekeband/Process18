
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "config.h"
#include "menu.h"
#include "buttons.h"
#include "lcddriver.h"

static void GetVersion(char* c)
{
  sprintf(c, "VER %i.%i", VER_H, VER_L);
}

static void GetEnumText(char* c) // int enum_position)
{
  sprintf(c, "ENUM TEXT");
}

int MenuValueFunction(char* c)
{
  sprintf(c, "MENUVALUE");
}

const char* display_datas_enum_array[] = {"PROCESS DATA","RAW DATA","SUMMA 1","SUMMA 2"};
uint8_t dt;
struct s_enum_item display_enum_datas = {&dt, 4, display_datas_enum_array};

struct s_menu_item display_datas_menus[] =
{ {&display_datas_menus[1], &display_datas_menus[1], GetEnumText, &display_enum_datas, {0,0,ENUM},(char*)"TOP LINE"}  ,
  {&display_datas_menus[0], &display_datas_menus[0], GetEnumText, &display_enum_datas, {0,0,ENUM}, (char*)"BOTTOM LINE"}};

struct s_menu_item display_menus[] =
{ {&display_menus[1], &display_menus[1], &MenuValueFunction, NULL, {0,0,FLOAT},(char*)"REFR"}  ,
  {&display_menus[0], &display_menus[0], &display_datas_menus, NULL, {0,HAS_SUBMENU,0}, (char*)"DATAS"}};

struct s_menu_item system_menus[] =
{ {&system_menus[1], &system_menus[2], NULL, NULL, {0,0,0}, (char*)"SERIAL"},
  {&system_menus[2], &system_menus[0], GetVersion, NULL, {0, VIEWED_VALUE, TEXT}, (char*)"VERSION"},
  {&system_menus[0], &system_menus[1], NULL, NULL, {0,0,0}, (char*)"RESET"}};

struct s_menu_item input_menus[] =
{ {&input_menus[1], &input_menus[2], NULL, NULL, {0,0,0}, (char*)"TYPE"},
  {&input_menus[2], &input_menus[0], NULL, NULL, {0,0,0}, (char*)"RANGE"},
  {&input_menus[0], &input_menus[1], NULL, NULL, {0,0,0}, (char*)"CALIB"}};

struct s_menu_item main_menus[] = 
//{ {&main_menus[1], &main_menus[3], &input_menus[0], NULL, {0,HAS_SUBMENU,0},(char*)"INPUT"},
{ {&main_menus[1], &main_menus[3], NULL, NULL, {0,0,0},(char*)"INPUT"},
  {&main_menus[2], &main_menus[0], NULL, NULL, {0,0,0},(char*)"OUTPUT"},
  {&main_menus[3], &main_menus[1], &display_menus[0], NULL, {0,HAS_SUBMENU,0},(char*)"DISPLAY"},
  {&main_menus[0], &main_menus[2], &system_menus[0], NULL, {0,HAS_SUBMENU,0},(char*)"SYSTEM"}};

uint8_t menu_stack_ptr = 0;
p_menu_item menu_stack[3] = {&main_menus[0], };

void PrintMenuValue(char* buffer, t_GetTextFunction gettext)
{
  gettext(buffer);
}

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
        if (menu_stack[menu_stack_ptr]->options.DATA_TYPE == TEXT)
        {
//          sprintf(VALBUFFER, "TEXT DATA");
          PrintMenuValue(VALBUFFER, menu_stack[menu_stack_ptr]->param1);
        } else
        if (menu_stack[menu_stack_ptr]->options.DATA_TYPE == ENUM)
        {
          sprintf(VALBUFFER, "ENUM DATA");
        }else
        if (menu_stack[menu_stack_ptr]->options.DATA_TYPE == FLOAT)
        {
          t_MenuValueFunction* F = menu_stack[menu_stack_ptr]->param1;
          F(VALBUFFER);
//          sprintf(VALBUFFER, "FLOAT DATA");
        }
        if (menu_stack[menu_stack_ptr]->options.DATA_TYPE == TINT)
        {
          sprintf(VALBUFFER, "INT DATA");
        }
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
