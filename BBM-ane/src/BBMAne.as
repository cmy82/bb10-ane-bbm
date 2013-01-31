package {   
   import flash.external.ExtensionContext;

   public class BBMAne {
      
      private var context:ExtensionContext;
      public var bbmspRegistration:BBMSP;
      public var bbmspInvite:BBMMessaging;
      public var bbmspImages:BBMSPUtil;
      
      public function BBMAne() {
         context = ExtensionContext.createExtensionContext( "BSLLC.library.ane.BBM", null );
         bbmspRegistration = new BBMSP(context);
         bbmspInvite = new BBMMessaging(context);
         bbmspImages = new BBMSPUtil(context);
      }      
      
      public function checkStatus():Object {
         trace("calling checkStatus");
         var result:Object = this.context.call( "sayHello" );
         trace("checkStatus returned");
         return result;
      }
   }
}