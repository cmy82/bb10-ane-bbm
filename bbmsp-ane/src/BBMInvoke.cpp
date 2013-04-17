/*
 * BBMInvoke.cpp
 *
 *  Created on: Apr 7, 2013
 *      Author: hu77
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



//======================================================================================//
//          CUSTOM FUNCTIONS
//======================================================================================//

FREObject bbm_ane_invoke_start_chat(FREContext ctx, void* functionData, uint32_t argc, FREObject argv[]){
   // create handler invocation
   navigator_invoke_invocation_t *invoke = NULL;
   navigator_invoke_invocation_create(&invoke);

   // set action
   navigator_invoke_invocation_set_action(invoke,"bb.action.BBMCHAT");
   if( argc > 0 ){
      const uint8_t *value;
      uint32_t      length=8;

      char *pin = (char*) malloc(length+1);
      FREGetObjectAsUTF8(argv[0], &length, &value );
      strncpy(pin,(char*)value,length);
      pin[length] = '\0';
      strncat(pin,pin,0);

      char *uri = (char*) malloc(length+5);
      strncpy(uri,"pin:",4);
      strncat(uri,pin,8);

      navigator_invoke_invocation_set_uri(invoke,uri);
   }

   // invoke the target
   int code = navigator_invoke_invocation_send(invoke);

   // clean up resources
   navigator_invoke_invocation_destroy(invoke);

   return NULL;
}

FREObject bbm_ane_invoke_change_avatar(FREContext ctx, void* functionData, uint32_t argc, FREObject argv[]){
   return NULL;
}

//Works
FREObject bbm_ane_invoke_invite_to_bbm(FREContext ctx, void* functionData, uint32_t argc, FREObject argv[]){
   // create handler invocation
   navigator_invoke_invocation_t *invoke = NULL;
   navigator_invoke_invocation_create(&invoke);

   // set action
   navigator_invoke_invocation_set_action(invoke,"bb.action.INVITEBBM");
   if( argc > 0 ){
      const uint8_t *value;
      uint32_t      length=8;

      char *pin = (char*) malloc(length+1);
      FREGetObjectAsUTF8(argv[0], &length, &value );
      strncpy(pin,(char*)value,length);
      pin[length] = '\0';
      strncat(pin,pin,0);

      char *uri = (char*) malloc(length+5);
      strncpy(uri,"pin:",4);
      strncat(uri,pin,8);

      cout << "Attempting to invite pin " << pin << endl;
      cout << "URI value " << uri << endl;
      navigator_invoke_invocation_set_uri(invoke,uri);
   } else
      navigator_invoke_invocation_set_uri(invoke,"pin:");

   // invoke the target
   int code = navigator_invoke_invocation_send(invoke);

   // clean up resources
   navigator_invoke_invocation_destroy(invoke);

   return NULL;
}

FREObject bbm_ane_invoke_send_message(FREContext ctx, void* functionData, uint32_t argc, FREObject argv[]){
   // create handler invocation
   navigator_invoke_invocation_t *invoke = NULL;
   navigator_invoke_invocation_create(&invoke);

   // set action
   navigator_invoke_invocation_set_action(invoke,"bb.action.SHARE");
   navigator_invoke_invocation_set_target(invoke,"sys.bbm.sharehandler");
   if( argc > 0 ){
      const uint8_t *value;
      uint32_t      length;

      FREGetObjectAsUint32(argv[1],&length);

      char *msg = (char*) malloc(length+1);
      FREGetObjectAsUTF8(argv[0], &length, &value );
      strncpy(msg,(char*)value,length);
      msg[length] = '\0';

      navigator_invoke_invocation_set_type(invoke,"text/plain");
      navigator_invoke_invocation_set_data(invoke,msg,length+1);
   } //else
     // navigator_invoke_invocation_set_uri(invoke,"pin:");

   // invoke the target
   int code = navigator_invoke_invocation_send(invoke);

   // clean up resources
   navigator_invoke_invocation_destroy(invoke);

   return NULL;
}

FREObject bbm_ane_invoke_send_file(FREContext ctx, void* functionData, uint32_t argc, FREObject argv[]){
   return NULL;
}

FREObject bbm_ane_invoke_send_image(FREContext ctx, void* functionData, uint32_t argc, FREObject argv[]){
   // create handler invocation
   navigator_invoke_invocation_t *invoke = NULL;
   navigator_invoke_invocation_create(&invoke);

   // set action
   navigator_invoke_invocation_set_action(invoke,"bb.action.SHARE");
   navigator_invoke_invocation_set_target(invoke,"sys.bbm.sharehandler");

   if( argc > 0 ){
      const uint8_t *value;
      uint32_t      length;

      //Get filename of image and set as uri
      FREGetObjectAsUint32(argv[1],&length);

      char *filename = (char*) malloc(length+1);
      FREGetObjectAsUTF8(argv[0], &length, &value );
      strncpy(filename,(char*)value,length);
      filename[length] = '\0';

      char *uri = (char*) malloc(length+8);
      strncpy(uri,"file://",7);
      uri[7] = '\0';
      strncat(uri,filename,length);

      cout << "Attempting to send image " << filename << endl;
      cout << "URI value " << uri << endl;
      navigator_invoke_invocation_set_uri(invoke,uri);
   }

   // invoke the target
   int code = navigator_invoke_invocation_send(invoke);

   // clean up resources
   navigator_invoke_invocation_destroy(invoke);

   return NULL;
}


#ifdef __cplusplus
}
#endif


