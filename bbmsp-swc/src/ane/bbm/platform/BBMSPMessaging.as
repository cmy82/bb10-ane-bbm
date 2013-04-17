package ane.bbm.platform {
   import flash.external.ExtensionContext;
   
   public class BBMSPMessaging {
      
      private var _context:ExtensionContext;
      
      public function BBMSPMessaging(context:ExtensionContext) {
         _context = context;
      }
      
      /**
       * Send a download invitation for a BBM contact to download your app.
       * @param none
       * @return none
       */
      public function sendDownloadInvite( ):void {
         var result:Object = this._context.call( "bbm_ane_bbmsp_send_download_invitation", null );         
      }
   }
}