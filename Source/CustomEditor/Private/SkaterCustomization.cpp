#pragma once
#include "SkaterCustomization.h"

#include "Character/Skater.h"
#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "HAL/PlatformApplicationMisc.h"
#include "IDetailChildrenBuilder.h"
#include "IPropertyUtilities.h"
#include "PropertyHandle.h"
#include "UObject/UnrealType.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/SWidget.h"
#include "Widgets/Text/STextBlock.h"


void FSkaterDetailsCustomization::CustomizeDetails(IDetailLayoutBuilder &DetailBuilder) { DetailBuilder.SortCategories(&SortCustomDetailsCategories); }

// Create the static instance of this detail customization needed for registering it in module startup.
TSharedRef<IDetailCustomization> FSkaterDetailsCustomization::MakeInstance() { return MakeShareable(new FSkaterDetailsCustomization); }

void FSkaterDetailsCustomization::SortCustomDetailsCategories(const TMap<FName, IDetailCategoryBuilder *> &AllCategoryMap) {
	TArray<FString> CategoriesInOrder = {"Inputs", "Skate Defaults"};

	for (int32 i = 0, limit = CategoriesInOrder.Num(); i < limit; i++) {
		FName CategoryName = FName(CategoriesInOrder[i]);
		if (AllCategoryMap.Contains(CategoryName)) {
			(*AllCategoryMap.Find(CategoryName))->SetSortOrder(i);
		}
	}
}
