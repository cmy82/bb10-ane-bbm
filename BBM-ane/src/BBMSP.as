package {
   import flash.external.ExtensionContext;

   public class BBMSP {
      
      private var _context:ExtensionContext;
      
      public function BBMSP(context:ExtensionContext) {
         _context = context;
      }
      
      public function startRegistration(uuid:String):void {
         trace("[BBMSP.as] bbm_ane_startRegistration called with value "+uuid); 
         _context.call( "bbm_ane_startRegistration", uuid );
         trace("Returned from call to bbm_ane_startRegistration");         
      }
      
      public function getDomain():Number {
         trace("[BBMSP.as] bbm_ane_bbmsp_get_domain"); 
         var out:Object = _context.call( "bbm_ane_bbmsp_get_domain", null );
         trace("Returned from call to bbm_ane_bbmsp_get_domain");
         trace( out.toString() );
         return Number(out);
      }
      
      public function getVersion():Number {
         trace("[BBMSP.as] bbm_ane_bbmsp_get_version"); 
         var out:Object = _context.call( "bbm_ane_bbmsp_get_version", null );
         trace("Returned from call to bbm_ane_bbmsp_get_version");
         trace( out.toString() );
         return Number(out);
      }
   }
}

