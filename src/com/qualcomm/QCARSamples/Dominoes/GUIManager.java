/*==============================================================================
            Copyright (c) 2010-2013 QUALCOMM Austria Research Center GmbH.
            All Rights Reserved.
            Qualcomm Confidential and Proprietary
==============================================================================*/

package com.qualcomm.QCARSamples.Dominoes;

import info.linuxpl.kajo.pdi1.R;

import java.lang.ref.WeakReference;

import android.app.ActionBar.LayoutParams;
import android.content.Context;
import android.os.Handler;
import android.os.Message;
import android.view.View;
import android.widget.Button;
import android.widget.RelativeLayout;
import android.widget.Toast;

public class GUIManager {

    // Custom views:
    private View overlayView;
    private Button firstButton;
    private Button secondButton;

    // A Handler for working with the GUI from other threads:
    static class MyActivityHandler extends Handler
    {
        private WeakReference<GUIManager> guiManager;
        private Context context;

        MyActivityHandler(GUIManager guim, Context c)
        {
            guiManager = new WeakReference<GUIManager>(guim);
            context = c;
        }

        public void handleMessage(Message msg)
        {
            Button firstButton = guiManager.get().firstButton;
            Button secondButton = guiManager.get().secondButton;

			switch (msg.what)
            {
                case SHOW_TRACKER_BUTTON:
					if (firstButton != null && secondButton != null) {
						int x = msg.arg1;
						int y = msg.arg2;
						String name = (String) msg.obj;
						
						RelativeLayout.LayoutParams params = new RelativeLayout.LayoutParams(LayoutParams.WRAP_CONTENT,LayoutParams.WRAP_CONTENT);
						params.leftMargin = x - firstButton.getWidth() / 2;
						params.topMargin = y - firstButton.getHeight() / 2;

						firstButton.setLayoutParams(params);
						firstButton.setText(name);
						firstButton.setVisibility(View.VISIBLE);
					}
                    break;

				case HIDE_TRACKER_BUTTON:
						if (firstButton != null && secondButton != null) {
							firstButton.setVisibility(View.GONE);
							
//							String found = (String) msg.obj;
//		
//							if (!found.contains("stones"))
//								firstButton.setVisibility(View.GONE);
//							if (!found.contains("chips"))
//								secondButton.setVisibility(View.GONE);
						}
                    break;

                case DISPLAY_INFO_TOAST:
                    String text = (String) msg.obj;
                    int duration = Toast.LENGTH_LONG;
                    Toast toast =
                        Toast.makeText(context, text, duration);
                    toast.show();
                    break;

                default:
                    break;
            }
        }
    }
    
    private MyActivityHandler mainActivityHandler;

    // Flags for our Handler:
    public static final int SHOW_TRACKER_BUTTON = 0;
    public static final int HIDE_TRACKER_BUTTON = 1;
    public static final int DISPLAY_INFO_TOAST = 2;


    /** Initialize the GUIManager. */
    public GUIManager(Context context)
    {
        // Load our overlay view:
        // NOTE: This view will add content on top of the camera / OpenGL view
        overlayView = View.inflate(context, R.layout.interface_overlay, null);

        // Create a Handler from the current thread:
        // This is the only thread that can make changes to the GUI,
        // so we require a handler for other threads to make changes
        mainActivityHandler = new MyActivityHandler(this, context);
    }


    /** Button clicks should call corresponding native functions. */
    public void initButtons()
    {
        if (overlayView == null)
            return;

        firstButton = (Button) overlayView.findViewById(R.id.delete_button);
        firstButton.setOnClickListener(new View.OnClickListener()
        {
            public void onClick(View v)
            {
            	System.out.println("Clicked : " + ((Button)v).getText());
        		Toast.makeText(mainActivityHandler.context, ((Button)v).getText(), Toast.LENGTH_SHORT).show();
            }
        });

        secondButton = (Button) overlayView.findViewById(R.id.delete_button);
        secondButton.setOnClickListener(new View.OnClickListener()
        {
        	public void onClick(View v)
        	{
        		System.out.println("Clicked : " + ((Button)v).getText());
        		Toast.makeText(mainActivityHandler.context, ((Button)v).getText(), Toast.LENGTH_SHORT).show();
        	}
        });
    }


    /** Clear the button listeners. */
    public void deinitButtons()
    {
        if (overlayView == null)
            return;

        if (firstButton != null)
        {
            firstButton.setOnClickListener(null);
            firstButton = null;
        }

        if (secondButton != null)
        {
        	secondButton.setOnClickListener(null);
        	secondButton = null;
        }
    }


    /** Send a message to our gui thread handler. */
    public void sendThreadSafeGUIMessage(Message message)
    {
        mainActivityHandler.sendMessage(message);
    }


    /** Getter for the overlay view. */
    public View getOverlayView()
    {
        return overlayView;
    }
}
