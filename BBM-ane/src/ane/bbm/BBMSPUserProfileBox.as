package ane.bbm {
   import ane.bbm.events.ANEUserProfileBoxEvent;
   
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
   
   public class BBMSPUserProfileBox extends EventDispatcher {
      
      private var _context:ExtensionContext;
      
      public function BBMSPUserProfileBox(context:ExtensionContext) {
         _context = context;
         _context.addEventListener( StatusEvent.STATUS, profileStatusUpdate );
      }
      
      
      
      //======================================= UPDATE FUNCTIONS =================================
      public function addItem(text:String,cookie:String,icnID:Number = -1):void {
         trace("Calling add profile box item");
         trace(text);
         trace(cookie);
         var result:Object = _context.call( "bbm_ane_bbmsp_user_profile_box_add_item",icnID,text,text.length,cookie,cookie.length);
         trace("result: "+result.toString());
      }
      
      //======================================= MISCELLANEOUS FUNCTIONS =================================
      private function profileStatusUpdate(e:StatusEvent):void {
         if(e.code == ANEUserProfileBoxEvent.PROFILE_BOX_LOADED){
            dispatchEvent( new ANEUserProfileBoxEvent(ANEUserProfileBoxEvent.PROFILE_BOX_LOADED) );            
         } else if(e.code == ANEUserProfileBoxEvent.PROFILE_BOX_UPDATED){
            dispatchEvent( new ANEUserProfileBoxEvent(ANEUserProfileBoxEvent.PROFILE_BOX_UPDATED) );
         }
      }
   }
}