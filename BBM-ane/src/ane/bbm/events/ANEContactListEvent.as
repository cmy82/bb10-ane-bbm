package ane.bbm.events {
   import flash.events.Event;
   
   public class ANEContactListEvent extends Event {
      public static const CONTACTLIST_LOADED:String    = "ANEContactListLoaded";
      
      public function ANEContactListEvent(type:String, bubbles:Boolean=false, cancelable:Boolean=false) {
         super(type, bubbles, cancelable);
      }
   }
}