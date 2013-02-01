package ane.bbm {
   import flash.external.ExtensionContext;
   
   public class BBMMessaging {
      
      private var _context:ExtensionContext;
      
      public function BBMMessaging(context:ExtensionContext) {
         _context = context;
      }
      
      public function sendDownloadInvite( ):void {
         trace("Calling send download invitation");
         var result:Object = this._context.call( "bbm_ane_bbmsp_send_download_invitation", null );
         trace("result was "+result.toString());
      }
   }
}