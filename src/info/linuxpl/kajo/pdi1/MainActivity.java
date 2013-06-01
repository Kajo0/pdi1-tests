package info.linuxpl.kajo.pdi1;

import com.qualcomm.QCARSamples.Dominoes.Dominoes;

import android.os.Bundle;
import android.app.Activity;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;

public class MainActivity extends Activity implements OnClickListener {

	public static String USER_PREFERENCES = "user_preferences";

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		SharedPreferences preferences = getSharedPreferences(
				MainActivity.USER_PREFERENCES, 0);

		TextView lt = (TextView) findViewById(R.id.login_label);
		TextView et = (TextView) findViewById(R.id.email_label);

		lt.setText(preferences.getString("login", "Anonymous"));
		et.setText(preferences.getString("email", "em@a.il"));

		((Button) findViewById(R.id.recognize_button)).setOnClickListener(this);
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		getMenuInflater().inflate(R.menu.main, menu);

		return true;
	}

	public boolean onOptionsItemSelected(MenuItem item) {
		switch (item.getItemId()) {
		case R.id.deactivate_auto_login:
			deactivateAutoLogin();
			return true;
		default:
			return super.onOptionsItemSelected(item);
		}
	}

	private void deactivateAutoLogin() {
		SharedPreferences preferences = getSharedPreferences(
				MainActivity.USER_PREFERENCES, 0);
		Editor editor = preferences.edit();

		editor.putBoolean("auto_login", false);

		editor.commit();
	}

	@Override
	public void onBackPressed() {
		finish();
	}

	@Override
	public void onClick(View v) {
		switch (v.getId()) {
		case R.id.recognize_button:
			 Intent myIntent = new Intent(this, Dominoes.class);
			 startActivity(myIntent);
			break;
		}

	}

}
