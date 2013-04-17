package ane.bbm.events {
   import flash.events.Event;
   
   public class ANEUserProfileEvent extends Event {
      public static const PROFILE_LOADED:String    = "ANEProfileLoaded";
      public static const PROFILE_UPDATED:String    = "ANEProfileUpdated";
      
      public function ANEUserProfileEvent(type:String, bubbles:Boolean=false, cancelable:Boolean=false) {
         super(type, bubbles, cancelable);
      }
   }
}