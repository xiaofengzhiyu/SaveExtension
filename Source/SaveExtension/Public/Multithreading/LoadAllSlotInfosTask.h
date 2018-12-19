// Copyright 2015-2018 Piperift. All Rights Reserved.

#pragma once

#include <Async/AsyncWork.h>
#include "FileAdapter.h"

#include "SlotInfo.h"

class USaveManager;

DECLARE_DELEGATE_OneParam(FOnAllInfosLoaded, const TArray<USlotInfo*>&);


/**
 * FLoadSlotInfosTask
 * Async task to load an SlotInfo
 */
class FLoadAllSlotInfosTask : public FNonAbandonableTask {
protected:

	const USaveManager* const Manager;
	const int32 SlotId;
	const bool bSortByRecent;

	FOnAllInfosLoaded Delegate;

	TArray<USlotInfo*> LoadedSlots;


public:

	/** All infos Constructor */
	explicit FLoadAllSlotInfosTask(const USaveManager* Manager, bool bSortByRecent, const FOnAllInfosLoaded& Delegate)
		: Manager(Manager)
		, SlotId(-1)
		, bSortByRecent(bSortByRecent)
		, Delegate(Delegate)
	{}

	void DoWork();

	/** Call only when task has finished */
	void CallDelegate() {
		Delegate.ExecuteIfBound(LoadedSlots);
	}

	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FLoadAllSlotInfosTask, STATGROUP_ThreadPoolAsyncTasks);
	}

private:

	void GetSlotFileNames(TArray<FString>& FoundFiles) const;

	USlotInfo* LoadInfoFromFile(const FString Name) const;


	/** Used to find next available slot id */
	class FFindSlotVisitor : public IPlatformFile::FDirectoryVisitor
	{
	public:
		bool bOnlyInfos = false;
		bool bOnlyDatas = false;
		TArray<FString>& FilesFound;

		FFindSlotVisitor(TArray<FString>& Files)
			: FilesFound(Files)
		{}

		virtual bool Visit(const TCHAR* FilenameOrDirectory, bool bIsDirectory) override;
	};
};
