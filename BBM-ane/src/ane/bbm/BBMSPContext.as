package ane.bbm {
   import flash.external.ExtensionContext;

   public class BBMSPContext {
      private var _context:ExtensionContext;
      
      public function BBMSPContext(context:ExtensionContext) {
         _context = context;
      }
      
      public function isAccessAllowed():Boolean {
         var result:Object = this._context.call( "bbm_ane_bbmsp_is_access_allowed" );
         if( Number(result) == 1 ) return true;
         return false;
      }
      
      public function canSendDownloadInvites():Boolean {
         var result:Object = this._context.call( "bbm_ane_bbmsp_can_send_bbm_invite" );
         if( Number(result) == 1 ) return true;
         return false;
      }
      
      public function canShowProfileBox():Boolean {
         var result:Object = this._context.call( "bbm_ane_bbmsp_can_show_profile_box" );
         if( Number(result) == 1 ) return true;
         return false;
      }
   }
}