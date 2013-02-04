package ane.bbm {
   import flash.external.ExtensionContext;

   public class BBMSP {
      
      private var _context:ExtensionContext;
      
      public function BBMSP(context:ExtensionContext) {
         _context = context;
      }
      
      public function startRegistration(uuid:String):void {
         _context.call( "bbm_ane_startRegistration", uuid );                  
      }
      
      public function getDomain():Number {
         var out:Object = _context.call( "bbm_ane_bbmsp_get_domain", null );
         return Number(out);
      }
      
      public function getVersion():Number {
         var out:Object = _context.call( "bbm_ane_bbmsp_get_version", null );
         return Number(out);
      }
   }
}

