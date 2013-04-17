package ane.bbm.events {
   import flash.display.Bitmap;
   import flash.events.Event;
   
   public class ANEImageEvent extends Event {
      public static const IMAGE_LOADED:String    = "ANEImageLoaded";
      public static const IMAGE_RETRIEVED:String = "ANEImageRetrieved";
      public static const PROF_IMAGE_LOADED:String    = "ANEProfileImageLoaded";
      public static const PROF_IMAGE_RETRIEVED:String = "ANEProfileImageRetrieved";
      
      private var _id:Number = NaN;
      private var _filename:String = "";
      private var _image:Bitmap = null;
      public function ANEImageEvent(type:String, value:Number, file:String = "", img:Bitmap = null, 
                                    bubbles:Boolean=false, cancelable:Boolean=false) {
         super(type, bubbles, cancelable);
         _id = value;
         _filename = file;
		 _image = img;
      }
      public function get id():Number {
         return _id;
      }
      public function get filename():String {
         return _filename;
      }
      public function get image():Bitmap {
         return _image;
      }
   }
}