package {
   import flash.display.Bitmap;
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
         _context.addEventListener( StatusEvent.STATUS, imageComplete );
         _imageList = new Vector.<ImageTracker>();
      }

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
            var image2:ImageTracker = find(imageFile.name);
            dispatchEvent( new ANEImageEvent(ANEImageEvent.IMAGE_LOADED,image2.id) );
            trace("Displateched loaded event");
         }
      }
           
      private function imageComplete(e:StatusEvent):void {
         dispatchEvent( new ANEImageEvent(ANEImageEvent.IMAGE_LOADED,Number(e.level)) );
         trace("ANE Image loaded event occured. Image ID: "+e.level);
      }
      
      private function search(img:String):Boolean {
         if( _imageList.length == 0 ) return false;
         var found:Boolean = false;
         for( var i:int = 0; i<_imageList.length; i++ ){
            if( _imageList[i].name == img ){
               found = true;
               trace("Image "+_imageList[i].name+" has previously been loaded");
               break;
            }
         }
         return found;
      }
      
      private function find(img:String):ImageTracker {
         var found:ImageTracker;
         for( var i:int = 0; i<_imageList.length; i++ ){
            if( _imageList[i].name == img ){
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

