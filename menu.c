
#include <htc.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "config.h"
#include "menu.h"
#include "buttons.h"
#include "lcddriver.h"


s_menu_edit_status MENU_STATUS;

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
struct s_int_data input_average_datas[ANALOG_CHANNELS] = {NULL, 4, 64};
struct s_int_data  refresh_data = {NULL, 1, 5};

struct s_float_data input_low_range_datas[ANALOG_CHANNELS] = {NULL, -999999, 999999};
struct s_float_data input_high_range_datas[ANALOG_CHANNELS] = {NULL, -999999, 999999};

struct s_menu_item display_datas_menus[] =
{ {&display_datas_menus[1], &display_datas_menus[1], &display_data_topline, &display_datas_enum_array, {0,0,ENUM},(char*)"TOP LINE"}  ,
  {&display_datas_menus[0], &display_datas_menus[0], &display_data_botline, &display_datas_enum_array, {0,0,ENUM}, (char*)"BOTTOM LINE"}};

struct s_menu_item display_menus[] =
{ {&display_menus[1], &display_menus[1], &refresh_data, NULL, {0,0,TINT},(char*)"REFRESH"}  ,
  {&display_menus[0], &display_menus[0], &display_datas_menus, NULL, {0,HAS_SUBMENU,0}, (char*)"DATAS"}};

struct s_menu_item system_menus[] =
{ {&system_menus[1], &system_menus[2], &serial_data, &serial_baud_enum_array, {0,0,ENUM}, (char*)"SERIAL"},
  {&system_menus[2], &system_menus[0], &version_data, NULL, {0, VIEWED_VALUE, TEXT}, (char*)"VERSION"},
  {&system_menus[0], &system_menus[1], NULL, NULL, {0,0,0}, (char*)"RESET"}};

struct s_menu_item input_menus[] =
{ {&input_menus[1], &input_menus[3], &input_type_datas[0], &input_type_enum_array[0], {0, 0, ENUM}, (char*)"TYPE"},
  {&input_menus[2], &input_menus[0], &input_low_range_datas[0], NULL, {0, 0, FLOAT}, (char*)"RANGE LOW"},
  {&input_menus[3], &input_menus[1], &input_high_range_datas[0], NULL, {0, 0, FLOAT}, (char*)"RANGE HIGH"},
  {&input_menus[4], &input_menus[2], &input_average_datas[0], NULL, {0, 0, TINT, CAN_ROLLING}, (char*)"AVERAGE"},
  {&input_menus[0], &input_menus[3], NULL, NULL, {0, 0, 0}, (char*)"CALIB"}};

struct s_menu_item main_menus[] =
{ {&main_menus[1], &main_menus[3], &input_menus[0], NULL, {0,HAS_SUBMENU,0},(char*)"INPUT"},
  {&main_menus[2], &main_menus[0], &input_type_datas[0], &input_type_enum_array[0], {0,0,ENUM},(char*)"OUTPUT"},
  {&main_menus[3], &main_menus[1], &display_menus[0], NULL, {0,HAS_SUBMENU,0},(char*)"DISPLAY"},
  {&main_menus[0], &main_menus[2], &system_menus[0], NULL, {0,HAS_SUBMENU,0},(char*)"SYSTEM"}};

uint8_t menu_stack_ptr = 0;
p_menu_item menu_stack[3] = {&main_menus[0], };

char IncrementDecNumber(char c, int suppress_minus, int suppress_space)
{
  switch (c)
  {
    case ' ':
      if (!suppress_minus)
      {
        c = '-';
      } else c = '0';

    break;
    case '-':
      c = '0';
    break;
    case '9':
      if (!suppress_space)
      {
        c = ' ';
      } else
        if (!suppress_minus)
        {
          c = '-';
        } else
          c = '0';
      break;
    default:
      c++;
      break;
  }
  return c;
}

char DecrementDecNumber(char c, int suppress_minus, int suppress_space)
{
  switch (c)
  {
    case '-':
      if (!suppress_minus)
      {
        c = ' ';
      } else c = '9';

    break;
    case ' ':
      c = '9';
    break;
    case '0':
      if (!suppress_space)
      {
        c = ' ';
      } else
        if (!suppress_minus)
        {
          c = '-';
        } else
          c = '9';
      break;
    default:
      c--;
      break;
  }
  return c;
}

void InitSettingDatas()
{ int i;
  display_data_topline.address = &(GetSettingPtr()->display_topline);
  display_data_botline.address = &(GetSettingPtr()->display_botline);
  serial_data.address = &(GetSettingPtr()->serial_speed);
  refresh_data.address = &(GetSettingPtr()->display_refresh_time);

  for (i = 0; i < ANALOG_CHANNELS; i++)
  {
    input_type_datas[i].address = &(GetAnalogDataPtr(i)->input_type);
    input_type_datas[i].address = &(GetAnalogDataPtr(i)->input_type);
    input_low_range_datas[i].address = &(GetAnalogDataPtr(i)->min_val);
    input_high_range_datas[i].address = &(GetAnalogDataPtr(i)->max_val);
  }
}

static void GetVersion(char* c)
{
  sprintf(c, "VER %i.%i", VER_H, VER_L);
}

int8_t TestEnumValue(int8_t enum_val, p_enum_data pe)
{
  if ((enum_val) >= pe->size) { enum_val = 0;};
  if ((enum_val) < 0) {enum_val = (pe->size) - 1;};
  return enum_val;
}

int EnumMenuValueFunction(void* enumdata, const char* enumlist[])
{ uint8_t enum_val;
  enum_val = *((p_enum_data)enumdata)->address;
  enum_val = TestEnumValue(enum_val, enumdata);
  strcpy(GetDisplayBuffer(2), enumlist[enum_val]);
}

int TestIntValue(int int_val, p_int_data pi)
{
  if ((int_val) > pi->maxvalue) { int_val = pi->maxvalue;};
  if ((int_val) < pi->minvalue)  {int_val = pi->minvalue;};
  return int_val;
}

float TestFloatValue(float float_val, p_float_data pf)
{
  if ((float_val) > pf->maxvalue) { float_val = pf->maxvalue;};
  if ((float_val) < pf->minvalue)  {float_val = pf->minvalue;};
  return float_val;
}

int FloatMenuValueFunction(void* p)
{ p_float_data pf;
  pf = (p_float_data)p;
  float val = TestFloatValue(*(pf->address), pf);
  sprintf(GetDisplayBuffer(2), "%8.2f", val);
}

int TextMenuValueFunction(void* p)
{ p_text_data pt;
  pt = (p_text_data)p;
  strcpy(GetDisplayBuffer(2), pt->address);
}

int IntMenuValueFunction(void* p)
{ p_int_data pi;
  pi = (p_int_data)p;
  int val = TestIntValue(*(pi->address), pi);
  sprintf(GetDisplayBuffer(2), "%8i", val);
}

/* If integer value can set with value rolling function to increment, and decrement value. */
int IntMenuEditFunctionwRolling(void* p)
{ p_int_data pi; int exit_edit = 0; uint8_t but;
  int int_val, old_int_val, retval = NO_CHANGED;
  pi = p;
  int_val = *(pi->address);
  old_int_val = *(pi->address);
  retval = 0;
  MENU_STATUS.MUST_REDRAW = 1;
  while (!exit_edit)
  {
    but = ButtonScan();
    switch (but)
    {
    case BUT_RG_OFF:
      int_val += 10;
      MENU_STATUS.MUST_REDRAW = 1;
      break;
    case BUT_LF_OFF:
      int_val -= 10;
      MENU_STATUS.MUST_REDRAW = 1;
      break;
    case BUT_UP_OFF:
      int_val++;
      MENU_STATUS.MUST_REDRAW = 1;
      break;
    case BUT_DN_OFF:
      int_val--;
      MENU_STATUS.MUST_REDRAW = 1;
      break;
    case BUT_OK_OFF:
      (*(pi->address)) = int_val;
      if (int_val != old_int_val)
      {
        retval = DATA_CHANGED;
      }
      exit_edit = 1;
      break;
    case BUT_ES_OFF:
      MENU_STATUS.MUST_SAVE = 0;
      LCDSendCmd(CUR_OFF);
      exit_edit = 1;
      break;
    }
    int_val = TestIntValue(int_val, pi);
    if (MENU_STATUS.MUST_REDRAW)
    {
      MENU_STATUS.MUST_SAVE = 1;
      sprintf(GetDisplayBuffer(2), "\xD9 %12i \xDA", int_val);
      LCDSendCmd(DD_RAM_ADDR2);
      LCDSendStr(GetDisplayBuffer(2));
      MENU_STATUS.MUST_REDRAW = 0;
    }
  }
  return retval;
}

int IntMenuEditFunction(void* p)
{ p_int_data pi; int retval =0; int exit_edit = 0;
  pi = (p_int_data)p; uint8_t but; int cursor_pos;
  int int_val, old_int_val;
  int_val = *(pi->address);
  old_int_val = *(pi->address);
  char IntEditBuffer[13];
  cursor_pos = 2;
  if ((int_val) > pi->maxvalue) { int_val = pi->maxvalue;};
  if ((int_val) < pi->minvalue)  {int_val = pi->minvalue;};
  sprintf(IntEditBuffer, "%12i", int_val);
  LCDSendCmd(CUR_ON_BLINK);

  MENU_STATUS.MUST_REDRAW = 1;
  while (!exit_edit)
  {
    but = ButtonScan();

    switch (but)
    {
    case BUT_RG_OFF:
      LCDSendCmd(CUR_RIGHT); cursor_pos++;
//      MENU_STATUS.MUST_REDRAW = 1;
      break;
    case BUT_LF_OFF:
      LCDSendCmd(CUR_LEFT); cursor_pos--;
//      MENU_STATUS.MUST_REDRAW = 1;
      break;
    case BUT_UP_OFF:
      IntEditBuffer[cursor_pos - 2] = IncrementDecNumber(IntEditBuffer[cursor_pos - 2], 0, 0);
      MENU_STATUS.MUST_REDRAW = 1;
      break;
    case BUT_DN_OFF:
      IntEditBuffer[cursor_pos - 2] = DecrementDecNumber(IntEditBuffer[cursor_pos - 2], 0, 0);
      MENU_STATUS.MUST_REDRAW = 1;
      break;
    case BUT_OK_OFF:
      (*(pi->address)) = int_val;
      exit_edit = 1;
      break;
    case BUT_ES_OFF:
      MENU_STATUS.MUST_SAVE = 0;
      LCDSendCmd(CUR_OFF);
      exit_edit = 1;
      break;
    }
    int_val = TestIntValue(int_val, pi);
    if (MENU_STATUS.MUST_REDRAW)
    {
      sprintf(GetDisplayBuffer(2), "\xD9 %12s \xDA", IntEditBuffer);
      LCDSendCmd(DD_RAM_ADDR2);
      LCDSendStr(GetDisplayBuffer(2));
      LCDSendCmd(DD_RAM_ADDR2 + cursor_pos);
      MENU_STATUS.MUST_REDRAW = 0;
    }
  }
  return retval;
}

int EnumMenuEditFunction(void* enumdata, const char* enumlist[])
{ uint8_t but; p_enum_data pe; int exit_edit = 0; 
  int8_t enum_val, old_enum_val;
  int retval = 0;
  pe = (p_enum_data)enumdata;
  enum_val = *(pe->address);
  old_enum_val = *(pe->address);
  MENU_STATUS.MUST_REDRAW = 1;
  while (!exit_edit)
  {
  but = ButtonScan();

  switch (but)
  {
    case BUT_UP_OFF:
      enum_val++; //
      MENU_STATUS.MUST_REDRAW = 1; // enum value was changed !
      break;
    case BUT_DN_OFF:
      enum_val--;
      MENU_STATUS.MUST_REDRAW = 1; // enum value changed !
      break;
    case BUT_OK_OFF:
      (*(pe->address)) = enum_val;
      exit_edit = 1;
      break;
    case BUT_ES_OFF:
      MENU_STATUS.MUST_SAVE = 0;
      exit_edit = 1;
      break;
    }
    if ((enum_val) >= pe->size) { enum_val = 0;};
    if ((enum_val) < 0) {enum_val = (pe->size) - 1;}
    if (MENU_STATUS.MUST_REDRAW)
    {
      MENU_STATUS.MUST_SAVE = 1;
      sprintf(GetDisplayBuffer(2), "\xD9 %12s \xDA", enumlist[enum_val]);
      LCDSendCmd(DD_RAM_ADDR2);
      LCDSendStr(GetDisplayBuffer(2));
      MENU_STATUS.MUST_REDRAW = 0;
    }
  }
  return retval;
}

int MenuProcess(uint8_t but, s_status* PROGRAM_STATUS)
{ 
  int retval = 0; int edit_retval;
  if (PROGRAM_STATUS->MUST_REDRAW)
    {
      LCDSendCmd(CLR_DISP);
      if (menu_stack[menu_stack_ptr]->options.MENU_TYPE == HAS_SUBMENU)
      { /* There are submenu. */
        sprintf(GetDisplayBuffer(1), "\xC8 %s \xC9", menu_stack[menu_stack_ptr]->title);
      } else
      { /* No submenu perhaps enumerate, or direct value setting. */
        sprintf(GetDisplayBuffer(1), "\xC8 %s :", menu_stack[menu_stack_ptr]->title);
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
      } else
      {
        switch (menu_stack[menu_stack_ptr]->options.DATA_TYPE)
        {
          case ENUM:
 
            edit_retval =  EnumMenuEditFunction(menu_stack[menu_stack_ptr]->param1, menu_stack[menu_stack_ptr]->param2);

            PROGRAM_STATUS->MUST_REDRAW = 1;
            break;
          case TINT:
            if (menu_stack[menu_stack_ptr]->options.IS_ROLLING)
            {
              if (IntMenuEditFunctionwRolling(menu_stack[menu_stack_ptr]->param1) == DATA_CHANGED)
              {
                LCDSendCmd(DD_RAM_ADDR2);
                LCDSendStr("DATA CHANGED");
              } else
              {
                LCDSendCmd(DD_RAM_ADDR2);
                LCDSendStr("DATA NO CHANGED");                
              }
                while (1);

            } else
            {
              edit_retval = IntMenuEditFunction(menu_stack[menu_stack_ptr]->param1);
            }
            PROGRAM_STATUS->MUST_REDRAW = 1;
            break;
          case FLOAT:
            break;
          case TEXT:
            break;
        }
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
