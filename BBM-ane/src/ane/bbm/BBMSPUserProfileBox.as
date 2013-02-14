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
      
      //======================================= INFO FUNCTIONS ==================================
      //Position 0 in the itemlist is the oldest item in the Profile Box
      
      /**
       * Retrieves a string representing the text shown in the specified profile box item.
       * @param item the position in the profile box itemlist to check
       * @return The text string displayed in the profile box item.
       * 
       */      
      public function getText(item:Number):String {
         var result:Object = result = _context.call( "bbm_ane_bbmsp_user_profile_box_item_get_text", item );
         return result.toString();
      }
      
      /**
       * Retrieves a string representing the cookie associated with the specified profile box item.
       * @param item the position in the profile box itemlist to check
       * @return The cookie text string associated with the profile box item.
       * 
       */      
      public function getCookie(item:Number):String {
         var result:Object = result = _context.call( "bbm_ane_bbmsp_user_profile_box_item_get_cookie", item );
         return result.toString();
      }
      
      /**
       * Retrieves the icon id for the image associated with the specified profile box item.
       * @param item the position in the profile box itemlist to check
       * @return Icon ID of associated image or -1 if no image is associated.
       * 
       */      
      public function getIconID(item:Number):Number {
         var result:Object = result = _context.call( "bbm_ane_bbmsp_user_profile_box_item_get_icon_id", item );
         return Number(result);
      }
      
      /**
       * Retrieves the number of items in the profile box
       * @param none
       * @return Number of items in profile box
       * 
       */      
      public function getProfileBoxSize():Number {
         var result:Object = _context.call( "bbm_ane_bbmsp_user_profile_box_items_size" );
         return Number(result);
      }           
      
      public function retrieveIcon(iconID:Number):Boolean {
         var result:Object = _context.call( "bbm_ane_bbmsp_user_profile_box_retrieve_icon", iconID );
         if( Number(result) < 0 ) return false;
         return true;
      }
      
      public function getImageIDFromIconID(iconID:Number):Number {
         var result:Object = _context.call( "bbm_ane_get_image_by_icon_id", iconID );
         return Number(result);
      }
      
      //======================================= UPDATE FUNCTIONS =================================
      public function addItem(text:String,cookie:String,icnID:Number = -1):void {
         var result:Object = _context.call( "bbm_ane_bbmsp_user_profile_box_add_item",icnID,text,text.length,cookie,cookie.length);         
      }
      
      public function removeItem(item:Number):void {
         _context.call( "bbm_ane_bbmsp_user_profile_box_itemlist_remove_at", item );
      }
      
      public function removeAllItems():void {
         _context.call( "bbm_ane_bbmsp_user_profile_box_remove_all_items" );
      }
           
      public function registerIcon(imageID:Number,iconID:Number):void {
         var result:Object = _context.call( "bbm_ane_bbmsp_user_profile_box_register_icon", iconID, imageID );                  
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
            dispatchEvent( new ANEUserProfileBoxEvent(ANEUserProfileBoxEvent.PROFILE_BOX_ICN_REG,Number(e.level)) );
         } else if(e.code == ANEUserProfileBoxEvent.PROFILE_BOX_ICN_RET){
            dispatchEvent( new ANEUserProfileBoxEvent(ANEUserProfileBoxEvent.PROFILE_BOX_ICN_RET,Number(e.level)) );
         }
      }
   }
}