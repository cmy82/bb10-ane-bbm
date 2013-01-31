/*
 * BBMSPContactlist.cpp
 *
 *  Created on: Jan 29, 2013
 *      Author: CHRIS
 */


#include "Globals.h"


#ifdef __cplusplus
extern "C" {
#endif


using namespace std;


//======================================================================================//
//          GLOBAL VARIABLES/DEFINITIONS
//======================================================================================//

typedef struct {
   uint32_t id;

} contact_data_s;

ane_contact_thread_status_e  contactThreadStatus = CONTACT_THREAD_INITIALIZING;
pthread_mutex_t              contactMutex;
std::queue<contact_data_s *> contactQueue;
map<int,bbmsp_contact_t *>   *bbmsp_contact_map = new map<int,bbmsp_contact_t*>();
bbmsp_contact_list_t         *contactList;

//======================================================================================//
//          CUSTOM FUNCTIONS
//======================================================================================//

void* initContactThread(void *data){
   bps_initialize();

   if( bbmsp_request_events(0) == BBMSP_FAILURE ){
      contactThreadStatus = CONTACT_THREAD_STOPPED;
      return NULL;
   }
   if( bbmsp_event_contact_list_register_event() == BBMSP_FAILURE ){
      contactThreadStatus = CONTACT_THREAD_STOPPED;
      return NULL;
   }

   pthread_mutex_init(&contactMutex,NULL);

   bbmsp_contact_t *bbmspContact;
   contact_data_s *contactData;
   bps_event_t   *bps_event;
   bbmsp_event_t *bbmsp_event;

   bool queueEmpty = true;
   ane_contact_thread_status_e cachedStatus;

   for(;;) {
      pthread_mutex_lock(&contactMutex);
      cachedStatus = contactThreadStatus;
      pthread_mutex_unlock(&contactMutex);

      //switch(contactThreadStatus){
      switch(cachedStatus){
         case CONTACT_THREAD_INITIALIZING:
            //Thread has just started. Make call to get list of contacts with app installed
            //and go into starting state to wait for list to be populated.
            if( bbmsp_contact_list_get() == BBMSP_FAILURE ){
               contactThreadStatus = CONTACT_THREAD_STOPPED;
               return NULL;
            } else
               contactThreadStatus = CONTACT_THREAD_STARTING;
            sleep(15);
            break;

         case CONTACT_THREAD_STARTING:
         {
            //Waiting on contact list to be populated. Thread can move into this state at any time.
            bps_get_event(&bps_event, -1);
            //If no BPS event is returned (ex if init failed) then cancel the event query
            if (!bps_event) return NULL;

            //If not a BBMSP event then go back to start of for loop
            if (bps_event_get_domain(bps_event) != bbmsp_get_domain()) break;

            // Handle a BBM Social Platform event.
            int event_category = 0;
            int event_type = 0;

            bbmsp_event_get_category(bps_event, &event_category);
            bbmsp_event_get_type(bps_event, &event_type);
            bbmsp_event_get(bps_event, &bbmsp_event);

            // Process registration events only.
            if (event_category == BBMSP_CONTACT_LIST && event_type == BBMSP_SP_EVENT_CONTACT_LIST_FULL) {
               bbmsp_event_contact_list_get_full_contact_list(bbmsp_event,&contactList);
               contactThreadStatus = WAITING_ON_CONTACT;
            }
            break;
         }
         case WAITING_ON_CONTACT:
            //If no images in queue then sleep for 15 seconds and check again. Once queue has
            //an image then process it
            if( queueEmpty ){
               sleep(15);
               pthread_mutex_lock(&contactMutex);
               queueEmpty = contactQueue.empty();
               pthread_mutex_unlock(&contactMutex);
            } else {
               contactThreadStatus = PROCESSING_CONTACT;
            }
            break;

         case PROCESSING_CONTACT:
            //If there is an contact in the queue waiting to be constructed, pull it from the
            //queue and create it. Then push an event to the main thread once it has been
            //completed passing the contact id so it can be passed back to as to allow the
            //user to decide what to do next
            pthread_mutex_lock(&contactMutex);
            contactData = contactQueue.front();
            contactQueue.pop();
            pthread_mutex_unlock(&contactMutex);

            bbmsp_contact_create(&bbmspContact);
            bbmsp_contact_map->insert( std::pair<int,bbmsp_contact_t*>(contactData->id,bbmspContact) );
            free(contactData);

            pthread_mutex_lock(&contactMutex);
            queueEmpty = contactQueue.empty();
            pthread_mutex_unlock(&contactMutex);

            if( queueEmpty ){
               contactThreadStatus = WAITING_ON_CONTACT;
            }
            break;

         case CONTACT_THREAD_STOPPING:
            //Stop all image processing and deleted stored images and data.
            break;

         case CONTACT_THREAD_STOPPED:
            //All data and variables cleaned up. Thread can be exited.
            break;
      }

      if( cachedStatus == CONTACT_THREAD_STOPPED ) break;
   }

   bps_shutdown();
   return NULL;
}

void startContactThread(){
   //pthread_mutex_lock(&contactMutex);
   //bbmsp_event_contact_list_register_event();
   //contactThreadStatus = CONTACT_THREAD_STARTING;
   //pthread_mutex_unlock(&contactMutex);
}

//======================================================================================//
//                   STANDARD FUNCTIONS FROM bbmsp_util.h QNX FILE
//======================================================================================//

//BBMSP_API bbmsp_result_t bbmsp_contact_create(bbmsp_contact_t** contact);
FREObject bbm_ane_bbmsp_contact_create(FREContext ctx, void* functionData, uint32_t argc, FREObject argv[]){
   uint32_t id = rand();
   contact_data_s *contactData;
   bbmsp_contact_t *image;

   contactData = (contact_data_s *)malloc( sizeof(contact_data_s) );
   contactData->id = id;

   pthread_mutex_lock(&contactMutex);
   contactQueue.push(contactData);
   pthread_mutex_unlock(&contactMutex);

   FREObject result;
   FRENewObjectFromUint32(id, &result);
   return result;
}

//BBMSP_API bbmsp_result_t bbmsp_contact_destroy(bbmsp_contact_t** contact);
FREObject bbm_ane_bbmsp_contact_destroy(FREContext ctx, void* functionData, uint32_t argc, FREObject argv[]){
   int imageID;
   FREGetObjectAsInt32(argv[0],&imageID);

   int code = 0;

   FREObject result;
   FRENewObjectFromUint32(code, &result);
   return result;
}

/**
* @brief Copy a source contact to a destination contact.
* @details Copies the source contact to the destination contact.
* @param destination A pointer to destination contact, which can be created
* using @c bbmsp_contact_create().
* @param source A pointer to source contact.
*
* @return @c BBMSP_SUCCESS if successful, @c BBMSP_FAILURE otherwise.
* @see BBMSP_SUCCESS, BBMSP_FAILURE
*/
//BBMSP_API bbmsp_result_t bbmsp_contact_copy(bbmsp_contact_t* destination, const bbmsp_contact_t* source);
FREObject bbm_ane_bbmsp_contact_copy(FREContext ctx, void* functionData, uint32_t argc, FREObject argv[]){
   uint32_t id = rand();
   contact_data_s *contactData;
   bbmsp_contact_t *image;

   contactData = (contact_data_s *)malloc( sizeof(contact_data_s) );
   contactData->id = id;

   pthread_mutex_lock(&contactMutex);
   contactQueue.push(contactData);
   pthread_mutex_unlock(&contactMutex);

   FREObject result;
   FRENewObjectFromUint32(id, &result);
   return result;
}

//BBMSP_API int bbmsp_contact_compare(const bbmsp_contact_t* contact, const bbmsp_contact_t* other_contact);
FREObject bbm_ane_bbmsp_contact_compare(FREContext ctx, void* functionData, uint32_t argc, FREObject argv[]){
   int contact1;
   int contact2;

   FREGetObjectAsInt32(argv[0],&contact1);
   bbmsp_contact_t *contactA = (*bbmsp_contact_map)[contact1];

   FREGetObjectAsInt32(argv[1],&contact2);
   bbmsp_contact_t *contactB = (*bbmsp_contact_map)[contact2];

   int code = bbmsp_contact_compare(contactA,contactB);

   FREObject result;
   FRENewObjectFromUint32(code, &result);
   return result;
}

//BBMSP_API bbmsp_result_t bbmsp_contact_get_display_name(const bbmsp_contact_t* contact, char* display_name,
//                                                        size_t display_name_buffer_size);
FREObject bbm_ane_bbmsp_contact_get_display_name(FREContext ctx, void* functionData,
                                                 uint32_t argc, FREObject argv[]){
   int contactID;
   FREGetObjectAsInt32(argv[0],&contactID);
   bbmsp_contact_t *contact = (*bbmsp_contact_map)[contactID];

   char *name = new char[BBMSP_CONTACT_DISPLAY_NAME_MAX];
   bbmsp_result_t code = bbmsp_contact_get_display_name(contact,name,BBMSP_CONTACT_DISPLAY_NAME_MAX);

   FREObject result;
   FRENewObjectFromUTF8((uint32_t)(strlen(name) + 1), (uint8_t*)name, &result);
   return result;
}

//BBMSP_API bbmsp_result_t bbmsp_contact_get_personal_message(const bbmsp_contact_t* contact, char* personal_message,
//                                                            size_t personal_message_buffer_size);
FREObject bbm_ane_bbmsp_contact_get_personal_message(FREContext ctx, void* functionData, uint32_t argc,
                                                     FREObject argv[]){
   int contactID;
   FREGetObjectAsInt32(argv[0],&contactID);
   bbmsp_contact_t *contact = (*bbmsp_contact_map)[contactID];

   char *msg = new char[BBMSP_CONTACT_PERSONAL_MSG_MAX];
   bbmsp_result_t code = bbmsp_contact_get_personal_message(contact,msg,BBMSP_CONTACT_PERSONAL_MSG_MAX);

   FREObject result;
   FRENewObjectFromUTF8((uint32_t)(strlen(msg) + 1), (uint8_t*)msg, &result);
   return result;
}

//BBMSP_API bbmsp_result_t bbmsp_contact_get_status(const bbmsp_contact_t* contact, bbmsp_presence_status_t* status);
FREObject bbm_ane_bbmsp_contact_get_status(FREContext ctx, void* functionData, uint32_t argc, FREObject argv[]){
   int contactID;
   FREGetObjectAsInt32(argv[0],&contactID);
   bbmsp_contact_t *contact = (*bbmsp_contact_map)[contactID];

   bbmsp_presence_status_t status;
   bbmsp_result_t code = bbmsp_contact_get_status(contact,&status);

   FREObject result;
   FRENewObjectFromUint32(status, &result);
   return result;
}

//BBMSP_API bbmsp_result_t bbmsp_contact_get_status_message(const bbmsp_contact_t* contact, char* status_message,
//                                                          size_t status_message_buffer_size);
FREObject bbm_ane_bbmsp_contact_get_status_message(FREContext ctx, void* functionData,
                                                   uint32_t argc, FREObject argv[]){
   int contactID;
   FREGetObjectAsInt32(argv[0],&contactID);
   bbmsp_contact_t *contact = (*bbmsp_contact_map)[contactID];

   char *msg = new char[BBMSP_CONTACT_STATUS_MSG_MAX];
   bbmsp_result_t code = bbmsp_contact_get_status_message(contact,msg,BBMSP_CONTACT_STATUS_MSG_MAX);

   FREObject result;
   FRENewObjectFromUTF8((uint32_t)(strlen(msg) + 1), (uint8_t*)msg, &result);
   return result;
}

//BBMSP_API bbmsp_result_t bbmsp_contact_get_ppid(const bbmsp_contact_t* contact, char* ppid, size_t ppid_buffer_size);
FREObject bbm_ane_bbmsp_contact_get_ppid(FREContext ctx, void* functionData, uint32_t argc, FREObject argv[]){
   int contactID;
   FREGetObjectAsInt32(argv[0],&contactID);
   bbmsp_contact_t *contact = (*bbmsp_contact_map)[contactID];

   char *ppid = new char[BBMSP_CONTACT_PPID_MAX];
   bbmsp_result_t code = bbmsp_contact_get_ppid(contact,ppid,BBMSP_CONTACT_PPID_MAX);

   FREObject result;
   FRENewObjectFromUTF8((uint32_t)(strlen(ppid) + 1), (uint8_t*)ppid, &result);
   return result;
}

//BBMSP_API bbmsp_result_t bbmsp_contact_get_handle(const bbmsp_contact_t* contact, char* handle,
//                                                  size_t handle_buffer_size);
FREObject bbm_ane_bbmsp_contact_get_handle(FREContext ctx, void* functionData, uint32_t argc, FREObject argv[]){
   int contactID;
   FREGetObjectAsInt32(argv[0],&contactID);
   bbmsp_contact_t *contact = (*bbmsp_contact_map)[contactID];

   char *handle = new char[BBMSP_CONTACT_HANDLE_MAX];
   bbmsp_result_t code = bbmsp_contact_get_handle(contact,handle,BBMSP_CONTACT_HANDLE_MAX);

   FREObject result;
   FRENewObjectFromUTF8((uint32_t)(strlen(handle) + 1), (uint8_t*)handle, &result);
   return result;
}

//BBMSP_API bbmsp_result_t bbmsp_contact_get_app_version(const bbmsp_contact_t* contact, char* app_version,
//                                                       size_t app_version_buffer_size);
FREObject bbm_ane_bbmsp_contact_get_app_version(FREContext ctx, void* functionData,
                                                uint32_t argc, FREObject argv[]){
   int contactID;
   FREGetObjectAsInt32(argv[0],&contactID);
   bbmsp_contact_t *contact = (*bbmsp_contact_map)[contactID];

   char *version = new char[BBMSP_CONTACT_APP_VERSION_MAX];
   bbmsp_result_t code = bbmsp_contact_get_handle(contact,version,BBMSP_CONTACT_APP_VERSION_MAX);

   FREObject result;
   FRENewObjectFromUTF8((uint32_t)(strlen(version)+1), (uint8_t*)version, &result);
   return result;
}

//BBMSP_API bbmsp_result_t bbmsp_contact_get_platform_version(const bbmsp_contact_t* contact, int* platform_version);
FREObject bbm_ane_bbmsp_contact_get_platform_version(FREContext ctx, void* functionData,
                                                     uint32_t argc, FREObject argv[]){
   int contactID;
   FREGetObjectAsInt32(argv[0],&contactID);
   bbmsp_contact_t *contact = (*bbmsp_contact_map)[contactID];

   int platformVersion;
   bbmsp_result_t code = bbmsp_contact_get_platform_version(contact,&platformVersion);
   FREObject result;
   FRENewObjectFromUint32(platformVersion, &result);
   return result;
}

/**
* @brief Retrieve a contact's display picture.
* @details If the contact's display picture has been previously retrieved, this
* function will return @c BBMSP_SUCCESS. If the contact's display picture has
* not been retrieved, this will return @c BBMSP_ASYNC and the data will come
* back through an event of category @c BBMSP_CONTACT_LIST, type @c
* BBMSP_SP_EVENT_CONTACT_CHANGED, and the field @c BBMSP_DISPLAY_PICTURE. If
* there is an error, @c BBMSP_FAILURE will be returned.
* @param contact A pointer to the contact whose display picture to get.
* @param display_picture is updated to the contact's display picture, created
* with @c bbmsp_image_create().
*
* @return @c BBMSP_SUCCESS if successful, @c BBMSP_ASYNC if the response will
* come back asynchronously, and @c BBMSP_FAILURE otherwise.
* @see bbmsp_image_t, BBMSP_SUCCESS, BBMSP_FAILURE
*/
//BBMSP_API bbmsp_result_t bbmsp_contact_get_display_picture(const bbmsp_contact_t* contact,
//                                                           bbmsp_image_t* display_picture);
FREObject bbm_ane_bbmsp_contact_get_display_picture(FREContext ctx, void* functionData,
                                                    uint32_t argc, FREObject argv[]);

/**
* @brief Retrieve the contact whose contact information has changed.
* @details When a contact changes their contact information, a
* @c BBMSP_SP_EVENT_CONTACT_CHANGED event is triggered.
* @param event A pointer to the @c bbmsp event that was triggered.
* @param contact is populated with the contact data whose information was
* changed. The memory allocated by this function needs to be freed using @c
* bbmsp_contact_destroy() when no longer needed.
*
* @return @c BBMSP_SUCCESS if successful, @c BBMSP_FAILURE otherwise.
* @see bbmsp_event_t, BBMSP_SUCCESS, BBMSP_FAILURE
*/
//BBMSP_API bbmsp_result_t bbmsp_event_contact_changed_get_contact(bbmsp_event_t* event, bbmsp_contact_t** contact);
FREObject bbm_ane_bbmsp_event_contact_changed_get_contact(FREContext ctx, void* functionData,
                                                          uint32_t argc, FREObject argv[]);

/**
* @brief Retrieve the field that was modified when a contact's profile is
* changed.
* @details When a contact changes their profile information, a
* @c BBMSP_SP_EVENT_PROFILE_CHANGED event is triggered.
* @param event A pointer to the @c bbmsp event that was triggered.
* @param update_type is updated with the field that was changed.
*
* @return @c BBMSP_SUCCESS if successful, @c BBMSP_FAILURE otherwise.
* @see bbmsp_event_t, bbmsp_presence_update_types_t, BBMSP_SUCCESS,
* BBMSP_FAILURE
*/
//BBMSP_API bbmsp_result_t bbmsp_event_contact_changed_get_presence_update_type(bbmsp_event_t* event,
//                                                                              bbmsp_presence_update_types_t* update_type);
FREObject bbm_ane_bbmsp_event_contact_changed_get_presence_update_type(FREContext ctx, void* functionData,
                                                                       uint32_t argc, FREObject argv[]);

/**
* @brief Asynchronously retrieve the user's contact list containing the contacts
* that have your application installed.
* @details A @c BBMSP_SP_EVENT_CONTACT_LIST_FULL event is triggered when the
* contact list is available.
*
* @return @c BBMSP_ASYNC if successful, @c BBMSP_FAILURE otherwise.
* @see bbmsp_event_contact_list, BBMSP_ASYNC, BBMSP_FAILURE
*/
//BBMSP_API bbmsp_result_t bbmsp_contact_list_get(void);
FREObject bbm_ane_bbmsp_contact_list_get(FREContext ctx, void* functionData, uint32_t argc, FREObject argv[]){
   bbmsp_result_t code = bbmsp_contact_list_get();
   contactThreadStatus = CONTACT_THREAD_STARTING;
   FREObject result;
   FRENewObjectFromUint32(code, &result);
   return result;
}

/**
* @brief Create a new contact list.
* @details Creates a new, empty contact list. When the contact list is no longer
* required, call @c bbmsp_contact_list_destroy() to free up the associated
* memory.
* @param contact_list is updated to an empty contact list.
*
* @return @c BBMSP_SUCCESS if successful, @c BBMSP_FAILURE otherwise.
* @see bbmsp_contact_list_destroy(), bbmsp_contact_list_t, BBMSP_SUCCESS,
* BBMSP_FAILURE
*/
//BBMSP_API bbmsp_result_t bbmsp_contact_list_create(bbmsp_contact_list_t** contact_list);
FREObject bbm_ane_bbmsp_contact_list_create(FREContext ctx, void* functionData, uint32_t argc, FREObject argv[]);

/**
* @brief Destroy a contact list.
* @details Destroys this contact list and frees up the associated memory.
*
* @param contact_list A pointer to the contact list to destroy.
*
* @return @c BBMSP_SUCCESS if successful, @c BBMSP_FAILURE otherwise.
* @see bbmsp_contact_list_t, BBMSP_SUCCESS, BBMSP_FAILURE
*/
//BBMSP_API bbmsp_result_t bbmsp_contact_list_destroy(bbmsp_contact_list_t** contact_list);
FREObject bbm_ane_bbmsp_contact_list_destroy(FREContext ctx, void* functionData, uint32_t argc, FREObject argv[]);

/**
* @brief Add a contact to this contact list.
* @details If the contact already exists it is replaced with this new value.
*
* @param contact_list A pointer to the contact list to add the contact to.
* @param contact A pointer to the contact to add.
*
* @return @c BBMSP_SUCCESS if successful, @c BBMSP_FAILURE otherwise.
* @see BBMSP_SUCCESS, BBMSP_FAILURE
*/
//BBMSP_API bbmsp_result_t bbmsp_contact_list_add_contact(bbmsp_contact_list_t* contact_list,
//                                                        bbmsp_contact_t* contact);
FREObject bbm_ane_bbmsp_contact_list_add_contact(FREContext ctx, void* functionData, uint32_t argc, FREObject argv[]);

/**
* @brief Check if a contact is in a contact list.
* @details The contact's handle is used to determine equality.
* @param contact_list A pointer to the contact list to check.
* @param contact A pointer to the contact to search for.
*
* @return @c BBMSP_SUCCESS if found, @c BBMSP_FAILURE otherwise.
* @see bbmsp_contact_list_t, bbmsp_contact_t, @ref contactlist_ids
*/
//BBMSP_API bbmsp_result_t bbmsp_contact_list_contains_contact(bbmsp_contact_list_t* contact_list,
//                                                             bbmsp_contact_t* contact);
FREObject bbm_ane_bbmsp_contact_list_contains_contact(FREContext ctx, void* functionData,
                                                      uint32_t argc, FREObject argv[]);

/**
* @brief Retrieve a contact with the handle from a contact list.
* @param contact_list A pointer to the contact list to search.
* @param handle A null-terminated pointer to the handle.
* @param contact is created using @c bbmsp_contact_create() and updated to the
* found contact.
*
* @return @c BBMSP_SUCCESS if found, @c BBMSP_FAILURE otherwise.
* @see bbmsp_contact_list_t, bbmsp_contact_t, BBMSP_SUCCESS, BBMSP_FAILURE,
* @ref contactlist_ids
*/
//BBMSP_API bbmsp_result_t bbmsp_contact_list_get_contact(const bbmsp_contact_list_t* contact_list, char* handle,
//                                                        bbmsp_contact_t* contact);
FREObject bbm_ane_bbmsp_contact_list_get_contact(FREContext ctx, void* functionData, uint32_t argc, FREObject argv[]);

/**
* @brief Return all contacts in the contact list.
* @details @c contact_array should be created to point to an array of @c
* bbmsp_contact_t pointers as large as the number of elements in the contact
* list.
* @param contact_list A pointer to the contact list.
* @param contact_array An updated array of @c bbmsp_contact_t pointers.
*
* @return @c BBMSP_SUCCESS if successful, @c BBMSP_FAILURE otherwise.
* @see BBMSP_SUCCESS, BBMSP_FAILURE
*/
//BBMSP_API bbmsp_result_t bbmsp_contact_list_get_all_contacts(const bbmsp_contact_list_t* contact_list,
//                                                             bbmsp_contact_t** contact_array);
FREObject bbm_ane_bbmsp_contact_list_get_all_contacts(FREContext ctx, void* functionData,
                                                      uint32_t argc, FREObject argv[]);

/**
* @brief Remove a contact from a contact list.
* @param contact_list A pointer to the contact list to remove the contact from.
* @param contact A pointer to the contact to remove.
*
* @return @c BBMSP_SUCCESS if successful, @c BBMSP_FAILURE otherwise.
* @see BBMSP_SUCCESS, BBMSP_FAILURE, @ref contactlist_ids
*/
//BBMSP_API bbmsp_result_t bbmsp_contact_list_remove_contact(bbmsp_contact_list_t* contact_list,
//                                                           const bbmsp_contact_t* contact);
FREObject bbm_ane_bbmsp_contact_list_remove_contact(FREContext ctx, void* functionData,
                                                    uint32_t argc, FREObject argv[]);

/**
* @brief Retrieve the total number of contacts in the contact list.
*
* @param contact_list A pointer to the contact list.
*
* @return The size of the contact list.
*/
//BBMSP_API size_t bbmsp_contact_list_get_size(const bbmsp_contact_list_t* contact_list);
FREObject bbm_ane_bbmsp_contact_list_get_size(FREContext ctx, void* functionData, uint32_t argc, FREObject argv[]){
   int size = bbmsp_contact_list_get_size(contactList);
   FREObject result;
   FRENewObjectFromUint32(size, &result);
   return result;
}




#ifdef __cplusplus
}
#endif


