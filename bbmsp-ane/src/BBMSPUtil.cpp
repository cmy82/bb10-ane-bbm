/*
 * BBMSPUtil.cpp
 *
 *  Created on: Jan 18, 2013
 *      Author: HU77
 */


#include "Globals.h"
#include <QImage>
#include <QFile>
#include <QString>
#include <QByteArray>


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


int ane_image_domain     = -1;
int ane_image_channel_id = -1;

ane_image_thread_status_e imageThreadStatus = IMAGE_THREAD_INITIALIZING;
pthread_mutex_t           imageMutex;
std::queue<image_data_s*> imageQueue;
map<int,ane_image_s*>     *ane_image_map = new map<int,ane_image_s*>();

//======================================================================================//
//          CUSTOM FUNCTIONS
//======================================================================================//

void* initImageThread(void *data){
   bps_initialize();
   ane_image_domain = bps_register_domain();
   ane_image_channel_id = bps_channel_get_active();

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
         {
            //If there is an image in the queue waiting to be constructed, pull it from the
            //queue and create it. Then push an event to the main thread once it has been
            //completed passing the image id so it can be passed back to AS to allow the
            //user to decide what to do next
            pthread_mutex_lock(&imageMutex);
            imageData = imageQueue.front();
            imageQueue.pop();
            pthread_mutex_unlock(&imageMutex);

            //Use QImage class to shrink the image down to an acceptable size by the profile and profile box
            //structs (<32k)
            QString fileExtension;
            switch(imageData->type){
               case 0: fileExtension.append("jpg"); break;
               case 1: fileExtension.append("png"); break;
               case 2: fileExtension.append("gif"); break;
               case 3: fileExtension.append("bmp"); break;
            }

            QString tempAvatar = QString("/accounts/1000/shared/photos");
            tempAvatar.append("/tempAvatar.");
            tempAvatar.append(fileExtension);
            QFile tFile(tempAvatar);
            tFile.remove();

            QImage image;
            QImage scaled;
            QByteArray imageDataAry;
            int maxWidth = 300;
            int maxHeight = 300;

            image.loadFromData((uchar *)imageData->data,(int)imageData->size,0);
            scaled = image.scaled(maxWidth,maxHeight,Qt::KeepAspectRatio);
            while( scaled.byteCount() > 32768 ){
               //To save image must be smaller than 32k
               maxWidth-=10;
               maxHeight-=10;
               scaled = image.scaled(maxWidth,maxHeight,Qt::KeepAspectRatio);
            }
            scaled.save(tempAvatar);

            bool errored = false;
            QFile tempFile(tempAvatar);
            if(tempFile.open(QFile::ReadOnly)){
               QDataStream in(&tempFile);

               char bytes[100];
               int bytesRead;
               while(not in.atEnd()){
                  bytesRead = in.readRawData(bytes, 100);
                  if(bytesRead <= 0) {
                     errored = true;
                     break;
                  }
                  imageDataAry.append(bytes, bytesRead);
               }
            }

            // Create the icon object and register the icon
            bbmsp_image_t *bbmspImage = NULL;
            bbmsp_image_t *bbmspAvatar = NULL;
            bbmsp_image_create_empty(&bbmspImage);
            bbmsp_image_create_empty(&bbmspAvatar);
            ane_image_s *images = (ane_image_s*)malloc(sizeof(ane_image_s));

            if( !errored ){
               bbmsp_image_create(&bbmspAvatar,(bbmsp_image_type_t)imageData->type,imageDataAry.data(), imageDataAry.size());
               bbmsp_image_create(&bbmspImage,(bbmsp_image_type_t)imageData->type,imageData->data,imageData->size);

               images->original = bbmspImage;
               images->profile = bbmspAvatar;
            } else {
               bbmsp_image_create(&bbmspImage,(bbmsp_image_type_t)imageData->type,imageData->data,imageData->size);

               images->original = bbmspImage;
               images->profile = NULL;
            }

            ane_image_map->insert( std::pair<int,ane_image_s*>(imageData->id,images) );
            notifyImageComplete(imageData->id);
            delete imageData->data;
            free(imageData);

            pthread_mutex_lock(&imageMutex);
            queueEmpty = imageQueue.empty();
            pthread_mutex_unlock(&imageMutex);

            if( queueEmpty ){
               imageThreadStatus = WAITING_ON_IMAGE;
            }
            cout << "returning from call" << endl;
         }
            break;

         case IMAGE_THREAD_STOPPING:
            //Stop all image processing and delete stored images and data.
            for (map<int,ane_image_s*>::iterator it=ane_image_map->begin(); it!=ane_image_map->end(); ++it){
               ane_image_s *images = it->second;
               bbmsp_image_destroy(&(images->original));
               bbmsp_image_destroy(&(images->profile));
            }
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
   char imgID[15];
   for( int i=0; i<15; ++i ) imgID[i] = '\0';
   itoa(id,imgID,10);
   FREDispatchStatusEventAsync(currentContext, (const uint8_t*)eventName, (const uint8_t*)imgID);
}

FREObject bbm_ane_image_exists(FREContext ctx, void* functionData,
                                     uint32_t argc, FREObject argv[]){
   int imageID;
   FREGetObjectAsInt32(argv[0],&imageID);
   ane_image_s *images = (ane_image_map->find(imageID))->second;

   int code = 0;
   if( images != NULL ) code = 1;

   FREObject result;
   FRENewObjectFromUint32(code, &result);
   return result;
}

//======================================================================================//
//                   STANDARD FUNCTIONS FROM bbmsp_util.h QNX FILE
//======================================================================================//

FREObject bbm_ane_bbmsp_image_create(FREContext ctx, void* functionData,
                                     uint32_t argc, FREObject argv[]){
   uint32_t      id = rand();
   image_data_s  *imageData;
   bbmsp_image_t *image;
   FREByteArray  imageBytes;
   const uint8_t *extension;
   uint32_t      extLength = 5;

   imageData = (image_data_s *)malloc( sizeof(image_data_s) );
   FREGetObjectAsUTF8(argv[0],&extLength,&extension);
   if( strcmp((const char*)extension,"JPG") == 0 )  imageData->type = BBMSP_IMAGE_TYPE_JPG;
   if( strcmp((const char*)extension,"JPEG") == 0 ) imageData->type = BBMSP_IMAGE_TYPE_JPG;
   if( strcmp((const char*)extension,"PNG") == 0 )  imageData->type = BBMSP_IMAGE_TYPE_PNG;
   if( strcmp((const char*)extension,"GIF") == 0 )  imageData->type = BBMSP_IMAGE_TYPE_GIF;
   if( strcmp((const char*)extension,"BMP") == 0 )  imageData->type = BBMSP_IMAGE_TYPE_BMP;

   FREGetObjectAsUint32(argv[1],&(imageData->size));
   imageData->id = id;

   FREAcquireByteArray(argv[2],&imageBytes);
   imageData->data = new char[imageData->size];
   for(uint32_t i=0; i<imageData->size; ++i){
      imageData->data[i] = imageBytes.bytes[i];
   }
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
   ane_image_s *images = (*ane_image_map)[imageID];

   int code = bbmsp_image_destroy(&(images->original));
   bbmsp_image_destroy(&(images->profile));

   ane_image_map->erase( ane_image_map->find(imageID) );

   FREObject result;
   FRENewObjectFromInt32(code, &result);
   return result;
}

//BBMSP_API bbmsp_image_type_t bbmsp_image_get_type(bbmsp_image_t* image);
FREObject bbm_ane_bbmsp_image_get_type(FREContext ctx, void* functionData,
                                       uint32_t argc, FREObject argv[]){
   int imageID;
   FREGetObjectAsInt32(argv[0],&imageID);
   ane_image_s *images = (*ane_image_map)[imageID];

   bbmsp_image_type_t code = bbmsp_image_get_type(images->original);

   FREObject result;
   FRENewObjectFromInt32((int32_t)code, &result);
   return result;
}

//BBMSP_API char* bbmsp_image_get_data(bbmsp_image_t* image);
FREObject bbm_ane_bbmsp_image_get_data(FREContext ctx, void* functionData,
                                       uint32_t argc, FREObject argv[]){
   int imageID;
   FREGetObjectAsInt32(argv[0],&imageID);
   ane_image_s *images = (*ane_image_map)[imageID];

   char *data = bbmsp_image_get_data(images->original);
   uint32_t size = bbmsp_image_get_data_size(images->original);

   FREByteArray imageData;
   FREAcquireByteArray(argv[1],&imageData);
   for(uint32_t i=0; i<size; ++i){
      imageData.bytes[i] = data[i];
   }
   FREReleaseByteArray(argv[1]);

   return NULL;
}

FREObject bbm_ane_bbmsp_image_get_profile_data(FREContext ctx, void* functionData,
                                               uint32_t argc, FREObject argv[]){
   int imageID;
   FREGetObjectAsInt32(argv[0],&imageID);
   ane_image_s *images = (*ane_image_map)[imageID];

   char *data = bbmsp_image_get_data(images->profile);
   uint32_t size = bbmsp_image_get_data_size(images->profile);

   FREByteArray imageData;
   FREAcquireByteArray(argv[1],&imageData);
   for(uint32_t i=0; i<size; ++i){
      imageData.bytes[i] = data[i];
   }
   FREReleaseByteArray(argv[1]);

   return NULL;
}

//BBMSP_API unsigned int bbmsp_image_get_data_size(bbmsp_image_t* image);
FREObject bbm_ane_bbmsp_image_get_data_size(FREContext ctx, void* functionData,
                                            uint32_t argc, FREObject argv[]){
   int imageID;
   FREGetObjectAsInt32(argv[0],&imageID);
   ane_image_s *images = (*ane_image_map)[imageID];

   unsigned int code = bbmsp_image_get_data_size(images->original);
   FREObject result;
   FRENewObjectFromUint32(code, &result);
   return result;
}

FREObject bbm_ane_bbmsp_image_get_profile_data_size(FREContext ctx, void* functionData,
                                                    uint32_t argc, FREObject argv[]){
   int imageID;
   FREGetObjectAsInt32(argv[0],&imageID);
   ane_image_s *images = (*ane_image_map)[imageID];

   unsigned int code = bbmsp_image_get_data_size(images->profile);
   FREObject result;
   FRENewObjectFromUint32(code, &result);
   return result;
}



#ifdef __cplusplus
}
#endif

