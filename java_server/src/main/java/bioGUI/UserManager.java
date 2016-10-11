package bioGUI;

import java.sql.*;
import java.util.ArrayList;

/**
 * Created by joppich on 10/4/16.
 */
public class UserManager {

    private Connection connect = null;

    private ArrayList<User> m_vUsers;

    public UserManager()
    {
        this(GlobalSettings.m_sDBHost, GlobalSettings.m_iDBPort, GlobalSettings.m_sDBName, GlobalSettings.m_sDBUser, GlobalSettings.m_sDBPW);
    }

    public UserManager(String sHost, int iHost, String sDBName, String sDBUser, String sDBPW)
    {

        try
        {
            connect = GlobalSettings.getDB(sHost, iHost, sDBName, sDBUser, sDBPW);

            this.update();

        } catch (Exception e)
        {
            // do something or not
            e.printStackTrace();
        }


    }

    public void update()
    {

        try {
            PreparedStatement oStatement = connect.prepareStatement("select * from users;");
            ResultSet oReturn = oStatement.executeQuery();

            m_vUsers = this.retrieveFromDatabase(oReturn);
        } catch (Exception e)
        {
            // nothing
        }

    }

    public User getUser(int iUserID)
    {

        for (User oUser : m_vUsers)
        {
            if (oUser.getID() == iUserID)
                return oUser;
        }

        return null;
    }


    private ArrayList<User> retrieveFromDatabase(ResultSet resultSet) throws SQLException {

        ArrayList<User> vUsers = new ArrayList<User>();

        while (resultSet.next()) {
            // It is possible to get the columns via name
            // also possible to get the columns via the column number
            // which starts at 1
            // e.g. resultSet.getSTring(2);
            int iUserID = resultSet.getInt("id");
            String sName = resultSet.getString("name");
            String sEmail = resultSet.getString("email");

            User oUser = new User(iUserID, sName, sEmail);

            System.out.println( oUser );

            vUsers.add(oUser);
        }

        return vUsers;
    }

    int addUser(User oUser)
    {

        try
        {

            PreparedStatement oStatement = connect.prepareStatement("select * from users where name = '"+oUser.getName()+"' and email = '"+oUser.getEmail()+"'");
            ResultSet foundUsers = oStatement.executeQuery();
            ArrayList<User> vExactUsers = retrieveFromDatabase(foundUsers);

            if (vExactUsers.size() >0)
                return vExactUsers.get(0).getID();

            oStatement = connect.prepareStatement("insert into users (name, email) values ('"+oUser.getName()+"','"+oUser.getEmail()+"');", Statement.RETURN_GENERATED_KEYS);
            int iUserKey = -1;
            oStatement.executeUpdate();

            ResultSet generatedKeys = oStatement.getGeneratedKeys();
            if (generatedKeys.next()) {

                int iAddedKey = (int) generatedKeys.getLong(1);

                this.update();

                return iAddedKey;
            }
            else {
                throw new SQLException("Creating user failed, no ID obtained.");
            }


        } catch (Exception e)
        {
            return -1;
        }

    }

}
