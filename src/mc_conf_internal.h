/**
 * @file mc_conf_internal.h
 * @author Forairaaaaa
 * @brief 
 * @version 0.1
 * @date 2023-08-19
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
#ifdef MC_USE_CUSTOM_CONF
    /* Try include */
    #include "../../mc_conf.h"
#else

#define MC_VERSION  "v0.2.0"


/* Database Key defines */
/* Use define to unify data's key and type */
/* For easier search and convert */
/* Display */
#define MC_DB_DISP_HOR                  "_DIS_HOR"
#define MC_DB_DISP_HOR_T                std::int16_t
#define MC_DB_DISP_VER                  "_DIS_VER"
#define MC_DB_DISP_VER_T                std::int16_t
#define MC_DB_DISP_BRIGHTNESS           "_DIS_BRI"
#define MC_DB_DISP_BRIGHTNESS_T         std::uint8_t

/* Power */
#define MC_DB_BATTERY_LEVEL             "_BAT_LV"
#define MC_DB_BATTERY_LEVEL_T           std::uint8_t
#define MC_DB_BATTERY_IS_CHARGING       "_BAT_CHG"
#define MC_DB_BATTERY_IS_CHARGING_T     bool


/* Enable to build that builtin apps */
#define MC_APPS_ENABLE_LVGL_BOOTANIM_LOGO_LABEL         0
#define MC_APPS_ENABLE_LVGL_BOOTANIM_LOGO_GIF           0
#define MC_APPS_ENABLE_LVGL_LAUNCHER_OPPO_WATCH         0


#endif