package bioGUI;

import org.zkoss.bind.annotation.*;
import org.zkoss.zk.ui.Component;
import org.zkoss.zk.ui.event.CheckEvent;
import org.zkoss.zk.ui.event.Event;
import org.zkoss.zk.ui.select.Selectors;
import org.zkoss.zk.ui.select.annotation.Wire;
import org.zkoss.zk.ui.select.annotation.WireVariable;
import org.zkoss.zul.*;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;

import java.util.*;
import javax.mail.*;
import javax.mail.internet.*;
import javax.activation.*;

public class TemplateRequestManager {


    private TemplateRequest templatecreation = new TemplateRequest();

    @Init
    public void init() {

    }

    public TemplateRequest getTemplatecreation() {
        return templatecreation;
    }

    public void setTemplatecreation(TemplateRequest templatecreation) {
        this.templatecreation = templatecreation;
    }

    @Command
    @NotifyChange({"templatecreation"})
    public void processRequest()
    {
        int iSecsSince0 =  (int) (System.currentTimeMillis() / 1000L);
        templatecreation.setTimestamp(iSecsSince0);

        String to = "joppich@bio.ifi.lmu.de";
        String from = templatecreation.getUser().getEmail();

        if ((from == null) || (from.length() == 0))
        {
            Messagebox.show("You must submit your email address.");
            return;
        }

        if (templatecreation.getDisplayname() == null)
        {
            Messagebox.show("You must enter a Template Name.");
            return;
        }

        String sTempName = templatecreation.getDisplayname();

        String host = "localhost";
        Properties properties = System.getProperties();
        properties.setProperty("mail.smtp.host", host);

        // Get the default Session object.
        Session session = Session.getDefaultInstance(properties);
        try {
            // Create a default MimeMessage object.
            MimeMessage message = new MimeMessage(session);
            message.setFrom(new InternetAddress(from));
            message.addRecipient(Message.RecipientType.TO, new InternetAddress(to));
            message.addRecipient(Message.RecipientType.CC, new InternetAddress(from));

            message.setSubject("New bioGUI template request!");
            message.setText( templatecreation.toMailText() );


            Transport.send(message);

            templatecreation = new TemplateRequest();

        } catch (MessagingException mex) {
            mex.printStackTrace();

            Messagebox.show("Error Sending Request");
        }
        Messagebox.show("Template request for " + sTempName + " sent!");

    }

    @AfterCompose
    public void afterCompose(@ContextParam(ContextType.VIEW) Component view){
        Selectors.wireComponents(view, this, false);
    }

}
