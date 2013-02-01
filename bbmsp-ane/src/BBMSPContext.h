/*
 * BBMSP.h
 *
 *  Created on: Jan 16, 2013
 *      Author: CHRIS
 */


#include "FlashRuntimeExtensions.h"

#ifndef BBMANE_BBMSP_CONTEXT_H_
#define BBMANE_BBMSP_CONTEXT_H_


#ifdef __cplusplus
extern "C" {
#endif

//======================================================================================//
//          CUSTOM FUNCTIONS
//======================================================================================//



//======================================================================================//
//                   STANDARD FUNCTIONS FROM bbmsp_context.h QNX FILE
//======================================================================================//

/**
 * @brief Determine whether access to the BBM Social Platform is allowed.
 *
 * @details On startup, this function will return 0. After the BBM Social
 * Platform calculates your status, your will receive a
 * @c BBMSP_REGISTRATION category event of type
 * @c BBMSP_SP_EVENT_ACCESS_CHANGED. After receiving this event, you can use
 * this function again to retrieve the current access status.
 *
 * The value returned by this function will reflect the latest received access
 * code.
 *
 * @return int 1 if access is @c BBMSP_ACCESS_ALLOWED, 0 otherwise.
 */
//BBMSP_API int bbmsp_is_access_allowed(void);
FREObject bbm_ane_bbmsp_is_access_allowed(FREContext ctx, void* functionData,
                                          uint32_t argc, FREObject argv[]);

/**
 * @brief Determine whether the user has allowed this application and its
 * application activities to appear in their BBM profile.
 *
 * @details You can use this function to determine whether your profile box is
 * being displayed on the user's profile. This function will return an accurate
 * value only if you have access to the BBM Social Platform (i.e. if
 * @c bbmsp_is_access_allowed() returns 1).
 *
 * The user can modify this permission through the global settings application.
 *
 * @return int 1 if allowed, 0 otherwise.
 *
 * @see bbmsp_user_profile_box.h
 */
//BBMSP_API int bbmsp_can_show_profile_box(void);
FREObject bbm_ane_bbmsp_can_show_profile_box(FREContext ctx, void* functionData,
                                             uint32_t argc, FREObject argv[]);

/**
 * @brief Determine whether this user has allowed other users of this
 * application to send this user invitations to become a BBM contact.
 *
 * @details This function will return an accurate value only if you have access
 * to the BBM Social Platform (i.e. if @c bbmsp_is_access_allowed() returns 1).
 *
 * The user can modify this permission through the global settings application.
 *
 * @return int 1 if allowed, 0 otherwise.
 */
//BBMSP_API int bbmsp_can_send_bbm_invite(void);
FREObject bbm_ane_bbmsp_can_send_bbm_invite(FREContext ctx, void* functionData,
                                            uint32_t argc, FREObject argv[]);




#ifdef __cplusplus
}
#endif


#endif


