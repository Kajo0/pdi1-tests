package info.linuxpl.kajo.pdi1.db.remote.tables;

import java.util.ArrayList;

import info.linuxpl.kajo.pdi1.db.remote.beans.LoggedUser;

public class LoggedUsersTable extends Table<LoggedUser> {

	public final static String TABLE_NAME = "logged_users";

	public final static String ID = "logged_user_id";

	@Override
	public ArrayList<LoggedUser> createObjects(String jsonResult) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public int insert(LoggedUser bean) {
		// TODO Auto-generated method stub
		return 0;
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

	@Override
	public ArrayList<LoggedUser> select(String sql) {
		// TODO Auto-generated method stub
		return null;
	}

}
