/*==============================================================================
            Copyright (c) 2010-2013 QUALCOMM Austria Research Center GmbH.
            All Rights Reserved.
            Qualcomm Confidential and Proprietary
            
@file 
    SampleUtils.h

@brief
    A utility class.

==============================================================================*/


#ifndef _QCAR_SAMPLEUTILS_H_
#define _QCAR_SAMPLEUTILS_H_

// Includes:
#include <android/log.h>

// Utility for logging:
#define LOG_TAG    "QCAR"
#define LOG(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

#endif // _QCAR_SAMPLEUTILS_H_
