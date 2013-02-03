/*
 * BBMSPUtil.h
 *
 *  Created on: Jan 18, 2013
 *      Author: HU77
 */


#include "FlashRuntimeExtensions.h"
#include <queue>


#ifndef BBMANE_BBMSPUTIL_H_
#define BBMANE_BBMSPUTIL_H_


#ifdef __cplusplus
extern "C" {
#endif


//======================================================================================//
//          GLOBAL VARIABLES/DEFINITIONS
//======================================================================================//

typedef enum {
   IMAGE_THREAD_INITIALIZING = 0,
   WAITING_ON_IMAGE,
   PROCESSING_IMAGE,
   IMAGE_THREAD_STOPPING,
   IMAGE_THREAD_STOPPED
} ane_image_thread_status_e;

//======================================================================================//
//          CUSTOM FUNCTIONS
//======================================================================================//

void* initImageThread(void *data);
static void notifyImageComplete(int id);

//======================================================================================//
//                   STANDARD FUNCTIONS FROM bbmsp_util.h QNX FILE
//======================================================================================//

/**
 * @brief Creates a new @c bbmsp_image_t to hold an image.
 *
 * @details Images should be constructed by using the complete image file data that is read from the
 *          image source file, not just the canvas data.
 *
 * @param type The image type. If an invalid MIME type is provided, the behavior of this method is undefined.
 * @param data_size The size of the image data in bytes. Must be > 0.
 * @param data A byte array containing the image data.
 *
 * @return An object id representing the image data. Once the image has been constructed,
 *         an event listener will be fired to notify the user that the image object is
 *         ready to be used
 */
//BBMSP_API bbmsp_result_t bbmsp_image_create(
//    bbmsp_image_t** image,
//    const bbmsp_image_type_t type,
//    const char* data,
//    const unsigned int data_size);
FREObject bbm_ane_bbmsp_image_create(FREContext ctx, void* functionData,
                                     uint32_t argc, FREObject argv[]);


/**
 * @brief Destroys a created @c bbmsp_image_t image.
 *
 * @param int The image id
 *
 * @return unknown
 */
//BBMSP_API int bbmsp_image_destroy(bbmsp_image_t** image);
FREObject bbm_ane_bbmsp_image_destroy(FREContext ctx, void* functionData,
                                      uint32_t argc, FREObject argv[]);

/**
 * @brief Returns the image MIME type.
 *
 * @details If an invalid MIME type was provided, the behavior of this method is undefined.
 *
 * @param int The image id to return the type for
 *
 * @return the image MIME type.
 * @see bbmsp_image_t, bbmsp_image_create
 */
//BBMSP_API bbmsp_image_type_t bbmsp_image_get_type(bbmsp_image_t* image);
FREObject bbm_ane_bbmsp_image_get_type(FREContext ctx, void* functionData,
                                       uint32_t argc, FREObject argv[]);

/**
 * @brief Returns the complete image object data
 *
 * @details This method returns the data for the complete image object as was used when
 *          the object was created
 *
 * @param int The image id to return the type for
 *        ByteArray A byte array to store the returned data
 *
 * @return none
 */
//BBMSP_API char* bbmsp_image_get_data(bbmsp_image_t* image);
FREObject bbm_ane_bbmsp_image_get_data(FREContext ctx, void* functionData,
                                       uint32_t argc, FREObject argv[]);

/**
 * @brief Returns the size of image object.
 *
 * @param int The image id to return the size of
 *
 * @return the image size
 */
//BBMSP_API unsigned int bbmsp_image_get_data_size(bbmsp_image_t* image);
FREObject bbm_ane_bbmsp_image_get_data_size(FREContext ctx, void* functionData,
                                            uint32_t argc, FREObject argv[]);



#ifdef __cplusplus
}
#endif



#endif /* BBMSPUTIL_H_ */
