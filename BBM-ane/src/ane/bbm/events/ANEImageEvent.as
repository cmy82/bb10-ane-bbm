package ane.bbm.events {
   import flash.events.Event;   
   
   public class ANEImageEvent extends Event {
      public static const IMAGE_LOADED:String    = "ANEImageLoaded";
      public static const IMAGE_RETRIEVED:String = "ANEImageRetrieved";
      
      private var _id:Number = NaN;
      private var _filename:String = "";
      public function ANEImageEvent(type:String, value:Number, file:String = "", bubbles:Boolean=false, cancelable:Boolean=false) {
         super(type, bubbles, cancelable);
         _id = value;
         _filename = file;
      }
      public function get id():Number {
         return _id;
      }
      public function get filename():String {
         return _filename;
      }
   }
}