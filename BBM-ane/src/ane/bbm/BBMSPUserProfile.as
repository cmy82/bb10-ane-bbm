package ane.bbm {
   import ane.bbm.events.ANEUserProfileEvent;
   
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
   
   public class BBMSPUserProfile extends EventDispatcher {
      
      private var _context:ExtensionContext;
      
      public function BBMSPUserProfile(context:ExtensionContext) {
         _context = context;
         _context.addEventListener( StatusEvent.STATUS, profileStatusUpdate );
      }
      
      public function getUserPersonalMessage():String {
         var result:Object = this._context.call( "bbm_ane_bbmsp_profile_get_personal_message" );
         return result.toString();
      }
      
      public function getUserStatus():String {
         var result:Object = this._context.call( "bbm_ane_bbmsp_profile_get_status" );
         return result.toString();
      }
      
      public function getUserStatusMessage():String {
         var result:Object = this._context.call( "bbm_ane_bbmsp_profile_get_status_message" );
         return result.toString();
      }
      
      public function getUserDisplayName():String {
         var result:Object = this._context.call( "bbm_ane_bbmsp_profile_get_display_name" );
         return result.toString();
      }
      
      public function getUserPPID():String {
         var result:Object = this._context.call( "bbm_ane_bbmsp_profile_get_ppid" );
         return result.toString();
      }
      
      public function getUserHandle():String {
         var result:Object = this._context.call( "bbm_ane_bbmsp_profile_get_handle" );
         return result.toString();
      }
      
      public function getUserAppVersion():String {
         var result:Object = this._context.call( "bbm_ane_bbmsp_profile_get_app_version" );
         return result.toString();
      }
      
      public function getUserProfilePicture():Number {
         var result:Object = this._context.call( "bbm_ane_bbmsp_profile_get_display_picture" );
         return Number(result);
      }
      
      //======================================= UPDATE FUNCTIONS =================================
      public function setDisplayPicture(id:Number):void {
         this._context.call( "bbm_ane_bbmsp_profile_set_user_display_picture", id );
      }
      
      public function setUserProfileDisplayPicture(id:Number):void {
         this._context.call( "bbm_ane_bbmsp_set_user_profile_display_picture", id );            
      }
      
      public function setStatusMessage(status:int,msg:String):void {
         trace("Set profile status msg: "+msg);
         if( msg == null ) msg = " ";
         this._context.call( "bbm_ane_bbmsp_profile_set_status", status, msg, msg.length );
      }
      
      public function setUserProfileStatus(status:int,msg:String):void {
         trace("Set user status msg: "+msg);
         if( msg == null ) msg = " ";
         this._context.call( "bbm_ane_bbmsp_set_user_profile_status", status, msg, msg.length );
      }
      
      public function setPersonalMessage(msg:String):void {
         trace("Set profile personal msg: "+msg);
         if( msg == null ) msg = " ";
         this._context.call( "bbm_ane_bbmsp_profile_set_personal_message", msg, msg.length );
      }
      
      public function setUserProfilePersonalMessage(msg:String):void {
         trace("Set user personal msg: "+msg);
         if( msg == null ) msg = " ";
         this._context.call( "bbm_ane_bbmsp_set_user_profile_personal_message", msg, msg.length );
      }
      
      //======================================= MISCELLANEOUS FUNCTIONS =================================
      private function profileStatusUpdate(e:StatusEvent):void {
         if(e.code == ANEUserProfileEvent.PROFILE_LOADED){
            dispatchEvent( new ANEUserProfileEvent(ANEUserProfileEvent.PROFILE_LOADED) );            
         } else if(e.code == ANEUserProfileEvent.PROFILE_UPDATED){
            dispatchEvent( new ANEUserProfileEvent(ANEUserProfileEvent.PROFILE_UPDATED) );
         }
      }
   }
}