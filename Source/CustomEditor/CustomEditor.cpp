#include "CustomEditor.h"

#include "Character/Skater.h"
#include "PropertyEditorDelegates.h"
#include "PropertyEditorModule.h"
#include "SkaterCustomization.h"


IMPLEMENT_GAME_MODULE(FCustomEditorModule, CustomEditor);

void FCustomEditorModule::StartupModule() { RegisterClass(ASkater::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&FSkaterDetailsCustomization::MakeInstance)); }

void FCustomEditorModule::ShutdownModule() {
	FPropertyEditorModule &PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	PropertyModule.UnregisterCustomClassLayout(ASkater::StaticClass()->GetFName());
}

void FCustomEditorModule::RegisterClass(FName ClassName, FOnGetDetailCustomizationInstance DetailLayoutDelegate) {
	FPropertyEditorModule &PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.RegisterCustomClassLayout(ClassName, DetailLayoutDelegate);
}