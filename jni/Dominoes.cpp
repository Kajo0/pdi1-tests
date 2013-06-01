/*============================================================================
            Copyright (c) 2010-2013 QUALCOMM Austria Research Center GmbH.
            All Rights Reserved.
            Qualcomm Confidential and Proprietary
  ============================================================================*/


#include "Dominoes.h"
#include "SampleUtils.h"

#include <string>

// ----------------------------------------------------------------------------
// Native functions called from Java
// ----------------------------------------------------------------------------

JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM* vm, void* nReserved)
{
    // Store VM so that we can retrieve the JNI environment any time from
    // any thread
    javaVM = vm;

    return JNI_VERSION_1_4;
}


JNIEXPORT void JNICALL
Java_com_qualcomm_QCARSamples_Dominoes_Dominoes_onQCARInitializedNative(JNIEnv* env, jobject)
{
    // Initialize the globals
    // Note: the GL context is not set up at this point
    
    jmethod_showTrackerButton = NULL;
    jmethod_hideTrackerButton = NULL;
    jmethod_displayMessage = NULL;

    displayedMessage = false;

    //QCAR::setHint(QCAR::HINT_MAX_SIMULTANEOUS_IMAGE_TARGETS, 2);
}


JNIEXPORT void JNICALL
Java_com_qualcomm_QCARSamples_Dominoes_DominoesRenderer_initNativeCallback(JNIEnv* env, jobject obj)
{
    // Store the calling object for later use
    // Make a global reference to keep it valid beyond the scope of this function
    javaObj = env->NewGlobalRef(obj);
    
    // Store the class of the calling object for later use
    jclass objClass = env->GetObjectClass(obj);
    javaClass = (jclass) env->NewGlobalRef(objClass);

    // Store class method IDs for later use
    JNIEnv* jniEnv = getJNIEnv();
    jmethod_showTrackerButton = jniEnv->GetMethodID(javaClass, "showTrackerButton", "(IILjava/lang/String;)V");
    jmethod_hideTrackerButton = jniEnv->GetMethodID(javaClass, "hideTrackerButton", "(Ljava/lang/String;)V");
    jmethod_displayMessage = jniEnv->GetMethodID(javaClass, "displayMessage", "(Ljava/lang/String;)V");
}


JNIEXPORT void JNICALL
Java_com_qualcomm_QCARSamples_Dominoes_DominoesRenderer_renderFrame(JNIEnv* , jobject)
{
    // Clear the color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Get the state from QCAR and mark the beginning of a rendering section
    QCAR::State state = QCAR::Renderer::getInstance().begin();
    
    // Explicitly render the Video Background
    QCAR::Renderer::getInstance().drawVideoBackground();
    
    // to no to hide
	std::vector<const char*> found;

    // Did we find any trackables this frame?
    if (state.getNumTrackableResults() > 0) {

    	for(int tIdx = 0; tIdx < state.getNumTrackableResults(); ++tIdx) {

			// Get the first trackable
			const QCAR::TrackableResult* trackableResult = state.getTrackableResult(tIdx);
			const QCAR::Trackable& trackable = trackableResult->getTrackable();

			found.push_back(trackable.getName());

			// The image target specific result:
			assert(trackableResult->getType() == QCAR::TrackableResult::IMAGE_TARGET_RESULT);
			const QCAR::ImageTargetResult* imageTargetResult =
				static_cast<const QCAR::ImageTargetResult*>(trackableResult);

			// If this is our first time seeing the target, display a tip
			if (!displayedMessage) {
				displayMessage("Find marker man!");
				displayedMessage = true;
			}


			//const QCAR::TrackerManager& trackerManager = QCAR::TrackerManager::getInstance();
			//const QCAR::Tracker* tracker = trackerManager.getTracker(QCAR::Tracker::IMAGE_TRACKER);
			const QCAR::CameraCalibration& cameraCalibration = QCAR::CameraDevice::getInstance().getCameraCalibration();

			QCAR::Vec2F cameraPoint = QCAR::Tool::projectPoint(cameraCalibration, trackableResult->getPose(), QCAR::Vec3F(0,0,0));
			QCAR::Vec2F xyPoint = cameraPointToScreenPoint(cameraPoint);

			showTrackerButton(xyPoint.data[0], xyPoint.data[1], trackable.getName());

		}

    } else {
    	hideTrackerButton(found);
    }
    
    QCAR::Renderer::getInstance().end();
}


extern bool isActivityInPortraitMode;

QCAR::Vec2F cameraPointToScreenPoint(QCAR::Vec2F cameraPoint)
{
    QCAR::VideoMode videoMode = QCAR::CameraDevice::getInstance().getVideoMode(QCAR::CameraDevice::MODE_DEFAULT);
    QCAR::VideoBackgroundConfig config = QCAR::Renderer::getInstance().getVideoBackgroundConfig();
    int xOffset = ((int) screenWidth - config.mSize.data[0]) / 2.0f + config.mPosition.data[0];
    int yOffset = ((int) screenHeight - config.mSize.data[1]) / 2.0f - config.mPosition.data[1];
    if (isActivityInPortraitMode)
    {
        // camera image is rotated 90 degrees
        int rotatedX = videoMode.mHeight - cameraPoint.data[1];
        int rotatedY = cameraPoint.data[0];
        return QCAR::Vec2F(rotatedX * config.mSize.data[0] / (float) videoMode.mHeight + xOffset,
                           rotatedY * config.mSize.data[1] / (float) videoMode.mWidth + yOffset);
    }
    else
    {
        return QCAR::Vec2F(cameraPoint.data[0] * config.mSize.data[0] / (float) videoMode.mWidth + xOffset,
                           cameraPoint.data[1] * config.mSize.data[1] / (float) videoMode.mHeight + yOffset);
    }
}

// ----------------------------------------------------------------------------
// Java static methods called from native
// ----------------------------------------------------------------------------

void
displayMessage(char* message)
{
    // Call a Java method that displays a message via a toast
    JNIEnv* jniEnv = getJNIEnv();
    jstring js = jniEnv->NewStringUTF(message);
    jniEnv->CallVoidMethod(javaObj, jmethod_displayMessage, js);
}

void
showTrackerButton(int x, int y, const char* name)
{
    // Call a Java method that displays a message via a toast
    JNIEnv* jniEnv = getJNIEnv();
    jstring js = jniEnv->NewStringUTF(name);
    jniEnv->CallVoidMethod(javaObj, jmethod_showTrackerButton, x, y, js);
}

void
hideTrackerButton(std::vector<const char*> vec)
{
    JNIEnv* jniEnv = getJNIEnv();

    std::string str = "";

    for (int i = 0 ; i < vec.size(); ++i) {
    	if (i != 0)
    		str += "|";
    	str += vec[i];
    }

    jstring js = jniEnv->NewStringUTF(str.c_str());

    jniEnv->CallVoidMethod(javaObj, jmethod_hideTrackerButton, js);
}



// ----------------------------------------------------------------------------
// JNI utility
// ----------------------------------------------------------------------------

JNIEnv*
getJNIEnv() {
    JNIEnv* env;
    if (javaVM == NULL || javaVM->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK) {
        return NULL;
    }

    return env;
} 

