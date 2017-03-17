package bioGUI;

import java.sql.*;
import java.util.ArrayList;
import java.util.HashSet;

/**
 * Created by joppich on 3/16/17.
 */
public class CategoryManager {

    private Connection connect = null;

    private ArrayList<Category> m_vCategories = null;

    public CategoryManager()
    {
        this(GlobalSettings.m_sDBHost, GlobalSettings.m_iDBPort, GlobalSettings.m_sDBName, GlobalSettings.m_sDBUser, GlobalSettings.m_sDBPW);
    }

    public CategoryManager(String sHost, int iHost, String sDBName, String sDBUser, String sDBPW)
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

    public ArrayList<Category> getCategories()
    {
        if (m_vCategories == null)
            this.update();

        return m_vCategories;
    }

    public void update()
    {

        try {
            PreparedStatement oStatement = connect.prepareStatement("select * from categories;");
            ResultSet oReturn = oStatement.executeQuery();

            m_vCategories = this.retrieveFromDatabase(oReturn);

        } catch (Exception e)
        {
            // nothing
        }

    }

    public Category getCategory(int id)
    {

        for (Category oCat : m_vCategories)
        {
            if (oCat.getID() == id)
                return oCat;
        }

        return null;
    }


    private ArrayList<Category> retrieveFromDatabase(ResultSet resultSet) throws SQLException {

        ArrayList<Category> vCats = new ArrayList<Category>();

        while (resultSet.next()) {
            // It is possible to get the columns via name
            // also possible to get the columns via the column number
            // which starts at 1
            // e.g. resultSet.getSTring(2);
            int iCatId = resultSet.getInt("catid");
            String sName = resultSet.getString("category");

            Category oCat = new Category(iCatId, sName);

            //System.out.println( oUser );

            vCats.add(oCat);
        }

        return vCats;
    }

    int addCategory(Category oCat)
    {

        try
        {

            PreparedStatement oStatement = connect.prepareStatement("select * from categories where name = ?;");
            oStatement.setString(1, oCat.getCategory());

            ResultSet foundUsers = oStatement.executeQuery();
            ArrayList<Category> vExactUsers = retrieveFromDatabase(foundUsers);

            if (vExactUsers.size() > 0)
                return vExactUsers.get(0).getID();

            oStatement = connect.prepareStatement("insert into categories (desc) values (?);", Statement.RETURN_GENERATED_KEYS);
            oStatement.setString(1, oCat.getCategory());

            int iCatKey = -1;
            oStatement.executeUpdate();

            ResultSet generatedKeys = oStatement.getGeneratedKeys();
            if (generatedKeys.next()) {

                int iAddedKey = (int) generatedKeys.getLong(1);

                this.update();

                return iAddedKey;
            }
            else {
                throw new SQLException("Creating category failed, no ID obtained.");
            }


        } catch (Exception e)
        {
            return -1;
        }

    }

    public ArrayList<Category> getCategoriesForTemplate(int iTemplate)
    {

        this.update();

        try {
            PreparedStatement oStatement = connect.prepareStatement("select * from tempcat where tid = ?;");
            oStatement.setInt(1, iTemplate);

            ResultSet oReturn = oStatement.executeQuery();

            HashSet<Integer> setCatIDs = new HashSet<Integer>();

            while (oReturn.next()) {

                int iCatId = oReturn.getInt("cid");

                setCatIDs.add(iCatId);
            }

            ArrayList<Category> oResult = new ArrayList<Category>();

            for (Category oCat : m_vCategories)
                if ( setCatIDs.contains( oCat.getID()) )
                {
                    oResult.add(oCat);
                }

            return oResult;

        } catch (Exception e)
        {
            // nothing
        }

        return null;
    }

    public void setCategories(int iAddTemplate, HashSet<Integer> setTemplateCategories) {


        try {

            for (Integer iCat : setTemplateCategories)
            {
                PreparedStatement oStatement = connect.prepareStatement("replace into tempcat (tid, cid) values (?, ?);");
                oStatement.setInt(1, iAddTemplate);
                oStatement.setInt(2, iCat);
                oStatement.executeUpdate();

            }

        } catch (Exception e)
        {
            // nothing
        }

    }
}
