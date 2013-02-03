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
pthread_mutex_t             profileMutex;

int ane_profile_domain     = -1;
int ane_profile_channel_id = -1;
int userProfilePictureID   = -1;

bbmsp_profile_t *userProfile;
bbmsp_image_t   *profileImage;

//======================================================================================//
//          CUSTOM FUNCTIONS
//======================================================================================//

void* initProfileThread(void *data){
   bps_initialize();
   ane_profile_domain = bps_register_domain();
   ane_profile_channel_id = bps_channel_get_active();

   bps_event_t *bps_event;

   for(;;) {
      switch(profileThreadStatus){
         case PROFILE_THREAD_INITIALIZING:
         {
            //Get next BPS event and block until one returns
            cout << "Waiting on BPS event in profile thread" << endl;
            bps_get_event(&bps_event, -1);
            //If no BPS event is returned (ex if init failed) then cancel the event query
            if (!bps_event) return NULL;
            cout << "BPS event received in profile thread" << endl;

            int eventDomain;
            int eventCode;

            eventDomain = bps_event_get_domain(bps_event);
            if( eventDomain == ane_profile_domain ){
               eventCode = bps_event_get_code(bps_event);
               if( eventCode == START_LOADING_PROFILE ){
                  profileThreadStatus = PROFILE_THREAD_STARTING;
                  cout << "Starting loading of profile" << endl;
               }
            }
         }
            break;

         case PROFILE_THREAD_STARTING:
            if( userProfile == NULL )
               bbmsp_profile_create(&userProfile);
            else {
               bbmsp_profile_destroy(&userProfile);
               bbmsp_profile_create(&userProfile);
            }
            if( bbmsp_get_user_profile(userProfile) != BBMSP_FAILURE ){
               profileThreadStatus = LOADED_PROFILE;
               notifyProfileChanged("ANEProfileLoaded");
               cout << "Loaded user profile" << endl;
            }
            break;

         case LOADING_PROFILE:
            break;

         case LOADED_PROFILE:
         {
            //Get next BPS event and block until one returns
            cout << "Waiting on BPS event in profile thread" << endl;
            bps_get_event(&bps_event, -1);
            //If no BPS event is returned (ex if init failed) then cancel the event query
            if (!bps_event) return NULL;
            cout << "BPS event received in profile thread" << endl;

            int eventDomain;
            int eventCode;

            eventDomain = bps_event_get_domain(bps_event);
            //Only check for event passed to the registration domain
            if( eventDomain == ane_profile_domain ){
               eventCode = bps_event_get_code(bps_event);
               if( eventCode == PROFILE_CHANGED_RELOAD )
                  break;

            }
         }
            break;

         case PROFILE_THREAD_STOPPING:
            bbmsp_profile_destroy(&userProfile);
            break;
         case PROFILE_THREAD_STOPPED:
            break;
      }
   }

   return NULL;
}

static void notifyProfileChanged(const char *event){
   const char *lvl = "";
   FREDispatchStatusEventAsync(currentContext, (const uint8_t*)event, (const uint8_t*)lvl);
}

//======================================================================================//
//                 STANDARD FUNCTIONS FROM bbmsp_profile.h QNX FILE
//======================================================================================//

//BBMSP_API bbmsp_result_t bbmsp_set_user_profile_status(bbmsp_presence_status_t status,
//                                                       const char* custom_status_message);
FREObject bbm_ane_bbmsp_set_user_profile_status(FREContext ctx, void* functionData,
                                                uint32_t argc, FREObject argv[]){
   int status;
   bbmsp_result_t code;
   const uint8_t * value;
   uint length = strlen((char*)argv[1])+1;
   bbmsp_presence_status_t t;

   FREGetObjectAsInt32(argv[0],&status);

   char *msg = (char*) malloc(length);
   msg[length-1] = '\0';
   FREGetObjectAsUTF8( argv[1], &length, &value );
   strncpy(msg,(char*)value,length);

   if( status == 0 ){
      code = bbmsp_set_user_profile_status(BBMSP_PRESENCE_STATUS_AVAILABLE,msg);
   } else if( status == 1 ){
      code = bbmsp_set_user_profile_status(BBMSP_PRESENCE_STATUS_BUSY,msg);
   }

   FREObject result;
   FRENewObjectFromUint32(code, &result);
   return result;
}

//BBMSP_API bbmsp_result_t bbmsp_set_user_profile_personal_message(const char* personal_message);
FREObject bbm_ane_bbmsp_set_user_profile_personal_message(FREContext ctx, void* functionData,
                                                          uint32_t argc, FREObject argv[]){
   const uint8_t * value;
   uint length = strlen((char*)argv[0])+1;

   char *msg = (char*) malloc(length);
   msg[length-1] = '\0';
   FREGetObjectAsUTF8( argv[0], &length, &value );
   strncpy(msg,(char *)value,length);

   bbmsp_result_t code = bbmsp_set_user_profile_personal_message((const char*)msg);
   FREObject result;
   FRENewObjectFromUint32(code, &result);
   return result;
}

//BBMSP_API bbmsp_result_t bbmsp_set_user_profile_display_picture(bbmsp_image_t* image);
FREObject bbm_ane_bbmsp_set_user_profile_display_picture(FREContext ctx, void* functionData,
                                                         uint32_t argc, FREObject argv[]){
   int imageID;
   FREGetObjectAsInt32(argv[0],&imageID);
   bbmsp_image_t *image = (*bbmsp_image_map)[imageID];

   bbmsp_result_t code = bbmsp_set_user_profile_display_picture(image);
   FREObject result;
   FRENewObjectFromUint32(code, &result);
   return result;
}

//BBMSP_API bbmsp_result_t bbmsp_profile_get_display_name(bbmsp_profile_t* profile,
//                                                        char* display_name,
//                                                        size_t buffer_size);
FREObject bbm_ane_bbmsp_profile_get_display_name(FREContext ctx, void* functionData,
                                                 uint32_t argc, FREObject argv[]){
   char *name = new char[BBMSP_PROFILE_DISPLAY_NAME_MAX];
   bbmsp_result_t code = bbmsp_profile_get_display_name(userProfile,name,BBMSP_PROFILE_DISPLAY_NAME_MAX);

   FREObject result;
   FRENewObjectFromUTF8((uint32_t)(strlen(name)+1), (uint8_t*)name, &result);
   return result;
}

//BBMSP_API bbmsp_result_t bbmsp_profile_get_personal_message(bbmsp_profile_t* profile,
//                                                            char* personal_message,
//                                                            size_t buffer_size);
FREObject bbm_ane_bbmsp_profile_get_personal_message(FREContext ctx, void* functionData,
                                                     uint32_t argc, FREObject argv[]){
   char *name = new char[BBMSP_PROFILE_PERSONAL_MSG_MAX];
   bbmsp_result_t code = bbmsp_profile_get_personal_message(userProfile,name,BBMSP_PROFILE_PERSONAL_MSG_MAX);

   FREObject result;
   FRENewObjectFromUTF8((uint32_t)(strlen(name)+1), (uint8_t*)name, &result);
   return result;
}

//BBMSP_API bbmsp_result_t bbmsp_profile_get_status(bbmsp_profile_t* profile,
//                                                  bbmsp_presence_status_t* status);
FREObject bbm_ane_bbmsp_profile_get_status(FREContext ctx, void* functionData,
                                           uint32_t argc, FREObject argv[]){
   bbmsp_presence_status_t usrStatus;
   bbmsp_profile_get_status(userProfile,&usrStatus);

   char *status;
   switch( usrStatus ){
      case BBMSP_PRESENCE_STATUS_AVAILABLE:
           status = "AVAILABLE";
           break;

      case BBMSP_PRESENCE_STATUS_BUSY:
           status = "BUSY";
           break;
   }

   FREObject result;
   FRENewObjectFromUTF8((uint32_t)(strlen(status)+1), (uint8_t*)status, &result);
   return result;
}

//BBMSP_API bbmsp_result_t bbmsp_profile_get_status_message(bbmsp_profile_t* profile,
//                                                          char* status_message,
//                                                          size_t buffer_size);
FREObject bbm_ane_bbmsp_profile_get_status_message(FREContext ctx, void* functionData,
                                                   uint32_t argc, FREObject argv[]){
   char *name = new char[BBMSP_PROFILE_STATUS_MSG_MAX];
   bbmsp_result_t code = bbmsp_profile_get_status_message(userProfile,name,BBMSP_PROFILE_STATUS_MSG_MAX);

   FREObject result;
   FRENewObjectFromUTF8((uint32_t)(strlen(name)+1), (uint8_t*)name, &result);
   return result;
}

//BBMSP_API bbmsp_result_t bbmsp_profile_get_ppid(bbmsp_profile_t* profile,
//                                                char* ppid,
//                                                size_t buffer_size);
FREObject bbm_ane_bbmsp_profile_get_ppid(FREContext ctx, void* functionData,
                                         uint32_t argc, FREObject argv[]){
   char *name = new char[BBMSP_PROFILE_PPID_MAX];
   bbmsp_result_t code = bbmsp_profile_get_ppid(userProfile,name,BBMSP_PROFILE_PPID_MAX);

   FREObject result;
   FRENewObjectFromUTF8((uint32_t)(strlen(name)+1), (uint8_t*)name, &result);
   return result;
}

//BBMSP_API bbmsp_result_t bbmsp_profile_get_handle(bbmsp_profile_t* profile,
//                                                  char* handle,
//                                                  size_t buffer_size);
FREObject bbm_ane_bbmsp_profile_get_handle(FREContext ctx, void* functionData,
                                           uint32_t argc, FREObject argv[]){
   char *name = new char[BBMSP_PROFILE_HANDLE_MAX];
   bbmsp_result_t code = bbmsp_profile_get_handle(userProfile,name,BBMSP_PROFILE_HANDLE_MAX);

   FREObject result;
   FRENewObjectFromUTF8((uint32_t)(strlen(name)+1), (uint8_t*)name, &result);
   return result;
}

//BBMSP_API bbmsp_result_t bbmsp_profile_get_app_version(bbmsp_profile_t* profile,
//                                                       char* app_version,
//                                                       size_t buffer_size);
FREObject bbm_ane_bbmsp_profile_get_app_version(FREContext ctx, void* functionData,
                                                uint32_t argc, FREObject argv[]){
   char *name = new char[25];
   bbmsp_result_t code = bbmsp_profile_get_app_version(userProfile,name,25);

   FREObject result;
   FRENewObjectFromUTF8((uint32_t)(strlen(name)+1), (uint8_t*)name, &result);
   return result;
}

//BBMSP_API bbmsp_result_t bbmsp_profile_get_display_picture(bbmsp_profile_t* profile,
//                                                           bbmsp_image_t* display_picture);
FREObject bbm_ane_bbmsp_profile_get_display_picture(FREContext ctx, void* functionData,
                                                    uint32_t argc, FREObject argv[]){
   bbmsp_image_t *usrPicture;
   bbmsp_image_create_empty(&usrPicture);
   bbmsp_profile_get_display_picture(userProfile,usrPicture);
   userProfilePictureID = rand();
   bbmsp_image_map->insert( std::pair<int,bbmsp_image_t*>(userProfilePictureID,usrPicture) );

   FREObject result;
   FRENewObjectFromInt32(userProfilePictureID, &result);
   return result;
}

//BBMSP_API bbmsp_result_t bbmsp_profile_set_status(bbmsp_profile_t* profile,
//                                                  bbmsp_presence_status_t status,
//                                                  const char* custom_status_message);
FREObject bbm_ane_bbmsp_profile_set_status(FREContext ctx, void* functionData,
                                           uint32_t argc, FREObject argv[]){
   int status;
   bbmsp_result_t code;
   const uint8_t * value;
   uint length = strlen((char*)argv[1])+1;
   bbmsp_presence_status_t t;
   FREGetObjectAsInt32(argv[0],&status);

   char *msg = (char*) malloc(length);
   msg[length-1] = '\0';
   FREGetObjectAsUTF8( argv[1], &length, &value );
   strncpy(msg,(char*)value,length);

   if( status == 0 ){
      code = bbmsp_profile_set_status(userProfile,BBMSP_PRESENCE_STATUS_AVAILABLE,msg);
   } else if( status == 1 ){
      code = bbmsp_profile_set_status(userProfile,BBMSP_PRESENCE_STATUS_BUSY,msg);
   }

   FREObject result;
   FRENewObjectFromUint32(code, &result);
   return result;
}

//BBMSP_API bbmsp_result_t bbmsp_profile_set_personal_message(bbmsp_profile_t* profile,
//                                                            const char* personal_message);
FREObject bbm_ane_bbmsp_profile_set_personal_message(FREContext ctx, void* functionData,
                                                     uint32_t argc, FREObject argv[]){
   const uint8_t * value;
   uint length = strlen((char*)argv[0])+1;

   char *msg = (char*) malloc(length);
   msg[length-1] = '\0';
   FREGetObjectAsUTF8( argv[0], &length, &value );
   strncpy(msg,(char *)value,length);

   bbmsp_result_t code = bbmsp_profile_set_personal_message(userProfile,(const char*)msg);
   FREObject result;
   FRENewObjectFromUint32(code, &result);
   return result;
}

//BBMSP_API bbmsp_result_t bbmsp_profile_set_display_picture(bbmsp_profile_t* profile,
//                                                           bbmsp_image_t* display_picture);
FREObject bbm_ane_bbmsp_profile_set_display_picture(FREContext ctx, void* functionData,
                                                    uint32_t argc, FREObject argv[]){
   int imageID;
   FREGetObjectAsInt32(argv[0],&imageID);
   bbmsp_image_t *image = (*bbmsp_image_map)[imageID];

   bbmsp_result_t code = bbmsp_profile_set_display_picture(userProfile,image);
   FREObject result;
   FRENewObjectFromUint32(code, &result);
   return result;
}

/**
 * @brief Retrieve the BBM user profile that was changed.
 *
 * @details When a user's profile is changed, a
 * @c BBMSP_SP_EVENT_PROFILE_CHANGED event is triggered.
 *
 * @param event A pointer to the @c bbmsp event that was triggered when the user
 * profile was changed.
 * @param profile An updated pointer to the BBM user profile that was changed.
 *
 * @return @c BBMSP_SUCCESS if successful, @c BBMSP_FAILURE otherwise.
 * @see bbmsp_event_category_t, bbmsp_event_type_t, BBMSP_SUCCESS, BBMSP_FAILURE
 */
//BBMSP_API bbmsp_result_t bbmsp_event_profile_changed_get_profile(bbmsp_event_t* event,
//                                                                 bbmsp_profile_t** profile);
FREObject bbm_ane_bbmsp_event_profile_changed_get_profile(FREContext ctx, void* functionData,
                                                          uint32_t argc, FREObject argv[]);

/**
 * @brief Retrieve the field that was changed in the user's BBM profile.
 *
 * @details When a user's profile is changed, a
 * @c BBMSP_SP_EVENT_PROFILE_CHANGED event is triggered.
 *
 * @param event A pointer to the @c bbmsp event that was triggered when the
 * field was changed.
 * @param update_type An updated pointer identifying the field that was updated.
 *
 * @return @c BBMSP_SUCCESS if successful, @c BBMSP_FAILURE otherwise.
 * @see bbmsp_event_t, bbmsp_presence_update_types_t, BBMSP_SUCCESS,
 * BBMSP_FAILURE
 */
//BBMSP_API bbmsp_result_t bbmsp_event_profile_changed_get_presence_update_type( bbmsp_event_t* event,
//                                                                               bbmsp_presence_update_types_t* update_type);
FREObject bbm_ane_bbmsp_event_profile_changed_get_presence_update_type(FREContext ctx, void* functionData,
                                                                       uint32_t argc, FREObject argv[]);

//BBMSP_API bbmsp_result_t bbmsp_profile_set_display_name(bbmsp_profile_t* profile,
//                                                        const char* display_name);
FREObject bbm_ane_bbmsp_profile_set_display_name(FREContext ctx, void* functionData,
                                                 uint32_t argc, FREObject argv[]){
   int status;
   bbmsp_result_t code;
   const uint8_t * value;
   uint length = strlen((char*)argv[0])+1;

   char *name = (char*)malloc(length);
   name[length-1] = '\0';
   FREGetObjectAsUTF8( argv[0], &length, &value );
   strncpy(name,(char*)value,length);

   code = bbmsp_profile_set_display_name(userProfile,name);

   FREObject result;
   FRENewObjectFromUint32(code, &result);
   return result;
}





#ifdef __cplusplus
}
#endif

