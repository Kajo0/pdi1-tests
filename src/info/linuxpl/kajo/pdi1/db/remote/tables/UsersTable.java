package info.linuxpl.kajo.pdi1.db.remote.tables;

import java.util.ArrayList;

import org.apache.http.NameValuePair;
import org.apache.http.message.BasicNameValuePair;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import info.linuxpl.kajo.pdi1.db.remote.beans.User;
import info.linuxpl.kajo.pdi1.utils.CustomHttpClient;

public class UsersTable extends Table<User> {

	public final static String TABLE_NAME = "users";

	public final static String ID = "user_id";
	public final static String LOGIN = "user_login";
	public final static String PASSWORD = "user_password";
	public final static String EMAIL = "user_email";
	public final static String LAST_LOGIN = "user_last_login";

	// public static boolean update(final PreparedStatement stmt, String sql) {
	// return DBManager.run(new Task<Boolean>() {
	// public Boolean execute(PreparedStatement ps) throws Exception {
	// ps = stmt;
	//
	// return ps.executeUpdate() > 0;
	// }
	// }, sql);
	// }

	@Override
	public ArrayList<User> createObjects(String jsonResult)
			throws JSONException {
		ArrayList<User> list = new ArrayList<User>();
		JSONArray ar = new JSONArray(jsonResult);

		for (int i = 0; i < ar.length(); i++) {
			JSONObject data = ar.getJSONObject(i);

			User u = new User();

			u.setId(data.getInt(ID));
			u.setLogin(data.getString(LOGIN));
			u.setPassword(data.getString(PASSWORD));
			u.setEmail(data.getString(EMAIL));
			u.setLastLogin(data.isNull(LAST_LOGIN) ? 0 : data
					.getLong(LAST_LOGIN));

			list.add(u);
		}

		return list;
	}

	@Override
	public ArrayList<User> select(String sql) {
		ArrayList<NameValuePair> params = new ArrayList<NameValuePair>();
		params.add(new BasicNameValuePair("query", sql));

		try {
			return createObjects(CustomHttpClient.executeHttpPost(sUrl, params));
		} catch (Exception e) {
			e.printStackTrace();

			return new ArrayList<User>();
		}
	}

	@Override
	public int insert(User bean) {
		// TODO md5
		String sql = String.format(
				"insert into %s (%s, %s, %s) values ('%s', '%s', '%s')",
				TABLE_NAME, LOGIN, PASSWORD, EMAIL, bean.getLogin(),
				bean.getPassword(), bean.getEmail());

		ArrayList<NameValuePair> params = new ArrayList<NameValuePair>();
		params.add(new BasicNameValuePair("query", sql));

		try {
			String result = CustomHttpClient.executeHttpPost(sUrl, params);

			System.out.println(result);

			return 1;
		} catch (Exception e) {
			e.printStackTrace();

			return -1;
		}
	}

	@Override
	public int delete(String sql) {
		// TODO Auto-generated method stub
		return 0;
	}

	@Override
	public int update(String sql) {
		// TODO Auto-generated method stub
		return 0;
	}

}
