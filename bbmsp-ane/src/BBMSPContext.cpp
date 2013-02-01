/*
 * BBMSPContext.cpp
 *
 *  Created on: Jan 17, 2013
 *      Author: CHRIS
 */


#include "Globals.h"


#ifdef __cplusplus
extern "C" {
#endif

//======================================================================================//
//          CUSTOM FUNCTIONS
//======================================================================================//



//======================================================================================//
//                   STANDARD FUNCTIONS FROM bbmsp_context.h QNX FILE
//======================================================================================//

//BBMSP_API int bbmsp_is_access_allowed(void);
FREObject bbm_ane_bbmsp_is_access_allowed(FREContext ctx, void* functionData,
                                          uint32_t argc, FREObject argv[]){
   int code = bbmsp_is_access_allowed();
   FREObject result;
   FRENewObjectFromInt32(code, &result);
   return result;
}

//BBMSP_API int bbmsp_can_show_profile_box(void);
FREObject bbm_ane_bbmsp_can_show_profile_box(FREContext ctx, void* functionData,
                                             uint32_t argc, FREObject argv[]){
   int code = bbmsp_can_show_profile_box();
   FREObject result;
   FRENewObjectFromInt32(code, &result);
   return result;
}

//BBMSP_API int bbmsp_can_send_bbm_invite(void);
FREObject bbm_ane_bbmsp_can_send_bbm_invite(FREContext ctx, void* functionData,
                                            uint32_t argc, FREObject argv[]){
   int code = bbmsp_can_send_bbm_invite();
   FREObject result;
   FRENewObjectFromInt32(code, &result);
   return result;
}



#ifdef __cplusplus
}
#endif



