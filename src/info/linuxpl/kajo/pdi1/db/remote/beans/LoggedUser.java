package info.linuxpl.kajo.pdi1.db.remote.beans;

public class LoggedUser {

	private int id;

	public LoggedUser() {

	}

	public LoggedUser(int id) {
		this.id = id;
	}

	public int getId() {
		return id;
	}

	public void setId(int id) {
		this.id = id;
	}

	public String toString() {
		return "ID: " + id;
	}

}
