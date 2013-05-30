package info.linuxpl.kajo.pdi1;

import java.util.ArrayList;

import info.linuxpl.kajo.pdi1.db.remote.beans.User;
import info.linuxpl.kajo.pdi1.db.remote.tables.UsersTable;
import android.net.ConnectivityManager;
import android.os.AsyncTask;
import android.os.Bundle;
import android.app.Activity;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.graphics.Color;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

public class LoginActivity extends Activity {

	private boolean isAutoLogin() {
		SharedPreferences preferences = getSharedPreferences(
				MainActivity.USER_PREFERENCES, 0);

		return preferences.getBoolean("auto_login", false);
	}

	private void updateUserInfo(User user) {
		// TODO poitns etc. shared all app or sqlite
		SharedPreferences preferences = getSharedPreferences(
				MainActivity.USER_PREFERENCES, 0);
		Editor editor = preferences.edit();

		editor.putBoolean("auto_login",
				((CheckBox) findViewById(R.id.auto_login_checkbox)).isChecked());
		editor.putString("login", user.getLogin());
		editor.putString("email", user.getEmail());

		editor.commit();
	}

	private void moveOn() {
		// Check internet access
		ConnectivityManager connectivityManager = (ConnectivityManager) getSystemService(Context.CONNECTIVITY_SERVICE);

		if (connectivityManager.getActiveNetworkInfo() == null) {
			Toast.makeText(LoginActivity.this, "No internet access!",
					Toast.LENGTH_SHORT).show();

			return;
		}

		Intent myIntent = new Intent(getApplicationContext(),
				MainActivity.class);
		// myIntent.putExtra("user", user);
		finish();
		startActivity(myIntent);
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		if (isAutoLogin()) {
			moveOn();
		}

		setContentView(R.layout.activity_login);

		TextView t = (TextView) findViewById(R.id.scroll_text);
		t.setText("1111111111111111\n1111111\n\n111111111\n1111111111111111\n1111111111111111\n1111\n1111\n11111111\n1111111111111111\n1111111111111111\n1111111111111111\n1111111111111111\n");

		((Button) findViewById(R.id.login_button))
				.setOnClickListener(new OnClickListener() {

					@Override
					public void onClick(View v) {

						String login = ((EditText) findViewById(R.id.login))
								.getText().toString();
						String passwd = ((EditText) findViewById(R.id.password))
								.getText().toString();

						String sql = String.format(
								"select * from %s where %s='%s' and %s='%s'",
								UsersTable.TABLE_NAME, UsersTable.LOGIN, login,
								UsersTable.PASSWORD, passwd);

						new LoginTask().execute(sql);
					}
				});
	}

	private class LoginTask extends AsyncTask<String, Void, Integer> {

		ProgressDialog dialog = ProgressDialog.show(LoginActivity.this, "",
				"Loading, Please wait...");

		@Override
		protected Integer doInBackground(String... params) {
			ArrayList<User> ul = new UsersTable().select(params[0]);
			int r = ul.size();

			if (r == 1) {
				updateUserInfo(ul.get(0));
			}

			return r;
		}

		protected void onPreExecute() {
			dialog.show();
		}

		protected void onPostExecute(Integer result) {
			if (dialog != null)
				dialog.dismiss();

			if (result == 0) {
				((TextView) findViewById(R.id.scroll_text))
						.setText("Wrong sth!");

				((TextView) findViewById(R.id.scroll_text))
						.setTextColor(Color.RED);
			} else {
				moveOn();
			}
		}

	}
}
