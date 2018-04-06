package bioGUI;

import java.lang.reflect.Array;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Date;

/**
 * Created by joppich on 10/4/16.
 */
public class TemplateRequest {

    private int type, timestamp;
    private boolean anonym;
    private String displayname, typeStr, omictoolslink;

    public void setOmictoolslink(String omictoolslink) {
        this.omictoolslink = omictoolslink;
    }
    public String getOmictoolslink() {
        return this.omictoolslink;
    }

    public String getDisplayname() {
        return displayname;
    }

    public void setDisplayname(String displayname) {
        this.displayname = displayname;
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


    private String template;

    private User user = null;

    public User getUser()
    {
        return user;
    }

    public void setUser(User oUser)
    {
        this.user = oUser;
    }

    public TemplateRequest()
    {
        this.displayname = "";
        this.template = "";
        this.user = new User();
    }

    public TemplateRequest(String sDisplayname, int iType, String sTemplate, int iTimestamp, boolean bAnonym, String sOmicTools) {

        this.displayname = sDisplayname;

        this.setType(iType);
        this.user = new User();
        this.template = sTemplate;
        this.timestamp = iTimestamp;
        this.anonym = bAnonym;
        this.omictoolslink = sOmicTools;

    }

    public String toMailText() throws RuntimeException {

        String sDispName = this.getDisplayname();
        if (sDispName == null)
        {
            throw new RuntimeException("no display name");
        }

        String sOmicLink = this.getOmictoolslink();
        if (sOmicLink == null)
        {
            sOmicLink = "";
        }

        StringBuilder oSB = new StringBuilder();

        oSB.append( "New bioGUI Template Request" + "\n\n" );
        oSB.append( "" + "\n" );
        oSB.append( "Submitter Name: " + this.user.getName() + "\n" );
        oSB.append( "Submitter eMail: "  + this.user.getEmail() + "\n" );
        oSB.append( "" + "\n" );
        oSB.append( "Requested Template Name: " + sDispName + "\n" );
        oSB.append( "Omictools Link: " + sOmicLink + "\n" );
        oSB.append( "Template Type: " + this.getTemplateTypeDescr() + "\n" );
        oSB.append( "" + "\n" );
        oSB.append( "This is an automatically generated eMail from the bioGUI repository." + "\n" );

        return oSB.toString();
    }
}
