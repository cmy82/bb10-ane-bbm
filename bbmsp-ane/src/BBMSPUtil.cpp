/*
 * BBMSPUtil.cpp
 *
 *  Created on: Jan 18, 2013
 *      Author: HU77
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

typedef struct {
   int32_t  type;
   uint32_t size;
   uint32_t id;
   char     *data;
} image_data_s;


ane_image_thread_status_e imageThreadStatus = IMAGE_THREAD_INITIALIZING;
pthread_mutex_t           imageMutex;
std::queue<image_data_s*> imageQueue;
map<int,bbmsp_image_t*>   *bbmsp_image_map = new map<int,bbmsp_image_t*>();

//======================================================================================//
//          CUSTOM FUNCTIONS
//======================================================================================//

void* initImageThread(void *data){
   bps_initialize();
   //Get the channel id of this thread so child threads can push events back to it
   //ane_image_channel_id = bps_channel_get_active();

   //Initialize mutex to control access of the image data in the map and queue
   pthread_mutex_init(&imageMutex,NULL);

   bbmsp_image_t *bbmspImage;
   image_data_s  *imageData;

   imageThreadStatus = WAITING_ON_IMAGE;
   bool queueEmpty = true;

   for(;;) {
      switch(imageThreadStatus){
         case WAITING_ON_IMAGE:
            //If no images in queue then sleep for 15 seconds and check again. Once queue has
            //an image then process it
            if( queueEmpty ){
               sleep(15);
               pthread_mutex_lock(&imageMutex);
               queueEmpty = imageQueue.empty();
               pthread_mutex_unlock(&imageMutex);
            } else {
               imageThreadStatus = PROCESSING_IMAGE;
            }
            break;

         case PROCESSING_IMAGE:
            //If there is an image in the queue waiting to be constructed, pull it from the
            //queue and create it. Then push an event to the main thread once it has been
            //completed passing the image id so it can be passed back to AS to allow the
            //user to decide what to do next
            pthread_mutex_lock(&imageMutex);
            cout << "Image in queue to be processed" <<endl;
            imageData = imageQueue.front();
            imageQueue.pop();
            cout << "Pulled Image from queue" <<endl;
            pthread_mutex_unlock(&imageMutex);

            cout << "Creating image and pushing into image map" <<endl;
            bbmsp_image_create(&bbmspImage,(bbmsp_image_type_t)imageData->type,imageData->data,imageData->size);
            bbmsp_image_map->insert( std::pair<int,bbmsp_image_t*>(imageData->id,bbmspImage) );
            notifyImageComplete(imageData->id);
            free(imageData);
            cout << "Created image and pushed into map. Notified AS of completion" <<endl;

            pthread_mutex_lock(&imageMutex);
            queueEmpty = imageQueue.empty();
            cout << "Checking to see if image queue is empty " << queueEmpty << endl;
            pthread_mutex_unlock(&imageMutex);

            if( queueEmpty ){
               imageThreadStatus = WAITING_ON_IMAGE;
            }
            break;

         case IMAGE_THREAD_STOPPING:
            //Stop all image processing and delete stored images and data.
            for (map<int,bbmsp_image_t*>::iterator it=bbmsp_image_map->begin(); it!=bbmsp_image_map->end(); ++it)
               bbmsp_image_destroy(&(it->second));
            delete bbmsp_image_map;
            break;

         case IMAGE_THREAD_STOPPED:
            //All data and variables cleaned up. Thread can be exited.
            break;
      }

      if( imageThreadStatus == IMAGE_THREAD_STOPPED ) break;
   }

   bps_shutdown();
   return NULL;
}

static void notifyImageComplete(int id){
   const char *eventName = "ANEImageLoaded";
   char imgID[5];
   itoa(id,imgID,10);

   //FREDispatchStatusEventAsync(*currentContext, (const uint8_t*)eventName, (const uint8_t*)imgID);
}

//======================================================================================//
//                   STANDARD FUNCTIONS FROM bbmsp_util.h QNX FILE
//======================================================================================//

//BBMSP_API int bbmsp_image_create_empty(bbmsp_image_t** image);
FREObject bbm_ane_bbmsp_image_create_empty(FREContext ctx, void* functionData,
                                           uint32_t argc, FREObject argv[]){
   int code = 0;
   FREObject result;
   FRENewObjectFromInt32(code, &result);
   return result;
}


FREObject bbm_ane_bbmsp_image_create(FREContext ctx, void* functionData,
                                     uint32_t argc, FREObject argv[]){
   uint32_t      id = rand();
   image_data_s  *imageData;
   bbmsp_image_t *image;
   FREByteArray  imageBytes;
   const uint8_t *extension;
   uint32_t      extLength = 5;

   cout << "Create image called. Gathering data: " << time(NULL) << endl;
   imageData = (image_data_s *)malloc( sizeof(image_data_s) );
   FREGetObjectAsUTF8(argv[0],&extLength,&extension);
   if( strcmp((const char*)extension,"JPG") == 0 )  imageData->type = BBMSP_IMAGE_TYPE_JPG;
   if( strcmp((const char*)extension,"JPEG") == 0 ) imageData->type = BBMSP_IMAGE_TYPE_JPG;
   if( strcmp((const char*)extension,"PNG") == 0 )  imageData->type = BBMSP_IMAGE_TYPE_PNG;
   if( strcmp((const char*)extension,"GIF") == 0 )  imageData->type = BBMSP_IMAGE_TYPE_GIF;
   if( strcmp((const char*)extension,"BMP") == 0 )  imageData->type = BBMSP_IMAGE_TYPE_BMP;
   cout << "Image type: " << imageData->type << endl;
   //FREGetObjectAsInt32(argv[0],&(imageData->type));
   FREGetObjectAsUint32(argv[1],&(imageData->size));
   imageData->id = id;
   FREAcquireByteArray(argv[2],&imageBytes);
   cout << "Acquired info: " << time(NULL) << ". About to copy " << imageBytes.length << " bytes" << endl;

   //create random id for image, copy data into struct, push struct into queue for
   //processing and return the id
   imageData->data = (char *)malloc(imageData->size);
   for(uint32_t i=0; i<imageData->size; ++i)
      imageData->data[i] = imageBytes.bytes[i];
   cout << "Data copied: " << time(NULL) << ". Bytes: " << sizeof(imageData->data) << endl;

   FREReleaseByteArray(argv[2]);

   pthread_mutex_lock(&imageMutex);
   imageQueue.push(imageData);
   pthread_mutex_unlock(&imageMutex);

   FREObject result;
   FRENewObjectFromUint32(id, &result);
   return result;
}

//BBMSP_API int bbmsp_image_destroy(bbmsp_image_t** image);
FREObject bbm_ane_bbmsp_image_destroy(FREContext ctx, void* functionData,
                                      uint32_t argc, FREObject argv[]){
   int imageID;
   FREGetObjectAsInt32(argv[0],&imageID);
   bbmsp_image_t *image = (*bbmsp_image_map)[imageID];

   int code = bbmsp_image_destroy(&image);
   bbmsp_image_map->erase( bbmsp_image_map->find(imageID) );

   FREObject result;
   FRENewObjectFromInt32(code, &result);
   return result;
}

//BBMSP_API bbmsp_image_type_t bbmsp_image_get_type(bbmsp_image_t* image);
FREObject bbm_ane_bbmsp_image_get_type(FREContext ctx, void* functionData,
                                       uint32_t argc, FREObject argv[]){
   int imageID;
   FREGetObjectAsInt32(argv[0],&imageID);
   bbmsp_image_t *image = (*bbmsp_image_map)[imageID];

   bbmsp_image_type_t code = bbmsp_image_get_type(image);

   FREObject result;
   FRENewObjectFromInt32((int32_t)code, &result);
   return result;
}

//BBMSP_API char* bbmsp_image_get_data(bbmsp_image_t* image);
FREObject bbm_ane_bbmsp_image_get_data(FREContext ctx, void* functionData,
                                       uint32_t argc, FREObject argv[]){
   int imageID;
   FREGetObjectAsInt32(argv[0],&imageID);
   bbmsp_image_t *image = (*bbmsp_image_map)[imageID];

   char *data = bbmsp_image_get_data(image);
   uint32_t size = bbmsp_image_get_data_size(image);

   FREByteArray imageData;
   FREAcquireByteArray(argv[1],&imageData);
   memcpy(imageData.bytes,data,size);
   FREReleaseByteArray(argv[1]);

   return NULL;
}

//BBMSP_API unsigned int bbmsp_image_get_data_size(bbmsp_image_t* image);
FREObject bbm_ane_bbmsp_image_get_data_size(FREContext ctx, void* functionData,
                                            uint32_t argc, FREObject argv[]){
   int imageID;
   FREGetObjectAsInt32(argv[0],&imageID);
   bbmsp_image_t *image = (*bbmsp_image_map)[imageID];

   unsigned int code = bbmsp_image_get_data_size(image);
   FREObject result;
   FRENewObjectFromUint32(code, &result);
   return result;
}



#ifdef __cplusplus
}
#endif

