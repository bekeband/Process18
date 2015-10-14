
#include <stdint.h>
#include "menu.h"

struct s_menu_item main_menus[MAIN_MENU_ITEMS] = 
  {{ 0x01, 0x00, 0b00000011, (char*)"INPUT"},{0x01, 0x01, 0b00000011, (char*)"OUTPUT"},
   { 0x01, 0x00, 0b00000011, (char*)"DISPLAY"},{0x01, 0x01, 0b00000011, (char*)"SYSTEM"}};

struct s_menu_item input_menus[INPUT_MENU_ITEMS] =
  {{ 0x01, 0x00, 0b00000011, (char*)"TYPE"},{0x01, 0x01, 0b00000011, (char*)"RANGE"},
   { 0x01, 0x00, 0b00000011, (char*)"CALIB"}};

struct s_menu_item display_menus[DISPLAY_MENU_ITEMS] =
  {{ 0x01, 0x00, 0b00000011, (char*)"REFRESH"},{0x01, 0x01, 0b00000011, (char*)"DATAS"}};

struct s_menu_item system_menus[SYSTEM_MENU_ITEMS] =
  {{ 0x01, 0x00, 0b00000011, (char*)"SERIAL"},{0x01, 0x01, 0b00000011, (char*)"RESET"}};