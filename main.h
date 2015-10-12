/* 
 * File:   main.h
 * Author: user
 *
 * Created on 2015. október 9., 13:19
 */

#ifndef MAIN_H
#define	MAIN_H

#ifdef	__cplusplus
extern "C" {
#endif


#define	LED			PORTBbits.RB4
#define	RELAY		PORTAbits.RA1

#define VERH  2
#define VERL  7

#define TMR0DATA  (0x080)
#define TMR2DATA  (0x06)  // 256 - 250 5M/16 = 312500 / 250 = 1250/s

#define MAIN_DISPLAY  56
#define MENU_DISPLAY  65

  typedef struct {
  union {
    unsigned DISPLAY_REFRESH: 1;
    unsigned AD_REFRESH:      1;
    unsigned MUST_REDRAW:     1;
  };
  union {
    uint8_t STATUS_BYTE;
  };
} s_status;

typedef struct  {
  uint8_t serial_speed;
  uint8_t button_beep;
  uint16_t display_refresh_time;
}s_setting_datas;

typedef struct s_analog_data {
  uint8_t input_type; // 0 = 4 - 20 mA, 1 = 0- 20 mA
  float min_val;
  float max_val;
  uint16_t min_eng;
  uint16_t max_eng;
  uint8_t input_dim;
  uint8_t average;
  float summa;
} s_analog_datas;

#ifdef	__cplusplus
}
#endif

#endif	/* MAIN_H */

