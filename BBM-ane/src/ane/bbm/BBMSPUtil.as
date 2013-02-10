package ane.bbm {
   import ane.bbm.events.ANEImageEvent;
   import ane.utils.images.JPGEncoder;
   
   import flash.display.Bitmap;
   import flash.display.BitmapData;
   import flash.display.Loader;
   import flash.display.LoaderInfo;
   import flash.events.Event;
   import flash.events.EventDispatcher;
   import flash.events.StatusEvent;
   import flash.external.ExtensionContext;
   import flash.filesystem.File;
   import flash.filesystem.FileMode;
   import flash.filesystem.FileStream;
   import flash.geom.Rectangle;
   import flash.net.FileFilter;
   import flash.net.URLRequest;
   import flash.utils.ByteArray;
   
   public class BBMSPUtil extends EventDispatcher {
      
      private var _context:ExtensionContext;
      private var _imageList:Vector.<ImageTracker>;
      
      public function BBMSPUtil(context:ExtensionContext) {
         _context = context;
         _context.addEventListener( StatusEvent.STATUS, imageStatusUpdate );
         _imageList = new Vector.<ImageTracker>();
      }

	   //================================== DATA FUNCTIONS ====================================
         
      /**
       * Gets the image type from an image loaded to the ANE by the ID. 
       * @param id Image ID of the image to check. ID is returned after image has been uploaded by a call to one of the
       *           loadImage functions.
       * @return  The image type/extension as a string.
       * 
       */
      public function getImageType(id:Number):String {
         for( var i:int = 0; i<_imageList.length; i++ ){
            if( _imageList[i].id == id ){
               _imageList[i] = null;
               var result:Object = this._context.call( "bbm_ane_bbmsp_image_get_type", id );
               var type:Number = Number(result);
               switch(type){
                  case 0:  return "JPG";  break;
                  case 1:  return "PNG";  break;
                  case 2:  return "GIF";  break
                  case 3:  return "BMP";  break;
               }              
            }
         }
         return "Unknown";
	   }
	  
      /**
       * Get the size of an image loaded to the ANE in bytes by the ID.
       * @param id Image ID of the image to check. ID is returned after image has been uploaded by a call to one of the
       *           loadImage functions.
       * @return The size of the image data in bytes.
       * 
       */
	   public function getImageDataSize(id:Number):Number {
         for( var i:int = 0; i<_imageList.length; i++ ){
            if( _imageList[i].id == id ){
	 			  _imageList[i] = null;
               var result:Object = this._context.call( "bbm_ane_bbmsp_image_get_data_size", id );
 				  return Number(result); 
			   }
		   }
		   return -1;  
	   }
	   
      /**
       * Deletes an image file from the ANE by the ID.
       * @param id Image ID of the image to check. ID is returned after image has been uploaded by a call to one of the
       *           loadImage functions.
       * @return Returns -1 if the delete operation failed.
       * 
       */
	   public function deleteImage(id:Number):Number {
	 	   for( var i:int = 0; i<_imageList.length; i++ ){
 			   if( _imageList[i].id == id ){ 
               var result:Object = this._context.call( "bbm_ane_bbmsp_image_destroy", id );
               if( Number(result) != -1 )  _imageList[i] = null;
				   return Number(result );
			   }
		   }
		   return -1;
	   }
      
      /**
       * Check to see whether an image has been loaded to the ANE by ID.
       * @param id Image ID of the image to check. ID is returned after image has been uploaded by a call to one of the
       *           loadImage functions.
       * @return True or false whether or no the image is located in the ANE.
       * 
       */        
      public function imageExists(id:Number):Boolean {
         var result:Object = this._context.call( "bbm_ane_image_exists", id );
         if( Number(result) == 0 ) return false;
         return true;
      }
      
      //================================== LOADING FUNCTIONS ====================================
      
      
      /**
       * Loads an image from the device using FilePicker.
       * @param none
       * @return none 
       * 
       */
      public function loadImage():void {
         var dir:File = File.userDirectory.resolvePath( 'shared/photos' );
         var imgFilter:FileFilter = new FileFilter("Images", "*.png;*.bmp;*.jpg;*.jpeg");
         dir.addEventListener(Event.SELECT,imageSelected);
         dir.addEventListener(Event.CANCEL,loadCanceled);
         dir.browseForOpen("Choose Image",[imgFilter]);
      }
      
      
      /**
       * Loads an image from file data to the ANE.
       * @param name Name to give to the loaded image file
       * @param img The bitmap image to load
       * @return none
       * 
       */
      public function loadImageFromBitmap(name:String,img:Bitmap):void {
         var rect:Rectangle = new Rectangle(0,0,img.width,img.height);
         var data:ByteArray = img.bitmapData.getPixels(rect);
         
         trace("trying to load from bitmap with a size of "+img.width+"x"+img.height);
         if( search(name) == false ){            
            var result:Object = 
               this._context.call( "bbm_ane_bbmsp_image_create_from_data", data.length, data, img.width, img.height );
            var image:ImageTracker = new ImageTracker(name,Number(result));
            _imageList.push(image);
         } else {
            var image2:ImageTracker = findByName(name);
            dispatchEvent( new ANEImageEvent(ANEImageEvent.IMAGE_LOADED,image2.id,name) );            
         }
      }
      
      /**
       * Loads an image file that was packaged with the application
       * @param loc The relative file location in respect to the File.applicationDirectory location.
       * @return none
       * 
       */
      public function loadImageFromResource(loc:String):void {
        var rsrcFile:File = File.applicationDirectory.resolvePath(loc);
        trace(rsrcFile.nativePath);
        var rsrcStream:FileStream = new FileStream();
        rsrcStream.open(rsrcFile,FileMode.READ);
        var rsrcBytes:ByteArray = new ByteArray();
        rsrcStream.readBytes(rsrcBytes);
        rsrcStream.close();
        
        if( search(rsrcFile.name) == false ){
           var result:Object = 
              this._context.call( "bbm_ane_bbmsp_image_create", rsrcFile.extension.toUpperCase(), rsrcBytes.length, rsrcBytes );
           var image:ImageTracker = new ImageTracker(rsrcFile.name,Number(result));
           _imageList.push(image);
        } else {
           var image2:ImageTracker = findByName(rsrcFile.name);
           dispatchEvent( new ANEImageEvent(ANEImageEvent.IMAGE_LOADED,image2.id,rsrcFile.name) );            
        }
      }
      
      private function loadCanceled(e:Event):void { }
      
      //Performs actual loading of the image after selected from FilePicker
      private function imageSelected(e:Event):void {
         var imageFile:File = e.target as File;
         var imageStream:FileStream = new FileStream();                  
         imageStream.open(imageFile,FileMode.READ);
         var imageBytes:ByteArray = new ByteArray();
         imageStream.readBytes(imageBytes);
         imageStream.close();
         
         if( search(imageFile.name) == false ){
            var result:Object = 
               this._context.call( "bbm_ane_bbmsp_image_create", imageFile.extension.toUpperCase(), imageBytes.length, imageBytes );
            var image:ImageTracker = new ImageTracker(imageFile.name,Number(result));
            _imageList.push(image);
         } else {
            var image2:ImageTracker = findByName(imageFile.name);
            dispatchEvent( new ANEImageEvent(ANEImageEvent.IMAGE_LOADED,image2.id,imageFile.name) );            
         }
      }
                
      //=============================== RETRIEVING FUNCTIONS ====================================
      
      /**
       * Calls for the full/original image with the specified ID to be loaded by the ANE.
       * @param id ID number of the image to load.
       * @return none
       * 
       */
      public function retrieveImage(id:Number):void {
         var size:Object = this._context.call( "bbm_ane_bbmsp_image_get_data_size", id );
         var imgData:ByteArray = new ByteArray();
         imgData.length = Number(size);
         this._context.call( "bbm_ane_bbmsp_image_get_data", id, imgData );
         var loader:Loader = new Loader();
         loader.contentLoaderInfo.addEventListener(Event.COMPLETE,imageRetrieved)
         loader.loadBytes(imgData);         
      }
                
      /**
       * Calls for the reduced/profile image with the specified ID to be loaded by the ANE. Returned image is suitable for use
       * as a profile picture or as a profile box item icon.
       * @param id ID number of the image to load.
       * @return none
       * 
       */
      public function retrieveProfileImage(id:Number):void {
         var size:Object = this._context.call( "bbm_ane_bbmsp_image_get_profile_data_size", id );
         var imgData:ByteArray = new ByteArray();
         imgData.length = Number(size);
         this._context.call( "bbm_ane_bbmsp_image_get_profile_data", id, imgData );
         var loader:Loader = new Loader();
         loader.contentLoaderInfo.addEventListener(Event.COMPLETE,profileImageRetrieved)
         loader.loadBytes(imgData);         
      }
         
      //Performs actual loading of retrieved data into a bitmap from retrieveProfile
      private function imageRetrieved(e:Event):void {
         var loaderInfo:LoaderInfo = e.target as LoaderInfo;
         var loader:Loader = loaderInfo.loader;     
		 
		   var bitmapData:BitmapData = new BitmapData(loaderInfo.width, loaderInfo.height, true, 0xFFFFFF);
		   bitmapData.draw(loaderInfo.loader);
		 
		   var bmp:Bitmap = new Bitmap(bitmapData);
		   dispatchEvent( new ANEImageEvent(ANEImageEvent.IMAGE_RETRIEVED,0,"",bmp) );
      }
      
      //Performs actual loading of retrieved data into a bitmap from retrieveProfileImage
      private function profileImageRetrieved(e:Event):void {
         var loaderInfo:LoaderInfo = e.target as LoaderInfo;
         var loader:Loader = loaderInfo.loader;     
         
         var bitmapData:BitmapData = new BitmapData(loaderInfo.width, loaderInfo.height, true, 0xFFFFFF);
         bitmapData.draw(loaderInfo.loader);
         
         var bmp:Bitmap = new Bitmap(bitmapData);
         dispatchEvent( new ANEImageEvent(ANEImageEvent.PROF_IMAGE_RETRIEVED,0,"",bmp) );
      }
      
      //======================================= MISCELLANEOUS FUNCTIONS =================================
      
      //Dispatches the status events 
      private function imageStatusUpdate(e:StatusEvent):void {
         if(e.code == ANEImageEvent.IMAGE_LOADED ){
            var id:Number = Number(e.level);
            var tracker:ImageTracker = findByID(id);
            if( tracker == null ){
               trace("ID: "+id+" not found");
               return;
            }
            var file:String = findByID(id).name;
            dispatchEvent( new ANEImageEvent(ANEImageEvent.IMAGE_LOADED,id,file) );            
         }
      }
      
      //Searches for filename and returns true if already stored
      private function search(img:String):Boolean {
         if( _imageList.length == 0 ) return false;
         var found:Boolean = false;
         for( var i:int = 0; i<_imageList.length; i++ ){
            if( _imageList[i].name == img ){
               found = true;
               break;
            }
         }
         return found;
      }
      
      //Searches if filename is already stored and returns ImageTracker object
      private function findByName(img:String):ImageTracker {
         var found:ImageTracker;
         for( var i:int = 0; i<_imageList.length; i++ ){
            if( _imageList[i].name == img ){
               found = _imageList[i];
               break;
            }
         }
         return found;
      }
      
      //Searches to see if ID is already stored and returns ImageTracker object
      private function findByID(id:Number):ImageTracker {
         var found:ImageTracker = null;
         for( var i:int = 0; i<_imageList.length; i++ ){
            if( _imageList[i].id == id ){
               found = _imageList[i];
               break;
            }
         }
         return found;
      }
   }
}



//Class used to internally track whether an image has been loaded
class ImageTracker {
   private var _name:String;
   private var _id:Number;
   public function ImageTracker(name:String,id:Number){
      _name = name;
      _id = id;
   }
   public function get id():Number {
      return _id;
   }
   public function get name():String {
      return _name;
   }
}

