package bioGUI;

import java.sql.Connection;
import java.sql.DriverManager;

/**
 * Created by mjopp on 04.10.2016.
 */
public class GlobalSettings {

    public static int m_iDBPort = 3306;
    public static String m_sDBHost = "localhost";

    public static String m_sDBName = "biogui";
    public static String m_sDBUser = "root";
    public static String m_sDBPW = null;

    public static Connection getDB()
    {
        return GlobalSettings.getDB( m_sDBHost, m_iDBPort, m_sDBName, m_sDBUser, m_sDBPW );
    }

    public static Connection getDB(String sHost, int iPort, String sDBName, String sDBUser, String sDBPW)
    {
        String sConnectionString = "jdbc:mysql://"+sHost+":"+iPort+"/"+sDBName+"?user=" + sDBUser;

        if (sDBPW != null)
        {
            sConnectionString += "&password=" + sDBPW;
        }

        try {
            Connection connect = DriverManager.getConnection( sConnectionString );

            return connect;
        } catch (Exception e) {
            return null;
        }
    }

}
