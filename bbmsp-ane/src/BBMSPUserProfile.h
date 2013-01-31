/*
 * BBMSPUserProfile.h
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


#ifndef BBMANE_BBMSPUSERPROFILE_H_
#define BBMANE_BBMSPUSERPROFILE_H_


#ifdef __cplusplus
extern "C" {
#endif


//======================================================================================//
//          GLOBAL VARIABLES/DEFINITIONS
//======================================================================================//

typedef enum {
   PROFILE_THREAD_INITIALIZING = 0,
   PROFILE_THREAD_STARTING,
   WAITING_ON_PROFILE,
   PROCESSING_PROFILE,
   PROFILE_THREAD_STOPPING,
   PROFILE_THREAD_STOPPED
} ane_profile_thread_status_e;

//======================================================================================//
//          CUSTOM FUNCTIONS
//======================================================================================//

void* initProfileThread(void *data);

//======================================================================================//
//                 STANDARD FUNCTIONS FROM bbmsp_profile.h QNX FILE
//======================================================================================//

/**
 * @brief Retrieve the user's BBM profile information.
 *
 * @details You can retrieve the user's profile information, for example,
 * display name, status, and personal message.
 *
 * @param profile A pointer to the user's BBM profile.
 *
 * @return @c BBMSP_SUCCESS if successful, @c BBMSP_FAILURE otherwise.
 * @see bbmsp_profile_t, BBMSP_SUCCESS, BBMSP_FAILURE
 */
//BBMSP_API bbmsp_result_t bbmsp_get_user_profile(bbmsp_profile_t* profile);
FREObject bbm_ane_bbmsp_get_user_profile(FREContext ctx, void* functionData,
                                         uint32_t argc, FREObject argv[]);

/**
 * @brief Send a request to set the user's BBM status message.
 *
 * @details A user's BBM status can be either available or busy.
 *
 * @param status The user's BBM status.
 * @param custom_status_message A pointer to the custom status message array.
 *
 * @return @c BBMSP_ASYNC if successful, @c BBMSP_FAILURE
 * otherwise.
 * @see bbmsp_presence_status_t, BBMSP_ASYNC, BBMSP_FAILURE
 */
//BBMSP_API bbmsp_result_t bbmsp_set_user_profile_status(bbmsp_presence_status_t status,
//                                                       const char* custom_status_message);
FREObject bbm_ane_bbmsp_set_user_profile_status(FREContext ctx, void* functionData,
                                                uint32_t argc, FREObject argv[]);

/**
 * @brief Send a request to set the user's BBM personal message.
 * @details Sets the user's BBM personal message. A dialog displays to allow the
 * user to accept or reject the change. A personal message is limited to 160
 * characters; anything longer is truncated. If null is passed in, the personal
 * message is cleared.
 *
 * @param personal_message A pointer to the personal message array, encoded as
 * UTF-8.
 *
 * @return @c BBMSP_ASYNC if successful in sending the request,
 * @c BBMSP_FAILURE otherwise.
 * @see BBMSP_ASYNC, BBMSP_FAILURE
 */
//BBMSP_API bbmsp_result_t bbmsp_set_user_profile_personal_message(const char* personal_message);
FREObject bbm_ane_bbmsp_set_user_profile_personal_message(FREContext ctx, void* functionData,
                                                          uint32_t argc, FREObject argv[]);

/**
 * @brief Send a request to set the user's BBM display picture.
 *
 * @details A dialog displays to allow the user to accept or reject the change
 * to the user's BBM display picture. The display picture is limited to 32kB of
 * data; an attempt to set a display picture that exceeds this limit will fail
 * and will not display the dialog.
 *
 * @param image A pointer to the display picture.
 *
 * @return @c BBMSP_ASYNC if successful in sending the request,
 * @c BBMSP_FAILURE otherwise.
 * @see BBMSP_ASYNC, BBMSP_FAILURE, BBMSP_PROFILE_DISPLAY_PICTURE_MAX
 */
//BBMSP_API bbmsp_result_t bbmsp_set_user_profile_display_picture(bbmsp_image_t* image);
FREObject bbm_ane_bbmsp_set_user_profile_display_picture(FREContext ctx, void* functionData,
                                                         uint32_t argc, FREObject argv[]);

/**
 * @brief Create a user profile.
 *
 * @details Creates a new user profile. When the user profile is no longer
 * required, call @c bbmsp_profile_destroy() to free up the associated memory.
 *
 * @param profile An updated pointer to the new profile.
 *
 * @return @c BBMSP_SUCCESS if successful, @c BBMSP_FAILURE otherwise.
 * @see bbmsp_profile_destroy(), BBMSP_SUCCESS, BBMSP_FAILURE
 */
//BBMSP_API bbmsp_result_t bbmsp_profile_create(bbmsp_profile_t** profile);
FREObject bbm_ane_bbmsp_profile_create(FREContext ctx, void* functionData,
                                       uint32_t argc, FREObject argv[]);

/**
 * @brief Destroy a user profile.
 * @details Destroys a user profile and frees up the associated memory.
 * @param profile A pointer to the profile to destroy.
 *
 * @return @c BBMSP_SUCCESS if successful, @c BBMSP_FAILURE otherwise.
 * @see BBMSP_SUCCESS, BBMSP_FAILURE
 */
//BBMSP_API bbmsp_result_t bbmsp_profile_destroy(bbmsp_profile_t** profile);
FREObject bbm_ane_bbmsp_profile_destroy(FREContext ctx, void* functionData,
                                        uint32_t argc, FREObject argv[]);

/**
 * @brief Retrieve the user's BBM display name.
 *
 * @param profile A pointer to the user's BBM profile.
 * @param display_name A pointer to the array that contains the user's display
 * name, encoded as UTF-8.
 * @param buffer_size The size of the @c display_name buffer array.
 *
 * @return @c BBMSP_SUCCESS if successful, @c BBMSP_FAILURE otherwise.
 * @see BBMSP_SUCCESS, BBMSP_FAILURE, BBMSP_PROFILE_DISPLAY_NAME_MAX
 */
//BBMSP_API bbmsp_result_t bbmsp_profile_get_display_name(bbmsp_profile_t* profile,
//                                                        char* display_name,
//                                                        size_t buffer_size);
FREObject bbm_ane_bbmsp_profile_get_display_name(FREContext ctx, void* functionData,
                                                 uint32_t argc, FREObject argv[]);

/**
 * @brief Retrieve the user's BBM personal message.
 * @details A personal message is limited to 160 characters; anything longer is
 * truncated.
 *
 * @param profile A pointer to the user's BBM profile.
 * @param personal_message An updated pointer to the array that contains the
 * user's personal message, encoded as UTF-8.
 * @param buffer_size The size of the @c personal_message buffer array.
 *
 * @return @c BBMSP_SUCCESS if successful, @c BBMSP_FAILURE otherwise.
 * @see BBMSP_SUCCESS, BBMSP_FAILURE, BBMSP_PROFILE_PERSONAL_MSG_MAX
 */
//BBMSP_API bbmsp_result_t bbmsp_profile_get_personal_message(bbmsp_profile_t* profile,
//                                                            char* personal_message,
//                                                            size_t buffer_size);
FREObject bbm_ane_bbmsp_profile_get_personal_message(FREContext ctx, void* functionData,
                                                     uint32_t argc, FREObject argv[]);

/**
 * @brief Retrieve the user's BBM status.
 *
 * @param profile A pointer to the user's BBM profile.
 * @param status An updated pointer to the user's status.
 *
 * @return @c BBMSP_SUCCESS if successful, @c BBMSP_FAILURE otherwise.
 * @see bbmsp_presence_status_t, BBMSP_SUCCESS, BBMSP_FAILURE
 */
//BBMSP_API bbmsp_result_t bbmsp_profile_get_status(bbmsp_profile_t* profile,
//                                                  bbmsp_presence_status_t* status);
FREObject bbm_ane_bbmsp_profile_get_status(FREContext ctx, void* functionData,
                                           uint32_t argc, FREObject argv[]);

/**
 * @brief Retrieve the user's BBM status message.
 *
 * @param profile A pointer to the user's BBM profile.
 * @param status_message An updated pointer to the buffer array that contains
 * the user's status message, encoded as UTF-8.
 * @param buffer_size The size of the @c status_message buffer array.
 *
 * @return @c BBMSP_SUCCESS if successful, @c BBMSP_FAILURE otherwise.
 * @see BBMSP_SUCCESS, BBMSP_FAILURE, BBMSP_PROFILE_STATUS_MSG_MAX
 */
//BBMSP_API bbmsp_result_t bbmsp_profile_get_status_message(bbmsp_profile_t* profile,
//                                                          char* status_message,
//                                                          size_t buffer_size);
FREObject bbm_ane_bbmsp_profile_get_status_message(FREContext ctx, void* functionData,
                                                   uint32_t argc, FREObject argv[]);

/**
 * @brief Retrieve the device-independent @c PPID of the user.
 * @details The user's @c PPID is the same across multiple instances of BBM (for
 * example, on a BlackBerry smartphone and a BlackBerry tablet), when the user
 * signs in with the same BlackBerry ID. @c PPID is encoded as a base64 string
 * using the ASCII character set.
 *
 * @param profile A pointer to the user's BBM profile.
 * @param ppid An updated pointer to the @c PPID buffer array, encoded as UTF-8.
 * @param buffer_size The size of the @c PPID buffer array.
 *
 * @return @c BBMSP_SUCCESS if successful, @c BBMSP_FAILURE otherwise.
 * @see BBMSP_SUCCESS, BBMSP_FAILURE, BBMSP_PROFILE_PPID_MAX
 * @ref contactlist_ids
 */
//BBMSP_API bbmsp_result_t bbmsp_profile_get_ppid(bbmsp_profile_t* profile,
//                                                char* ppid,
//                                                size_t buffer_size);
FREObject bbm_ane_bbmsp_profile_get_ppid(FREContext ctx, void* functionData,
                                         uint32_t argc, FREObject argv[]);

/**
 * @brief Retrieve the handle of the user.
 * @details The @c handle is an identifier for a user on the BBM Social Platform
 * whose device is running BlackBerry 7 or earlier. It is provided for backwards
 * compatibility and should NOT be used as a persistent identifier for a user.
 * Use the @c PPID instead.
 *
 * The user's @c handle is the same for each instance of BBM (for example, on a
 * BlackBerry smartphone and a BlackBerry tablet). @c Handle is encoded as a
 * Base64 string using the ASCII character set.
 *
 * @param profile A pointer to the user's BBM profile.
 * @param handle An updated pointer to the @c handle buffer array, encoded as
 * UTF-8.
 * @param buffer_size The size of the @c handle buffer array.
 *
 * @return @c BBMSP_SUCCESS if successful, @c BBMSP_FAILURE otherwise.
 * @see BBMSP_SUCCESS, BBMSP_FAILURE, BBMSP_PROFILE_HANDLE_MAX,
 * bbmsp_profile_get_ppid()
 * @ref contactlist_ids
 */
//BBMSP_API bbmsp_result_t bbmsp_profile_get_handle(bbmsp_profile_t* profile,
//                                                  char* handle,
//                                                  size_t buffer_size);
FREObject bbm_ane_bbmsp_profile_get_handle(FREContext ctx, void* functionData,
                                           uint32_t argc, FREObject argv[]);

/**
 * @brief Retrieve the version of the BBM connected app that the user is
 * running.
 *
 * @param profile A pointer to the contact's BBM profile.
 * @param app_version An updated pointer to the app version array, encoded as
 * UTF-8.
 * @param buffer_size The size of the @c app_version buffer array.
 *
 * @return @c BBMSP_SUCCESS if successful, @c BBMSP_FAILURE otherwise.
 * @see BBMSP_SUCCESS, BBMSP_FAILURE
 */
//BBMSP_API bbmsp_result_t bbmsp_profile_get_app_version(bbmsp_profile_t* profile,
//                                                       char* app_version,
//                                                       size_t buffer_size);
FREObject bbm_ane_bbmsp_profile_get_app_version(FREContext ctx, void* functionData,
                                                uint32_t argc, FREObject argv[]);

/*!@cond PRIVATE
 * @deprecated This method updates @c platform_version to 1 if @c
 * platform_version is a valid pointer and returns @c BBMSP_SUCCESS. Otherwise,
 * it returns @c BBMSP_FAILURE. Use @c BBMSP_VERSION to get the BBM Social
 * Platform version.
 * @see BBMSP_VERSION, BBMSP_SUCCESS, BBMSP_FAILURE
 */
//BBMSP_API BBMSP_API_DEPRECATED bbmsp_result_t bbmsp_profile_get_platform_version(bbmsp_profile_t* profile,
//                                                                                 int* platform_version);
//!@endcond PRIVATE

/**
 * @brief Retrieve the user's display picture.
 *
 * @param profile A pointer to the user's profile.
 * @param display_picture An updated pointer to the user's display picture.
 *
 * @return @c BBMSP_SUCCESS if successful, @c BBMSP_FAILURE otherwise.
 * @see BBMSP_SUCCESS, BBMSP_FAILURE
 */
//BBMSP_API bbmsp_result_t bbmsp_profile_get_display_picture(bbmsp_profile_t* profile,
//                                                           bbmsp_image_t* display_picture);
FREObject bbm_ane_bbmsp_profile_get_display_picture(FREContext ctx, void* functionData,
                                                    uint32_t argc, FREObject argv[]);

/**
 * @brief Set the status and status message in the current user's BBM profile.
 *
 * @details A dialog displays to allow the user to accept or reject the change.
 *
 * @param profile A pointer to the user's BBM profile.
 * @param status The user's BBM status.
 * @param custom_status_message A pointer to the custom status message array.
 *
 * @return @c BBMSP_SUCCESS if successful, @c BBMSP_FAILURE otherwise.
 * @see bbmsp_presence_status_t, BBMSP_SUCCESS, BBMSP_FAILURE
 */
//BBMSP_API bbmsp_result_t bbmsp_profile_set_status(bbmsp_profile_t* profile,
//                                                  bbmsp_presence_status_t status,
//                                                  const char* custom_status_message);
FREObject bbm_ane_bbmsp_profile_set_status(FREContext ctx, void* functionData,
                                           uint32_t argc, FREObject argv[]);

/**
 * @brief Set the personal message in the current user's BBM profile.
 *
 * @details A personal message is limited to 160 characters; anything beyond is
 * truncated. If null is passed in, the personal message is cleared. A dialog
 * displays to allow the user to accept or reject the change.
 *
 *
 * @param profile A pointer to the user's BBM profile.
 * @param personal_message A pointer to the user's personal message as an array,
 * encoded as UTF-8.
 *
 * @return @c BBMSP_SUCCESS if successful, @c BBMSP_FAILURE otherwise.
 * @see BBMSP_SUCCESS, BBMSP_FAILURE
 */
//BBMSP_API bbmsp_result_t bbmsp_profile_set_personal_message(bbmsp_profile_t* profile,
//                                                            const char* personal_message);
FREObject bbm_ane_bbmsp_profile_set_personal_message(FREContext ctx, void* functionData,
                                                     uint32_t argc, FREObject argv[]);

/**
 * @brief Sets the display picture in the current user's BBM profile.
 *
 * @details The display picture change only affects the profile
 * data structure passed in.
 *
 * @param profile A pointer to the user's BBM profile.
 * @param display_picture A pointer to the display picture.
 *
 * @return @c BBMSP_SUCCESS if successful, @c BBMSP_FAILURE otherwise.
 * @see BBMSP_SUCCESS, BBMSP_FAILURE
 */
//BBMSP_API bbmsp_result_t bbmsp_profile_set_display_picture(bbmsp_profile_t* profile,
//                                                           bbmsp_image_t* display_picture);
FREObject bbm_ane_bbmsp_profile_set_display_picture(FREContext ctx, void* functionData,
                                                    uint32_t argc, FREObject argv[]);

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

/**
 * @brief Does not update the user's display name. Only updates the provided
 * @c profile instance.
 *
 * @details Upon receiving an update to the user's display name, you can update
 * your own copy of the @c profile with the new display name by using this
 * function. Your app cannot trigger a change to the user's display name through
 * this function.
 *
 * @param profile A pointer to the user's BBM profile.
 * @param display_name A pointer to the user's display name as an array,
 * encoded as UTF-8.
 *
 * @return @c BBMSP_SUCCESS if successful, @c BBMSP_FAILURE otherwise.
 * @see BBMSP_SUCCESS, BBMSP_FAILURE
 */
//BBMSP_API bbmsp_result_t bbmsp_profile_set_display_name(bbmsp_profile_t* profile,
//                                                        const char* display_name);
FREObject bbm_ane_bbmsp_profile_set_display_name(FREContext ctx, void* functionData, uint32_t argc, FREObject argv[]);




#ifdef __cplusplus
}
#endif


#endif /* BBMSPUSERPROFILE_H_ */
