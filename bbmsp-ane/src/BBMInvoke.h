/*
 * BBMInvoke.h
 *
 *  Created on: Apr 7, 2013
 *      Author: hu77
 */


#include "FlashRuntimeExtensions.h"
#include <bps/navigator_invoke.h>


#ifndef BBMANE_BBMINVOKE_H_
#define BBMANE_BBMINVOKE_H_


#ifdef __cplusplus
extern "C" {
#endif


FREObject bbm_ane_invoke_start_chat(FREContext ctx, void* functionData, uint32_t argc, FREObject argv[]);

FREObject bbm_ane_invoke_change_avatar(FREContext ctx, void* functionData, uint32_t argc, FREObject argv[]);

FREObject bbm_ane_invoke_invite_to_bbm(FREContext ctx, void* functionData, uint32_t argc, FREObject argv[]);

FREObject bbm_ane_invoke_send_message(FREContext ctx, void* functionData, uint32_t argc, FREObject argv[]);

FREObject bbm_ane_invoke_send_file(FREContext ctx, void* functionData, uint32_t argc, FREObject argv[]);

FREObject bbm_ane_invoke_send_image(FREContext ctx, void* functionData, uint32_t argc, FREObject argv[]);


#ifdef __cplusplus
}
#endif


#endif /* BBMINVOKE_H_ */
