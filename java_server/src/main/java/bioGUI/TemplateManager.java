package bioGUI;

import java.sql.*;
import java.util.ArrayList;
import java.util.Vector;

/**
 * Created by joppich on 10/4/16.
 */
public class TemplateManager {

    private Connection connect = null;
    private ArrayList<Template> m_vTemplates;

    private UserManager m_oUserManager = null;

    public TemplateManager()
    {

        m_vTemplates = new ArrayList<Template>();


        try
        {
            connect = DriverManager.getConnection("jdbc:mysql://mysql2-ext:3308/bioGUI?user=ewms&password=EwMsA0");

            PreparedStatement oStatement = connect.prepareStatement("select * from templates;");

            ResultSet oReturn = oStatement.executeQuery();

            m_oUserManager = new UserManager();

            this.processTemplates(oReturn, m_oUserManager);

        } catch (Exception e)
        {
            // do something or not
            e.printStackTrace();
        }




    }


    private void processTemplates(ResultSet resultSet, UserManager oUManager) {

        try
        {


            while (resultSet.next()) {

                int iTemplateID = resultSet.getInt("id");
                int iUserID = resultSet.getInt("user");
                String sDisplayname = resultSet.getString("displayname");
                int iType = resultSet.getInt("type");
                String sTemplate = resultSet.getString("template");
                Timestamp iTimestamp = resultSet.getTimestamp("timestamp");
                boolean bAnonym = resultSet.getBoolean("anonym");

                long iSecsTo0 = iTimestamp.getTime() / 1000;

                Template oNewTemplate = new Template(iTemplateID, iUserID, sDisplayname, iType, sTemplate, (int) iSecsTo0 , bAnonym);
                User oTemplateUser = oUManager.getUser(iUserID);

                oNewTemplate.setUser(oTemplateUser);

                System.out.println( oNewTemplate );

                m_vTemplates.add(oNewTemplate);
            }

        } catch (Exception e)
        {
            m_vTemplates.clear();
        }




    }

    public Template getTemplate(int iTemplateID)
    {

        for (Template oTemp : m_vTemplates)
        {
            if ( oTemp.getTemplateID() == iTemplateID)
                return oTemp;
        }

        return null;

    }

    public ArrayList<Template> getTemplates()
    {
        return m_vTemplates;
    }

}
