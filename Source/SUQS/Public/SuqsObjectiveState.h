// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "SuqsQuestState.h"
#include "UObject/Object.h"
#include "SuqsObjectiveState.generated.h"


UENUM(BlueprintType)
enum class ESuqsObjectiveStatus : uint8
{
	/// No progress has been made
	NotStarted = 0,
    /// At least one element of progress has been made
    InProgress = 4,
    /// All mandatory elements have been completed
    Completed = 8,
    /// This item has been failed and cannot be progressed without being explicitly reset
    Failed = 20,
	/// This objective is being skipped because it is on an inactive branch
	Skipped = 30
};

/**
 * Objective state
 */
UCLASS()
class SUQS_API USuqsObjectiveState : public UObject
{
	GENERATED_BODY()

	friend class USuqsQuestState;
protected:
	/// Whether this objective has been started, completed, failed (quick access to looking at tasks)
	UPROPERTY(BlueprintReadOnly, Category="Objective Status")
	ESuqsObjectiveStatus Status = ESuqsObjectiveStatus::NotStarted;

	/// List of detailed task status
	UPROPERTY(BlueprintReadOnly, Category="Objective Status")
	TArray<USuqsTaskState*> Tasks;

	const FSuqsObjective* ObjectiveDefinition;
	TWeakObjectPtr<USuqsQuestState> ParentQuest;
	TWeakObjectPtr<USuqsPlayState> PlayState;

	int MandatoryTasksNeededToComplete;

	
	void Initialise(const FSuqsObjective* ObjDef, USuqsQuestState* QuestState, USuqsPlayState* Root);
	void Tick(float DeltaTime);
	// Private fail/complete since users should only ever call task fail/complete
	void Fail();
	void Complete();
	void ChangeStatus(ESuqsObjectiveStatus NewStatus);

public:
	// C++ access
	
	ESuqsObjectiveStatus GetStatus() const { return Status; }
	const TArray<USuqsTaskState*>& GetTasks() { return Tasks; }

	UFUNCTION(BlueprintCallable)
    const FName& GetIdentifier() const { return ObjectiveDefinition->Identifier; }
	UFUNCTION(BlueprintCallable)
    USuqsQuestState* GetParentQuest() const { return ParentQuest.Get(); }
	
	void NotifyTaskStatusChanged();
};