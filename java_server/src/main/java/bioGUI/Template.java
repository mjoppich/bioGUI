package bioGUI;

import java.lang.reflect.Array;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Date;

/**
 * Created by joppich on 10/4/16.
 */
public class Template {

    private int templateid, userid, type, timestamp;
    private boolean anonym;
    private String displayname, typeStr, omictoolslink, categories;

    public String getCategories()
    {
        return categories;
    }

    public void setCategories(String cats)
    {
        this.categories = cats;
    }

    public void setOmictoolslink(String omictoolslink) {
        this.omictoolslink = omictoolslink;
    }
    public String getOmictoolslink() {
        return this.omictoolslink;
    }

    public String getTemplate() {
        return template;
    }

    public void setTemplate(String template) {
        this.template = template;
    }

    public String getDisplayname() {
        return displayname;
    }

    public void setDisplayname(String displayname) {
        this.displayname = displayname;
    }

    public boolean isAnonym() {
        return anonym;
    }

    public void setAnonym(boolean anonym) {
        this.anonym = anonym;
    }

    public int getTimestamp() {
        return timestamp;
    }

    public void setTimestamp(int timestamp) {
        this.timestamp = timestamp;
    }

    public String getTypeStr() {
        return "" + this.type;
    }

    public void setType(int type) {
        this.type = type;

        this.typeStr = "" + type;
    }

    public void setTypeStr(String type) {

        this.setType( Integer.parseInt(type) );
    }

    public String getTemplateTypeDescr()
    {
        switch( type )
        {

            case 0: return "Template";
            case 1: return "Install Template";

            default:
                return null;
        }
    }

    public int getUserid() {
        return userid;
    }

    public void setUserid(int userid) {
        this.userid = userid;
    }

    private String template;

    private User user = null;

    public Template()
    {
        this.displayname = "";
        this.template = "";

    }

    public int getTemplateid() {
        return templateid;
    }

    public void setTemplateid(int templateid) {
        this.templateid = templateid;
    }

    public Template(int iTemplateID, int iUserID, String sDisplayname, int iType, String sTemplate, int iTimestamp, boolean bAnonym, String sOmicTools) {

        this.templateid = iTemplateID;
        this.userid = iUserID;
        this.displayname = sDisplayname;

        this.setType(iType);

        this.template = sTemplate;
        this.timestamp = iTimestamp;
        this.anonym = bAnonym;
        this.omictoolslink = sOmicTools;

    }

    public String getTimestampDate()
    {
        Date oDate = new Date(1000L * timestamp);

        return new SimpleDateFormat("yyyy-MM-dd").format(oDate);
    }

    public String getAuthor()
    {
        if ((!anonym) && (user != null))
            return user.getName() + " " + user.getEmail();

        return "Anonymous";
    }

    public void setUser(User oUser)
    {
        user = oUser;
    }
    public User getUser() {return user;}

    public String toString()
    {
        String sDel = "\t";

        String sUserPart = "" + userid;
        if (user != null)
            sUserPart = user.getName() + sDel + user.getEmail();

        return "" + templateid + sDel + sUserPart + sDel + displayname + sDel + timestamp + sDel + anonym;

    }

    public String getFullTemplate()
    {
        return template;
    }

}
