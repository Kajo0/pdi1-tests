package info.linuxpl.kajo.pdi1.db.remote;

import java.sql.ResultSet;

import info.linuxpl.kajo.pdi1.db.remote.beans.LoggedUser;
import info.linuxpl.kajo.pdi1.db.remote.beans.User;
import info.linuxpl.kajo.pdi1.db.remote.tables.LoggedUsersTable;
import info.linuxpl.kajo.pdi1.db.remote.tables.UsersTable;

public class BeanConverterFactory {

	public final static ResultSetToBean<User> userConverter = new ResultSetToBean<User>() {

		@Override
		public User convert(ResultSet rs) throws Exception {
			User u = new User();

			u.setId(rs.getInt(UsersTable.ID));
			u.setLogin(rs.getString(UsersTable.LOGIN));
			u.setPassword(rs.getString(UsersTable.PASSWORD));
			u.setEmail(rs.getString(UsersTable.EMAIL));
			u.setLastLogin(rs.getLong(UsersTable.LAST_LOGIN));

			return u;
		}

	};

	public final static ResultSetToBean<LoggedUser> loggedUserConverter = new ResultSetToBean<LoggedUser>() {

		@Override
		public LoggedUser convert(ResultSet rs) throws Exception {
			LoggedUser lu = new LoggedUser();

			lu.setId(rs.getInt(LoggedUsersTable.ID));

			return lu;
		}

	};

}
