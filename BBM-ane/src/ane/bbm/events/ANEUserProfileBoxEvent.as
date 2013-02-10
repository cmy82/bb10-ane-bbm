package ane.bbm.events {
   import flash.events.Event;
   
   public class ANEUserProfileBoxEvent extends Event {
      public static const PROFILE_BOX_LOADED:String  = "ANEProfileBoxLoaded";
      public static const PROFILE_BOX_UPDATED:String = "ANEProfileBoxUpdated";
      public static const PROFILE_BOX_ITM_ADD:String = "ANEProfileBoxItemAdded";
      public static const PROFILE_BOX_ITM_DEL:String = "ANEProfileBoxItemDeleted";
      public static const PROFILE_BOX_ICN_REG:String = "ANEProfileBoxIconRegistered";
      public static const PROFILE_BOX_ICN_RET:String = "ANEProfileBoxIconRetrieved";
      
      public function ANEUserProfileBoxEvent(type:String, bubbles:Boolean=false, cancelable:Boolean=false) {
         super(type, bubbles, cancelable);
      }
   }
}