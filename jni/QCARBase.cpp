/*==============================================================================
            Copyright (c) 2010-2013 QUALCOMM Austria Research Center GmbH.
            All Rights Reserved.
            Qualcomm Confidential and Proprietary
            
@file 
    QCARBase.cpp

@brief
    QCAR basic initialization for Dominoes

==============================================================================*/


#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <QCAR/QCAR.h>
#include <QCAR/UpdateCallback.h>
#include <QCAR/CameraDevice.h>
#include <QCAR/Renderer.h>
#include <QCAR/VideoBackgroundConfig.h>
#include <QCAR/Trackable.h>
#include <QCAR/Tool.h>
#include <QCAR/Tracker.h>
#include <QCAR/TrackerManager.h>
#include <QCAR/ImageTracker.h>
#include <QCAR/CameraCalibration.h>
#include <QCAR/DataSet.h>

#include "SampleUtils.h"

#ifdef __cplusplus
extern "C"
{
#endif

// OpenGL ES 2.0 specific:
unsigned int shaderProgramID    = 0;
GLint vertexHandle              = 0;
GLint normalHandle              = 0;
GLint textureCoordHandle        = 0;
GLint mvpMatrixHandle           = 0;
GLint texSampler2DHandle        = 0;

// Screen dimensions:
unsigned int screenWidth        = 0;
unsigned int screenHeight       = 0;

QCAR::Matrix44F projectionMatrix;

// Indicates whether screen is in portrait (true) or landscape (false) mode
bool isActivityInPortraitMode   = false;

// Constants:
static const float kObjectScale = 3.f;

QCAR::DataSet* dataSet          = 0;
QCAR::DataSet* dataSetStonesAndChips = 0;


JNIEXPORT void JNICALL
Java_com_qualcomm_QCARSamples_Dominoes_Dominoes_setActivityPortraitMode(JNIEnv *, jobject, jboolean isPortrait)
{
    isActivityInPortraitMode = isPortrait;
}


JNIEXPORT int JNICALL
Java_com_qualcomm_QCARSamples_Dominoes_Dominoes_initTracker(JNIEnv *, jobject)
{
    LOG("Java_com_qualcomm_QCARSamples_Dominoes_Dominoes_initTracker");
    
    // Initialize the image tracker:
    QCAR::TrackerManager& trackerManager = QCAR::TrackerManager::getInstance();
    QCAR::Tracker* tracker = trackerManager.initTracker(QCAR::Tracker::IMAGE_TRACKER);
    if (tracker == NULL)
    {
        LOG("Failed to initialize ImageTracker.");
        return 0;
    }
    
    LOG("Successfully initialized ImageTracker.");
    return 1;
}


JNIEXPORT void JNICALL
Java_com_qualcomm_QCARSamples_Dominoes_Dominoes_deinitTracker(JNIEnv *, jobject)
{
    LOG("Java_com_qualcomm_QCARSamples_Dominoes_Dominoes_deinitTracker");
    
    // Deinit the image tracker:
    QCAR::TrackerManager& trackerManager = QCAR::TrackerManager::getInstance();
    trackerManager.deinitTracker(QCAR::Tracker::IMAGE_TRACKER);
}


JNIEXPORT int JNICALL
Java_com_qualcomm_QCARSamples_Dominoes_Dominoes_loadTrackerData(JNIEnv *, jobject)
{
    LOG("Java_com_qualcomm_QCARSamples_Dominoes_Dominoes_loadTrackerData");
    
    // Get the image tracker:
    QCAR::TrackerManager& trackerManager = QCAR::TrackerManager::getInstance();
    QCAR::ImageTracker* imageTracker = static_cast<QCAR::ImageTracker*>(
                    trackerManager.getTracker(QCAR::Tracker::IMAGE_TRACKER));
    if (imageTracker == NULL)
    {
        LOG("Failed to load tracking data set because the ImageTracker has not"
            " been initialized.");
        return 0;
    }
    
    // Create the data set:
    dataSet = imageTracker->createDataSet();
    if (dataSet == 0)
    {
        LOG("Failed to create a new tracking data.");
        return 0;
    }
    
    // Load the data set:
//    if (!dataSet->load("StonesAndChips.xml", QCAR::DataSet::STORAGE_APPRESOURCE))
    if (!dataSet->load("PDI1.xml", QCAR::DataSet::STORAGE_APPRESOURCE))
    {
        LOG("Failed to load data set.");
        return 0;
    }
    
    // Activate the data set:
    if (!imageTracker->activateDataSet(dataSet))
    {
        LOG("Failed to activate data set.");
        return 0;
    }
    
    // Create the data set:
    dataSetStonesAndChips = imageTracker->createDataSet();
    if (dataSetStonesAndChips == 0)
    {
        LOG("Failed to create a new tracking data.");
        return 0;
    }

    // Load the data set:
    if (!dataSetStonesAndChips->load("StonesAndChips.xml", QCAR::DataSet::STORAGE_APPRESOURCE))
    {
        LOG("Failed to load data set.");
        return 0;
    }

    // Activate the data set:
    if (!imageTracker->activateDataSet(dataSetStonesAndChips))
    {
        LOG("Failed to activate data set.");
        return 0;
    }

    LOG("Successfully loaded and activated data set.");
    return 1;
}


JNIEXPORT int JNICALL
Java_com_qualcomm_QCARSamples_Dominoes_Dominoes_destroyTrackerData(JNIEnv *, jobject)
{
    LOG("Java_com_qualcomm_QCARSamples_Dominoes_Dominoes_destroyTrackerData");
    
    // Get the image tracker:
    QCAR::TrackerManager& trackerManager = QCAR::TrackerManager::getInstance();
    QCAR::ImageTracker* imageTracker = static_cast<QCAR::ImageTracker*>(
                    trackerManager.getTracker(QCAR::Tracker::IMAGE_TRACKER));
    if (imageTracker == NULL)
    {
        LOG("Failed to destroy the tracking data set because the ImageTracker has not"
            " been initialized.");
        return 0;
    }
    
    if (dataSet != 0)
    {
        if (!imageTracker->deactivateDataSet(dataSet))
        {
            LOG("Failed to destroy the tracking data set because the data set "
                "could not be deactivated.");
            return 0;
        }
        
        if (!imageTracker->destroyDataSet(dataSet))
        {
            LOG("Failed to destroy the tracking data set.");
            return 0;
        }
        
        LOG("Successfully destroyed the data set.");
        dataSet = 0;
        return 1;
    }

    if (dataSetStonesAndChips != 0)
    {
        if (!imageTracker->deactivateDataSet(dataSetStonesAndChips))
        {
            LOG("Failed to destroy the tracking data set because the data set "
                "could not be deactivated.");
            return 0;
        }

        if (!imageTracker->destroyDataSet(dataSetStonesAndChips))
        {
            LOG("Failed to destroy the tracking data set.");
            return 0;
        }

        LOG("Successfully destroyed the data set.");
        dataSetStonesAndChips = 0;
        return 1;
    }
    
    LOG("No tracker data set to destroy.");
    return 0;
}


void
configureVideoBackground()
{
    // Get the default video mode:
    QCAR::CameraDevice& cameraDevice = QCAR::CameraDevice::getInstance();
    QCAR::VideoMode videoMode = cameraDevice.
                                getVideoMode(QCAR::CameraDevice::MODE_DEFAULT);

    // Configure the video background
    QCAR::VideoBackgroundConfig config;
    config.mEnabled = true;
    config.mSynchronous = true;
    config.mPosition.data[0] = 0.0f;
    config.mPosition.data[1] = 0.0f;
    
    if (isActivityInPortraitMode)
    {
        //LOG("configureVideoBackground PORTRAIT");
        config.mSize.data[0] = videoMode.mHeight
                                * (screenHeight / (float)videoMode.mWidth);
        config.mSize.data[1] = screenHeight;

        if(config.mSize.data[0] < screenWidth)
        {
            LOG("Correcting rendering background size to handle missmatch between screen and video aspect ratios.");
            config.mSize.data[0] = screenWidth;
            config.mSize.data[1] = screenWidth * 
                              (videoMode.mWidth / (float)videoMode.mHeight);
        }
    }
    else
    {
        //LOG("configureVideoBackground LANDSCAPE");
        config.mSize.data[0] = screenWidth;
        config.mSize.data[1] = videoMode.mHeight
                            * (screenWidth / (float)videoMode.mWidth);

        if(config.mSize.data[1] < screenHeight)
        {
            LOG("Correcting rendering background size to handle missmatch between screen and video aspect ratios.");
            config.mSize.data[0] = screenHeight
                                * (videoMode.mWidth / (float)videoMode.mHeight);
            config.mSize.data[1] = screenHeight;
        }
    }

    // Set the config:
    QCAR::Renderer::getInstance().setVideoBackgroundConfig(config);
}


JNIEXPORT void JNICALL
Java_com_qualcomm_QCARSamples_Dominoes_Dominoes_initApplicationNative(
                            JNIEnv* env, jobject obj, jint width, jint height)
{
    LOG("Java_com_qualcomm_QCARSamples_Dominoes_Dominoes_initApplicationNative");
    
    // Store screen dimensions
    screenWidth = width;
    screenHeight = height;
        
}


JNIEXPORT void JNICALL
Java_com_qualcomm_QCARSamples_Dominoes_Dominoes_deinitApplicationNative(
                                                        JNIEnv* env, jobject obj)
{
    LOG("Java_com_qualcomm_QCARSamples_Dominoes_Dominoes_deinitApplicationNative");
}


JNIEXPORT void JNICALL
Java_com_qualcomm_QCARSamples_Dominoes_Dominoes_startCamera(JNIEnv *a,
                                                                         jobject b)
{
    LOG("Java_com_qualcomm_QCARSamples_Dominoes_Dominoes_startCamera");

    // Initialize the camera:
    if (!QCAR::CameraDevice::getInstance().init())
        return;

    // Configure the video background
    configureVideoBackground();

    // Select the default mode:
    if (!QCAR::CameraDevice::getInstance().selectVideoMode(
                                QCAR::CameraDevice::MODE_DEFAULT))
        return;

    // Start the camera:
    if (!QCAR::CameraDevice::getInstance().start())
        return;

    // Start the tracker:
    QCAR::TrackerManager& trackerManager = QCAR::TrackerManager::getInstance();
    QCAR::Tracker* imageTracker = trackerManager.getTracker(QCAR::Tracker::IMAGE_TRACKER);
    if(imageTracker != 0)
        imageTracker->start();
}


JNIEXPORT void JNICALL
Java_com_qualcomm_QCARSamples_Dominoes_Dominoes_stopCamera(JNIEnv *,
                                                                   jobject)
{
    LOG("Java_com_qualcomm_QCARSamples_Dominoes_Dominoes_stopCamera");

    // Stop the tracker:
    QCAR::TrackerManager& trackerManager = QCAR::TrackerManager::getInstance();
    QCAR::Tracker* imageTracker = trackerManager.getTracker(QCAR::Tracker::IMAGE_TRACKER);
    if(imageTracker != 0)
        imageTracker->stop();
    
    QCAR::CameraDevice::getInstance().stop();
    QCAR::CameraDevice::getInstance().deinit();
}

JNIEXPORT void JNICALL
Java_com_qualcomm_QCARSamples_Dominoes_Dominoes_setProjectionMatrix(JNIEnv *, jobject)
{
    LOG("Java_com_qualcomm_QCARSamples_Dominoes_Dominoes_setProjectionMatrix");

    // Cache the projection matrix:
    const QCAR::CameraCalibration& cameraCalibration =
                                QCAR::CameraDevice::getInstance().getCameraCalibration();
    projectionMatrix = QCAR::Tool::getProjectionGL(cameraCalibration, 2.0f, 2500.0f);
}

JNIEXPORT jboolean JNICALL
Java_com_qualcomm_QCARSamples_Dominoes_Dominoes_autofocus(JNIEnv*, jobject)
{
    return QCAR::CameraDevice::getInstance().setFocusMode(QCAR::CameraDevice::FOCUS_MODE_TRIGGERAUTO) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_com_qualcomm_QCARSamples_Dominoes_Dominoes_setFocusMode(JNIEnv*, jobject, jint mode)
{
    int qcarFocusMode;

    switch ((int)mode)
    {
        case 0:
            qcarFocusMode = QCAR::CameraDevice::FOCUS_MODE_NORMAL;
            break;
        
        case 1:
            qcarFocusMode = QCAR::CameraDevice::FOCUS_MODE_CONTINUOUSAUTO;
            break;
            
        case 2:
            qcarFocusMode = QCAR::CameraDevice::FOCUS_MODE_INFINITY;
            break;
            
        case 3:
            qcarFocusMode = QCAR::CameraDevice::FOCUS_MODE_MACRO;
            break;
    
        default:
            return JNI_FALSE;
    }
    
    return QCAR::CameraDevice::getInstance().setFocusMode(qcarFocusMode) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT void JNICALL
Java_com_qualcomm_QCARSamples_Dominoes_DominoesRenderer_initRendering(
                                                    JNIEnv* env, jobject obj)
{
    LOG("Java_com_qualcomm_QCARSamples_Dominoes_DominoesRenderer_initRendering");

    // Define clear color
    glClearColor(0.0f, 0.0f, 0.0f, QCAR::requiresAlpha() ? 0.0f : 1.0f);
    
    vertexHandle        = glGetAttribLocation(shaderProgramID,
                                                "vertexPosition");
    normalHandle        = glGetAttribLocation(shaderProgramID,
                                                "vertexNormal");
    textureCoordHandle  = glGetAttribLocation(shaderProgramID,
                                                "vertexTexCoord");
    mvpMatrixHandle     = glGetUniformLocation(shaderProgramID,
                                                "modelViewProjectionMatrix");
    texSampler2DHandle  = glGetUniformLocation(shaderProgramID, 
                                                "texSampler2D");

}


JNIEXPORT void JNICALL
Java_com_qualcomm_QCARSamples_Dominoes_DominoesRenderer_updateRendering(
                        JNIEnv* env, jobject obj, jint width, jint height)
{
    LOG("Java_com_qualcomm_QCARSamples_Dominoes_DominoesRenderer_updateRendering");
    
    // Update screen dimensions
    screenWidth = width;
    screenHeight = height;

    // Reconfigure the video background
    configureVideoBackground();
}


#ifdef __cplusplus
}
#endif
