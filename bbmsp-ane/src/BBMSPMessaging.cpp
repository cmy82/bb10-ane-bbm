/*
 * BBMSPMessaging.cpp
 *
 *  Created on: Jan 18, 2013
 *      Author: HU77
 */


#include "Globals.h"


#ifdef __cplusplus
extern "C" {
#endif


//BBMSP_API int bbmsp_send_download_invitation(void);
FREObject bbm_ane_bbmsp_send_download_invitation(FREContext ctx, void* functionData,
                                                 uint32_t argc, FREObject argv[]){
   int code = bbmsp_send_download_invitation();
   FREObject result;
   FRENewObjectFromInt32(code, &result);
   return result;
}



#ifdef __cplusplus
}
#endif




