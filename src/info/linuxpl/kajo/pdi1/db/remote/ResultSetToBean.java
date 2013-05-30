package info.linuxpl.kajo.pdi1.db.remote;

import java.sql.ResultSet;

public interface ResultSetToBean<BeanType> {
	BeanType convert(ResultSet rs) throws Exception;
}
