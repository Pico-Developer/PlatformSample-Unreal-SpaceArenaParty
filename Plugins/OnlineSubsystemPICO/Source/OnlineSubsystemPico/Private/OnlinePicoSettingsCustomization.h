/*******************************************************************************
Copyright © 2015-2022 PICO Technology Co., Ltd.All rights reserved.

NOTICE：All information contained herein is, and remains the property of
PICO Technology Co., Ltd. The intellectual and technical concepts
contained herein are proprietary to PICO Technology Co., Ltd. and may be
covered by patents, patents in process, and are protected by trade secret or
copyright law. Dissemination of this information or reproduction of this
material is strictly forbidden unless prior written permission is obtained from
PICO Technology Co., Ltd.
*******************************************************************************/
// This plugin incorporates portions of the Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc.In the United States of America and elsewhere.
// Unreal® Engine, Copyright 1998 – 2022, Epic Games, Inc.All rights reserved.

#pragma once
#if WITH_EDITOR
#include "IDetailCustomization.h"
#include "OnlinePicoSettings.h"
#include "Input/Reply.h"
#include "Widgets/Input/SCheckBox.h"

class SCheckBox;

class FOnlinePicoSettingsCustomization : public IDetailCustomization
{
public:
    static TSharedRef<IDetailCustomization> MakeInstance();
    virtual void CustomizeDetails(IDetailLayoutBuilder& DetailLayout) override;
    FOnlinePicoSettingsCustomization();
    void ShowDialogWidget();
private:
    IDetailLayoutBuilder* SavedLayoutBuilder;
    static TSharedPtr<SWindow> MsgWindow;
    bool bCheckBoxChecked;
    UOnlinePicoSettings* OnlinePicoSettings;
    bool bIsIgnoreShowDialog;
    FReply OnOKButtonClick();
    FReply OnIgnoreButtonClick();
    void OnCheckBoxChange(ECheckBoxState NewCheckedState);
};
#endif
