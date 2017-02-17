package bioGUI;

import org.zkoss.bind.annotation.Command;
import org.zkoss.bind.annotation.Init;
import org.zkoss.bind.annotation.NotifyChange;
import org.zkoss.zk.ui.event.Event;
import org.zkoss.zul.Messagebox;
import org.zkoss.zul.Textbox;

import java.util.ArrayList;

public class ViewTemplates {

	private int count;

	private User usercreation = new User();
	private Template templatecreation = new Template();

	private TemplateManager oTempManager = new TemplateManager();

	private ArrayList templates = oTempManager.getTemplates();

	private String filterText;

	@Init
	public void init() {
		count = 100;
	}

	@Command
	@NotifyChange({"templatecreation", "usercreation", "templates"})
	public void addTemplate()
	{
		int iSecsSince0 =  (int) (System.currentTimeMillis() / 1000L);
		templatecreation.setTimestamp(iSecsSince0);

		templatecreation.setUser(usercreation);

		UserManager oUserManager = oTempManager.getUserManager();

		int iUserID = oUserManager.addUser( usercreation );

		if (iUserID != -1) {

			templatecreation.setUserid(iUserID);

			int iAddTemplate = oTempManager.addTemplate(templatecreation);

			if (iAddTemplate != -1)
			{
				usercreation = new User();
				templatecreation = new Template();

				Messagebox.show("Template inserted.");
			} else {
				Messagebox.show("Error adding template.");
			}


		} else {
			Messagebox.show("Error creating user.");
		}

	}

	@Command
	@NotifyChange("templates")
	public void changeFilter() {


		templates = oTempManager.getTemplates( filterText );

		//System.out.println("templates: " + templates.size());

	}

	public ArrayList<Template> getTemplates()
	{
		return templates;
	}

	public int getCount() {
		return count;
	}

	public User getUsercreation() {
		return usercreation;
	}

	public void setUsercreation(User usercreation) {
		this.usercreation = usercreation;
	}

	public Template getTemplatecreation() {
		return templatecreation;
	}

	public void setTemplatecreation(Template templatecreation) {
		this.templatecreation = templatecreation;
	}

	public String getFilterText() {
		return filterText;
	}

	public void setFilterText(String filterText) {

		//System.out.println(filterText);

		this.filterText = filterText;
	}
}
