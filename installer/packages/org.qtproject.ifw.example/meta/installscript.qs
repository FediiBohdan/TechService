function Component()
{
	installer.setDefaultPageVisible(QInstaller.ComponentSelection, false);
}

Component.prototype.createOperations = function()
{
        component.createOperations();

        if (installer.value("os") === "win")
	{
	component.addOperation("CreateShortcut", 
                            "@TargetDir@/TechService.exe",// target
			    "@StartMenuDir@/TechService.lnk",
                            "workingDirectory=@TargetDir@",// working-dir
                            "iconPath=@TargetDir@/TechService.exe", "iconId=0",// icon
                            "description=Запустить TechService");// description
	component.addOperation("CreateShortcut",
				"@TargetDir@/TechService.exe",
				"@DesktopDir@/TechService.lnk",
				"workingDirectory=@TargetDir@",
				"iconPath=@TargetDir@/TechService.exe",
				"description=Запустить TechService");
	if ((systemInfo.productVersion === "10") || (systemInfo.productVersion === "7"))
	{
		component.addOperation("CreateShortcut",
			"@TargetDir@/TechService.exe",
			"@UserStartMenuProgramsPath@/Startup/TechService.lnk",
			"workingDirectory=@TargetDir@",
			"iconPath=@TargetDir@/TechService.exe",
			"description=Запустить TechService");
	}
		
// Check if redistributable is installed
var registryVC2015x64 = installer.execute("reg", new Array("QUERY", "HKEY_CLASSES_ROOT\\Installer\\Dependencies\\VC,redist.x86,x86,14.26,bundle", "/v", "Version"))[0];

//Check if SQL server is installed
var registrySqlServerx64 = installer.execute("reg", new Array("QUERY", "HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\Microsoft\\Microsoft SQL Server Native Client 10.0\\CurrentVersion", "/v", "Version"))[0];

	if (!registryVC2015x64)
	{
	component.addOperation("Execute", "{0,1602,1638,3010}", "@TargetDir@\\drivers\\VC_redist.x86.exe", "/passive", "/norestart");
	}
	
	if ((systemInfo.currentCpuArchitecture === "x86_64") || (systemInfo.currentCpuArchitecture === "x64"))
	{
		if (!registrySqlServerx64)
		{
		component.addElevatedOperation("Execute", "{0,1602,1638,3010}", "@TargetDir@\\drivers\\sqlncli_amd64.exe", "/passive", "/norestart");
		}
	}		
	}
}