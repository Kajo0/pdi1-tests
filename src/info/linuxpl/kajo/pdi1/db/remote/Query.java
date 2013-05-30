package info.linuxpl.kajo.pdi1.db.remote;

import java.sql.PreparedStatement;

public interface Query {
	void prepareQuery(PreparedStatement ps) throws Exception;
}
