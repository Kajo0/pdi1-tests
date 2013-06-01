/*============================================================================
            Copyright (c) 2010-2013 QUALCOMM Austria Research Center GmbH.
            All Rights Reserved.
            Qualcomm Confidential and Proprietary
  ============================================================================*/


#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <sys/time.h>
#include <math.h>

#include <vector>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <QCAR/QCAR.h>
#include <QCAR/UpdateCallback.h>
#include <QCAR/CameraDevice.h>
#include <QCAR/Renderer.h>
#include <QCAR/VideoBackgroundConfig.h>
#include <QCAR/Trackable.h>
#include <QCAR/TrackableResult.h>
#include <QCAR/ImageTargetResult.h>
#include <QCAR/VirtualButtonResult.h>
#include <QCAR/Tool.h>
#include <QCAR/Tracker.h>
#include <QCAR/TrackerManager.h>
#include <QCAR/ImageTracker.h>
#include <QCAR/CameraCalibration.h>
#include <QCAR/ImageTarget.h>
#include <QCAR/VirtualButton.h>
#include <QCAR/Rectangle.h>
#include <QCAR/DataSet.h>


JavaVM* javaVM;
jobject javaObj;
jclass javaClass;

jmethodID jmethod_displayMessage;
jmethodID jmethod_showTrackerButton;
jmethodID jmethod_hideTrackerButton;

bool displayedMessage;


#ifdef __cplusplus
extern "C"
{
#endif
    
    // OpenGL ES 2.0 specific:
    extern unsigned int shaderProgramID;
    extern GLint vertexHandle;
    extern GLint normalHandle;
    extern GLint textureCoordHandle;
    extern GLint mvpMatrixHandle;
    extern GLint texSampler2DHandle;
    
    extern int screenWidth;
    extern int screenHeight;
    
    extern QCAR::Matrix44F projectionMatrix;
    
    extern QCAR::DataSet* dataSet;
        
    JNIEXPORT jint JNICALL
    JNI_OnLoad(JavaVM* vm, void* nReserved);
    
    JNIEXPORT void JNICALL
    Java_com_qualcomm_QCARSamples_Dominoes_Dominoes_onQCARInitializedNative(JNIEnv* env, jobject);
    
    JNIEXPORT void JNICALL
    Java_com_qualcomm_QCARSamples_Dominoes_DominoesRenderer_initNativeCallback(JNIEnv* env, jobject obj);
    
    JNIEXPORT void JNICALL
    Java_com_qualcomm_QCARSamples_Dominoes_DominoesRenderer_renderFrame(JNIEnv*, jobject);
    
#ifdef __cplusplus
}
#endif

QCAR::Vec2F cameraPointToScreenPoint(QCAR::Vec2F cameraPoint);

void showTrackerButton(int x, int y, const char* name);
void hideTrackerButton(std::vector<const char*> vec);
void displayMessage(char* message);

JNIEnv* getJNIEnv();
