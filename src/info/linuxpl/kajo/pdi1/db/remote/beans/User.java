package info.linuxpl.kajo.pdi1.db.remote.beans;

public class User {

	private int id;
	private String login;
	private String password;
	private String email;
	private long lastLogin;

	public User() {

	}

	public User(int id, String login, String password, String email,
			long lastLogin) {
		this.id = id;
		this.login = login;
		this.password = password;
		this.email = email;
		this.lastLogin = lastLogin;
	}

	public int getId() {
		return id;
	}

	public void setId(int id) {
		this.id = id;
	}

	public String getLogin() {
		return login;
	}

	public void setLogin(String login) {
		this.login = login;
	}

	public String getPassword() {
		return password;
	}

	public void setPassword(String password) {
		this.password = password;
	}

	public String getEmail() {
		return email;
	}

	public void setEmail(String email) {
		this.email = email;
	}

	public long getLastLogin() {
		return lastLogin;
	}

	public void setLastLogin(long lastLogin) {
		this.lastLogin = lastLogin;
	}

	public String toString() {
		return String
				.format("ID: %d \tLogin: %s \tPassword: %s \tEmail: %s \tLast login: %d",
						id, login, password, email, lastLogin);
	}

}
