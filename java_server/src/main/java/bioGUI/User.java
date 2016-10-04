package bioGUI;

/**
 * Created by joppich on 10/4/16.
 */
public class User {

    private int m_iID = -1;
    private String name, email;

    public User()
    {
        this.name = "";
        this.email = "";
    }

    public User(int iID, String sName, String sEmail)
    {
        this.m_iID = iID;
        this.name = sName;
        this.email = sEmail;
    }

    public int getID()
    {
        return m_iID;
    }

    public String toString()
    {
        String sDel = "\t";

        return "" + m_iID + sDel + name + sDel + email;

    }

    public void setName(String sName) {name = sName;};
    public String getName()
    {
        return name;
    }

    public void setEmail(String sEMail) {email = sEMail;};
    public String getEmail()
    {
        return email;
    }

}
