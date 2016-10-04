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

        m_vUsers = new ArrayList<User>();

        try
        {
            connect = DriverManager.getConnection("jdbc:mysql://mysql2-ext:3308/bioGUI?user=ewms&password=EwMsA0");

            PreparedStatement oStatement = connect.prepareStatement("select * from users;");

            ResultSet oReturn = oStatement.executeQuery();

            this.addTemplates(oReturn);

        } catch (Exception e)
        {
            // do something or not
            e.printStackTrace();
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


    private void addTemplates(ResultSet resultSet) throws SQLException {
        // ResultSet is initially before the first data set
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

            m_vUsers.add(oUser);
        }
    }

}
