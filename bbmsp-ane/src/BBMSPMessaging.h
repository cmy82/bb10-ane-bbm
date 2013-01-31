/*
 * BBMSPMessaging.h
 *
 *  Created on: Jan 18, 2013
 *      Author: HU77
 */

#include "FlashRuntimeExtensions.h"
#include <bps/bps.h>
#include <bps/event.h>
#include <bbmsp/bbmsp.h>
#include <bbmsp/bbmsp_events.h>
#include <bbmsp/bbmsp_context.h>
#include <bbmsp/bbmsp_util.h>
#include <bbmsp/bbmsp_messaging.h>
#include <bbmsp/bbmsp_contactlist.h>


#ifndef BBMANE_BBMSPMESSAGING_H_
#define BBMANE_BBMSPMESSAGING_H_


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Send an invitation to a user's BBM contacts to download your app.
 *
 * @details The contact picker displays for the user to select the contacts to
 * invite. Only those contacts who have not yet installed the app will be
 * displayed in the contact picker. The BBM Social Platform allows up to 10
 * download invitations per minute.
 *
 * @return @c BBMSP_SUCCESS if successful, @c BBMSP_FAILURE otherwise.
 * @see BBMSP_SUCCESS, BBMSP_FAILURE
 */
//BBMSP_API int bbmsp_send_download_invitation(void);
FREObject bbm_ane_bbmsp_send_download_invitation(FREContext ctx, void* functionData,
                                                 uint32_t argc, FREObject argv[]);




#ifdef __cplusplus
}
#endif

#endif /* BBMSPMESSAGING_H_ */
