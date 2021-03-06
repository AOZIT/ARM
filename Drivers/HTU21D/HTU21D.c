/**
 * @brief       HTU21D.h
 * @details     Digital Relative Humidity sensor with Temperature output.
 *              Functions file.
 *
 *
 * @return      NA
 *
 * @author      Manuel Caballero
 * @date        6/July/2017
 * @version     6/July/2017    The ORIGIN
 * @pre         NaN
 * @warning     NaN
 * @pre         This code belongs to AqueronteBlog ( http://unbarquero.blogspot.com ).
 */

 #include "HTU21D.h"


 /**
 * @brief       HTU21D_Init ( I2C_parameters_t )
 *
 * @details     It configures the I2C peripheral.
 *
 * @param[in]    myI2Cparameters:       I2C parameters.
 *
 * @param[out]   NaN.
 *
 *
 * @return       Status of HTU21D_Init.
 *
 *
 * @author      Manuel Caballero
 * @date        18/October/2017
 * @version     18/October/2017   The ORIGIN
 * @pre         NaN
 * @warning     NaN.
 */
HTU21D_status_t  HTU21D_Init ( I2C_parameters_t myI2Cparameters )
{
    i2c_status_t aux;

    aux  =   i2c_init ( myI2Cparameters );



    if ( aux == I2C_SUCCESS )
        return   HTU21D_SUCCESS;
    else
        return   HTU21D_FAILURE;
}



/**
 * @brief       HTU21D_Conf    ( I2C_parameters_t , HTU21D_measurement_resolution_t , HTU21D_on_chip_heater_t )
 * @details     Configure the HTU21D device.
 *
 * @param[in]    myI2Cparameters:   I2C parameters.
 * @param[in]    myResolution:      HTU21D Resolution.
 * @param[in]    myHeater:          HTU21D Heater Enabled or Disabled.
 *
 * @param[out]   Status of HTU21D_Init.
 *
 *
 * @return      NA
 *
 * @author      Manuel Caballero
 * @date        6/July/2017
 * @version     22/October/2017    The library was improved, better architecture.
 *              18/October/2017    Adapted to the new I2C driver.
 *              6/July/2017        The ORIGIN
 * @pre         NaN
 * @warning     NaN.
 */
HTU21D_status_t  HTU21D_Conf    ( I2C_parameters_t myI2Cparameters, HTU21D_measurement_resolution_t myResolution, HTU21D_on_chip_heater_t myHeater )
{
    uint8_t      cmd[]              =    { HTU21D_READ_REGISTER, 0 };
    i2c_status_t aux;



    // Reserved bits must not be changed. Therefore, for any writing to user register, default values of reserved bits must be read first
    // Datasheet: User register p.13.
    aux = i2c_write ( myI2Cparameters, &cmd[0], 1, I2C_STOP_BIT );
    aux = i2c_read  ( myI2Cparameters, &cmd[1], 1 );

    cmd[1]          &=   ~( USER_REGISTER_RESOLUTION_MASK | USER_REGISTER_STATUS_END_BATTERY_MASK | USER_REGISTER_HEATER_MASK | USER_REGISTER_OTP_MASK );
    cmd[1]          |=    ( myResolution | myHeater | USER_REGISTER_OTP_DISABLED );
    cmd[0]           =   HTU21D_WRITE_REGISTER;

    aux = i2c_write  ( myI2Cparameters, &cmd[0], 2, I2C_STOP_BIT );




    if ( aux == I2C_SUCCESS  )
        return   HTU21D_SUCCESS;
    else
        return   HTU21D_FAILURE;
}



 /**
 * @brief       HTU21D_SoftReset   ( I2C_parameters_t )
 * @details     Rebooting the HTU21D sensor switching the power off and on again.
 *
 * @param[in]    myI2Cparameters:   I2C parameters.
 *
 * @param[out]   Status of HTU21D_SoftReset.
 *
 *
 * @return      NA
 *
 * @author      Manuel Caballero
 * @date        6/July/2017
 * @version     22/October/2017    The library was improved, better architecture.
 *              18/October/2017    Adapted to the new I2C driver.
 *              6/July/2017        The ORIGIN
 * @pre         NaN
 * @warning     The soft reset takes less than 15ms. The user must take this
 *              into account.
 */
HTU21D_status_t  HTU21D_SoftReset   ( I2C_parameters_t myI2Cparameters )
{
    uint8_t      cmd[]               =   { HTU21D_SOFT_RESET };
    i2c_status_t aux;


    aux = i2c_write ( myI2Cparameters, &cmd[0], 1, I2C_STOP_BIT );


    if ( aux == I2C_SUCCESS )
       return   HTU21D_SUCCESS;
    else
       return   HTU21D_FAILURE;
}


 /**
 * @brief       HTU21D_TriggerTemperature   ( I2C_parameters_t , HTU21D_master_mode_t )
 * @details     Trigger a new temperature measurement.
 *
 * @param[in]    myI2Cparameters:   I2C parameters.
 * @param[in]    myMode:            Hold/No Hold mode.
 *
 * @param[out]   Status of HTU21D_TriggerTemperature.
 *
 *
 * @return      NA
 *
 * @author      Manuel Caballero
 * @date        6/July/2017
 * @version     22/October/2017    The library was improved, better architecture.
 *              18/October/2017    Adapted to the new I2C driver.
 *              6/July/2017        The ORIGIN
 * @pre         NaN
 * @warning     The user MUST respect the total conversion time.
 *              14-bit temperature: 44ms ( 50ms max )
 *              13-bit temperature: 22ms ( 25ms max )
 *              12-bit temperature: 11ms ( 13ms max )
 *              11-bit temperature:  6ms (  7ms max )
 */
HTU21D_status_t  HTU21D_TriggerTemperature    ( I2C_parameters_t myI2Cparameters, HTU21D_master_mode_t myMode )
{
    uint8_t      cmd        =    0;
    uint8_t      myI2C_stop =    I2C_STOP_BIT;

    i2c_status_t aux;


    // Check the mode if it is HOLD MASTER MODE, then not generate a stop bit
    if ( myMode == HTU21D_HOLD_MASTER_MODE )
    {
        cmd         =    HTU21D_TRIGGER_TEMPERATURE_MEASUREMENT_HOLD_MASTER;
        myI2C_stop  =    I2C_NO_STOP_BIT;
    }
    else
    {
        cmd         =    HTU21D_TRIGGER_TEMPERATURE_MEASUREMENT_NO_HOLD_MASTER;
        myI2C_stop  =    I2C_STOP_BIT;
    }



    aux = i2c_write ( myI2Cparameters, &cmd, 1, myI2C_stop );


    // NOTE: The user has to respect the total conversion time!



    if ( aux == I2C_SUCCESS )
       return   HTU21D_SUCCESS;
    else
       return   HTU21D_FAILURE;
}


 /**
 * @brief       HTU21D_ReadTemperature   ( I2C_parameters_t , HTU21D_vector_data_t* )
 * @details     Read a new temperature measurement.
 *
 * @param[in]    myI2Cparameters:   I2C parameters.
 * @param[in]    myTemperature:     Variable to store the temperature.
 *
 * @param[out]   Status of HTU21D_ReadTemperature.
 *
 *
 * @return      NA
 *
 * @author      Manuel Caballero
 * @date        6/July/2017
 * @version     22/October/2017    The library was improved, better architecture.
 *              18/October/2017    Adapted to the new I2C driver.
 *              6/July/2017        The ORIGIN
 * @pre         NaN
 * @warning     The measuring time depends on the chosen resolution. The user
 *              must take this into account.
 * @warning     HTU21D_TriggerTemperature MUST be call before.
 */
HTU21D_status_t  HTU21D_ReadTemperature    ( I2C_parameters_t myI2Cparameters, HTU21D_vector_data_t* myTemperature )
{
    uint8_t      myRawTemp[]         =   { 0, 0, 0};
    i2c_status_t aux;


    aux = i2c_read ( myI2Cparameters, &myRawTemp[0], 3 );


    myTemperature->Temperature     =   ( myRawTemp[0] << 8 ) | myRawTemp[1];
    myTemperature->Temperature    /=   65536.0;
    myTemperature->Temperature    *=   175.72;
    myTemperature->Temperature    -=   46.85;


    if ( aux == I2C_SUCCESS )
       return   HTU21D_SUCCESS;
    else
       return   HTU21D_FAILURE;
}



/**
 * @brief       HTU21D_ReadRawTemperature   ( I2C_parameters_t , HTU21D_vector_data_t* )
 * @details     Read a new raw temperature measurement.
 *
 * @param[in]    myI2Cparameters:   I2C parameters.
 * @param[in]    myRawTemperature:  Variable to store the temperature.
 *
 * @param[out]   Status of HTU21D_ReadTemperature.
 *
 *
 * @return      NA
 *
 * @author      Manuel Caballero
 * @date        11/July/2017
 * @version     22/October/2017    The library was improved, better architecture.
 *              18/October/2017    Adapted to the new I2C driver.
 *              11/July/2017       The ORIGIN
 * @pre         NaN
 * @warning     The measuring time depends on the chosen resolution. The user
 *              must take this into account.
 * @warning     No Hold Master is ONLY implemented.
 * @warning     HTU21D_TriggerTemperature MUST be call before.
 */
HTU21D_status_t  HTU21D_ReadRawTemperature    ( I2C_parameters_t myI2Cparameters, HTU21D_vector_data_t* myRawTemperature )
{
    uint8_t      myRawTemp[]         =   { 0, 0, 0};
    i2c_status_t aux;


    aux = i2c_read ( myI2Cparameters, &myRawTemp[0], 3 );


    myRawTemperature->Temperature     =   ( myRawTemp[0] << 8 ) | myRawTemp[1];



    if ( aux == I2C_SUCCESS )
       return   HTU21D_SUCCESS;
    else
       return   HTU21D_FAILURE;
}


 /**
 * @brief       HTU21D_TriggerHumidity   ( I2C_parameters_t , HTU21D_master_mode_t )
 * @details     Trigger a new humidity measurement.
 *
 * @param[in]    myI2Cparameters:   I2C parameters.
 * @param[in]    myMode:            Hold/No Hold mode.
 *
 * @param[out]   Status of HTU21D_TriggerHumidity.
 *
 *
 * @return      NA
 *
 * @author      Manuel Caballero
 * @date        6/July/2017
 * @version     22/October/2017    The library was improved, better architecture.
 *              18/October/2017    Adapted to the new I2C driver.
 *              6/July/2017        The ORIGIN
 * @pre         NaN
 * @warning     The user MUST respect the total conversion time.
 *              12-bit RH: 14ms   ( 16ms max )
 *              11-bit RH:  7ms   (  8ms max )
 *              10-bit RH:  4ms   (  5ms max )
 *               8-bit RH:  2ms   (  3ms max )
 */
HTU21D_status_t  HTU21D_TriggerHumidity    ( I2C_parameters_t myI2Cparameters, HTU21D_master_mode_t myMode )
{
    uint8_t      cmd        =    0;
    uint8_t      myI2C_stop =    I2C_STOP_BIT;

    i2c_status_t aux;


    // Check the mode if it is HOLD MASTER MODE, then not generate a stop bit
    if ( myMode == HTU21D_HOLD_MASTER_MODE )
    {
        cmd         =    HTU21D_TRIGGER_HUMIDITY_MEASUREMENT_HOLD_MASTER;
        myI2C_stop  =    I2C_NO_STOP_BIT;
    }
    else
    {
        cmd         =    HTU21D_TRIGGER_HUMIDITY_MEASUREMENT_NO_HOLD_MASTER;
        myI2C_stop  =    I2C_STOP_BIT;
    }



    aux = i2c_write ( myI2Cparameters, &cmd, 1, myI2C_stop );


    // NOTE: The user has to respect the total conversion time!


    if ( aux == I2C_SUCCESS )
       return   HTU21D_SUCCESS;
    else
       return   HTU21D_FAILURE;
}


/**
 * @brief       HTU21D_ReadHumidity   ( I2C_parameters_t , HTU21D_vector_data_t* )
 * @details     Read a new humidity measurement.
 *
 * @param[in]    myI2Cparameters:   I2C parameters.
 * @param[in]    myHumidity:        Variable to store the humidity.
 *
 * @param[out]   Status of HTU21D_ReadHumidity.
 *
 *
 * @return      NA
 *
 * @author      Manuel Caballero
 * @date        6/July/2017
 * @version     22/October/2017    The library was improved, better architecture.
 *              18/October/2017    Adapted to the new I2C driver.
 *              6/July/2017        The ORIGIN
 * @pre         NaN
 * @warning     The measuring time depends on the chosen resolution. The user
 *              must take this into account.
 * @warning     HTU21D_TriggerHumidity MUST be call before.
 */
HTU21D_status_t  HTU21D_ReadHumidity    ( I2C_parameters_t myI2Cparameters, HTU21D_vector_data_t* myHumidity )
{
    uint8_t      myRawRH[]           =    { 0, 0, 0};
    i2c_status_t aux;


    aux = i2c_read ( myI2Cparameters, &myRawRH[0], 3 );


    myHumidity->RelativeHumidity    =   ( myRawRH[0] << 8 ) | myRawRH[1];
    myHumidity->RelativeHumidity   /=   65536.0;
    myHumidity->RelativeHumidity   *=   125.0;
    myHumidity->RelativeHumidity   -=   6.0;



    if ( aux == I2C_SUCCESS )
       return   HTU21D_SUCCESS;
    else
       return   HTU21D_FAILURE;
}



/**
 * @brief       HTU21D_ReadRawHumidity   ( I2C_parameters_t , HTU21D_vector_data_t* )
 * @details     Read a new raw humidity measurement.
 *
 * @param[in]    myI2Cparameters:   I2C parameters.
 * @param[in]    myHumidity:        Variable to store the humidity.
 *
 * @param[out]   Status of HTU21D_ReadHumidity.
 *
 *
 * @return      NA
 *
 * @author      Manuel Caballero
 * @date        11/July/2017
 * @version     22/October/2017    The library was improved, better architecture.
 *              18/October/2017    Adapted to the new I2C driver.
 *              11/July/2017       The ORIGIN
 * @pre         NaN
 * @warning     The measuring time depends on the chosen resolution. The user
 *              must take this into account.
 * @warning     No Hold Master is ONLY implemented.
 * @warning     HTU21D_TriggerHumidity MUST be call before.
 */
HTU21D_status_t  HTU21D_ReadRawHumidity    ( I2C_parameters_t myI2Cparameters, HTU21D_vector_data_t* myHumidity )
{
    uint8_t      myRawRH[]           =    { 0, 0, 0};
    i2c_status_t aux;


    aux = i2c_read ( myI2Cparameters, &myRawRH[0], 3 );


    myHumidity->RelativeHumidity    =   ( myRawRH[0] << 8 ) | myRawRH[1];



    if ( aux == I2C_SUCCESS )
       return   HTU21D_SUCCESS;
    else
       return   HTU21D_FAILURE;
}


/**
 * @brief       HTU21D_BatteryStatus   ( I2C_parameters_t , HTU21D_vector_data_t* )
 * @details     Read the user register to check the battery status.
 *
 * @param[in]    myI2Cparameters:   I2C parameters.
 * @param[in]    myBattStatus:      Variable to store the battery status.
 *
 * @param[out]   Status of HTU21D_BatteryStatus.
 *
 *
 * @return      NA
 *
 * @author      Manuel Caballero
 * @date        6/July/2017
 * @version     22/October/2017    The library was improved, better architecture.
 *              18/October/2017    Adapted to the new I2C driver.
 *              6/July/2017        The ORIGIN
 * @pre         NaN
 * @warning     NaN.
 */
HTU21D_status_t  HTU21D_BatteryStatus      ( I2C_parameters_t myI2Cparameters, HTU21D_vector_data_t* myBattStatus )
{
    uint8_t      cmd               =   HTU21D_READ_REGISTER;
    i2c_status_t aux;


    aux = i2c_write ( myI2Cparameters, &cmd, 1, I2C_STOP_BIT );
    aux = i2c_read  ( myI2Cparameters, &cmd, 1 );


    myBattStatus->BatteryStatus   =   ( cmd & USER_REGISTER_STATUS_END_BATTERY_MASK );



    if ( aux == I2C_SUCCESS )
       return   HTU21D_SUCCESS;
    else
       return   HTU21D_FAILURE;
}
