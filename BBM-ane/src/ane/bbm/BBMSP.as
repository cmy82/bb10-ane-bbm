package ane.bbm {
   import flash.external.ExtensionContext;

   public class BBMSP {
      
      private var _context:ExtensionContext;
      
      public function BBMSP(context:ExtensionContext) {
         _context = context;
      }
      
      /**
       * Starts the registration process with the BBM Social Platform to allow the application access to the service.
       * @param uuid The UUID to use to register the application with the BBM Social Platform.
       * @return none
       * 
       */      
      public function startRegistration(uuid:String):void {
         _context.call( "bbm_ane_startRegistration", uuid );                  
      }
      
      /**
       * Retrieves the domain number in use by the BBM Social Platform.
       * @param none
       * @return The registered domain id of the BBM Social Platform on the device.
       * 
       */      
      public function getDomain():Number {
         var out:Object = _context.call( "bbm_ane_bbmsp_get_domain", null );
         return Number(out);
      }
      
      /**
       * Retrieves a number representing the version of the BBM Social Platform on the device.
       * @param none
       * @return The version number of the BBM Social Platform.
       * 
       */      
      public function getVersion():Number {
         var out:Object = _context.call( "bbm_ane_bbmsp_get_version", null );
         return Number(out);
      }
   }
}

