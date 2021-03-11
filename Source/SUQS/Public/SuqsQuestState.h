// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "SuqsQuest.h"
#include "UObject/Object.h"

#include "SuqsQuestState.generated.h"


class USuqsObjectiveState;
class USuqsTaskState;

UENUM(BlueprintType)
enum class ESuqsQuestStatus : uint8
{
	/// No progress has been made
	NotStarted = 0,
    /// At least one element of progress has been made
    InProgress = 4,
    /// All mandatory elements have been completed
    Completed = 8,
    /// This quest has been failed and cannot be progressed without being explicitly reset
    Failed = 20,
	/// Quest is not available because it's never been accepted
	Unavailable = 30
};

/**
 * Quest state
 */
UCLASS()
class SUQS_API USuqsQuestState : public UObject
{
	GENERATED_BODY()

	friend class USuqsPlayState;
protected:

	/// Whether this objective has been started, completed, failed (quick access to looking at tasks)
	UPROPERTY(BlueprintReadOnly, Category="Quest Status")
	ESuqsQuestStatus Status = ESuqsQuestStatus::NotStarted;

	/// List of detailed objective status
	UPROPERTY(BlueprintReadOnly, Category="Quest Status")
	TArray<USuqsObjectiveState*> Objectives;

	/// List of active branches, which affects which objectives will be considered
	UPROPERTY(BlueprintReadOnly, Category="Quest Status")
	TArray<FName> ActiveBranches;

	UPROPERTY()
	TMap<FName, USuqsTaskState*> FastTaskLookup;

	// Pointer to quest definition, for convenience (this is static data)
	FSuqsQuest* QuestDefinition;
	TWeakObjectPtr<USuqsPlayState> PlayState;

	void Initialise(FSuqsQuest* Def, USuqsPlayState* Root);
	void Tick(float DeltaTime);
	
public:
	ESuqsQuestStatus GetStatus() const { return Status; }
	const TArray<USuqsObjectiveState*>& GetObjectives() const { return Objectives; }
	const TArray<FName>& GetActiveBranches() const { return ActiveBranches; }

	UFUNCTION(BlueprintCallable)
    const FName& GetIdentifier() const { return QuestDefinition->Identifier; }
	
	/// Find a task with the given identifier in this quest
	USuqsTaskState* FindTask(const FName& Identifier) const;

	/// Set an objective branch to be active in this quest. Objectives associated with this branch will then be allowed
	/// to activate.
	UFUNCTION(BlueprintCallable)
	void SetBranchActive(FName Branch, bool bActive);

	/// Return whether an objective branch is active or not
	UFUNCTION(BlueprintCallable)
    bool IsBranchActive(FName Branch);

	void NotifyObjectiveStatusChanged();
	
};