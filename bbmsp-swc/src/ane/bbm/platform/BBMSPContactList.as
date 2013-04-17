package ane.bbm.platform {
   import ane.bbm.events.ANEImageEvent;   
   import flash.events.Event;
   import flash.events.EventDispatcher;
   import flash.events.StatusEvent;
   import flash.external.ExtensionContext;
   
   public class BBMSPContactList extends EventDispatcher {
      private var _context:ExtensionContext;
      
      public function BBMSPContactList(context:ExtensionContext) {
         _context = context;        
      }
   }
}