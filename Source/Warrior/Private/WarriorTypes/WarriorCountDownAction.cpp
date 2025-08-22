// Fill out your copyright notice in the Description page of Project Settings.


#include "WarriorTypes/WarriorCountDownAction.h"

void FWarriorCountDownAction::CancelAction()
{
    bNeedToCancel = true;
}