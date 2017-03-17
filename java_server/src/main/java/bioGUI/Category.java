package bioGUI;

/**
 * Created by joppich on 3/16/17.
 */
public class Category {

    private int id;
    private String sCategory;

    public Category(int id, String sCategory)
    {
        this.id = id;
        this.sCategory = sCategory;

    }

    public String getCategory()
    {
        return sCategory;
    }

    public void setCategory(String sBla)
    {
     // do nothing
    }

    public int getID()
    {
        return id;
    }

}
