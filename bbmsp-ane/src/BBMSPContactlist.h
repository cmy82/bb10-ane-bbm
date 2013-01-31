/*
 * BBMSPContactlist.h
 *
 *  Created on: Jan 19, 2013
 *      Author: HU77
 */


#include "FlashRuntimeExtensions.h"
#include <queue>
#include <bps/bps.h>
#include <bps/event.h>
#include <bbmsp/bbmsp.h>
#include <bbmsp/bbmsp_events.h>
#include <bbmsp/bbmsp_context.h>
#include <bbmsp/bbmsp_util.h>
#include <bbmsp/bbmsp_messaging.h>
#include <bbmsp/bbmsp_contactlist.h>


#ifndef BBMANE_BBMSPCONTACTLIST_H_
#define BBMANE_BBMSPCONTACTLIST_H_


#ifdef __cplusplus
extern "C" {
#endif


//======================================================================================//
//          GLOBAL VARIABLES/DEFINITIONS
//======================================================================================//

typedef enum {
   CONTACT_THREAD_INITIALIZING = 0,
   CONTACT_THREAD_STARTING,
   WAITING_ON_CONTACT,
   PROCESSING_CONTACT,
   CONTACT_THREAD_STOPPING,
   CONTACT_THREAD_STOPPED
} ane_contact_thread_status_e;

typedef enum {
   CREATE_CONTACT = 0,
   COPY_CONTACT,
   DELETE_CONTACT
} ane_contact_thread_action_e;

//======================================================================================//
//          CUSTOM FUNCTIONS
//======================================================================================//

void* initContactThread(void *data);

//======================================================================================//
//                   STANDARD FUNCTIONS FROM bbmsp_util.h QNX FILE
//======================================================================================//

/**
* @brief Create a new BBM contact structure.
* @details Creates a BBM contact structure. When the contact is no longer required,
* call @c bbmsp_contact_destroy() to free up the associated memory.
* @param contact An updated pointer to the new contact.
*
* @return @c BBMSP_SUCCESS if successful, @c BBMSP_FAILURE otherwise.
* @see bbmsp_contact_destroy(), BBMSP_SUCCESS, BBMSP_FAILURE
*/
//BBMSP_API bbmsp_result_t bbmsp_contact_create(bbmsp_contact_t** contact);
FREObject bbm_ane_bbmsp_contact_create(FREContext ctx, void* functionData, uint32_t argc, FREObject argv[]);

/**
* @brief Destroy a BBM contact structure.
* @details Destroys this BBM contact structure and frees up the associated memory.
* @param contact A pointer to the contact to destroy.
*
* @return @c BBMSP_SUCCESS if successful, @c BBMSP_FAILURE otherwise.
* @see BBMSP_SUCCESS, BBMSP_FAILURE
*/
//BBMSP_API bbmsp_result_t bbmsp_contact_destroy(bbmsp_contact_t** contact);
FREObject bbm_ane_bbmsp_contact_destroy(FREContext ctx, void* functionData, uint32_t argc, FREObject argv[]);

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
FREObject bbm_ane_bbmsp_contact_copy(FREContext ctx, void* functionData, uint32_t argc, FREObject argv[]);

/**
* @brief Compare two contacts by display name.
* @details You can use this function to create a list of contacts that are
* sorted alphabetically.
* @param contact A pointer to the first contact name.
* @param other_contact A pointer to the contact name to compare to.
*
* @return A negative integer if @c contact display name is lexicographically
* less than @c other_contact display name; zero if @c contact display name is
* lexicographically equal to @c other_contact display name; a positive
* integer if @c contact display name is lexicographically greater than
* @c other_contact display name. -1 is returned if invalid parameters are
* provided.
*/
//BBMSP_API int bbmsp_contact_compare(const bbmsp_contact_t* contact, const bbmsp_contact_t* other_contact);
FREObject bbm_ane_bbmsp_contact_compare(FREContext ctx, void* functionData, uint32_t argc, FREObject argv[]);

/**
* @brief Retrieve a contact's display name.
*
* @param contactID ID of the contact whose display name to retrieve.
*
* @return string containing the contacts display name
*/
//BBMSP_API bbmsp_result_t bbmsp_contact_get_display_name(const bbmsp_contact_t* contact, char* display_name,
//                                                        size_t display_name_buffer_size);
FREObject bbm_ane_bbmsp_contact_get_display_name(FREContext ctx, void* functionData, uint32_t argc, FREObject argv[]);

/**
* @brief Retrieve a contact's personal message.
*
* @param contactID ID of the contact to get the message for.
*
* @return string containing the contacts personal message
*/
//BBMSP_API bbmsp_result_t bbmsp_contact_get_personal_message(const bbmsp_contact_t* contact, char* personal_message,
//                                                            size_t personal_message_buffer_size);
FREObject bbm_ane_bbmsp_contact_get_personal_message(FREContext ctx, void* functionData, uint32_t argc,
                                                     FREObject argv[]);

/**
* @brief Retrieve a contact's status.
* @details A contact's status can be either busy or available.
* @param contact A pointer to the contact whose status to get.
* @param status is updated with the contact's status.
*
* @return @c BBMSP_SUCCESS if successful, @c BBMSP_FAILURE otherwise.
* @see BBMSP_SUCCESS, BBMSP_FAILURE, bbmsp_presence_status_t
*/
//BBMSP_API bbmsp_result_t bbmsp_contact_get_status(const bbmsp_contact_t* contact, bbmsp_presence_status_t* status);
FREObject bbm_ane_bbmsp_contact_get_status(FREContext ctx, void* functionData, uint32_t argc, FREObject argv[]);

/**
* @brief Retrieve a contact's status message.
*
* @param contactID ID of the contact to get the message for.
*
* @return string containing the contacts status message
*/
//BBMSP_API bbmsp_result_t bbmsp_contact_get_status_message(const bbmsp_contact_t* contact, char* status_message,
//                                                          size_t status_message_buffer_size);
FREObject bbm_ane_bbmsp_contact_get_status_message(FREContext ctx, void* functionData, uint32_t argc, FREObject argv[]);

/**
* @brief Retrieve a contact's PPID.
* @details The user's PPID is the same across multiple instances of BBM (for
* example, on a BlackBerry smartphone and a BlackBerry tablet), when the user
* signs in with the same BlackBerry ID. PPID is encoded as a Base64 string using
* the ASCII character set.
*
* @param contact A pointer to the contact whose PPID to get.
* @param ppid A buffer that will be updated with the PPID encoded as Base64.
* The resulting buffer will be null-terminated on success.
* @param ppid_buffer_size The size of the @c ppid buffer. This should be @c
* BBMSP_CONTACT_PPID_MAX.
*
* @return @c BBMSP_SUCCESS if successful, @c BBMSP_FAILURE otherwise.
* @see BBMSP_SUCCESS, BBMSP_FAILURE, BBMSP_CONTACT_PPID_MAX
*/
//BBMSP_API bbmsp_result_t bbmsp_contact_get_ppid(const bbmsp_contact_t* contact, char* ppid, size_t ppid_buffer_size);
FREObject bbm_ane_bbmsp_contact_get_ppid(FREContext ctx, void* functionData, uint32_t argc, FREObject argv[]);

/**
* @brief Retrieve a contact's handle.
* @details The @c handle is an identifier for a user on the BBM Social Platform
* whose device is running BlackBerry 7 or earlier. It is provided for backwards
* compatibility and should NOT be used as a persistent identifier for a user.
* Use the @c PPID instead.
*
* A user's @c handle is the same for each instance of BBM (for example, on a
* BlackBerry smartphone and a BlackBerry tablet). @c Handle is encoded as a
* Base64 string using the ASCII character set.
*
* @param contact A pointer to the contact whose handle to get.
* @param handle A buffer that will be updated with the handle encoded as Base64.
* The resulting buffer will be null-terminated on success.
* @param handle_buffer_size The size of the @c handle buffer. This should be
* @c BBMSP_CONTACT_HANDLE_MAX.
*
* @return @c BBMSP_SUCCESS if successful, @c BBMSP_FAILURE otherwise.
* @see BBMSP_SUCCESS, BBMSP_FAILURE, BBMSP_CONTACT_HANDLE_MAX,
* @ref contactlist_ids
*/
//BBMSP_API bbmsp_result_t bbmsp_contact_get_handle(const bbmsp_contact_t* contact, char* handle,
//                                                  size_t handle_buffer_size);
FREObject bbm_ane_bbmsp_contact_get_handle(FREContext ctx, void* functionData, uint32_t argc, FREObject argv[]);

/**
* @brief Retrieve a contact's version of the app that they have installed.
* @param contact A pointer to the contact whose app version to get.
* @param app_version A buffer that will be updated with the app version encoded
* as UTF-8. The resulting buffer will be null-terminated on success.
* @param app_version_buffer_size The size of the @c app_version buffer. This
* should be @c BBMSP_CONTACT_APP_VERSION_MAX.
*
* @return @c BBMSP_SUCCESS if successful, @c BBMSP_FAILURE otherwise.
* @see BBMSP_SUCCESS, BBMSP_FAILURE, BBMSP_CONTACT_APP_VERSION_MAX
*/
//BBMSP_API bbmsp_result_t bbmsp_contact_get_app_version(const bbmsp_contact_t* contact, char* app_version,
//                                                       size_t app_version_buffer_size);
FREObject bbm_ane_bbmsp_contact_get_app_version(FREContext ctx, void* functionData, uint32_t argc, FREObject argv[]);

/**
* @brief Retrieve a contact's version of the BBM Social Platform.
* @param contact A pointer to the contact whose BBM Social Platform version to
* get.
* @param platform_version is updated to the BBM Social Platform version of the
* contact.
*
* @return @c BBMSP_SUCCESS if successful, @c BBMSP_FAILURE otherwise.
* @see BBMSP_SUCCESS, BBMSP_FAILURE
*/
//BBMSP_API bbmsp_result_t bbmsp_contact_get_platform_version(const bbmsp_contact_t* contact, int* platform_version);
FREObject bbm_ane_bbmsp_contact_get_platform_version(FREContext ctx, void* functionData,
                                                     uint32_t argc, FREObject argv[]);

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
FREObject bbm_ane_bbmsp_contact_list_get(FREContext ctx, void* functionData, uint32_t argc, FREObject argv[]);

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
FREObject bbm_ane_bbmsp_contact_list_get_size(FREContext ctx, void* functionData, uint32_t argc, FREObject argv[]);



#ifdef __cplusplus
}
#endif


#endif /* BBMSPCONTACTLIST_H_ */



