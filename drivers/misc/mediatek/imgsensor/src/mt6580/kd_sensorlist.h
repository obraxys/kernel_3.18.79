/*
 * Copyright (C) 2015 MediaTek Inc.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

//s_add new sensor driver here
//export funtions
/*OV*/
UINT32 OV9760_MIPI_RAW_SensorInit(PSENSOR_FUNCTION_STRUCT *pfFunc);
UINT32 OV5693_MIPI_RAW_SensorInit(PSENSOR_FUNCTION_STRUCT *pfFunc);
/*SP*/
UINT32 SP2508_MIPI_RAW_SensorInit(PSENSOR_FUNCTION_STRUCT *pfFunc);
UINT32 SP5409_MIPI_RAW_SensorInit(PSENSOR_FUNCTION_STRUCT *pfFunc);

//! Add Sensor Init function here
//! Note:
//! 1. Add by the resolution from ""large to small"", due to large sensor
//!    will be possible to be main sensor.
//!    This can avoid I2C error during searching sensor.
//! 2. This file should be the same as mediatek\custom\common\hal\imgsensor\src\sensorlist.cpp
ACDK_KD_SENSOR_INIT_FUNCTION_STRUCT kdSensorList[MAX_NUM_OF_SUPPORT_SENSOR+1] =
{
/*OV (OmniVision)*/
#if defined(OV9760_MIPI_RAW)
    {OV9760_SENSOR_ID, SENSOR_DRVNAME_OV9760_MIPI_RAW, OV9760_MIPI_RAW_SensorInit},
#endif
#if defined(OV5693_MIPI_RAW)
    {OV5693_SENSOR_ID, SENSOR_DRVNAME_OV5693_MIPI_RAW, OV5693_MIPI_RAW_SensorInit},
#endif
/*SP*/
#if defined(SP2508_MIPI_RAW)
    {SP2508_SENSOR_ID, SENSOR_DRVNAME_SP2508_MIPI_RAW, SP2508_MIPI_RAW_SensorInit},
#endif
#if defined(SP5409_MIPI_RAW)
    {SP5409_SENSOR_ID, SENSOR_DRVNAME_SP5409_MIPI_RAW, SP5409_MIPI_RAW_SensorInit},
#endif

/*  ADD sensor driver before this line */
    {0,{0},NULL}, //end of list
};
//e_add new sensor driver here

