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
   import flash.net.FileFilter;
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
	  
	   public function getImageSize(id:Number):Number {
         for( var i:int = 0; i<_imageList.length; i++ ){
            if( _imageList[i].id == id ){
	 			  _imageList[i] = null;
               var result:Object = this._context.call( "bbm_ane_bbmsp_image_get_data_size", id );
 				  return Number(result); 
			   }
		   }
		   return -1;  
	   }
	   
	   public function deleteImage(id:Number):Number {
	 	   for( var i:int = 0; i<_imageList.length; i++ ){
 			   if( _imageList[i].id == id ){ 
				   _imageList[i] = null;
				   var result:Object = this._context.call( "bbm_ane_bbmsp_image_destroy", id );
				   return Number(result );
			   }
		   }
		   return -1;
	   }
	  
      
      public function imageExists(id:Number):Boolean {
         var result:Object = this._context.call( "bbm_ane_image_exists", id );
         if( Number(result) == 0 ) return false;
         return true;
      }
      
      //================================== LOADING FUNCTIONS ====================================
      public function loadImage():void {
         var dir:File = File.userDirectory.resolvePath( 'shared/photos' );
         var imgFilter:FileFilter = new FileFilter("Images", "*.png;*.bmp;*.jpg;*.jpeg");
         dir.addEventListener(Event.SELECT,imageSelected);
         dir.addEventListener(Event.CANCEL,loadCanceled);
         dir.browseForOpen("Choose Image",[imgFilter]);
      }
      
      private function loadCanceled(e:Event):void {
         
      }
      
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
      public function retrieveImage(id:Number):void {
         var size:Object = this._context.call( "bbm_ane_bbmsp_image_get_data_size", id );
         var imgData:ByteArray = new ByteArray();
         imgData.length = Number(size);
         this._context.call( "bbm_ane_bbmsp_image_get_data", id, imgData );
         var loader:Loader = new Loader();
         loader.contentLoaderInfo.addEventListener(Event.COMPLETE,imageRetrieved)
         loader.loadBytes(imgData);         
      }
      
      public function retrieveProfileImage(id:Number):void {
         var size:Object = this._context.call( "bbm_ane_bbmsp_image_get_profile_data_size", id );
         var imgData:ByteArray = new ByteArray();
         imgData.length = Number(size);
         this._context.call( "bbm_ane_bbmsp_image_get_profile_data", id, imgData );
         var loader:Loader = new Loader();
         loader.contentLoaderInfo.addEventListener(Event.COMPLETE,profileImageRetrieved)
         loader.loadBytes(imgData);         
      }
                               
      private function imageRetrieved(e:Event):void {
         var loaderInfo:LoaderInfo = e.target as LoaderInfo;
         var loader:Loader = loaderInfo.loader;     
		 
		   var bitmapData:BitmapData = new BitmapData(loaderInfo.width, loaderInfo.height, true, 0xFFFFFF);
		   bitmapData.draw(loaderInfo.loader);
		 
		   var bmp:Bitmap = new Bitmap(bitmapData);
		   dispatchEvent( new ANEImageEvent(ANEImageEvent.IMAGE_RETRIEVED,0,"",bmp) );
      }
      
      private function profileImageRetrieved(e:Event):void {
         var loaderInfo:LoaderInfo = e.target as LoaderInfo;
         var loader:Loader = loaderInfo.loader;     
         
         var bitmapData:BitmapData = new BitmapData(loaderInfo.width, loaderInfo.height, true, 0xFFFFFF);
         bitmapData.draw(loaderInfo.loader);
         
         var bmp:Bitmap = new Bitmap(bitmapData);
         dispatchEvent( new ANEImageEvent(ANEImageEvent.PROF_IMAGE_RETRIEVED,0,"",bmp) );
      }
      
      //======================================= MISCELLANEOUS FUNCTIONS =================================
      private function imageStatusUpdate(e:StatusEvent):void {
         if(e.code == ANEImageEvent.IMAGE_LOADED ){
            var id:Number = Number(e.level);
            var file:String = findByID(id).name;
            dispatchEvent( new ANEImageEvent(ANEImageEvent.IMAGE_LOADED,id,file) );            
         }
      }
      
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
      
      private function findByID(id:Number):ImageTracker {
         var found:ImageTracker;
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

