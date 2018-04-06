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

public class ViewTemplates {

	private int count;

	private User usercreation = new User();
	private Template templatecreation = new Template();

	private TemplateManager oTempManager = new TemplateManager();
	private CategoryManager oCatManager = new CategoryManager();

	private ArrayList<Template> templates = oTempManager.getTemplates();
	private ArrayList<Category> categories = oCatManager.getCategories();

	private String filterText;

	@Init
	public void init() {
		count = 100;

		if (templates != null)
		{
			for (Template oTemp: templates)
			{

				oTemp.setCategories( this.getCategoriesForTemplate(oTemp.getTemplateid()) );

			}
		}
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

				HashSet<Integer> setTemplateCategories = new HashSet<Integer>();

				// insert categories
				for( Object obj : catgrid.getRows().getChildren() ){
					Row comp = (Row) obj;
					Checkbox ck = (Checkbox) comp.getChildren().get(0);

					if (ck.isChecked())
						setTemplateCategories.add( Integer.parseInt(ck.getValue().toString()) );
				}

				oCatManager.setCategories(iAddTemplate, setTemplateCategories);

				Messagebox.show("Template inserted and currently under review.");
			} else {
				Messagebox.show("Error adding template.");
			}


		} else {
			Messagebox.show("Error creating user.");
		}

	}

	@AfterCompose
	public void afterCompose(@ContextParam(ContextType.VIEW) Component view){
		Selectors.wireComponents(view, this, false);
	}

	@Wire("#catgrid")
	private Grid catgrid;

	@Wire("#allCatCheckbox")
	private Checkbox allCatCheckbox;

	@Command
	public void toggleAllCategories(){
		List components = catgrid.getRows().getChildren();

		for(Object obj:components){
			Row comp = (Row) obj;
			Checkbox ck = (Checkbox) comp.getChildren().get(0);
			ck.setChecked(allCatCheckbox.isChecked());
		}
	}

	@Command
	@NotifyChange("templates")
	public void changeFilter() {


		templates = oTempManager.getTemplates( filterText );

		for (Template oTemp: templates)
		{
			oTemp.setCategories( this.getCategoriesForTemplate(oTemp.getTemplateid()) );
		}

		//System.out.println("templates: " + templates.size());

	}

	public String getCategoriesForTemplate(int iTemplateID)
	{
		ArrayList<Category> oCats = oCatManager.getCategoriesForTemplate(iTemplateID);

		StringBuilder oSB = new StringBuilder();

		for (int i = 0; i < oCats.size(); ++i)
		{
			if (i > 0)
				oSB.append(", ");

			oSB.append(oCats.get(i).getCategory());
		}

		return oSB.toString();
	}

	public ArrayList<Category> getCategories()
	{
		return categories;
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
