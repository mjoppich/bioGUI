package bioGUI;

import jdk.nashorn.internal.objects.Global;
import org.zkoss.zul.Messagebox;

import javax.mail.Message;
import javax.mail.MessagingException;
import javax.mail.Session;
import javax.mail.Transport;
import javax.mail.internet.InternetAddress;
import javax.mail.internet.MimeMessage;
import javax.xml.transform.Result;
import java.sql.*;
import java.util.ArrayList;
import java.util.Properties;
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
        this(GlobalSettings.m_sDBHost, GlobalSettings.m_iDBPort, GlobalSettings.m_sDBName, GlobalSettings.m_sDBUser, GlobalSettings.m_sDBPW);
    }

    public UserManager getUserManager()
    {
        return m_oUserManager;
    }

    public TemplateManager(String sHost, int iHost, String sDBName, String sDBUser, String sDBPW)
    {

        try
        {
            connect = GlobalSettings.getDB(sHost, iHost, sDBName, sDBUser, sDBPW);
            m_oUserManager = new UserManager();

            this.update();

        } catch (Exception e)
        {
            // do something or not
            e.printStackTrace();
        }




    }

    public void update()
    {

        try
        {
            m_oUserManager.update();

            PreparedStatement oStatement = connect.prepareStatement("select * from templates;");
            ResultSet oReturn = oStatement.executeQuery();
            m_vTemplates = this.processTemplates(oReturn, m_oUserManager);
        } catch (Exception e)
        {
            // do nothing
            System.err.println("error updating templates");

        }

    }


    private ArrayList<Template> processTemplates(ResultSet resultSet, UserManager oUManager) {

        ArrayList<Template> vReturn = new ArrayList<Template>();

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
                boolean bVisible = resultSet.getBoolean("visible");
                String sOmicTools = resultSet.getString("omictools");

                if (!bVisible)
                {
                    continue;
                }

                long iSecsTo0 = iTimestamp.getTime() / 1000;

                Template oNewTemplate = new Template(iTemplateID, iUserID, sDisplayname, iType, sTemplate, (int) iSecsTo0 , bAnonym, sOmicTools);
                User oTemplateUser = oUManager.getUser(iUserID);

                oNewTemplate.setUser(oTemplateUser);

                //System.out.println( oNewTemplate );

                vReturn.add(oNewTemplate);
            }

        } catch (Exception e)
        {
            vReturn.clear();
        }

        return vReturn;

    }

    int addTemplate(Template oTemp)
    {

        try
        {

            PreparedStatement oStatement = connect.prepareStatement("select * from templates where displayname = ? and type = ? and user = ?;");

            oStatement.setString(1, oTemp.getDisplayname());
            oStatement.setInt(2, Integer.parseInt(oTemp.getTypeStr()));
            oStatement.setInt(3, oTemp.getUserid());

            ResultSet oExisting = oStatement.executeQuery();


            ArrayList vExisting = this.processTemplates(oExisting, m_oUserManager);

            if (vExisting.size() > 0)
                return -1;

            oStatement = connect.prepareStatement("insert into templates (displayname, template, type, user, anonym, omictools) values (?,?, ? , ?, ?, ?);", Statement.RETURN_GENERATED_KEYS);
            oStatement.setString(1, oTemp.getDisplayname());
            oStatement.setString(2, oTemp.getFullTemplate());
            oStatement.setInt(3, Integer.parseInt(oTemp.getTypeStr()));
            oStatement.setInt(4, oTemp.getUserid());

            boolean bAnonym = oTemp.isAnonym() ? true : false;
            oStatement.setBoolean(5, bAnonym);

            oStatement.setString(6, oTemp.getOmictoolslink());

            oStatement.executeUpdate();

            ResultSet generatedKeys = oStatement.getGeneratedKeys();
            if (generatedKeys.next()) {

                int iAdded = (int) generatedKeys.getLong(1);

                this.update();

                this.notifyEmailNewTemplate(oTemp);

                return iAdded;
            }
            else {
                throw new SQLException("Creating user failed, no ID obtained.");
            }


        } catch (Exception e)
        {
            return -1;
        }

    }

    private void notifyEmailNewTemplate(Template oTemp) {

        try {

            String to = "joppich@bio.ifi.lmu.de";
            String from = to;

            String host = "localhost";
            Properties properties = System.getProperties();
            properties.setProperty("mail.smtp.host", host);

            // Get the default Session object.
            Session session = Session.getDefaultInstance(properties);

            // Create a default MimeMessage object.
            MimeMessage message = new MimeMessage(session);
            message.setFrom(new InternetAddress(from));
            message.addRecipient(Message.RecipientType.TO, new InternetAddress(to));

            message.setSubject("New bioGUI template added!");
            message.setText( oTemp.toMailText() );
            Transport.send(message);

        } catch (Exception ex) {
            ex.printStackTrace();
        }

    }

    public Template getTemplate(int iTemplateID)
    {

        for (Template oTemp : m_vTemplates)
        {
            if ( oTemp.getTemplateid() == iTemplateID)
                return oTemp;
        }

        return null;

    }

    public ArrayList<Template> getTemplates()
    {
        return m_vTemplates;
    }

    public ArrayList<Template> getTemplates(String sFilter)
    {

        ArrayList<Template> vReturn = new ArrayList<Template>();

        for (Template oTemp : m_vTemplates)
        {

            boolean bInsert = false;

            if (sFilter == null)
            {
                bInsert = true;
            } else if (sFilter.length() == 0)
            {
                bInsert = true;
            }

            if (!bInsert) {

                if (oTemp.getAuthor().toUpperCase().contains(sFilter.toUpperCase()))
                    bInsert = true;

                if (oTemp.getDisplayname().toUpperCase().contains(sFilter.toUpperCase()))
                    bInsert = true;

            }


            if (bInsert)
                vReturn.add(oTemp);

        }

        return vReturn;

    }

    public static void main(String args [])
    {
        TemplateManager oTM = new TemplateManager();
        oTM.update();

        for (Template oTemp : oTM.getTemplates())
        {
            System.out.println(oTemp);
        }

    }
}
