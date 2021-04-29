function Controller()
{
}

Controller.prototype.StartMenuDirectoryPageCallback = function()
{
	if ((installer.isInstaller()) || (installer.isUninstaller()) || (installer.isUpdater()))
	{
		gui.clickButton(buttons.NextButton);
	}
}