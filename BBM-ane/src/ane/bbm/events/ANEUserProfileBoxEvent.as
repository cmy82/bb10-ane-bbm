package ane.bbm.events {
   import flash.events.Event;
   
   public class ANEUserProfileBoxEvent extends Event {
      public static const PROFILE_BOX_LOADED:String    = "ANEProfileBoxLoaded";
      public static const PROFILE_BOX_UPDATED:String    = "ANEProfileBoxUpdated";
      
      public function ANEUserProfileBoxEvent(type:String, bubbles:Boolean=false, cancelable:Boolean=false) {
         super(type, bubbles, cancelable);
      }
   }
}