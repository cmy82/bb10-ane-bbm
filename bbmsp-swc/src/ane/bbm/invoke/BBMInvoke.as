package ane.bbm.invoke {
   import flash.events.Event;
   import flash.external.ExtensionContext;
   import flash.filesystem.File;
   import flash.net.FileFilter;
   import flash.utils.ByteArray;
   
   public class BBMInvoke {
      private var _context:ExtensionContext;
      
      public function BBMInvoke(context:ExtensionContext) {
         _context = context;
      }
      
      /**
       * Open a BBM chat window 
       * Use this function when the BBM Contact PIN is known, otherwise no BBM Card will display.
       * @param String Contacts PIN 
       * @return none
       * 
       */
      public function startChat(contact:String=null):void {         
         if(contact!=null) this._context.call( "bbm_ane_invoke_start_chat", contact );         
         else this._context.call( "bbm_ane_invoke_start_chat" );
      }
      
      /**
       * Send a message over BBM
       * Opens contact picker to allow user to choose contact
       * @param String Message to send
       * @return none
       * 
       */
      public function sendMessage(message:String=null):void {         
         if(message!=null) this._context.call( "bbm_ane_invoke_send_message", message, message.length );
         else this._context.call( "bbm_ane_invoke_send_message" );         
      }
      
      /**
       * Send an image over BBM
       * Opens contact picker to allow user to choose contact
       * @param String Image to send
       * @return none
       * 
       */
      public function sendImage(image:String=null):void {         
         if(image!=null) this._context.call( "bbm_ane_invoke_send_image", image, image.length );
         else {
            var dir:File = File.userDirectory.resolvePath( 'shared/photos' );
            var imgFilter:FileFilter = new FileFilter("Images", "*.png;*.bmp;*.jpg;*.jpeg");
            dir.addEventListener(Event.SELECT,imageSelected);
            dir.browseForOpen("Choose Image",[imgFilter]);
            //this._context.call( "bbm_ane_invoke_send_message" );         
         }
      }
      
      /**
       * Invite user to become a BBM Contact
       * @param String PIN to invite
       * @return none
       * 
       */
      public function invite(pin:String=null):void {         
         if(pin!=null) this._context.call( "bbm_ane_invoke_invite_to_bbm", pin );         
         else this._context.call( "bbm_ane_invite_to_bbm" );
      }
      
      //============================================ PRIVATE FUNCTIONS ==============================================//
      private function imageSelected(e:Event):void {
         var imageFile:File = e.target as File;
         trace(imageFile.nativePath)
         trace(imageFile.name);
         trace(imageFile.extension);
         this._context.call( "bbm_ane_invoke_send_image", imageFile.nativePath, imageFile.nativePath.length );
      }
   }
}