//#include "MyWorker.h" // Change this to reference the header file above
//#include "gladiators/boss_wizard.h"
//
//#pragma region Main Thread Code
//// This code will be run on the thread that invoked this thread (i.e. game thread)
//
//
//FMyWorker::FMyWorker(/* You can pass in inputs here */)
//{
//	// Constructs the actual thread object. It will begin execution immediately
//	// If you've passed in any inputs, set them up before calling this.
//	Thread = FRunnableThread::Create(this, TEXT("MyThrd"));
//}
//
//
//FMyWorker::FMyWorker(class Aboss_wizard* wiz, int32 _from, int32 _to) {
//	wizard = wiz;
//	from = _from;
//	to = _to;
//	Thread = FRunnableThread::Create(this, TEXT("MyThrd"));
//
//}
//
//FMyWorker::~FMyWorker()
//{
//	if (Thread)
//	{
//		// Kill() is a blocking call, it waits for the thread to finish.
//		// Hopefully that doesn't take too long
//		Thread->Kill();
//		delete Thread;
//	}
//}
//
//
//#pragma endregion
//// The code below will run on the new thread.
//
//
//bool FMyWorker::Init()
//{
//	UE_LOG(LogTemp, Warning, TEXT("My custom thread has been initialized"));
//	bRunThread = true;
//
//		// Return false if you want to abort the thread
//		return true;
//}
//
//
//uint32 FMyWorker::Run()
//{
//	// Peform your processor intensive task here. In this example, a neverending
//	// task is created, which will only end when Stop is called.
//	while (bRunThread)
//	{
//		
//		while (from <= to && from >= 0) {
//			wizard->canMoveForward[from] = true;
//			from++;
//			UE_LOG(LogTemp, Warning, TEXT("Running thread"));
//		}
//		Stop();
//	}
//
//	return 0;
//}
//
//
//// This function is NOT run on the new thread!
//void FMyWorker::Stop()
//{
//	// Clean up memory usage here, and make sure the Run() function stops soon
//	// The main thread will be stopped until this finishes!
//
//	// For this example, we just need to terminate the while loop
//	// It will finish in <= 1 sec, due to the Sleep()
//	bRunThread = false;
//}