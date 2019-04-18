/**
 * @brief       MCP9808.h
 * @details     �0.5�C Maximum Accuracy Digital Temperature Sensor.
 *              Header file.
 *
 *
 * @return      N/A
 *
 * @author      Manuel Caballero
 * @date        15/April/2019
 * @version     15/April/2019    The ORIGIN
 * @pre         N/A.
 * @warning     N/A
 * @pre         This code belongs to Nimbus Centre ( http://www.nimbus.cit.ie ). All rights reserved.
 */


#include "stdint.h"
#include "stdbool.h"
#include "i2c.h"


/**
  * @brief   DEFAULT ADDRESSES
  */
typedef enum
{
  MCP9808_ADDRESS_0     =   0b0011000,        /*!<   I2C slave address byte: A2A1A0: 000          */
  MCP9808_ADDRESS_1     =   0b0011001,        /*!<   I2C slave address byte: A2A1A0: 001          */
  MCP9808_ADDRESS_2     =   0b0011010,        /*!<   I2C slave address byte: A2A1A0: 010          */
  MCP9808_ADDRESS_3     =   0b0011011,        /*!<   I2C slave address byte: A2A1A0: 011          */
  MCP9808_ADDRESS_4     =   0b0011100,        /*!<   I2C slave address byte: A2A1A0: 100          */
  MCP9808_ADDRESS_5     =   0b0011101,        /*!<   I2C slave address byte: A2A1A0: 101          */
  MCP9808_ADDRESS_6     =   0b0011110,        /*!<   I2C slave address byte: A2A1A0: 110          */
  MCP9808_ADDRESS_7     =   0b0011111         /*!<   I2C slave address byte: A2A1A0: 111          */
} MCP9808_addresses_t;



/**
  * @brief   REGISTERS
  */
typedef enum
{
  MCP9808_CONFIG          =   0x01,           /*!<  CONFIG register                               */
  MCP9808_TUPPER          =   0x02,           /*!<  T_UPPER register                              */
  MCP9808_TLOWER          =   0x03,           /*!<  T_LOWER register                              */
  MCP9808_TCRIT           =   0x04,           /*!<  T_CRIT register                               */
  MCP9808_TA              =   0x05,           /*!<  T_A register                                  */
  MCP9808_MANUFACTURER_ID =   0x06,           /*!<  Manufacturer ID register                      */
  MCP9808_DEVICE_ID       =   0x07,           /*!<  Device ID/Revision register                   */
  MCP9808_RESOLUTION      =   0x08            /*!<  Resolution register                           */
} MCP9808_registers_t;



/**
  * @brief   CONFIG Register
  */
/* T_HYST <9:10>: TUPPER AND TLOWER LIMIT HYSTERESIS BITS
 *    NOTE:
 *      � This bit can not be altered when either of the Lock bits are set ( bit 6 and bit 7 )
 *      � This bit can be programmed in Shutdown mode
 */
typedef enum
{
    CONFIG_T_HYST_MASK                =   ( 0b11 << 9U ),   /*!<  T_HYST mask                                     */
    CONFIG_T_HYST_0_C                 =   ( 0b00 << 9U ),   /*!<  T_HYST: 0C                          [ Default ] */
    CONFIG_T_HYST_1_5_C               =   ( 0b01 << 9U ),   /*!<  T_HYST: +1.5C                                   */
    CONFIG_T_HYST_3_0_C               =   ( 0b10 << 9U ),   /*!<  T_HYST: +3.0C                                   */
    CONFIG_T_HYST_6_0_C               =   ( 0b11 << 9U ),   /*!<  T_HYST: +6.0C                                   */
} MCP9808_config_thyst_t;



/* SHDN <8>: SHUTDOWN MODE BIT
 *    NOTE: 
 *      � This bit can not be set to '1' when either of the Lock bits are set ( bit 6 and bit 7 ), however,
 *        it can be cleared to '0' for continuous conversion while locked.
 */
typedef enum
{
    CONFIG_SHDN_MASK                  =   ( 1U << 8U ),     /*!<  SHDN mask                                       */
    CONFIG_SHDN_CONTINUOUS_CONVERSION =   ( 0U << 8U ),     /*!<  SHDN: Continuous conversion         [ Default ] */
    CONFIG_SHDN_SHUTDOWN              =   ( 1U << 8U )      /*!<  SHDN: Shutdown ( Low-power mode )               */
} MCP9808_config_shdn_t;



/* CRIT_LOCK <7>: T_CRIT LOCKED BIT
 *    NOTE: N/A.
 */
typedef enum
{
    CONFIG_CRIT_LOCK_MASK             =   ( 1U << 7U ),     /*!<  CRIT_LOCK mask                                  */
    CONFIG_CRIT_LOCK_UNLOCKED         =   ( 0U << 7U ),     /*!<  T_CRIT register can be written      [ Default ] */
    CONFIG_CRIT_LOCK_LOCKED           =   ( 1U << 7U )      /*!<  T_CRIT register can not be written              */
} MCP9808_config_crit_lock_t;



/* WIN_LOCK <6>: CORRECTION INTERRUPT ENABLE 
 *    NOTE: N/A.
 */
typedef enum
{
    CONFIG_WIN_LOCK_MASK              =   ( 1U << 6U ),     /*!<  WIN_LOCK mask                                   */
    CONFIG_WIN_LOCK_UNLOCKED          =   ( 0U << 6U ),     /*!<  TUPPER and TLOWER can be written    [ Default ] */
    CONFIG_WIN_LOCK_LOCKED            =   ( 1U << 6U )      /*!<  TUPPER and TLOWER can not be written            */
} MCP9808_config_win_lock_t;



/* INT_CLEAR <5>: INTERRUPT CLEAR BIT 
 *    NOTE: 
 *      � This bit can not be set to '1' in Shutdown mode, but it can be cleared after the device enters Shutdown mode
 */
typedef enum
{
    CONFIG_INT_CLEAR_MASK              =   ( 1U << 5U ),     /*!<  INT_CLEAR mask                                 */
    CONFIG_INT_CLEAR_NO_EFFECT         =   ( 0U << 5U ),     /*!<  No effect                          [ Default ] */
    CONFIG_INT_CLEAR_CLEAR_INT_OUTPUT  =   ( 1U << 5U )      /*!<  Clear interrupt output, when read, returns '0' */
} MCP9808_conf_int_clear_t;



/* ALERT_STAT <4>: ALERT OUTPUT STATUS BIT 
 *    NOTE: N/A.
 */
typedef enum
{
    CONFIG_ALERT_STAT_MASK              =   ( 1U << 4U ),     /*!<  ALERT_STAT mask                               */
    CONFIG_ALERT_STAT_NOT_ASSERTED      =   ( 0U << 4U ),     /*!<  ALERT_STAT is not asserted        [ Default ] */
    CONFIG_ALERT_STAT_ASSERTED          =   ( 1U << 4U )      /*!<  ALERT_STAT is asserted                        */
} MCP9808_config_alert_stat_t;



/* ALERT_CNT <3>: ALERT OUTPUT CONTROL BIT 
 *    NOTE: N/A.
 */
typedef enum
{
    CONFIG_ALERT_CNT_MASK               =   ( 1U << 3U ),     /*!<  ALERT_CNT mask                               */
    CONFIG_ALERT_CNT_DISABLED           =   ( 0U << 3U ),     /*!<  ALERT_CNT disabled               [ Default ] */
    CONFIG_ALERT_CNT_ENABLED            =   ( 1U << 3U )      /*!<  ALERT_CNT enabled                            */
} MCP9808_config_alert_cnt_t;



/* ALERT_SEL <2>: ALERT OUTPUT SELECT BIT 
 *    NOTE: N/A.
 */
typedef enum
{
    CONFIG_ALERT_SEL_MASK                 =   ( 1U << 2U ),   /*!<  ALERT_SEL mask                                            */
    CONFIG_ALERT_SEL_TUPPER_TLOWER_TCRIT  =   ( 0U << 2U ),   /*!<  Alert output for T_UPPER, T_LOWER and T_CRIT  [ Default ] */
    CONFIG_ALERT_SEL_TA_GREATER_TCRIT     =   ( 1U << 2U )    /*!<  Alert output for T_A greater T_CRIT                       */
} MCP9808_config_alert_sel_t;



/* ALERT_POL <1>: ALERT OUTPUT POLARITY BIT 
 *    NOTE: N/A.
 */
typedef enum
{
    CONFIG_ALERT_POL_MASK               =   ( 1U << 1U ),     /*!<  ALERT_POL mask                               */
    CONFIG_ALERT_POL_ACTIVE_LOW         =   ( 0U << 1U ),     /*!<  ALERT_POL active-low             [ Default ] */
    CONFIG_ALERT_POL_ACTIVE_HIGH        =   ( 1U << 1U )      /*!<  ALERT_POL active-high                        */
} MCP9808_config_alert_pol_t;



/* ALERT_MOD <0>: ALERT OUTPUT MODE BIT 
 *    NOTE: N/A.
 */
typedef enum
{
    CONFIG_ALERT_MOD_MASK               =   ( 1U << 0U ),     /*!<  ALERT_MOD mask                               */
    CONFIG_ALERT_MOD_COMPARATOR_OUTPUT  =   ( 0U << 0U ),     /*!<  ALERT_MOD comparator output      [ Default ] */
    CONFIG_ALERT_MOD_INTERRUPT_OUTPUT   =   ( 1U << 0U )      /*!<  ALERT_MOD interrupt output                   */
} MCP9808_config_alert_mod_t;




/**
  * @brief   T_UPPER/T_LOWER/T_CRIT TEMPERATURE LIMIT REGISTERS
  */
/* SIGN <12>: SIGN BIT
 *    NOTE: N/A.
 */
typedef enum
{
    TEMPERATURE_LIMIT_SIGN_MASK         =   ( 1U << 12U ),    /*!<  SIGN mask                                   */
    TEMPERATURE_LIMIT_SIGN_TA_POSITIVE  =   ( 0U << 12U ),    /*!<  T_A greater or iqual 0C         [ Default ] */
    TEMPERATURE_LIMIT_SIGN_TA_NEGATIVE  =   ( 1U << 12U )     /*!<  T_A lower 0C                                */
} MCP9808_temperature_limit_sign_t;



/* INTEGRAL <8:4>: TEMPERATURE INTEGRAL BOUNDARY PART
 *    NOTE: N/A.
 */
typedef enum
{
    TEMPERATURE_LIMIT_INTEGRAL_PART_MASK  =   ( 0b11111111 << 4U ) /*!<  Temperature limit, integral part      */
} MCP9808_temperature_limit_integral_boundary_t;



/* DECIMAL <3:2>: TEMPERATURE DECIMAL BOUNDARY PART
 *    NOTE: N/A.
 */
typedef enum
{
    TEMPERATURE_LIMIT_DECIMAL_PART        =   ( 0b11 << 2U )  /*!<  Temperature limit, decimal part             */
} MCP9808_temperature_limit_decimal_boundary_t;




/**
  * @brief   T_A AMBIENT TEMPERATURE REGISTERS
  */
/* TA_VS_TCRIT <15>
 *    NOTE: N/A.
 */
typedef enum
{
    T_A_TA_VS_TCRIT_MASK                    =   ( 1U << 15U ),  /*!<  T_A mask                                    */
    T_A_TA_VS_TCRIT_TA_LOWER_TCRIT          =   ( 0U << 15U ),  /*!<  T_A lower T_CRIT                            */
    T_A_TA_VS_TCRIT_TA_GREATER_EQUAL_TCRIT  =   ( 1U << 15U )   /*!<  T_A greater or equal T_CRIT                 */
} MCP9808_t_a_ta_vs_tcrit_t;



/* TA_VS_TUPPER <14>
 *    NOTE: N/A.
 */
typedef enum
{
    T_A_TA_VS_TUPPER_MASK                     =   ( 1U << 14U ),  /*!<  T_A mask                                    */
    T_A_TA_VS_TUPPER_TA_LOWER__IQUAL_TUPPER   =   ( 0U << 14U ),  /*!<  T_A lower or equal T_UPPER                  */
    T_A_TA_VS_TUPPER_TA_GREATER_TUPPER        =   ( 1U << 14U )   /*!<  T_A greater T_UPPER                         */
} MCP9808_t_a_ta_vs_tupper_t;



/* TA_VS_LOWER <13>
 *    NOTE: N/A.
 */
typedef enum
{
    T_A_TA_VS_TLOWER_MASK                     =   ( 1U << 13U ),  /*!<  T_A mask                                    */
    T_A_TA_VS_TLOWER_TA_LOWER_TLOWER          =   ( 1U << 13U ),  /*!<  T_A lower or equal T_LOWER                  */
    T_A_TA_VS_TLOWER_TA_GREATER_EQUAL_TLOWER  =   ( 0U << 13U )   /*!<  T_A greater T_LOWER                         */
} MCP9808_t_a_ta_vs_tlower_t;



/* TA_SIGN <12>
 *    NOTE: N/A.
 */
typedef enum
{
    T_A_TA_SIGN_MASK                          =   ( 1U << 12U ),  /*!<  T_A SIGN mask                               */
    T_A_TA_SIGN_POSITIVE                      =   ( 0U << 12U ),  /*!<  T_A positive                                */
    T_A_TA_SIGN_NEGATIVE                      =   ( 1U << 12U )   /*!<  T_A negative                                */
} MCP9808_t_a_sign_t;



/* TA_INTEGRAL <11:4>
 *    NOTE: N/A.
 */
typedef enum
{
    T_A_TA_INTEGRAL_PART_MASK         =   ( 0b11111111 << 4U )    /*!<  T_A integral part                         */
} MCP9808_t_a_integral_t;



/* TA_DECIMAL <3:0>
 *    NOTE: N/A.
 */
typedef enum
{
    T_A_TA_DECIMAL_PART_MASK         =   ( 0b1111 << 0U )         /*!<  T_A decimal part                         */
} MCP9808_t_a_decimal_t;




/**
  * @brief   RESOLUTION REGISTER
  */
/* RESOLUTION <1:0>
 *    NOTE: N/A.
 */
typedef enum
{
    RESOLUTION_MASK                   =   ( 0b11 << 0U ),         /*!<  Resolution mask                       */
    RESOLUTION_0_5_C                  =   ( 0b00 << 0U ),         /*!<  Resolution: +0.5C                     */
    RESOLUTION_0_25_C                 =   ( 0b01 << 0U ),         /*!<  Resolution: +0.25C                    */
    RESOLUTION_0_125_C                =   ( 0b10 << 0U ),         /*!<  Resolution: +0.125C                   */
    RESOLUTION_0_0625_C               =   ( 0b11 << 0U )          /*!<  Resolution: +0.0625C      [ Default ] */
} MCP9808_resolution_t;







#ifndef MCP9808_VECTOR_STRUCT_H
#define MCP9808_VECTOR_STRUCT_H
typedef struct
{
    float                       t_a;              /*!<  Ambient temperature value     */
    uint8_t                     t_a_raw;          /*!<  Raw ambient temperature value */

    MCP9808_t_a_sign_t          t_a_sign;         /*!<  Ambient temperature sign      */
    MCP9808_t_a_ta_vs_tcrit_t   ta_vs_tcrit;      /*!<  T_A vs T_CRIT  result         */
    MCP9808_t_a_ta_vs_tupper_t  ta_vs_tupper;     /*!<  T_A vs T_UPPER result         */
    MCP9808_t_a_ta_vs_tlower_t  ta_vs_tlower;     /*!<  T_A vs T_LOWER result         */

    MCP9808_config_alert_stat_t alert_stat;       /*!<  Alert Output Status bit       */

    float                       t_upper;          /*!<  T_UPPER limit                 */
    float                       t_lower;          /*!<  T_LOWER limit                 */
    float                       t_crit;           /*!<  T_CRIT limit                  */

    uint16_t                    manufacturerID;   /*!<  Manufacturer ID               */
    uint8_t                     deviceID;         /*!<  Device ID                     */
    uint8_t                     deviceRevision;   /*!<  Device Revision               */
} MCP9808_data_t;
#endif




/**
  * @brief   INTERNAL CONSTANTS
  */
typedef enum
{
    MCP9808_SUCCESS     =       0,
    MCP9808_FAILURE     =       1
} MCP9808_status_t;




/**
  * @brief   FUNCTION PROTOTYPES
  */
/** It configures the I2C peripheral.
  */
MCP9808_status_t MCP9808_Init            ( I2C_parameters_t myI2Cparameters                                            );

/** It sets T_UPPER and T_LOWER Limit Hysteresis.
  */
MCP9808_status_t MCP9808_SetT_HYST       ( I2C_parameters_t myI2Cparameters, MCP9808_config_thyst_t myT_HYST           );

/** It sets device power mode.
  */
MCP9808_status_t MCP9808_SetPowerMode    ( I2C_parameters_t myI2Cparameters, MCP9808_config_shdn_t mySHDN              );

/** It sets T_CRIT lock bit.
  */
MCP9808_status_t MCP9808_SetCRITLock     ( I2C_parameters_t myI2Cparameters, MCP9808_config_crit_lock_t myT_CRIT_Lock  );

/** It sets T_UPPER and T_LOWER window lock bit.
  */
MCP9808_status_t MCP9808_SetWinLock      ( I2C_parameters_t myI2Cparameters, MCP9808_config_win_lock_t myWinLock       );

/** It sets interrupt Clear bit.
  */
MCP9808_status_t MCP9808_SetIntClear     ( I2C_parameters_t myI2Cparameters, MCP9808_conf_int_clear_t myIntClear       );

/** It gets alert output status bit.
  */
MCP9808_status_t MCP9808_GetAlertStat    ( I2C_parameters_t myI2Cparameters, MCP9808_data_t* myAlertStat               );

/** It sets alert output control bit.
  */
MCP9808_status_t MCP9808_SetAlertCnt     ( I2C_parameters_t myI2Cparameters, MCP9808_config_alert_cnt_t myAlertCnt     );

/** It sets alert output select bit.
  */
MCP9808_status_t MCP9808_SetAlertSel     ( I2C_parameters_t myI2Cparameters, MCP9808_config_alert_sel_t myAlertSel     );

/** It sets alert output polarity bit.
  */
MCP9808_status_t MCP9808_SetAlertPol     ( I2C_parameters_t myI2Cparameters, MCP9808_config_alert_pol_t myAlertPol     );

/** It sets alert output mode bit.
  */
MCP9808_status_t MCP9808_SetAlertMod     ( I2C_parameters_t myI2Cparameters, MCP9808_config_alert_mod_t myAlertMod     );

