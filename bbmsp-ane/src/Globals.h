/*
 * Globals.h
 *
 *  Created on: Jan 16, 2013
 *      Author: CHRIS
 */


#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <map>

#include <bps/bps.h>
#include <bps/event.h>
#include <bbmsp/bbmsp.h>
#include <bbmsp/bbmsp_events.h>
#include <bbmsp/bbmsp_context.h>
#include <bbmsp/bbmsp_util.h>
#include <bbmsp/bbmsp_messaging.h>
#include <bbmsp/bbmsp_contactlist.h>
#include <bbmsp/bbmsp_userprofile.h>
#include <bbmsp/bbmsp_presence.h>

#include <pthread.h>
#include <unistd.h>

#include "FlashRuntimeExtensions.h"

#include "BBMSP.h"
#include "BBMSPUtil.h"
#include "BBMSPMessaging.h"
#include "BBMSPContactlist.h"
#include "BBMSPUserProfile.h"


#ifndef BBMANE_GLOBALS_H_
#define BBMANE_GLOBALS_H_


#ifdef __cplusplus
extern "C" {
#endif



extern int ane_master_domain;           //Used to check the domain for events from the ANE
extern int ane_master_channel_id;       //Used so child channels can push events back to main channel
extern int ane_registration_channel_id;
extern int ane_image_channel_id;

extern int runCount;

extern std::map<int,bbmsp_image_t *>                      *bbmsp_image_map;
extern std::map<int,bbmsp_contact_t *>                    *bbmsp_contact_map;
//extern std::map<int,bbmsp_user_profile_box_item_t *>      bbmsp_profile_item_map;
//extern std::map<int,bbmsp_user_profile_box_icon_t *>      bbmsp_profile_icon_map;
//extern std::map<int,bbmsp_user_profile_box_item_list_t *> bbmsp_profile_item_list_map;

typedef enum {
   ANE_PENDING_REGISTRATION = 0,
   ANE_REGISTERED,
   ANE_REGISTRATION_FAILED
} ane_bbmsp_status_e;

extern ane_bbmsp_status_e          bbmStatus;
extern ane_image_thread_status_e   imageThreadStatus;
extern ane_contact_thread_status_e contactThreadStatus;
extern ane_profile_thread_status_e profileThreadStatus;

extern FREContext currentContext;


#ifdef __cplusplus
}
#endif


#endif /* BBMANE_GLOBALS_H_ */
