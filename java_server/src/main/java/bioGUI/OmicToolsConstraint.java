package bioGUI;

/**
 * Created by joppich on 3/16/17.
 */
import org.zkoss.zk.ui.Component;
import org.zkoss.zk.ui.WrongValueException;
import org.zkoss.zul.Constraint;

public class OmicToolsConstraint implements Constraint {


    public void validate(Component comp, Object value) throws WrongValueException {

        String sValue = (String) value;

        if (sValue == null)
            return;

        if (sValue.length() == 0)
            throw new WrongValueException(comp, "Please specify a link to omictools.com");

        if (!sValue.matches("^(http[s]?://)?omictools\\.com/.*$"))
            throw new WrongValueException(comp, "Please specify a link to omictools.com");
    }
}
