#ifndef NN_INIT_INIT_STARTUP_H_
#define NN_INIT_INIT_STARTUP_H_

// Startup declarations reconstructed from RE-Pepper/ctrsdk
// 82b1d4e162d69b434afb6aa0c2638eab90d959e1, init_StartUp.h/.cpp.
extern "C" {
void nninitSetupDefault();
void nninitSetupDaemons() __attribute__((notailcall));
void nninitSetup();
}

#endif
