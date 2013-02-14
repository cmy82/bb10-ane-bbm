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
      
      /**
       * Retrieve users personal message.
       * @param none 
       * @return Current personal message on users BBM profile.
       * 
       */
      public function getUserPersonalMessage():String {
         var result:Object = this._context.call( "bbm_ane_bbmsp_profile_get_personal_message" );
         return result.toString();
      }
      
      /**
       * Retrieve users current BBM status (Busy or Available).
       * @param none 
       * @return Users current status.
       * 
       */
      public function getUserStatus():String {
         var result:Object = this._context.call( "bbm_ane_bbmsp_profile_get_status" );
         return result.toString();
      }
      
      /**
       * Retrieve users current BBM custom status message.
       * @param none 
       * @return Users current custom status message.
       * 
       */
      public function getUserStatusMessage():String {
         var result:Object = this._context.call( "bbm_ane_bbmsp_profile_get_status_message" );
         return result.toString();
      }
      
      /**
       * Retrieve users BBM display name.
       * @param none 
       * @return Users display name.
       * 
       */
      public function getUserDisplayName():String {
         var result:Object = this._context.call( "bbm_ane_bbmsp_profile_get_display_name" );
         return result.toString();
      }
      
      /**
       * Retrieve users PPID. The PPID is a device independent string attached to a users BBID.
       * @param none 
       * @return Users PPID.
       * 
       */
      public function getUserPPID():String {
         var result:Object = this._context.call( "bbm_ane_bbmsp_profile_get_ppid" );
         return result.toString();
      }
      
      /**
       * Retrieve users handle. The handle is a device dependent string used for backwards compatibility with BlackBerry 
       * 7 and earlier.
       * @param none 
       * @return Users PPID.
       * 
       */
      public function getUserHandle():String {
         var result:Object = this._context.call( "bbm_ane_bbmsp_profile_get_handle" );
         return result.toString();
      }
      
      /**
       * Retrieve current installed version of app.
       * @param none 
       * @return Users current app version.
       * 
       */
      public function getUserAppVersion():String {
         var result:Object = this._context.call( "bbm_ane_bbmsp_profile_get_app_version" );
         return result.toString();
      }
      
      /**
       * Retrieve users current BBM profile pic ID (ID number as returned by one of the image load functions).
       * @param none 
       * @return ID number of current profile pic.
       * 
       */
      public function getUserProfilePicture():Number {
         var result:Object = this._context.call( "bbm_ane_bbmsp_profile_get_display_picture" );
         return Number(result);
      }
      
      //======================================= UPDATE FUNCTIONS =================================
           
      /**
       * Set the display picture on the cached user profile object. Does not update the display picture
       * for the BBM Profile; to update that use the setUserProfileDisplayPicture. 
       * @param id The ID of the picture in the ANE image cache as returned by one of the load functions.
       * @return none
       * 
       */
      public function setDisplayPicture(id:Number):void {
         this._context.call( "bbm_ane_bbmsp_profile_set_user_display_picture", id );
      }
      
      /**
       * Set the display picture on the actual user profile object. Prompts the user to accept the changes
       * to the profile. 
       * @param id The ID of the picture in the ANE image cache as returned by one of the load functions.
       * @return none
       * 
       */
      public function setUserProfileDisplayPicture(id:Number):void {
         this._context.call( "bbm_ane_bbmsp_set_user_profile_display_picture", id );            
      }
      
      /**
       * Set the status and an optional status message on the cached user profile object. Does not update
       * the actual BBM Profile; to update that use the setUserProfileStatusMessage. 
       * @param id The user status (1 for Busy or 0 for Available)
       * @param msg The custom status message to post on the profile
       * @return none
       * 
       */
      public function setStatusMessage(status:int,msg:String = ""):void {
         if( msg == null ) msg = " ";
         this._context.call( "bbm_ane_bbmsp_profile_set_status", status, msg, msg.length );
      }
      
      /**
       * Set the status and an optional status message on the actual user profile object. Displays a 
       * prompt requesting the user accept the changes.
       * @param id The user status (1 for Busy or 0 for Available)
       * @param msg The custom status message to post on the profile
       * @return none
       * 
       */
      public function setUserProfileStatus(status:int,msg:String):void {
         if( msg == null ) msg = " ";
         this._context.call( "bbm_ane_bbmsp_set_user_profile_status", status, msg, msg.length );
      }
      
      /**
       * Set the personal message on the cached user profile object. Does not update
       * the actual BBM Profile; to update that use the setUserPersonalMessage. 
       * @param msg The personal message to post on the profile
       * @return none
       * 
       */
      public function setPersonalMessage(msg:String):void {
         if( msg == null ) msg = " ";
         this._context.call( "bbm_ane_bbmsp_profile_set_personal_message", msg, msg.length );
      }
      
      /**
       * Set the personal message on the actual user profile object. Displays a 
       * prompt requesting the user accept the changes.
       * @param msg The personal message to post on the profile
       * @return none
       * 
       */
      public function setUserProfilePersonalMessage(msg:String):void {
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