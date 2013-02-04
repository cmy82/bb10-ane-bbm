/*
 * Copyright (c) 2011 Research In Motion Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "Globals.h"


using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

//======================================================================================//
//          INITIALIZE NEEDED GLOBAL VARIABLES & FUNCTION STUBS
//======================================================================================//

FREContext currentContext;

int runCount = 0;
int contextInit = 0;
int extensionInit = 0;
char * test;

int ane_master_domain;           //Used to check the domain for events from the ANE
int ane_master_channel_id = -1;  //Used so child channels can push events back to main channel

typedef enum {
   INITIALIZING = 0,
   STARTING,
   STARTED,
   STOPPING,
   EXITING
} ane_thread_state_e;

ane_thread_state_e aneThreadState = INITIALIZING;


void BBMANEExtensionInitializer(void** extDataToSet,
		                          FREContextInitializer* ctxInitializerToSet,
		                          FREContextFinalizer* ctxFinalizerToSet);
void BBMANEExtensionFinalizer();

void BBMANEContextInitializer(void* extData, const uint8_t* ctxType, FREContext ctx,
		                        uint32_t* numFunctionsToSet, const FRENamedFunction** functionsToSet);
void BBMANEContextFinalizer(FREContext ctx);

FREObject checkStatus(FREContext ctx, void* functionData, uint32_t argc, FREObject argv[]);
static void bpsEventComplete(bps_event_t *event);

//======================================================================================//
//          CUSTOM FUNCTIONS
//======================================================================================//

FREObject checkStatus(FREContext ctx, void* functionData, uint32_t argc,
		             FREObject argv[]) {

   //currentContext = ctx; //=======//

   const char *out  = "Hello from the native code! \nANE Thread state: ";
	const char *out2 = ". \nBBM status: ";
	const char *out3 = ". \nANE domain and channel: ";
	const char *out4 = ":";
	const char *out5 = ". \nImage thread: ";
	const char *out6 = " [";
	const char *out7 = "]. \nContact thread: ";
	const char *out8 = " [";
   const char *out9 = "]. \nProfile thread: ";
   const char *out10 = ".";

	char temp[200];
	char temp2[13];
	char temp3[13];
	char temp4[5];
	char temp5[5];
	char temp6[13];
	char temp7[5];
	char temp8[13];
	char temp9[5];
	char temp10[13];


	itoa(runCount,temp3,10);
	itoa(ane_master_domain,temp4,10);
	itoa(ane_master_channel_id,temp5,10);
	itoa(bbmsp_image_map->size(),temp7,10);
	itoa(bbmsp_contact_map->size(),temp9,10);

	switch( aneThreadState ){
	   case INITIALIZING: strncpy(temp2,"INITIALIZING\0",13); break;
	   case STARTING:     strncpy(temp2,"STARTING\0",9); break;
	   case STARTED:      strncpy(temp2,"STARTED\0",8); break;
	   case STOPPING:     strncpy(temp2,"STOPPING\0",9); break;
	   case EXITING:      strncpy(temp2,"EXITING\0",8); break;
	}

	switch( bbmStatus ){
	   case ANE_PENDING_REGISTRATION: strncpy(temp3,"REG PENDING\0",12); break;
	   case ANE_REGISTERED:           strncpy(temp3,"REGISTERED\0",11); break;
	   case ANE_REGISTRATION_FAILED:  strncpy(temp3,"REG FAILED\0",10); break;
	}

	switch( imageThreadStatus ){
      case IMAGE_THREAD_INITIALIZING: strncpy(temp6,"INIT\0",5); break;
      case WAITING_ON_IMAGE:          strncpy(temp6,"WAITING\0",8); break;
      case PROCESSING_IMAGE:          strncpy(temp6,"PROCESSING\0",11); break;
      case IMAGE_THREAD_STOPPING:     strncpy(temp6,"STOPPING\0",9); break;
      case IMAGE_THREAD_STOPPED:      strncpy(temp6,"STOPPED\0",8); break;
   }

	switch( contactThreadStatus ){
      case CONTACT_THREAD_INITIALIZING: strncpy(temp8,"INIT\0",5); break;
      case CONTACT_THREAD_STARTING:     strncpy(temp8,"STARTING\0",9); break;
      case WAITING_ON_CONTACT:          strncpy(temp8,"WAITING\0",8); break;
      case PROCESSING_CONTACT:          strncpy(temp8,"PROCESSING\0",11); break;
      case CONTACT_THREAD_STOPPING:     strncpy(temp8,"STOPPING\0",9); break;
      case CONTACT_THREAD_STOPPED:      strncpy(temp8,"STOPPED\0",8); break;
   }

	switch( profileThreadStatus ){
      case PROFILE_THREAD_INITIALIZING: strncpy(temp10,"INIT\0",5); break;
      case PROFILE_THREAD_STARTING:     strncpy(temp10,"STARTING\0",9); break;
      case LOADING_PROFILE:             strncpy(temp10,"LOADING\0",8); break;
      case LOADED_PROFILE:              strncpy(temp10,"LOADED\0",11); break;
      case PROFILE_THREAD_STOPPING:     strncpy(temp10,"STOPPING\0",9); break;
      case PROFILE_THREAD_STOPPED:      strncpy(temp10,"STOPPED\0",8); break;
   }

	strcpy(temp,out);
	strcat(temp,temp2);
	strcat(temp,out3);
	strcat(temp,temp4);
	strcat(temp,out4);
	strcat(temp,temp5);
   strcat(temp,out2);
   strcat(temp,temp3);
   strcat(temp,out5);
   strcat(temp,temp6);
   strcat(temp,out6);
   strcat(temp,temp7);
   strcat(temp,out7);
   strcat(temp,temp8);
   strcat(temp,out8);
   strcat(temp,temp9);
   strcat(temp,out9);
   strcat(temp,temp10);
   strcat(temp,out10);

	FREObject result;
	FRENewObjectFromUTF8((uint32_t)(strlen(temp) + 1), (uint8_t*)temp, &result);
	return result;
}

static void* initAneThread(void *data){
   //Init bps services
   bps_initialize();
   //Get an event domain from the BPS library to use when pushing events to child threads
   ane_master_domain = bps_register_domain();
   //Get the channel id of this tread so child threads can push events back to it
   ane_master_channel_id = bps_channel_get_active();

   // Fatal error. Cannot use BBM APIs. Restarting the application may produce
   // a different result. Check that you are using the BlackBerry Messenger
   // permission in your bar-descriptor.xml file.
   if (bbmsp_request_events(0) == BBMSP_FAILURE) return NULL;
   cout << "Request to receive BBMSP events successful for ANE thread [" <<
         ane_master_channel_id << "]" << endl;

   bps_event_t   *event;
   bbmsp_event_t *bbmspEvent;

   for(;;){
      switch( aneThreadState ){
         //In this state the ANE thread will create and start each of the needed child threads
         //as well as initialize any needed global variables
         case INITIALIZING:
              pthread_create(NULL, NULL, initRegistrationThread, NULL);
              pthread_create(NULL, NULL, initImageThread, NULL);
              pthread_create(NULL, NULL, initContactThread, NULL);
              pthread_create(NULL, NULL, initProfileThread, NULL);

              aneThreadState = STARTING;
              break;

         //In this state the ANE has initialized all variables and is awaiting the status of
         //the registration thread to see if access has been granted to the BBMSP
         case STARTING:
         {
              int eventDomain;
              int eventCode;

              cout << "Waiting on BPS event in ANE thread" << endl;
              bps_get_event(&event, -1);
              eventDomain = bps_event_get_domain(event);

              cout << "BPS Event received in ANE thread" << endl;

              if (bps_event_get_domain(event) == bbmsp_get_domain()) {
                 bbmsp_event_t *bbmsp_event;

                 cout << "BBMSP event received in ANE thread" << endl;

                 // Handle a BBM Social Platform event.
                 int event_category = 0;
                 int event_type = 0;

                 bbmsp_event_get_category(event, &event_category);
                 bbmsp_event_get_type(event, &event_type);
                 bbmsp_event_get(event, &bbmsp_event);

                 // Process registration events only at this point.
                 if (event_category == BBMSP_REGISTRATION && event_type == BBMSP_SP_EVENT_ACCESS_CHANGED) {
                    bps_event_t *aneRegistrationEvent;
                    bps_event_create(&aneRegistrationEvent, ane_registration_domain,
                                     bbmsp_event_access_changed_get_access_error_code(bbmsp_event),
                                     NULL, &bpsEventComplete);
                    bps_channel_push_event(ane_registration_channel_id, aneRegistrationEvent);
                 }
                 break;
              }

              //Only check for event passed to the ANEs domain
              if( eventDomain == ane_master_domain ){
                 eventCode = bps_event_get_code(event);
                 cout << "Custom event for registration status received in ANE thread" << endl;
                 //Check to see which code was passed
                 switch(eventCode){
                    case ANE_REGISTERED:
                         bps_event_t *aneProfileEvent;
                         bps_event_create(&aneProfileEvent, ane_profile_domain,
                                          START_LOADING_PROFILE, NULL, &bpsEventComplete);
                         bps_channel_push_event(ane_profile_channel_id, aneProfileEvent);

                         aneThreadState = STARTED;
                         break;
                    case ANE_REGISTRATION_FAILED:
                         aneThreadState = STOPPING;
                         break;
                 }
              }
         }
              break;

         //In this state the ANE thread is running and dispatching events to child threads as
         //needed
         case STARTED:
         {
              bps_get_event(&event, -1);
              if (!event) break;

              //If not a BBMSP event then exit switch statement
              if (bps_event_get_domain(event) != bbmsp_get_domain()) break;

              // Handle a BBM Social Platform event.
              int eventCategory = 0;
              int eventType = 0;

              bbmsp_event_get_category(event, &eventCategory);
              bbmsp_event_get_type(event, &eventType);
              bbmsp_event_get(event, &bbmspEvent);

              if( eventCategory == BBMSP_USER_PROFILE ){
                 if( eventType == BBMSP_SP_EVENT_PROFILE_CHANGED ){
                    bps_event_t         *aneProfileEvent;
                    bps_event_payload_t payload;

                    bbmsp_event_t *copy;
                    //copy = (bbmsp_event_t*)malloc(sizeof(*copy));
                    //memcpy(copy,bbmspEvent,sizeof(*copy));

                    payload.data1 = (uintptr_t)copy;

                    bps_event_create(&aneProfileEvent, ane_profile_domain,
                                     PROFILE_CHANGED_RELOAD, &payload, &bpsEventComplete);
                    bps_channel_push_event(ane_profile_channel_id, aneProfileEvent);
                 }
              }

              if( eventCategory == BBMSP_USER_PROFILE_BOX ){

              }

              if( eventCategory == BBMSP_CONTACT_LIST ){
                 //These event should be caught by contact thread since those events are
                 //registered separately
                 break;
              }

              if( eventCategory == BBMSP_CONNECTION ){

              }
         }
              break;

         //In this state the ANE thread is waiting for all child threads to shutdown
         case STOPPING:
              break;

         //In this state all child threads have shutdown and the ANE thread will dispose of any
         //needed variables before returning
         case EXITING:
              break;
      }

      if( aneThreadState == EXITING ) break;
   }

   //Shutdown the bps services when this thread completes
   bps_shutdown();
   return NULL;
}


static void bpsEventComplete(bps_event_t *event){
   bps_event_destroy(event);
}

//======================================================================================//
//          NATIVE FUNCTIONS
//======================================================================================//


/**
 * The runtime calls this method once when it loads an ActionScript extension.
 * Implement this function to do any initializations that your extension requires.
 * Then set the output parameters.
 *
 * @param extDataToSet
 *             A pointer to a pointer to the extension data of the ActionScript extension.
 *             Create a data structure to hold extension-specific data. For example, allocate
 *             the data from the heap, or provide global data. Set extDataToSet to a pointer
 *             to the allocated data.
 * @param ctxInitializerToSet
 *             A pointer to the pointer to the FREContextInitializer() function. Set
 *             ctxInitializerToSet to the FREContextInitializer() function you defined.
 * @param ctxFinalizerToSet
 *             A pointer to the pointer to the FREContextFinalizer() function. Set
 *             ctxFinalizerToSet to the FREContextFinalizer() function you defined. You can
 *             set this pointer to NULL.
 */
void BBMANEExtensionInitializer(void** extDataToSet,
		FREContextInitializer* ctxInitializerToSet,
		FREContextFinalizer* ctxFinalizerToSet) {
	*extDataToSet = NULL;
	*ctxInitializerToSet = &BBMANEContextInitializer;
	*ctxFinalizerToSet = &BBMANEContextFinalizer;

	pthread_create(NULL, NULL, initAneThread, NULL);
}

/**
 * The runtime calls this function when it disposes of the ExtensionContext instance
 * for this extension context.
 */
void BBMANEExtensionFinalizer() {
}

/**
 * The runtime calls this method when the ActionScript side calls ExtensionContext.createExtensionContext().
 *
 * @param extData
 *             A pointer to the extension data of the ActionScript extension.
 * @param ctxType
 *             A string identifying the type of the context. You define this string as
 *             required by your extension. The context type can indicate any agreed-to meaning
 *             between the ActionScript side and native side of the extension. If your extension
 *             has no use for context types, this value can be Null. This value is a UTF-8
 *             encoded string, terminated with the null character.
 * @param ctx
 *             An FREContext variable. The runtime creates this value and passes it to FREContextInitializer().
 * @param numFunctionsToSet
 *             A pointer to a unint32_t. Set numFunctionsToSet to a unint32_t variable containing
 *             the number of functions in the functionsToSet parameter.
 * @param functionsToSet
 *             A pointer to an array of FRNamedFunction elements. Each element contains a pointer
 *             to a native function, and the string the ActionScript side uses in the ExtensionContext
 *             instance's call() method.
 */
void BBMANEContextInitializer(void* extData, const uint8_t* ctxType, FREContext ctx,
		                        uint32_t* numFunctionsToSet, const FRENamedFunction** functionsToSet) {
	char *temp = NULL;
	int i;

	// define an array of functions
	const char *functionNames[] = { //TEST
	                                "checkStatus",
	                                //REGISTRATION
	                                "bbm_ane_startRegistration",
	                                "bbm_ane_bbmsp_get_version",
	                                "bbm_ane_bbmsp_get_domain",
	                                //INVITES
	                                "bbm_ane_bbmsp_send_download_invitation",
	                                //IMAGES
	                                "bbm_ane_bbmsp_image_create",
	                                "bbm_ane_bbmsp_image_destroy",
                                   "bbm_ane_bbmsp_image_get_type",
                                   "bbm_ane_bbmsp_image_get_data",
                                   "bbm_ane_bbmsp_image_get_data_size",
                                   //ACCESS
                                   "bbm_ane_bbmsp_is_access_allowed",
                                   "bbm_ane_bbmsp_can_show_profile_box",
                                   "bbm_ane_bbmsp_can_send_bbm_invite",
                                   //PROFILE
                                   "bbm_ane_bbmsp_profile_get_display_name",
                                   "bbm_ane_bbmsp_profile_get_personal_message",
                                   "bbm_ane_bbmsp_profile_get_status",
                                   "bbm_ane_bbmsp_profile_get_status_message",
                                   "bbm_ane_bbmsp_profile_get_handle",
                                   "bbm_ane_bbmsp_profile_get_ppid",
                                   "bbm_ane_bbmsp_profile_get_app_version",
                                   "bbm_ane_bbmsp_profile_get_display_picture",
                                   "bbm_ane_bbmsp_profile_set_display_picture",
                                   "bbm_ane_bbmsp_set_user_profile_display_picture",
                                   "bbm_ane_bbmsp_profile_set_status",
                                   "bbm_ane_bbmsp_profile_set_personal_message",
                                   "bbm_ane_bbmsp_set_user_profile_personal_message",
                                   "bbm_ane_bbmsp_set_user_profile_status",
                                   NULL };
	FREFunction functionPtrs[] = { checkStatus,
	                               bbm_ane_startRegistration,
	                               bbm_ane_bbmsp_get_version,
                                  bbm_ane_bbmsp_get_domain,
                                  bbm_ane_bbmsp_send_download_invitation,
                                  bbm_ane_bbmsp_image_create,
	                               bbm_ane_bbmsp_image_destroy,
                                  bbm_ane_bbmsp_image_get_type,
                                  bbm_ane_bbmsp_image_get_data,
                                  bbm_ane_bbmsp_image_get_data_size,
                                  bbm_ane_bbmsp_is_access_allowed,
                                  bbm_ane_bbmsp_can_show_profile_box,
                                  bbm_ane_bbmsp_can_send_bbm_invite,
                                  bbm_ane_bbmsp_profile_get_display_name,
                                  bbm_ane_bbmsp_profile_get_personal_message,
                                  bbm_ane_bbmsp_profile_get_status,
                                  bbm_ane_bbmsp_profile_get_status_message,
                                  bbm_ane_bbmsp_profile_get_handle,
                                  bbm_ane_bbmsp_profile_get_ppid,
                                  bbm_ane_bbmsp_profile_get_app_version,
                                  bbm_ane_bbmsp_profile_get_display_picture,
                                  bbm_ane_bbmsp_profile_set_display_picture,
                                  bbm_ane_bbmsp_set_user_profile_display_picture,
                                  bbm_ane_bbmsp_profile_set_status,
                                  bbm_ane_bbmsp_profile_set_personal_message,
                                  bbm_ane_bbmsp_set_user_profile_personal_message,
                                  bbm_ane_bbmsp_set_user_profile_status,
                                  NULL };

	// count number of functions
	*numFunctionsToSet = 0;
	while (functionPtrs[*numFunctionsToSet] != NULL) {
		(*numFunctionsToSet)++;
	}

	FRENamedFunction *functionSet = (FRENamedFunction *)calloc(*numFunctionsToSet,
			sizeof(FRENamedFunction));

	for (i = 0; i < *numFunctionsToSet; i++) {
		int bufferSize = sizeof(char) * (strlen(functionNames[i]) + 1);
		temp = (char*) malloc(bufferSize);
		strncpy(temp, functionNames[i], bufferSize);
		temp[strlen(functionNames[i])] = '\0';
		functionSet[i].name = (uint8_t*) temp;
		functionSet[i].functionData = NULL;
		functionSet[i].function = functionPtrs[i];
	}

	*functionsToSet = functionSet;
	currentContext = ctx;
}

/**
 * The runtime calls this function when it disposes of the ExtensionContext instance for this extension context.
 *
 * @param ctx
 *             The FREContext variable that represents this extension context.
 */
void BBMANEContextFinalizer(FREContext ctx) { }

#ifdef __cplusplus
}
#endif

