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
               notifyProfileBoxChanged("ANEProfileBoxLoaded",0);
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
               if( (eventCode == PROFILE_BOX_CHANGED_ADD) ){
                  //If item is added or removed then reload the list
                  profileBoxThreadStatus = PBOX_THREAD_STARTING;
                  notifyProfileBoxChanged("ANEProfileBoxItemAdded",0);
                  break;
               }

               if( (eventCode == PROFILE_BOX_CHANGED_DEL) ){
                  //If item is added or removed then reload the list
                  profileBoxThreadStatus = PBOX_THREAD_STARTING;
                  notifyProfileBoxChanged("ANEProfileBoxItemDeleted",0);
                  break;
               }

               if( eventCode == PROFILE_BOX_CHANGED_ICN_ADD ){
                  bps_event_payload_t *payload = bps_event_get_payload(bps_event);
                  int32_t iconID = (int32_t)payload->data1;
                  notifyProfileBoxChanged("ANEProfileBoxIconRegistered",iconID);
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
                        notifyProfileBoxChanged("ANEProfileBoxIconRetrieved",iconID);
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
                     notifyProfileBoxChanged("ANEProfileBoxIconRetrieved",id);
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

static void notifyProfileBoxChanged(char *event,int value){
   const char *lvl = "";
   char data[15];
   itoa(value,data,10);
   FREDispatchStatusEventAsync(currentContext, (const uint8_t*)event, (const uint8_t*)data);
}

//======================================================================================//
//             STANDARD FUNCTIONS FROM bbmsp_user_profile_box.h QNX FILE
//======================================================================================//

//BBMSP_API bbmsp_result_t bbmsp_user_profile_box_item_get_item_id(const bbmsp_user_profile_box_item_t* item,
//                                                                 char* item_id_buffer, size_t buffer_size);
FREObject bbm_ane_bbmsp_user_profile_box_item_get_item_id(FREContext ctx, void* functionData,
                                                          uint32_t argc, FREObject argv[]){
   uint32_t itemNum;
   char id[15];

   id[14] = '\0';
   FREGetObjectAsUint32(argv[0],&itemNum);

   const bbmsp_user_profile_box_item_t *item = bbmsp_user_profile_box_itemlist_get_at(profileBoxList,itemNum);
   bbmsp_result_t code = bbmsp_user_profile_box_item_get_item_id(item,id,15);

   FREObject result;
   FRENewObjectFromUTF8((uint32_t)(strlen(id)+1), (uint8_t*)id, &result);
   return result;
}

//BBMSP_API bbmsp_result_t bbmsp_user_profile_box_item_get_cookie(const bbmsp_user_profile_box_item_t* item,
//                                                                char* cookie_buffer, size_t buffer_size);
FREObject bbm_ane_bbmsp_user_profile_box_item_get_cookie(FREContext ctx, void* functionData,
                                                         uint32_t argc, FREObject argv[]){
   uint32_t itemNum;
   char cookie[BBMSP_PROFILE_BOX_ITEM_COOKIE_MAX+1];

   cookie[BBMSP_PROFILE_BOX_ITEM_COOKIE_MAX] = '\0';
   FREGetObjectAsUint32(argv[0],&itemNum);

   const bbmsp_user_profile_box_item_t *item = bbmsp_user_profile_box_itemlist_get_at(profileBoxList,itemNum);
   bbmsp_result_t code = bbmsp_user_profile_box_item_get_cookie(item,cookie,BBMSP_PROFILE_BOX_ITEM_COOKIE_MAX);

   FREObject result;
   FRENewObjectFromUTF8((uint32_t)(strlen(cookie)+1), (uint8_t*)cookie, &result);
   return result;
}

//BBMSP_API bbmsp_result_t bbmsp_user_profile_box_item_get_text(const bbmsp_user_profile_box_item_t* item,
//                                                              char* text_buffer, size_t buffer_size);
FREObject bbm_ane_bbmsp_user_profile_box_item_get_text(FREContext ctx, void* functionData,
                                                       uint32_t argc, FREObject argv[]){
   uint32_t itemNum;
   char txt[BBMSP_PROFILE_BOX_ITEM_TEXT_MAX+1];

   txt[BBMSP_PROFILE_BOX_ITEM_TEXT_MAX] = '\0';
   FREGetObjectAsUint32(argv[0],&itemNum);

   const bbmsp_user_profile_box_item_t *item = bbmsp_user_profile_box_itemlist_get_at(profileBoxList,itemNum);
   bbmsp_result_t code = bbmsp_user_profile_box_item_get_cookie(item,txt,BBMSP_PROFILE_BOX_ITEM_TEXT_MAX);

   FREObject result;
   FRENewObjectFromUTF8((uint32_t)(strlen(txt)+1), (uint8_t*)txt, &result);
   return result;
}

//BBMSP_API bbmsp_result_t bbmsp_user_profile_box_item_get_icon_id(const bbmsp_user_profile_box_item_t* item,
//                                                                 int32_t* icon_id);
FREObject bbm_ane_bbmsp_user_profile_box_item_get_icon_id(FREContext ctx, void* functionData,
                                                          uint32_t argc, FREObject argv[]){
   uint32_t itemNum;
   int id;

   FREGetObjectAsUint32(argv[0],&itemNum);

   const bbmsp_user_profile_box_item_t *item = bbmsp_user_profile_box_itemlist_get_at(profileBoxList,itemNum);
   bbmsp_result_t code = bbmsp_user_profile_box_item_get_icon_id(item,&id);

   FREObject result;
   FRENewObjectFromInt32(id, &result);
   return result;
}

//BBMSP_API bbmsp_result_t bbmsp_user_profile_box_add_item(const char* text, int32_t icon_id,
//                                                         const char* cookie);
FREObject bbm_ane_bbmsp_user_profile_box_add_item(FREContext ctx, void* functionData,
                                                  uint32_t argc, FREObject argv[]){
   cout << endl << "====== Adding profile box item ======" << endl;
   int icon;
   uint32_t cklen;
   uint32_t txlen;
   const uint8_t  *ckie;
   const uint8_t  *txt;
   char text[BBMSP_PROFILE_BOX_ITEM_TEXT_MAX+1];
   char cookie[BBMSP_PROFILE_BOX_ITEM_COOKIE_MAX+1];

   FREGetObjectAsInt32(argv[0],&icon);
   FREGetObjectAsUint32(argv[2],&txlen);
   FREGetObjectAsUTF8( argv[1],&txlen,&txt );
   strncpy(text,(char*)txt,BBMSP_PROFILE_BOX_ITEM_TEXT_MAX);
   FREGetObjectAsUint32(argv[4],&cklen);
   FREGetObjectAsUTF8( argv[3],&cklen,&ckie );
   strncpy(cookie,(char*)ckie,BBMSP_PROFILE_BOX_ITEM_COOKIE_MAX);
   cout << txt << ":" << text << endl << ckie << ":" << cookie << endl << "Icon: " << icon << endl << endl;

   cookie[BBMSP_PROFILE_BOX_ITEM_COOKIE_MAX] = '\0';
   text[BBMSP_PROFILE_BOX_ITEM_TEXT_MAX] = '\0';

   bbmsp_result_t code;
   if( icon > 0 ) code = bbmsp_user_profile_box_add_item(text,icon,cookie);
   else code = bbmsp_user_profile_box_add_item_no_icon(text,cookie);
   FREObject result;
   FRENewObjectFromInt32(code, &result);
   return result;
}

//BBMSP_API unsigned int bbmsp_user_profile_box_items_size(bbmsp_user_profile_box_item_list_t* item_list);
FREObject bbm_ane_bbmsp_user_profile_box_items_size(FREContext ctx, void* functionData,
                                                    uint32_t argc, FREObject argv[]){
   uint32_t code = bbmsp_user_profile_box_items_size(profileBoxList);
   FREObject result;
   FRENewObjectFromUint32(code, &result);
   return result;
}

//BBMSP_API bbmsp_result_t bbmsp_user_profile_box_itemlist_remove_at(bbmsp_user_profile_box_item_list_t* item_list,
//                                                                   unsigned int index);
FREObject bbm_ane_bbmsp_user_profile_box_itemlist_remove_at(FREContext ctx, void* functionData,
                                                            uint32_t argc, FREObject argv[]){
   uint32_t itemNum;
   char id[15];
   FREGetObjectAsUint32(argv[0],&itemNum);

   id[14] = '\0';
   const bbmsp_user_profile_box_item_t *item = bbmsp_user_profile_box_itemlist_get_at(profileBoxList,itemNum);
   bbmsp_user_profile_box_item_get_item_id(item,id,15);

   bbmsp_user_profile_box_remove_item(id);
   bbmsp_result_t code = bbmsp_user_profile_box_itemlist_remove_at(profileBoxList,itemNum);
   FREObject result;
   FRENewObjectFromInt32(code, &result);
   return result;
}

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
   bbmsp_result_t code = bbmsp_user_profile_box_register_icon(iconID,icon);
   FREObject result;
   FRENewObjectFromInt32(code, &result);
   return result;
}

//BBMSP_API bbmsp_result_t bbmsp_user_profile_box_retrieve_icon(const int32_t icon_id);
FREObject bbm_ane_bbmsp_user_profile_box_retrieve_icon(FREContext ctx, void* functionData,
                                                       uint32_t argc, FREObject argv[]){
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

