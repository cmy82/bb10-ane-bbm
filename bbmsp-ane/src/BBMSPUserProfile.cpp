/*
 * BBMSPProfile.cpp
 *
 *  Created on: Jan 31, 2013
 *      Author: CHRIS
 */



#include "Globals.h"
#include <iostream>


#ifdef __cplusplus
extern "C" {
#endif


using namespace std;


//======================================================================================//
//          INITIALIZE NEEDED GLOBAL VARIABLES
//======================================================================================//

ane_profile_thread_status_e profileThreadStatus = PROFILE_THREAD_INITIALIZING;
pthread_mutex_t profileMutex;



//======================================================================================//
//          CUSTOM FUNCTIONS
//======================================================================================//

void* initProfileThread(void *data){

   return NULL;
}


#ifdef __cplusplus
}
#endif

