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
         trace(icnID);
         var result:Object = _context.call( "bbm_ane_bbmsp_user_profile_box_add_item",icnID,text,text.length,cookie,cookie.length);
         trace("result: "+result.toString());
      }
      
      public function registerIcon(imageID:Number,iconID:Number):void {
         trace("registering icon "+imageID+" with id "+iconID);
         var result:Object = _context.call( "bbm_ane_bbmsp_user_profile_box_register_icon", iconID, imageID );
         trace("Result: "+result.toString());
         if( Number(result) < 0 ){
            result = _context.call( "bbm_ane_bbmsp_user_profile_box_retrieve_icon", iconID );
            trace("Result: "+result.toString());
            trace();
         }
      }
      
      //======================================= MISCELLANEOUS FUNCTIONS =================================
      private function profileStatusUpdate(e:StatusEvent):void {
         if(e.code == ANEUserProfileBoxEvent.PROFILE_BOX_LOADED){
            dispatchEvent( new ANEUserProfileBoxEvent(ANEUserProfileBoxEvent.PROFILE_BOX_LOADED) );            
         } else if(e.code == ANEUserProfileBoxEvent.PROFILE_BOX_UPDATED){
            dispatchEvent( new ANEUserProfileBoxEvent(ANEUserProfileBoxEvent.PROFILE_BOX_UPDATED) );
         } else if(e.code == ANEUserProfileBoxEvent.PROFILE_BOX_ITM_ADD){
            dispatchEvent( new ANEUserProfileBoxEvent(ANEUserProfileBoxEvent.PROFILE_BOX_ITM_ADD) );
         } else if(e.code == ANEUserProfileBoxEvent.PROFILE_BOX_ITM_DEL){
            dispatchEvent( new ANEUserProfileBoxEvent(ANEUserProfileBoxEvent.PROFILE_BOX_ITM_DEL) );
         } else if(e.code == ANEUserProfileBoxEvent.PROFILE_BOX_ICN_REG){
            dispatchEvent( new ANEUserProfileBoxEvent(ANEUserProfileBoxEvent.PROFILE_BOX_ICN_REG) );
         } else if(e.code == ANEUserProfileBoxEvent.PROFILE_BOX_ICN_RET){
            dispatchEvent( new ANEUserProfileBoxEvent(ANEUserProfileBoxEvent.PROFILE_BOX_ICN_RET) );
         }
      }
   }
}