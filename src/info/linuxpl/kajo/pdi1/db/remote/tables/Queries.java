package info.linuxpl.kajo.pdi1.db.remote.tables;

import java.util.ArrayList;

import org.json.JSONException;

public interface Queries<B> {

	int insert(B bean);

	int delete(String sql);

	int update(String sql);

	ArrayList<B> select(String sql);

	ArrayList<B> createObjects(String jsonResult) throws JSONException;

}
