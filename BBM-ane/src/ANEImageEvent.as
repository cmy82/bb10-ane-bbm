package {
   import flash.events.Event;   
   
   public class ANEImageEvent extends Event {
      public static const IMAGE_LOADED:String = "ANEImageLoaded";
      private var _id:Number = NaN;
      public function ANEImageEvent(type:String, value:Number, bubbles:Boolean=false, cancelable:Boolean=false) {
         super(type, bubbles, cancelable);
         _id = value;
      }
      public function get id():Number {
         return _id;
      }
   }
}