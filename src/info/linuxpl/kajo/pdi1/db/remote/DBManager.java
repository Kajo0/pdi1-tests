package info.linuxpl.kajo.pdi1.db.remote;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;

/**
 *
 * @deprecated Only desktop work with...
 */
public class DBManager {

	private static String url = "jdbc:mysql://95.211.176.132:3306/kajo_pdi1";
	private static String username = "kajo_pdi1";
	private static String password = "pdi1";

	static {
		try {
			Class.forName("com.mysql.jdbc.Driver");
		} catch (ClassNotFoundException e) {
			 e.printStackTrace();
		}
	}

	public static Connection getConnection() throws SQLException {
		return DriverManager.getConnection(url, username, password);
	}

	public static PreparedStatement getEmptyPreparedStatement(String sql)
			throws SQLException {
		return getConnection().prepareStatement(sql);
	}

	public static <R> R run(Task<R> task, String sql) {
		R result = null;
		Connection conn = null;

		try {
			conn = getConnection();
			PreparedStatement stmt = null;

			try {
				stmt = conn.prepareStatement(sql);
				result = task.execute(stmt);

				conn.commit();

				return result;
			} catch (Exception e) {
				System.err.println("Cannot execute a statement : "
						+ e.getMessage());

				conn.rollback();

				throw new RuntimeException(e);
			} finally {
				if (stmt != null)
					stmt.close();
			}
		} catch (Exception e) {
			System.err.println("Cannot open a connection : " + e.getMessage());

			throw new RuntimeException(e);
		} finally {
			try {
				if (conn != null)
					conn.close();
			} catch (Exception ignore) {
			}
		}
	}

	public static <R, B> List<B> run(Query query, ResultSetToBean<B> converter,
			String sql) {
		Connection conn = null;

		try {
			conn = getConnection();
			List<B> list = new ArrayList<B>();
			PreparedStatement stmt = null;

			try {
				stmt = conn.prepareStatement(sql);
				query.prepareQuery(stmt);
				ResultSet rs = stmt.executeQuery();

				try {
					while (rs.next())
						list.add(converter.convert(rs));

					return list;
				} catch (Exception e) {
					System.err.println("Cannot convert bean : "
							+ e.getMessage());

					throw new RuntimeException(e);
				} finally {
					rs.close();
				}
			} catch (Exception e) {
				System.err.println("Cannot execute a statement : "
						+ e.getMessage());

				throw new RuntimeException(e);
			} finally {
				if (stmt != null)
					stmt.close();
			}
		} catch (Exception e) {
			System.err.println("Cannot open a connection : " + e.getMessage());

			throw new RuntimeException(e);
		} finally {
			try {
				if (conn != null)
					conn.close();
			} catch (Exception ignore) {
			}
		}
	}

}
