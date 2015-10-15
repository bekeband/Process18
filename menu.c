
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "menu.h"
#include "buttons.h"
#include "lcddriver.h"

p_menu_item menu_stack = NULL;

struct s_menu_item display_menus[] =
  {{ &display_menus[1], &display_menus[1], NULL, (char*)"REFR"},{&display_menus[0], &display_menus[0], NULL, (char*)"DATAS"}};

struct s_menu_item system_menus[] =
  {{&system_menus[1], &system_menus[1], NULL, (char*)"SERIAL"},{&system_menus[0], &system_menus[0], NULL, (char*)"RESET"}};

struct s_menu_item input_menus[] =
{{&input_menus[1], &input_menus[2], NULL, (char*)"TYPE"},{&input_menus[2], &input_menus[0], NULL, (char*)"RANGE"},
 {&input_menus[0], &input_menus[1], NULL, (char*)"CALIB"}};

struct s_menu_item main_menus[] = 
  {{&main_menus[1], &main_menus[3], &input_menus[0], (char*)"INPUT"},  {&main_menus[2], &main_menus[0], NULL, (char*)"OUTPUT"},
  { &main_menus[3], &main_menus[1], &display_menus, (char*)"DISPLAY"},{&main_menus[0], &main_menus[2], &system_menus[0], (char*)"SYSTEM"}};


p_menu_item MenuProcess(uint8_t but, p_menu_item menuptr, s_status* PROGRAM_STATUS)
{ char DPBUFFER[DISPLAY_WIDTH + 1]; p_menu_item retval = menuptr;

  if (PROGRAM_STATUS->MUST_REDRAW)
    {
      LCDSendCmd(CLR_DISP);
      if (menu_stack)
      {
        sprintf(DPBUFFER, "%s - %s", menu_stack->title, menuptr->title);
      } else
      {
        sprintf(DPBUFFER, "%s", menuptr->title);
      }
      LCDSendCmd(DD_RAM_ADDR + ((DISPLAY_WIDTH - strlen(DPBUFFER)) / 2));
      LCDSendStr(DPBUFFER);
      PROGRAM_STATUS->MUST_REDRAW = 0;
    }

  switch (but)
  {
    case BUT_UP_OFF:
      retval = menuptr->next_menu;
      PROGRAM_STATUS->MUST_REDRAW = 1;
    break;
    case BUT_DN_OFF:
      retval = menuptr->prev_menu;
      PROGRAM_STATUS->MUST_REDRAW = 1;
    break;
        /* Enter the submenu, if there are.*/
    case BUT_OK_OFF:
      /* We are selected the ACTUAL_MENU submenu... */
      if (menuptr->sub_menu)
      {
        menu_stack = menuptr;
        retval = menuptr->sub_menu;
        PROGRAM_STATUS->MUST_REDRAW = 1;
      }
    break;
        /* Back the menu level, if there, or MAIN display.*/
    case BUT_ES_OFF:
      /* We are selected the higher menu level...  */
      if (menu_stack)
      {
        retval = menu_stack;
        menu_stack = NULL;
        PROGRAM_STATUS->MUST_REDRAW = 1;
      } else
      {
        
      }
    break;
  }
  return retval;    /* There was'nt menu selection */


}