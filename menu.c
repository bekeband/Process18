
#include <htc.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "config.h"
#include "menu.h"
#include "buttons.h"
#include "lcddriver.h"

static void GetVersion(char* c);

const char* display_datas_enum_array[] = {"PROCESS DATA","RAW DATA","SUMMA 1","SUMMA 2"};
const char* serial_baud_enum_array[] = {"4800", "9600", "19200"};
const char* input_type_enum_array[] = {"4-20 mA", "0-20 mA"};
const char* on_off_enum_array[] = {"OFF", "ON"};

struct s_text_data version_data = {"VER 2.7", 10};
struct s_enum_data display_data_topline = {NULL, 4};
struct s_enum_data display_data_botline = {NULL, 4};
struct s_enum_data serial_data = {NULL, 3};
struct s_enum_data input_type_datas[ANALOG_CHANNELS] = {NULL, 2};

struct s_float_data input_low_range_datas[ANALOG_CHANNELS] = {NULL, -999999, 999999};
struct s_float_data input_high_range_datas[ANALOG_CHANNELS] = {NULL, -999999, 999999};

struct s_menu_item display_datas_menus[] =
{ {&display_datas_menus[1], &display_datas_menus[1], &display_data_topline, &display_datas_enum_array, {0,0,ENUM},(char*)"TOP LINE"}  ,
  {&display_datas_menus[0], &display_datas_menus[0], &display_data_botline, &display_datas_enum_array, {0,0,ENUM}, (char*)"BOTTOM LINE"}};

struct s_menu_item display_menus[] =
{ {&display_menus[1], &display_menus[1], NULL, NULL, {0,0,FLOAT},(char*)"REFR"}  ,
  {&display_menus[0], &display_menus[0], &display_datas_menus, NULL, {0,HAS_SUBMENU,0}, (char*)"DATAS"}};

struct s_menu_item system_menus[] =
{ {&system_menus[1], &system_menus[2], &serial_data, &serial_baud_enum_array, {0,0,ENUM}, (char*)"SERIAL"},
  {&system_menus[2], &system_menus[0], &version_data, NULL, {0, VIEWED_VALUE, TEXT}, (char*)"VERSION"},
  {&system_menus[0], &system_menus[1], NULL, NULL, {0,0,0}, (char*)"RESET"}};

struct s_menu_item input_menus[] =
{ {&input_menus[1], &input_menus[3], &input_type_datas[0], &input_type_enum_array[0], {0,0,ENUM}, (char*)"TYPE"},
  {&input_menus[2], &input_menus[0], &input_low_range_datas[0], NULL, {0,0,FLOAT}, (char*)"RANGE LOW"},
  {&input_menus[3], &input_menus[1], &input_high_range_datas[0], NULL, {0,0,FLOAT}, (char*)"RANGE HIGH"},
  {&input_menus[0], &input_menus[2], NULL, NULL, {0,0,0}, (char*)"CALIB"}};

struct s_menu_item main_menus[] =
{ {&main_menus[1], &main_menus[3], &input_menus[0], NULL, {0,HAS_SUBMENU,0},(char*)"INPUT"},
  {&main_menus[2], &main_menus[0], &input_type_datas[0], &input_type_enum_array[0], {0,0,ENUM},(char*)"OUTPUT"},
  {&main_menus[3], &main_menus[1], &display_menus[0], NULL, {0,HAS_SUBMENU,0},(char*)"DISPLAY"},
  {&main_menus[0], &main_menus[2], &system_menus[0], NULL, {0,HAS_SUBMENU,0},(char*)"SYSTEM"}};

uint8_t menu_stack_ptr = 0;
p_menu_item menu_stack[3] = {&main_menus[0], };

void InitSettingDatas()
{
  display_data_topline.address = &(GetSettingPtr()->display_topline);
  display_data_botline.address = &(GetSettingPtr()->display_botline);
  serial_data.address = &(GetSettingPtr()->serial_speed);
  input_type_datas[0].address = &(GetAnalogDataPtr(0))->input_type;
  input_type_datas[0].address = &(GetAnalogDataPtr(0))->input_type;
  input_low_range_datas[0].address = &(GetAnalogDataPtr(0))->min_val;
  input_high_range_datas[0].address = &(GetAnalogDataPtr(0))->max_val;
}

static void GetVersion(char* c)
{
  sprintf(c, "VER %i.%i", VER_H, VER_L);
}

int EnumMenuValueFunction(void* enumdata, const char* enumlist[])
{ p_enum_data pe; p_enum_data pe;
  pe = (p_enum_data)enumdata;
  uint8_t val = *(pe->address);
  if ((val >= 0) && (val < pe->size))
    strcpy(GetDisplayBuffer(2), enumlist[val]);
  else strcpy(GetDisplayBuffer(2), "N/A");
}

int FloatMenuValueFunction(void* p)
{ p_float_data pf;
  pf = (p_float_data)p;
  float val = *(pf->address);
  if ((val >= pf->minvalue) && (val <= pf->maxvalue))
    sprintf(GetDisplayBuffer(2), "%8.2f", val);
  else strcpy(GetDisplayBuffer(2), "N/A");
}

int TextMenuValueFunction(void* p)
{ p_text_data pt;
  pt = (p_text_data)p;
  strcpy(GetDisplayBuffer(2), pt->address);
}

int IntMenuValueFunction(void* p)
{
  sprintf(GetDisplayBuffer(2), "INT TYPE");
}

int MenuProcess(uint8_t but, s_status* PROGRAM_STATUS)
{ 
  int retval = 0;
  if (PROGRAM_STATUS->MUST_REDRAW)
    {
      LCDSendCmd(CLR_DISP);
      if (menu_stack[menu_stack_ptr]->options.MENU_TYPE == HAS_SUBMENU)
      { /* There are submenu. */
        sprintf(GetDisplayBuffer(1), "<- %s ->", menu_stack[menu_stack_ptr]->title);
      } else
      { /* No submenu perhaps enumerate, or direct value setting. */
        sprintf(GetDisplayBuffer(1), "<- %s :", menu_stack[menu_stack_ptr]->title);
        switch (menu_stack[menu_stack_ptr]->options.DATA_TYPE)
        {
          case ENUM:
            EnumMenuValueFunction(menu_stack[menu_stack_ptr]->param1, menu_stack[menu_stack_ptr]->param2);
            break;
          case TINT:
            IntMenuValueFunction(menu_stack[menu_stack_ptr]->param1);
            break;
          case FLOAT:
            FloatMenuValueFunction(menu_stack[menu_stack_ptr]->param1);
            break;
          case TEXT:
            TextMenuValueFunction(menu_stack[menu_stack_ptr]->param1);
            break;
        }
      }
      LCDSendCmd(DD_RAM_ADDR + ((DISPLAY_WIDTH - strlen(GetDisplayBuffer(1))) / 2));
      LCDSendStr(GetDisplayBuffer(1));
      if (menu_stack[menu_stack_ptr]->options.MENU_TYPE != HAS_SUBMENU)
      {
        LCDSendCmd(DD_RAM_ADDR2 + ((DISPLAY_WIDTH - strlen(GetDisplayBuffer(2))) / 2));
        LCDSendStr(GetDisplayBuffer(2));
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
