/*
 * BBMSPUserProfileBox.h
 *
 *  Created on: Jan 19, 2013
 *      Author: HU77
 */

#ifndef BBMANE_BBMSPUSERPROFILEBOX_H_
#define BBMANE_BBMSPUSERPROFILEBOX_H_


#ifdef __cplusplus
extern "C" {
#endif



/**
 * @brief Create a new, empty profile box item.
 * @details Create a new, empty profile box item. Does not change the user's
 * BBM profile box. This can be used with
 * @c bbmsp_user_profile_box_item_copy() to copy an item.
 *
 * @param item An updated pointer to the new profile box item.
 *
 * @return @c BBMSP_SUCCESS if successful, @c BBMSP_FAILURE otherwise.
 * @see BBMSP_SUCCESS, BBMSP_FAILURE, bbmsp_user_profile_box_item_copy
 */
//BBMSP_API bbmsp_result_t bbmsp_user_profile_box_item_create(bbmsp_user_profile_box_item_t** item);
FREObject bbm_ane_bbmsp_user_profile_box_item_create(FREContext ctx, void* functionData,
                                                     uint32_t argc, FREObject argv[]);

/**
 * @brief Destroy a profile box item.
 * @details Destroys this profile box item and frees up the associated memory.
 * Does not change the user's BBM profile box.
 *
 * @param item A pointer to the item in the profile box to destroy.
 *
 * @return @c BBMSP_SUCCESS if successful, @c BBMSP_FAILURE otherwise.
 * @see BBMSP_SUCCESS, BBMSP_FAILURE
 */
//BBMSP_API bbmsp_result_t bbmsp_user_profile_box_item_destroy(bbmsp_user_profile_box_item_t** item);
FREObject bbm_ane_bbmsp_user_profile_box_item_destroy(FREContext ctx, void* functionData,
                                                      uint32_t argc, FREObject argv[]);

/**
 * @brief Create a copy of a profile box item.
 * @details Once your app finishes processing a profile box event and returns
 * to the @c bps event loop, the @c bps library releases the original @c bps
 * event from memory, including the profile box item. This function copies a
 * profile box item into memory for use after your app calls @c bps_get_event
 * again.
 *
 * @param destination_item A pointer to an item created using
 * @c bbmsp_user_profile_box_item_create().
 * @param source_item A pointer to the profile box item to copy.
 *
 * @return @c BBMSP_SUCCESS if successful, @c BBMSP_FAILURE otherwise.
 * @see BBMSP_SUCCESS, BBMSP_FAILURE, bbmsp_user_profile_box_item_create
 */
//BBMSP_API bbmsp_result_t bbmsp_user_profile_box_item_copy(bbmsp_user_profile_box_item_t* destination_item,
//                                                          bbmsp_user_profile_box_item_t* source_item);
FREObject bbm_ane_bbmsp_user_profile_box_item_copy(FREContext ctx, void* functionData,
                                                   uint32_t argc, FREObject argv[]);

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
 * @brief Add an item to a user's profile box (no icon).
 * @details Add an item to a user's profile box with only text and a
 * customizable string (cookie).
 * @c text and @c cookie must be null-terminated C-strings, encoded as UTF-8.
 * The item text must not be null or empty. It can have a maximum of 100
 * characters with no more
 * than 2 new line characters. The cookie can be null and can have a maximum of
 * 128 characters.
 * If @c bbmsp_can_show_profile_box() returns false, this method will return @c
 * BBMSP_FAILURE.
 * @param text A pointer to the buffer that contains the item text.
  * @param cookie A pointer to the buffer that contains the item cookie.
 *
 * @return BBMSP_ASYNC if successful, BBMSP_FAILURE if the application does not
 * have access to the BBMSP platform,
 * or if a parameter is invalid.
 */
//BBMSP_API bbmsp_result_t bbmsp_user_profile_box_add_item_no_icon(const char* text, const char* cookie);
FREObject bbm_ane_bbmsp_user_profile_box_add_item_no_icon(FREContext ctx, void* functionData,
                                                          uint32_t argc, FREObject argv[]);

/**
 * @brief Retrieve an item from a profile box by using the item ID.
 * @details Each profile box item is identified by a unique, non-negative,
 * numeric ID. If @c bbmsp_can_show_profile_box() returns false, this method
 * will return @c BBMSP_FAILURE.
 *
 * @param id A pointer to the buffer that contains the item ID.
 * @c id must be passed in as a null-terminated C-string, encoded as UTF-8.
 * @param item An updated pointer to the item.
 *
 * @return @c BBMSP_SUCCESS if successful, @c BBMSP_FAILURE otherwise.
 * @see BBMSP_SUCCESS, BBMSP_FAILURE
 */
//BBMSP_API bbmsp_result_t bbmsp_user_profile_box_get_item(const char* id,
//                                                         bbmsp_user_profile_box_item_t* item);
FREObject bbm_ane_bbmsp_user_profile_box_get_item(FREContext ctx, void* functionData,
                                                  uint32_t argc, FREObject argv[]);

/**
 * @brief Retrieve the list of items in the user's profile box.
 * @details Retrieves the list of items in the profile box. If @c
 * bbmsp_can_show_profile_box() returns false, this method will return @c
 * BBMSP_FAILURE.
 *
 * @param item_list A pointer to the list retrieve.
 *
 * @return @c BBMSP_SUCCESS if successful, @c BBMSP_FAILURE otherwise.
 * @see BBMSP_SUCCESS, BBMSP_FAILURE
 */
//BBMSP_API bbmsp_result_t bbmsp_user_profile_box_get_items(bbmsp_user_profile_box_item_list_t* item_list);
FREObject bbm_ane_bbmsp_user_profile_box_get_items(FREContext ctx, void* functionData,
                                                   uint32_t argc, FREObject argv[]);


/**
 * @brief Create a profile box item list.
 * @details Creates a new, empty profile box item list. Does not change the
 * user's BBM profile box. If @c bbmsp_can_show_profile_box() returns false,
 * this method will return @c BBMSP_FAILURE.
 *
 * @param item_list An updated pointer to the item list.
 *
 * @return @c BBMSP_SUCCESS if successful, @c BBMSP_FAILURE otherwise.
 * @see BBMSP_SUCCESS, BBMSP_FAILURE
 */
//BBMSP_API bbmsp_result_t bbmsp_user_profile_box_item_list_create(bbmsp_user_profile_box_item_list_t** item_list);
FREObject bbm_ane_bbmsp_user_profile_box_item_list_create(FREContext ctx, void* functionData,
                                                          uint32_t argc, FREObject argv[]);

/**
 * @brief Destroy a profile box item list struct.
 * @details Destroys this list of profile box items and frees up the associated
 * memory. Does not change the user's BBM profile box. If @c
 * bbmsp_can_show_profile_box() returns false, this method will return @c
 * BBMSP_FAILURE.
 *
 * @param item_list A pointer to the item list to destroy.
 *
 * @return @c BBMSP_SUCCESS if successful, @c BBMSP_FAILURE otherwise.
 * @see BBMSP_SUCCESS, BBMSP_FAILURE
 */
//BBMSP_API bbmsp_result_t bbmsp_user_profile_box_item_list_destroy(bbmsp_user_profile_box_item_list_t** item_list);
FREObject bbm_ane_bbmsp_user_profile_box_item_list_destroy(FREContext ctx, void* functionData,
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
 * @brief Retrieve an item from a profile box item list.
 * @details Individual items in a profile box list are accessed by index number.
 * Index numbers begin at 0.
 * If @c bbmsp_can_show_profile_box() returns false, this method will return a
 * null pointer.
 * @param item_list A pointer to the item list that contains the item to
 * retrieve.
 * @param index The index of the item to retrieve.
 *
 * @return A pointer to the item.
 */
//BBMSP_API const bbmsp_user_profile_box_item_t*
//                bbmsp_user_profile_box_itemlist_get_at(bbmsp_user_profile_box_item_list_t* item_list,
//                                                       unsigned int index);
FREObject bbm_ane_bbmsp_user_profile_box_itemlist_get_at(FREContext ctx, void* functionData,
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
 * @brief Remove a profile box item from the user's BBM profile box.
 * @details This process takes place asynchronously. If @c
 * bbmsp_can_show_profile_box() returns false, this method will return @c
 * BBMSP_FAILURE.
 *
 * @param itemid A pointer to the buffer that contains the @c itemid of the item
 * to remove.
 * @c itemid must be a null-terminated C-string, encoded as UTF-8.
 *
 * @return @c BBMSP_ASYNC is successful, @c BBMSP_FAILURE otherwise.
 * @see BBMSP_FAILURE, BBMSP_ASYNC
 */
//BBMSP_API bbmsp_result_t bbmsp_user_profile_box_remove_item(const char* itemid);
FREObject bbm_ane_bbmsp_user_profile_box_remove_item(FREContext ctx, void* functionData,
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
FREObject bbm_ane_bbmsp_user_profile_box_remove_all_itemse(FREContext ctx, void* functionData,
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
FREObject bbm_ane_bbmsp_user_profile_box_retrieve(FREContext ctx, void* functionData,
                                                  int32_t argc, FREObject argv[]);

/**
 * @brief Retrieve an item that was added to a user's profile box.
 * @details When an item is added to a user's profile box, the
 * BBMSP_SP_EVENT_USER_PROFILE_BOX_ITEM_ADDED event is triggered.
 * If @c bbmsp_can_show_profile_box() returns false, this method will return @c
 * BBMSP_FAILURE.
 * @param event A pointer to the event that was triggered when the user added
 * the item.
 * @param item An updated pointer to the item that was added.
 *
 * @return @c BBMSP_SUCCESS if successful, @c BBMSP_FAILURE otherwise.
 * @see bbmsp_event_type_t, BBMSP_SUCCESS, BBMSP_FAILURE
 */
//BBMSP_API bbmsp_result_t bbmsp_event_user_profile_box_item_added_get_item(bbmsp_event_t* event,
//                                                                          bbmsp_user_profile_box_item_t* item);
FREObject bbm_ane_bbmsp_event_user_profile_box_item_added_get_item(FREContext ctx, void* functionData,
                                                                   uint32_t argc, FREObject argv[]);

/**
 * @brief Retrieve an item that was removed from a user's profile box.
 * @details When an item is removed from a user's profile box, the
 * BBMSP_SP_EVENT_USER_PROFILE_BOX_ITEM_REMOVED event is triggered.
 * If @c bbmsp_can_show_profile_box() returns false, this method will return @c
 * BBMSP_FAILURE.
 * @param event A pointer to the event that was triggered when the user removed
 * the item.
 * @param item An updated pointer to the item that was removed.
 *
 * @return @c BBMSP_SUCCESS if successful, @c BBMSP_FAILURE otherwise.
 * @see bbmsp_event_type_t, BBMSP_SUCCESS, BBMSP_FAILURE
 */
//BBMSP_API bbmsp_result_t bbmsp_event_user_profile_box_item_removed_get_item(bbmsp_event_t* event,
//                                                                            bbmsp_user_profile_box_item_t* item);
FREObject bbm_ane_bbmsp_event_user_profile_box_item_removed_get_item(FREContext ctx, void* functionData,
                                                                     uint32_t argc, FREObject argv[]);

/**
 * @brief Retrieve the @c icon_id for an image that was registered using
 * @c bbmsp_user_profile_box_register_icon().
 * @details When an image is added, the
 * BBMSP_SP_EVENT_USER_PROFILE_BOX_ICON_ADDED event is triggered.
 * If @c bbmsp_can_show_profile_box() returns false, this method will return @c
 * BBMSP_FAILURE.
 * @param event A pointer to the event that was triggered when the image was
 * registered.
 * @param icon_id Updated with the id of the icon.
 *
 * @return @c BBMSP_SUCCESS if successful; otherwise, return @c BBMSP_FAILURE.
 * @see bbmsp_event_type_t, BBMSP_SUCCESS, BBMSP_FAILURE,
 * BBMSP_SP_EVENT_USER_PROFILE_BOX_ICON_ADDED,
 * bbmsp_user_profile_box_register_icon
 */
//BBMSP_API bbmsp_result_t bbmsp_event_user_profile_box_icon_added_get_icon_id(bbmsp_event_t* event,
//                                                                             int32_t* icon_id);
FREObject bbm_ane_bbmsp_event_user_profile_box_icon_added_get_icon_id(FREContext ctx, void* functionData,
                                                                      uint32_t argc, FREObject argv[]);

/**
 * @brief Retrieve the @c icon_id for an image that was retrieved from the
 * user's profile box.
 * @details When an image is retrieved from a user's profile box, the
 * BBMSP_SP_EVENT_USER_PROFILE_BOX_ICON_RETRIEVED event is triggered.
 * If @c bbmsp_can_show_profile_box() returns false, this method will return @c
 * BBMSP_FAILURE.
 * @param event A pointer to the event that was triggered when the image was
 * retrieved.
 * @param icon_id Updated with the id of the icon
 *
 * @return @c BBMSP_SUCCESS if successful, @c BBMSP_FAILURE otherwise.
 * @see bbmsp_event_type_t, BBMSP_SUCCESS, BBMSP_FAILURE
 */
//BBMSP_API bbmsp_result_t bbmsp_event_user_profile_box_icon_retrieved_get_icon_id(bbmsp_event_t* event,
//                                                                                 int32_t* icon_id);
FREObject bbm_ane_bbmsp_event_user_profile_box_icon_retrieved_get_icon_id(FREContext ctx, void* functionData,
                                                                          uint32_t argc, FREObject argv[]);

/**
 * @brief Retrieve the image that was retrieved from a user's profile box.
 * @details When a user's profile box image is retrieved, the
 * BBMSP_SP_EVENT_USER_PROFILE_BOX_ICON_RETRIEVED event is triggered.
 * If @c bbmsp_can_show_profile_box() returns false, this method will return @c
 * BBMSP_FAILURE.
 * @param event A pointer to the event that was triggered when the image was
 * retrieved.
 * @param icon_image An updated pointer to the image.
 *
 * @return @c BBMSP_SUCCESS if successful, @c BBMSP_FAILURE otherwise.
 * @see bbmsp_event_type_t, BBMSP_SUCCESS, BBMSP_FAILURE
 */
//BBMSP_API bbmsp_result_t bbmsp_event_user_profile_box_icon_retrieved_get_icon_image(bbmsp_event_t* event,
//                                                                                    bbmsp_image_t** icon_image);
FREObject bbm_ane_bbmsp_event_user_profile_box_icon_retrieved_get_icon_image(FREContext ctx,
                                                                             void* functionData,
                                                                             uint32_t argc, FREObject argv[]);




#ifdef __cplusplus
}
#endif


#endif /* BBMSPUSERPROFILEBOX_H_ */
