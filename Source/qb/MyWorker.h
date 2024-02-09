//// Copyright notice
//
//#pragma once
//
//#include "CoreMinimal.h"
//#include "HAL/Runnable.h"
//
///**
// *
// */
//class QB_API FMyWorker : public FRunnable
//{
//public:
//
//	// Constructor, create the thread by calling this
//	FMyWorker();
//
//	FMyWorker(class Aboss_wizard* wiz, int32 _from, int32 _to);
//
//	// Destructor
//	virtual ~FMyWorker() override;
//
//
//	// Overriden from FRunnable
//	// Do not call these functions youself, that will happen automatically
//	bool Init() override; // Do your setup here, allocate memory, ect.
//	uint32 Run() override; // Main data processing happens here
//	void Stop() override; // Clean up any memory you allocated here
//
//	UPROPERTY()
//		class  Aboss_wizard* wizard = nullptr;
//
//	int32 from = 0;
//	int32 to = 0;
//
//
////private:
//
//	// Thread handle. Control the thread using this, with operators like Kill and Suspend
//	FRunnableThread* Thread;
//
//	// Used to know when the thread should exit, changed in Stop(), read in Run()
//	bool bRunThread;
//};