/*==============================================================================
            Copyright (c) 2010-2013 QUALCOMM Austria Research Center GmbH.
            All Rights Reserved.
            Qualcomm Confidential and Proprietary

@file
    DominoesRenderer.java

@brief
    Sample for Dominoes

==============================================================================*/


package com.qualcomm.QCARSamples.Dominoes;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.opengl.GLSurfaceView;
import android.os.Message;

import com.qualcomm.QCAR.QCAR;


/** The renderer class for the Dominoes sample. */
public class DominoesRenderer implements GLSurfaceView.Renderer
{
    public boolean mIsActive = false;

    // GUI managers:
    private GUIManager mGUIManager;


    /** Native function for initializing the renderer. */
    public native void initRendering();


    /** Native function to update the renderer. */
    public native void updateRendering(int width, int height);


    /** Native function to store Java environment information for callbacks. */
    public native void initNativeCallback();


    /** Called when the surface is created or recreated. */
    public void onSurfaceCreated(GL10 gl, EGLConfig config)
    {
        DebugLog.LOGD("GLRenderer::onSurfaceCreated");

        // Call native function to initialize rendering:
        initRendering();

        // Call QCAR function to (re)initialize rendering after first use
        // or after OpenGL ES context was lost (e.g. after onPause/onResume):
        QCAR.onSurfaceCreated();

        // Call native function to store information about the Java environment
        // It is important that we make this call from this thread (the
        // rendering thread) as the native code will want to make callbacks
        // from this thread:
        initNativeCallback();
    }


    /** Called when the surface changed size. */
    public void onSurfaceChanged(GL10 gl, int width, int height)
    {
        DebugLog.LOGD("GLRenderer::onSurfaceChanged");

        // Call native function to update rendering when render surface
        // parameters have changed:
        updateRendering(width, height);

        // Call QCAR function to handle render surface size changes:
        QCAR.onSurfaceChanged(width, height);
    }


    /** The native render function. */
    public native void renderFrame();


    /** Called to draw the current frame. */
    public void onDrawFrame(GL10 gl)
    {
        if (!mIsActive)
            return;

        // Call our native function to render content:
        renderFrame();
    }

    /** Called from native to display a message. */
    public void displayMessage(String text)
    {
        Message message = new Message();
        message.what = GUIManager.DISPLAY_INFO_TOAST;
        message.obj = text;
        mGUIManager.sendThreadSafeGUIMessage(message);
    }

    public void showTrackerButton(int x, int y, String name) {
    	 Message message = new Message();
         message.what = GUIManager.SHOW_TRACKER_BUTTON;
         message.arg1 = x;
         message.arg2 = y;
         message.obj = name;
         mGUIManager.sendThreadSafeGUIMessage(message);
    }
    
    public void hideTrackerButton(String found) {
    	 Message message = new Message();
         message.what = GUIManager.HIDE_TRACKER_BUTTON;
         message.obj = found;
         mGUIManager.sendThreadSafeGUIMessage(message);
    }


    /** Setter for the GUI manager. */
    public void setGUIManager(GUIManager guiManager)
    {
        mGUIManager = guiManager;
    }
}
