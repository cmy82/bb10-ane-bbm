/*
 * BBMSPUserProfileBox.h
 *
 *  Created on: Jan 19, 2013
 *      Author: HU77
 */


#include "FlashRuntimeExtensions.h"


#ifndef BBMANE_BBMSPUSERPROFILEBOX_H_
#define BBMANE_BBMSPUSERPROFILEBOX_H_


#ifdef __cplusplus
extern "C" {
#endif



//======================================================================================//
//          GLOBAL VARIABLES/DEFINITIONS
//======================================================================================//

typedef enum {
   PROFILE_BOX_THREAD_INIT = 0,
   PBOX_THREAD_ACCESS_PEND,
   PBOX_THREAD_STARTING,
   PROFILE_BOX_LOADED,
   PBOX_THREAD_STOPPING,
   PBOX_THREAD_STOPPED
} ane_profile_box_thread_status_e;

typedef enum {
   START_LOADING_PROFILE_BOX,
   PROFILE_BOX_CHANGED_ADD,
   PROFILE_BOX_CHANGED_DEL,
   PROFILE_BOX_CHANGED_ICN_ADD,
   PROFILE_BOX_CHANGED_ICN_RET
} ane_profile_box_event_e;

//======================================================================================//
//          CUSTOM FUNCTIONS
//======================================================================================//

void* initProfileBoxThread(void *data);
static void notifyProfileBoxChanged(char *event);


//======================================================================================//
//            STANDARD FUNCTIONS FROM bbmsp_user_profile_box.h QNX FILE
//======================================================================================//

/**
 * @brief Retrieve the ID associated with a profile box item.
 * @details Each profile box item is identified by a unique,
 * non-negative numeric ID. If @c bbmsp_can_show_profile_box() returns false,
 * this method will return @c BBMSP_FAILURE.
 *
 * @param item A pointer to the profile box item.
 * @param item_id_buffer A pointer to the buffer where the ID will be
 * stored.
 * @param buffer_size The size of the @c itemid buffer.
 *
 * @return @c BBMSP_SUCCESS if successful, @c BBMSP_FAILURE otherwise.
 * @see BBMSP_SUCCESS, BBMSP_FAILURE, BBMSP_PROFILE_BOX_ITEM_ID_MAX
 */
//BBMSP_API bbmsp_result_t bbmsp_user_profile_box_item_get_item_id(const bbmsp_user_profile_box_item_t* item,
//                                                                 char* item_id_buffer, size_t buffer_size);
FREObject bbm_ane_bbmsp_user_profile_box_item_get_item_id(FREContext ctx, void* functionData,
                                                          uint32_t argc, FREObject argv[]);

/**
 * @brief Retrieve the cookie associated with a profile box item.
 * @details A cookie is a string that you can customize for your app. If @c
 * bbmsp_can_show_profile_box() returns false, this method will return @c
 * BBMSP_FAILURE.
 *
 * @param item A pointer to the profile box item.
 * @param cookie_buffer A pointer to the buffer where the cookie will be
 * stored.
 * @param buffer_size The size of the @c cookie_buffer.
 *
 * @return @c BBMSP_SUCCESS if successful, @c BBMSP_FAILURE otherwise.
 * @see BBMSP_SUCCESS, BBMSP_FAILURE, BBMSP_PROFILE_BOX_ITEM_COOKIE_MAX
 */
//BBMSP_API bbmsp_result_t bbmsp_user_profile_box_item_get_cookie(const bbmsp_user_profile_box_item_t* item,
//                                                                char* cookie_buffer, size_t buffer_size);
FREObject bbm_ane_bbmsp_user_profile_box_item_get_cookie(FREContext ctx, void* functionData,
                                                         uint32_t argc, FREObject argv[]);

/**
 * @brief Retrieve the text associated with a profile box item.
 * @details If @c bbmsp_can_show_profile_box() returns false, this method will
 * return @c BBMSP_FAILURE.
 * @param item A pointer to the profile box item.
 * @param text_buffer A pointer to the buffer where the text will be stored.
 * @param buffer_size The size of the @c text_buffer.
 *
 * @return @c BBMSP_SUCCESS if successful, @c BBMSP_FAILURE otherwise.
 * @see BBMSP_SUCCESS, BBMSP_FAILURE, BBMSP_PROFILE_BOX_ITEM_TEXT_MAX
 */
//BBMSP_API bbmsp_result_t bbmsp_user_profile_box_item_get_text(const bbmsp_user_profile_box_item_t* item,
//                                                              char* text_buffer, size_t buffer_size);
FREObject bbm_ane_bbmsp_user_profile_box_item_get_text(FREContext ctx, void* functionData,
                                                       uint32_t argc, FREObject argv[]);

/**
 * @brief Retrieve the image ID associated with this profile box item.
 * @details An image in a profile box is identified by a unique, non-negative
 * numeric ID. If @c bbmsp_can_show_profile_box() returns false, this method
 * will return @c BBMSP_FAILURE.
 *
 * @param item A pointer to the profile box item.
 * @param icon_id Updated with the image ID.
 *
 * @return @c BBMSP_SUCCESS if successful, @c BBMSP_FAILURE otherwise.
 * @see BBMSP_SUCCESS, BBMSP_FAILURE
 */
//BBMSP_API bbmsp_result_t bbmsp_user_profile_box_item_get_icon_id(const bbmsp_user_profile_box_item_t* item,
//                                                                 int32_t* icon_id);
FREObject bbm_ane_bbmsp_user_profile_box_item_get_icon_id(FREContext ctx, void* functionData,
                                                          uint32_t argc, FREObject argv[]);

/**
 * @brief Add an item to a user's profile box.
 * @details A profile box item consists of an image, text, and a customizable
 * string (cookie).
 * @c text and @c cookie must be null-terminated C-strings, encoded as UTF-8.
 * The item text must not be null or empty. It can have a maximum of 100
 * characters,
 * with no more than 2 new line characters. The cookie can be null, with a
 * maximum of 128 characters.
 * The @c icon_id must be that of a registered image, or < 0 if this item has no
 * image.
 * If @c bbmsp_can_show_profile_box() returns false, this method will return @c
 * BBMSP_FAILURE.
 *
 * @param text A pointer to the buffer that contains the item text.
 * @param icon_id The ID of a registered image, or < 0 if this item has no
 * image.
 * @param cookie A pointer to the buffer that contains the cookie.
 *
 * @return @c BBMSP_ASYNC if successful, @c BBMSP_FAILURE otherwise.
 * @see bbmsp_user_profile_box_register_icon()
 */
//BBMSP_API bbmsp_result_t bbmsp_user_profile_box_add_item(const char* text, int32_t icon_id,
//                                                         const char* cookie);
FREObject bbm_ane_bbmsp_user_profile_box_add_item(FREContext ctx, void* functionData,
                                                  uint32_t argc, FREObject argv[]);

/**
 * @brief Count the number of items in a list of profile box items.
 * @details If @c bbmsp_can_show_profile_box() returns false, this method will
 * return 0.
 * @param item_list A pointer to the list of the items to count.
 *
 * @return The number of items in the list.
 */
//BBMSP_API unsigned int bbmsp_user_profile_box_items_size(bbmsp_user_profile_box_item_list_t* item_list);
FREObject bbm_ane_bbmsp_user_profile_box_items_size(FREContext ctx, void* functionData,
                                                    uint32_t argc, FREObject argv[]);

/**
 * @brief Remove an item from the list of items in a profile box.
 * @details Individual items in a list of profile box items are accessed by
 * index number. Index numbers begin at 0.
 * The memory associated with the item that is removed will be released.
 * If @c bbmsp_can_show_profile_box() returns false, this method will return @c
 * BBMSP_FAILURE.
 * @param item_list A pointer to the list that contains the item to remove.
 * @param index The index of the item to remove.
 *
 * @return @c BBMSP_SUCCESS if successful, @c BBMSP_FAILURE otherwise.
 * @see BBMSP_SUCCESS, BBMSP_FAILURE
 */
//BBMSP_API bbmsp_result_t bbmsp_user_profile_box_itemlist_remove_at(bbmsp_user_profile_box_item_list_t* item_list, unsigned int index);
FREObject bbm_ane_bbmsp_user_profile_box_itemlist_remove_at(FREContext ctx, void* functionData,
                                                            uint32_t argc, FREObject argv[]);

/**
 * @brief Remove all items in the profile box from the user's BlackBerry device.
 * @details This process takes place asynchronously. If @c
 * bbmsp_can_show_profile_box() returns false, this method will return @c
 * BBMSP_FAILURE.
 *
 * @return @c BBMSP_ASYNC if successful, @c BBMSP_FAILURE otherwise.
 * @see BBMSP_FAILURE, BBMSP_ASYNC
 */
//BBMSP_API bbmsp_result_t bbmsp_user_profile_box_remove_all_items(void);
FREObject bbm_ane_bbmsp_user_profile_box_remove_all_items(FREContext ctx, void* functionData,
                                                           uint32_t argc, FREObject argv[]);

/**
 * @brief Register an image with the BBM Social Platform for use with profile
 * box items.
 * @details Registering an image uploads it to the user's BlackBerry device for
 * use when the app is not running.
 * This process takes place asynchronously. Images should be registered using
 * the same ID on different devices and in
 * different versions of your app because images are distributed on a
 * peer-to-peer basis between devices.
 * Registered images cannot be null or empty.
 * If @c bbmsp_can_show_profile_box() returns false, this method will return @c
 * BBMSP_FAILURE.
 * @param icon_id The unique ID of the image. Must be > 0.
 * @param icon A pointer to the image.
 *
 * @return @c BBMSP_ASYNC is successful, @c BBMSP_FAILURE otherwise.
 * @see bbmsp_image_t, BBMSP_FAILURE, BBMSP_ASYNC
 */
//BBMSP_API bbmsp_result_t bbmsp_user_profile_box_register_icon(const int32_t icon_id,
//                                                              const bbmsp_image_t* icon);
FREObject bbm_ane_bbmsp_user_profile_box_register_icon(FREContext ctx, void* functionData,
                                                       uint32_t argc, FREObject argv[]);

/**
 * @brief Retrieve a profile box image that is registered with the BBM Social
 * Platform.
 * @details Retrieves the image that was uploaded to the user's BlackBerry
 * device when the image was registered.
 * This process takes place asynchronously.
 * If @c bbmsp_can_show_profile_box() returns false, this method will return @c
 * BBMSP_FAILURE.
 * @param icon_id The unique ID of the image. Must be > 0.
 * @return @c BBMSP_ASYNC if successful, @c BBMSP_FAILURE otherwise.
 * @see BBMSP_FAILURE, BBMSP_ASYNC
*/
//BBMSP_API bbmsp_result_t bbmsp_user_profile_box_retrieve_icon(const int32_t icon_id);
FREObject bbm_ane_bbmsp_user_profile_box_retrieve_icon(FREContext ctx, void* functionData,
                                                       int32_t argc, FREObject argv[]);




#ifdef __cplusplus
}
#endif


#endif /* BBMSPUSERPROFILEBOX_H_ */
