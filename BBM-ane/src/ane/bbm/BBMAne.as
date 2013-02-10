package ane.bbm {   
   import flash.external.ExtensionContext;

   public class BBMAne {      
      private var context:ExtensionContext;
      public var bbmspRegistration:BBMSP;
      public var bbmspContext:BBMSPContext;
      public var bbmspInvite:BBMMessaging;
      public var bbmspImages:BBMSPUtil;
      public var bbmspUserProfile:BBMSPUserProfile;
      public var bbmspUserProfileBox:BBMSPUserProfileBox;
      
      public function BBMAne() {
         context = ExtensionContext.createExtensionContext( "BSLLC.library.ane.BBM", null );
         bbmspRegistration   = new BBMSP(context);
         bbmspContext        = new BBMSPContext(context);
         bbmspInvite         = new BBMMessaging(context);
         bbmspImages         = new BBMSPUtil(context);
         bbmspUserProfile    = new BBMSPUserProfile(context);
         bbmspUserProfileBox = new BBMSPUserProfileBox(context);
      }      
      
      public function checkStatus():Object {
         var result:Object = this.context.call( "checkStatus" );         
         return result;
      }
   }
}