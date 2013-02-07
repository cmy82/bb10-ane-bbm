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

int ane_profile_box_domain     = -1;
int ane_profile_box_channel_id = -1;

bbmsp_user_profile_box_item_list_t       *profileBoxList;
map<int,bbmsp_user_profile_box_item_t*>  *bbmsp_profile_box_item_map = new map<int,bbmsp_user_profile_box_item_t*>();
ane_profile_box_thread_status_e          profileBoxThreadStatus = PROFILE_BOX_THREAD_INIT;
pthread_mutex_t                          profileBoxMutex;

//======================================================================================//
//          CUSTOM FUNCTIONS
//======================================================================================//

void* initProfileBoxThread(void *data){
   bps_initialize();
   ane_profile_box_domain = bps_register_domain();
   ane_profile_box_channel_id = bps_channel_get_active();

   bps_event_t *bps_event;

   for(;;) {
      switch(profileBoxThreadStatus){
         case PROFILE_BOX_THREAD_INIT:
         {
            //Get next BPS event and block until one returns
            cout << "Waiting on BPS event in profile box thread" << endl;
            bps_get_event(&bps_event, -1);
            //If no BPS event is returned (ex if init failed) then cancel the event query
            if (!bps_event) return NULL;
            cout << "BPS event received in profile box thread" << endl;

            int eventDomain;
            int eventCode;

            eventDomain = bps_event_get_domain(bps_event);
            if( eventDomain == ane_profile_box_domain ){
               eventCode = bps_event_get_code(bps_event);
               if( eventCode == START_LOADING_PROFILE_BOX ){
                  profileBoxThreadStatus = PBOX_THREAD_ACCESS_PEND;
                  cout << "Starting loading of profile box" << endl;
               }
            }
         }
            break;

         case PBOX_THREAD_ACCESS_PEND:
            if( bbmsp_can_show_profile_box() )
               profileBoxThreadStatus = PBOX_THREAD_STARTING;
            else
               sleep(60);
            break;

         case PBOX_THREAD_STARTING:
            if( profileBoxList == NULL )
               bbmsp_user_profile_box_item_list_create(&profileBoxList);
            else {
               bbmsp_user_profile_box_item_list_destroy(&profileBoxList);
               bbmsp_user_profile_box_item_list_create(&profileBoxList);
            }

            if( bbmsp_user_profile_box_get_items(profileBoxList) != BBMSP_FAILURE ){
               profileBoxThreadStatus = PROFILE_BOX_LOADED;
               notifyProfileBoxChanged("ANEProfileBoxLoaded");
               cout << "Loaded user profile box" << endl;
            } else
               sleep(30);
            break;

         case PROFILE_BOX_LOADED:
         {
            cout << "Waiting on BPS event in profile box thread" << endl;
            bps_get_event(&bps_event, -1);
            if (!bps_event) return NULL;
            cout << "BPS event received in profile box thread" << endl;

            int eventDomain;
            int eventCode;

            eventDomain = bps_event_get_domain(bps_event);
            if( eventDomain == ane_profile_box_domain ){
               eventCode = bps_event_get_code(bps_event);
               if( (eventCode == PROFILE_BOX_CHANGED_ADD) || (eventCode == PROFILE_BOX_CHANGED_DEL) ){
                  //If item is added or removed then reload the list
                  profileBoxThreadStatus = PBOX_THREAD_STARTING;
                  break;
               }

               if( eventCode == PROFILE_BOX_CHANGED_ICN_ADD ){

               }

               if( eventCode == PROFILE_BOX_CHANGED_ICN_RET ){
                  //Each icon that is retrieved store in the image map if not already present
                  bps_event_payload_t *payload = bps_event_get_payload(bps_event);
                  int32_t iconID = (int32_t)payload->data1;
                  bool found = false;
                  for (map<int,ane_image_s*>::iterator it=ane_image_map->begin(); it!=ane_image_map->end(); ++it){
                     ane_image_s *images = it->second;
                     if( images->iconID == iconID ){
                        found = true;
                        break;
                     }
                  }

                  if( !found ){
                     int id = rand();
                     ane_image_s *images = (ane_image_s*)malloc(sizeof(ane_image_s));
                     images->iconID = iconID;
                     images->original = (bbmsp_image_t*)payload->data2;
                     images->profile  = (bbmsp_image_t*)payload->data2;
                     ane_image_map->insert( std::pair<int,ane_image_s*>(id,images) );
                  }
               }
            }
         }
            break;

         case PBOX_THREAD_STOPPING:
            bbmsp_user_profile_box_item_list_destroy(&profileBoxList);
            break;

         case PBOX_THREAD_STOPPED:
            break;
      }
   }

   return NULL;
}

static void notifyProfileBoxChanged(char *event){

}

//======================================================================================//
//             STANDARD FUNCTIONS FROM bbmsp_user_profile_box.h QNX FILE
//======================================================================================//

//BBMSP_API bbmsp_result_t bbmsp_user_profile_box_item_get_item_id(const bbmsp_user_profile_box_item_t* item,
//                                                                 char* item_id_buffer, size_t buffer_size);
FREObject bbm_ane_bbmsp_user_profile_box_item_get_item_id(FREContext ctx, void* functionData,
                                                          uint32_t argc, FREObject argv[]){
   int itemNum;
   char id[15];

   id[14] = '\0';
   FREGetObjectAsInt32(argv[0],&itemNum);

   bbmsp_profile_box_item *item = bbmsp_user_profile_box_itemlist_get_at(profileBoxList,itemNum);
   bbmsp_result_t code = bbmsp_user_profile_box_item_get_id(item,id,15);

   FREObject result;
   FRENewObjectFromUTF8((uint32_t)(strlen(id)+1), (uint8_t*)id, &result);
   return result;
}

//BBMSP_API bbmsp_result_t bbmsp_user_profile_box_item_get_cookie(const bbmsp_user_profile_box_item_t* item,
//                                                                char* cookie_buffer, size_t buffer_size);
FREObject bbm_ane_bbmsp_user_profile_box_item_get_cookie(FREContext ctx, void* functionData,
                                                         uint32_t argc, FREObject argv[]){
   int itemNum;
   char cookie[BBMSP_PROFILE_BOX_ITEM_COOKIE_MAX+1];

   cookie[BBMSP_PROFILE_BOX_ITEM_COOKIE_MAX] = '\0';
   FREGetObjectAsInt32(argv[0],&itemNum);

   bbmsp_profile_box_item *item = bbmsp_user_profile_box_itemlist_get_at(profileBoxList,itemNum);
   bbmsp_result_t code = bbmsp_user_profile_box_item_get_cookie(item,cookie,BBMSP_PROFILE_BOX_ITEM_COOKIE_MAX);

   FREObject result;
   FRENewObjectFromUTF8((uint32_t)(strlen(cookie)+1), (uint8_t*)cookie, &result);
   return result;
}

//BBMSP_API bbmsp_result_t bbmsp_user_profile_box_item_get_text(const bbmsp_user_profile_box_item_t* item,
//                                                              char* text_buffer, size_t buffer_size);
FREObject bbm_ane_bbmsp_user_profile_box_item_get_text(FREContext ctx, void* functionData,
                                                       uint32_t argc, FREObject argv[]){
   int itemNum;
   char txt[BBMSP_PROFILE_BOX_ITEM_TEXT_MAX+1];

   txt[BBMSP_PROFILE_BOX_ITEM_TEXT_MAX] = '\0';
   FREGetObjectAsInt32(argv[0],&itemNum);

   bbmsp_profile_box_item *item = bbmsp_user_profile_box_itemlist_get_at(profileBoxList,itemNum);
   bbmsp_result_t code = bbmsp_user_profile_box_item_get_cookie(item,txt,BBMSP_PROFILE_BOX_ITEM_TEXT_MAX);

   FREObject result;
   FRENewObjectFromUTF8((uint32_t)(strlen(txt)+1), (uint8_t*)txt, &result);
   return result;
}

//BBMSP_API bbmsp_result_t bbmsp_user_profile_box_item_get_icon_id(const bbmsp_user_profile_box_item_t* item,
//                                                                 int32_t* icon_id);
FREObject bbm_ane_bbmsp_user_profile_box_item_get_icon_id(FREContext ctx, void* functionData,
                                                          uint32_t argc, FREObject argv[]){
   int itemNum;
   int id;

   FREGetObjectAsInt32(argv[0],&itemNum);

   bbmsp_profile_box_item *item = bbmsp_user_profile_box_itemlist_get_at(profileBoxList,itemNum);
   bbmsp_result_t code = bbmsp_user_profile_box_item_get_icon_id(item,&id);

   FREObject result;
   FRENewObjectFromInt32(id, &result);
   return result;
}

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
                                                  uint32_t argc, FREObject argv[]){
   int item;
   FREGetObjectAsInt32(argv[0],&item);

   bbmsp_result_t code = bbmsp_user_profile_box_itemlist_remove_at(profileBoxList,item);
   FREObject result;
   FRENewObjectFromInt32(code, &result);
   return result;
}


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
//FREObject bbm_ane_bbmsp_user_profile_box_get_item(FREContext ctx, void* functionData,
//                                                  uint32_t argc, FREObject argv[]){
//
//}

//BBMSP_API unsigned int bbmsp_user_profile_box_items_size(bbmsp_user_profile_box_item_list_t* item_list);
FREObject bbm_ane_bbmsp_user_profile_box_items_size(FREContext ctx, void* functionData,
                                                    uint32_t argc, FREObject argv[]){
   bbmsp_result_t code = bbmsp_user_profile_box_items_size(profileBoxList);
   FREObject result;
   FRENewObjectFromInt32(code, &result);
   return result;
}

//BBMSP_API bbmsp_result_t bbmsp_user_profile_box_itemlist_remove_at(bbmsp_user_profile_box_item_list_t* item_list,
//                                                                   unsigned int index);
FREObject bbm_ane_bbmsp_user_profile_box_itemlist_remove_at(FREContext ctx, void* functionData,
                                                            uint32_t argc, FREObject argv[]){
   int item;
   char id[15];
   FREGetObjectAsInt32(argv[0],&item);

   id[14] = '\0';
   bbmsp_profile_box_item *item = bbmsp_user_profile_box_itemlist_get_at(profileBoxList,itemNum);
   bbmsp_user_profile_box_item_get_id(item,id,15);

   bbmsp_user_profile_box_remove_item(id);
   bbmsp_result_t code = bbmsp_user_profile_box_itemlist_remove_at(profileBoxList,item);
   FREObject result;
   FRENewObjectFromInt32(code, &result);
   return result;
}

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
//FREObject bbm_ane_bbmsp_user_profile_box_remove_item(FREContext ctx, void* functionData,
//                                                     uint32_t argc, FREObject argv[]){
//   int itemID;
//   FREGetObjectAsInt32(argv[0],&itemID);
//   char id[15];
//   itoa(itemID,id,10);
//
//   bbmsp_result_t code = bbmsp_user_profile_box_remove_item(id);
//   FREObject result;
//   FRENewObjectFromInt32(code, &result);
//   return result;
//}

//BBMSP_API bbmsp_result_t bbmsp_user_profile_box_remove_all_items(void);
FREObject bbm_ane_bbmsp_user_profile_box_remove_all_items(FREContext ctx, void* functionData,
                                                           uint32_t argc, FREObject argv[]){
   bbmsp_result_t code = bbmsp_user_profile_box_remove_all_items();
   FREObject result;
   FRENewObjectFromInt32(code, &result);
   return result;
}

//BBMSP_API bbmsp_result_t bbmsp_user_profile_box_register_icon(const int32_t icon_id,
//                                                              const bbmsp_image_t* icon);
FREObject bbm_ane_bbmsp_user_profile_box_register_icon(FREContext ctx, void* functionData,
                                                       uint32_t argc, FREObject argv[]){
   int iconID;
   int imageID;
   FREGetObjectAsInt32(argv[0],&iconID);
   FREGetObjectAsInt32(argv[1],&imageID);

   bbmsp_image_t *icon = (*ane_image_map)[imageID]->profile;
   bbmsp_result_t code = bbmsp_user_profile_box_register_icon(id,icon);
   FREObject result;
   FRENewObjectFromInt32(code, &result);
   return result;
}

//BBMSP_API bbmsp_result_t bbmsp_user_profile_box_retrieve_icon(const int32_t icon_id);
FREObject bbm_ane_bbmsp_user_profile_box_retrieve_icon(FREContext ctx, void* functionData,
                                                       int32_t argc, FREObject argv[]){
   int iconID;
   FREGetObjectAsInt32(argv[0],&iconID);

   bbmsp_result_t code = bbmsp_user_profile_box_retrieve_icon(iconID);
   FREObject result;
   FRENewObjectFromInt32(code, &result);
   return result;
}






#ifdef __cplusplus
}
#endif

