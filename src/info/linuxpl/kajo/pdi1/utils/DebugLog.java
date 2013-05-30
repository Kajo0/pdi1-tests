package info.linuxpl.kajo.pdi1.utils;

import android.util.Log;

public class DebugLog {
	private static final String LOG_TAG = "KJ";

	public static final void LOGE(String msg) {
		Log.e(LOG_TAG, msg);
	}

	public static final void LOGW(String msg) {
		Log.w(LOG_TAG, msg);
	}

	public static final void LOGD(String msg) {
		Log.d(LOG_TAG, msg);
	}

	public static final void LOGI(String msg) {
		Log.i(LOG_TAG, msg);
	}
}
