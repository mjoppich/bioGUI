function Controller() {
    installer.autoRejectMessageBoxes();

    installer.installationFinished.connect(function() {
        gui.clickButton(buttons.NextButton);
    });

    installer.setMessageBoxAutomaticAnswer("OverwriteTargetDirectory", QMessageBox.Yes);
    installer.setMessageBoxAutomaticAnswer("stopProcessesForUpdates", QMessageBox.Ignore);

    var page = gui.pageWidgetByObjectName("WelcomePage")
    page.completeChanged.connect(welcomepageFinished)
}



welcomepageFinished = function()
{
    //completeChange() -function is called also when other pages visible
    //Make sure that next button is clicked only when in welcome page
    if(gui.currentPageWidget().objectName == "WelcomePage") {
        gui.clickButton( buttons.NextButton);   
    }
}

Controller.prototype.WelcomePageCallback = function() {
    gui.clickButton(buttons.NextButton);
}

Controller.prototype.CredentialsPageCallback = function() {
    gui.clickButton(buttons.NextButton);
}

Controller.prototype.IntroductionPageCallback = function() {
    gui.clickButton(buttons.NextButton);
}

Controller.prototype.TargetDirectoryPageCallback = function() {
//    gui.currentPageWidget().TargetDirectoryLineEdit.setText("/where/to/install/Qt");

    gui.clickButton(buttons.NextButton);
}

Controller.prototype.ComponentSelectionPageCallback = function() {
    var widget = gui.currentPageWidget();

    widget.deselectAll();

    widget.selectComponent("qt.qt5.5111.gcc_64");

    gui.clickButton(buttons.NextButton);
}

Controller.prototype.LicenseAgreementPageCallback = function() {
    gui.currentPageWidget().AcceptLicenseRadioButton.setChecked(true);

    gui.clickButton(buttons.NextButton);
}

Controller.prototype.StartMenuDirectoryPageCallback = function() {
//    gui.clickButton(buttons.NextButton);
}

Controller.prototype.ReadyForInstallationPageCallback = function() {
    gui.clickButton(buttons.NextButton);
}

Controller.prototype.FinishedPageCallback = function() {
    var checkBoxForm = gui.currentPageWidget().LaunchQtCreatorCheckBoxForm;

    if (checkBoxForm && checkBoxForm.launchQtCreatorCheckBox)
        checkBoxForm.launchQtCreatorCheckBox.checked = false;

    gui.clickButton(buttons.FinishButton);
}
