package bioGUI;

import org.zkoss.bind.annotation.Command;
import org.zkoss.bind.annotation.Init;
import org.zkoss.bind.annotation.NotifyChange;

import java.util.ArrayList;

public class ViewTemplates {

	private int count;

	private User usercreation = new User();
	private Template templatecreation = new Template();


	@Init
	public void init() {
		count = 100;
	}

	@Command
	@NotifyChange("count")
	public void cmd() {
		++count;
	}

	@Command
	@NotifyChange("fetch")
	public void addTemplate() {




	}

	public ArrayList<Template> getTemplates()
	{
		TemplateManager oTempIT = new TemplateManager();

		return oTempIT.getTemplates();
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
}
