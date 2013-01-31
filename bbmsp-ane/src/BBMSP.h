/*
 * BBMSP.h
 *
 *  Created on: Jan 16, 2013
 *      Author: CHRIS
 */


#include "FlashRuntimeExtensions.h"
//#include <bps/bps.h>
#include <bps/event.h>
//#include <bbmsp/bbmsp.h>
//#include <bbmsp/bbmsp_events.h>
#include <bbmsp/bbmsp_context.h>


#ifndef BBMANE_BBMSP_H_
#define BBMANE_BBMSP_H_


#ifdef __cplusplus
extern "C" {
#endif



//======================================================================================//
//          GLOBAL VARIABLES/DEFINITIONS
//======================================================================================//

typedef enum {
   REGISTRATION_INITIALIZING = 0,
   REGISTRATION_STARTING,
   REGISTRATION_STARTED,
   REGISTRATION_STOPPED
} ane_bbm_registration_progress_e;

//======================================================================================//
//          CUSTOM FUNCTIONS
//======================================================================================//

/**
 * @brief Registers your application with the BBMSocial Platform.
 *
 * @details This functions programmatically takes care of registering your application
 *  with the BBM Social Platform. It returns once registration is complete or if it fails.
 *
 * @param uuid the uuid to register or check access with the BBM Social Platform
 *
 * @return
 *
 * TODO: Implement the callbacks to notify AS of changes in the registration process
 */
FREObject bbm_ane_startRegistration(FREContext ctx, void* functionData,
                                    uint32_t argc, FREObject argv[]);

void* initRegistrationThread(void *data);
static void registrationEventComplete(bps_event_t *event);
static void bbmspProcessRegistration(const bbmsp_access_error_codes_t status);
static void registrationComplete();

//======================================================================================//
//                   STANDARD FUNCTIONS FROM bbmsp.h QNX FILE
//======================================================================================//

/**
 * @brief Retrieve the version number of the BBM Social Platform.
 *
 * @details Retrieves the version of the BBM Social Platform library
 * that your application is using. You can use the version number to check
 * whether your application is compatible with this version of the BBM Social
 * Platform.
 *
 * @return The version number of the BBM Social Platform library in the format
 * described in @c BBMSP_VERSION.
 *
 * @see BBMSP_VERSION, BBMSP_VERSION_STRING
 */
//BBMSP_API int bbmsp_get_version(void);
FREObject bbm_ane_bbmsp_get_version(FREContext ctx, void* functionData, uint32_t argc, FREObject argv[]);

/**
 * @brief Retrieve the unique BPS domain ID for the BBM Social Platform.
 *
 * @details Each event in BPS is associated with a domain that
 * represents the service that generated the event (for example, contactlist or
 * userprofile). You can use @c bbmsp_get_domain() to determine the service
 * (event domain) that generated a particular event.
 *
 * @return int The domain ID for the service.
 */
//BBMSP_API int bbmsp_get_domain(void);
FREObject bbm_ane_bbmsp_get_domain(FREContext ctx, void* functionData, uint32_t argc, FREObject argv[]);



#ifdef __cplusplus
}
#endif


#endif /* BBMSP_H_ */
