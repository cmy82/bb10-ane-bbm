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
      
      public function BBMSPUtil(context:ExtensionContext) {
         _context = context;
         _context.addEventListener( StatusEvent.STATUS, imageComplete );
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
         
         this._context.call( "bbm_ane_bbmsp_image_create", imageBytes );
      }
           
      private function imageComplete(e:StatusEvent):void {
         dispatchEvent( new ANEImageEvent(ANEImageEvent.IMAGE_LOADED,Number(e.level)) );
         trace("ANE Image loaded event occured. Image ID: "+e.level);
      }
   }
}