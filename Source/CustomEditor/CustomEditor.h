#pragma once
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"
#include "PropertyEditorDelegates.h"

class FCustomEditorModule : public IModuleInterface {

private:
	void RegisterClass(FName ClassName, FOnGetDetailCustomizationInstance DetailLayoutDelegate);

public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};