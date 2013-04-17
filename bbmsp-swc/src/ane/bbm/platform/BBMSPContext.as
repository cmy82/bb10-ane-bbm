package ane.bbm.platform {
   import flash.external.ExtensionContext;

   public class BBMSPContext {
      private var _context:ExtensionContext;
      
      public function BBMSPContext(context:ExtensionContext) {
         _context = context;
      }
      
      /**
       * Determine whether user has been granted access to the BBM Socail Platform.
       * @param none 
       * @return True or False whether the application has access.
       * 
       */
      public function isAccessAllowed():Boolean {
         var result:Object = this._context.call( "bbm_ane_bbmsp_is_access_allowed" );
         if( Number(result) == 1 ) return true;
         return false;
      }
      
      /**
       * Determine whether user can send download invites to BBM contacts.
       * @param none 
       * @return True or False whether the application has access to send invites;
       * 
       */
      public function canSendDownloadInvites():Boolean {
         var result:Object = this._context.call( "bbm_ane_bbmsp_can_send_bbm_invite" );
         if( Number(result) == 1 ) return true;
         return false;
      }
      
      /**
       * Determine whether user can access the Profile Box.
       * @param none 
       * @return True or False whether the user can access the profile box.
       * 
       */
      public function canShowProfileBox():Boolean {
         var result:Object = this._context.call( "bbm_ane_bbmsp_can_show_profile_box" );
         if( Number(result) == 1 ) return true;
         return false;
      }
   }
}