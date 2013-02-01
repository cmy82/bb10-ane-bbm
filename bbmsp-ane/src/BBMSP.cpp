/*
 * BBMSP.cpp
 *
 *  Created on: Jan 16, 2013
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

bps_event_t                     *event       = NULL;                      //Holds the most recent bps event struct
bbmsp_event_t                   *bbmsp_event = NULL;                      //Holds the most recent bbmsp event struct
ane_bbm_registration_progress_e  progress    = REGISTRATION_INITIALIZING; //Used to track registration status
ane_bbmsp_status_e               bbmStatus   = ANE_PENDING_REGISTRATION;

char *uuid;

//======================================================================================//
//          CUSTOM FUNCTIONS
//======================================================================================//

FREObject bbm_ane_startRegistration(FREContext ctx, void* functionData, uint32_t argc, FREObject argv[]){
   const uint8_t * value;
   uint length = 36;

   uuid = (char*) malloc(37);
   uuid[36] = '\0';
   FREGetObjectAsUTF8( argv[0], &length, &value );
   strncpy(uuid,(char *)value,length);

   progress = REGISTRATION_STARTING;
   cout << "Attempting to register with BBMSP [" << uuid << "]" << endl;
   bbmspProcessRegistration(bbmsp_get_access_code());

   return NULL;
}

void* initRegistrationThread(void *data){
   bps_initialize();
   //Get the channel id of this thread so child threads can push events back to it
   //ane_registration_channel_id = bps_channel_get_active();

   if (bbmsp_request_events(0) == BBMSP_FAILURE) return NULL;
   cout << "Request to receive BBMSP events successful" << endl;

   bps_event_t   *bps_event;
   bbmsp_event_t *bbmsp_event;

   for(;;) {
      //Get next BPS event and block until one returns
      bps_get_event(&bps_event, -1);
      //If no BPS event is returned (ex if init failed) then cancel the event query
      if (!bps_event) return NULL;
      cout << "BPS event received in BBMSP thread" << endl;

      //If not a BBMSP event then go back to start of for loop
      if (bps_event_get_domain(bps_event) != bbmsp_get_domain()) continue;
      cout << "BBMSP event received in BBMSP thread" << endl;

      // Handle a BBM Social Platform event.
      int event_category = 0;
      int event_type = 0;

      bbmsp_event_get_category(bps_event, &event_category);
      bbmsp_event_get_type(bps_event, &event_type);
      bbmsp_event_get(bps_event, &bbmsp_event);

      // Process registration events only.
      if (event_category == BBMSP_REGISTRATION && event_type == BBMSP_SP_EVENT_ACCESS_CHANGED) {
         cout << "Registration event received" << endl;
         bbmspProcessRegistration(bbmsp_event_access_changed_get_access_error_code(bbmsp_event));
      }

      if(progress == REGISTRATION_STOPPED){
         registrationComplete();
         break;
      }
   }

   //Shutdown the bps services when this thread completes
   bps_shutdown();
   return NULL;
}

static void registrationEventComplete(bps_event_t *event){
   bps_event_destroy(event);
}

static void registrationComplete(){
   cout << "Registration done. Passing event back to main thread." << endl;
   bps_event_t *aneRegistrationEvent;
   bps_event_create(&aneRegistrationEvent, ane_master_domain, bbmStatus, NULL, &registrationEventComplete);
   bps_channel_push_event(ane_master_channel_id, aneRegistrationEvent);
   cout << "Event passed back." << endl;
}

static void bbmspProcessRegistration(const bbmsp_access_error_codes_t status){
   // Got a registration access status event. Based on the status, decide whether
   // we need to register. If we already registered successfully once (i.e. on a
   // previous application run), then we will not call bbmsp_register() again.

   // Determine what to do based on the progress.
   switch (progress) {
      //Register method has been called. Check to see if app has already been registered
      case REGISTRATION_STARTING:
         // Access is allowed, the application is registered.
         if (bbmsp_is_access_allowed()) {
            progress = REGISTRATION_STOPPED;
            bbmStatus = ANE_REGISTERED;
            cout << "Access granted" << endl;
            registrationComplete();
            return;
         }
         // Need to register.

         // Status is not yet known. Wait for an event that will deliver the status.
         if (status == BBMSP_ACCESS_UNKNOWN){
            cout << "BBMSP access is unknown" << endl;
         //   return;
         }

         // Start registration.
         // Attempt to register the application with the UUID.
         // Every application is required to have its own unique UUID. You should
         // keep using the same UUID when you release a new version of your
         // application.
         // You can generate one here: http://www.guidgenerator.com/
         cout << "Calling register for uuid: " << uuid << endl;
         if (bbmsp_register(uuid)) {
            // Registration started. The user will see a dialog informing them that
            // your application is connecting to BBM.
            progress = REGISTRATION_STARTED;
            return;
         }
         // Could not start registration. No dialogs were shown.
         progress = REGISTRATION_STOPPED;
         bbmStatus = ANE_REGISTRATION_FAILED;
         return;

      // Registration is in progress. Check the status to see if it finished.
      case REGISTRATION_STARTED:
         // Ignore. This means the registration is in progress.
         if (status == BBMSP_ACCESS_PENDING) return;
         // Registration is done.
         if (bbmsp_is_access_allowed()) {
            progress = REGISTRATION_STOPPED;
            bbmStatus = ANE_REGISTERED;
            return;
         }
         progress = REGISTRATION_STOPPED;
         //Access not granted
         bbmStatus = ANE_REGISTRATION_FAILED;
         return;

      default:
         // Ignoring these cases.
         return;
   }
}

//======================================================================================//
//                   STANDARD FUNCTIONS FROM bbmsp.h QNX FILE
//======================================================================================//

//BBMSP_API int bbmsp_get_version(void);
FREObject bbm_ane_bbmsp_get_version(FREContext ctx, void* functionData, uint32_t argc, FREObject argv[]){
   int version = bbmsp_get_version();
   FREObject result;
   FRENewObjectFromInt32(version, &result);
   return result;
}

//BBMSP_API int bbmsp_get_domain(void);
FREObject bbm_ane_bbmsp_get_domain(FREContext ctx, void* functionData, uint32_t argc, FREObject argv[]){
   int domain = bbmsp_get_domain();
   FREObject result;
   FRENewObjectFromInt32(domain, &result);
   return result;
}



#ifdef __cplusplus
}
#endif

