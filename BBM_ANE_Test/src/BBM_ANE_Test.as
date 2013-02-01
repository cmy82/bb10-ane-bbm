  package {
   import ane.bbm.events.ANEImageEvent;
   import ane.bbm.BBMAne;
   
   
   import flash.desktop.NativeApplication;
   import flash.desktop.SystemIdleMode;
   import flash.display.Sprite;
   import flash.display.StageAlign;
   import flash.display.StageScaleMode;
   import flash.events.Event;
   import flash.events.MouseEvent;
   
   import qnx.fuse.ui.buttons.LabelButton;
   import qnx.fuse.ui.text.Label;
   
   public class BBM_ANE_Test extends Sprite {
      
      var testLbl:Label;
      var imgHolder:Sprite;
      var bbmExtension:BBMAne;
      //var helloAIR:HelloAIR;    
      
      private var uuid:String = "99b1d456-46e6-4f75-aca0-dacb4b878987"; //7-
      
      public function BBM_ANE_Test() {
         super();
         
         // support autoOrients
         stage.align = StageAlign.TOP_LEFT;
         stage.scaleMode = StageScaleMode.NO_SCALE;
         
         testLbl = new Label();
         //testLbl.width = 600;
         testLbl.width = stage.stageWidth - 215;
         testLbl.height = 500;
         //testLbl.x = stage.stageWidth / 2 - testLbl.width / 2;
         testLbl.x = 5;
         testLbl.y = 50;
         testLbl.maxLines = 12;
         testLbl.minLines = 8;
         
         imgHolder = new Sprite();
         imgHolder.x = stage.stageWidth - 205;
         imgHolder.y = 25;
         imgHolder.graphics.beginFill(0xAAAAAA,0.8);
         imgHolder.graphics.drawRoundRect(0,0,200,250,20);
         imgHolder.graphics.endFill();
                 
         var testBtn:LabelButton = new LabelButton();
         testBtn.label = "Status";
         testBtn.width = 250;
         testBtn.x = (stage.stageWidth - 550) / 2;
         testBtn.y = testLbl.y + 550; 
      
         var regBtn:LabelButton = new LabelButton();
         regBtn.label = "Reg UUID";
         regBtn.width = 250;
         regBtn.x = testBtn.x + 300;
         regBtn.y = testBtn.y;
         
         var inviteBtn:LabelButton = new LabelButton();
         inviteBtn.label = "Dwnld Invite";
         inviteBtn.width = 250;
         inviteBtn.x = (stage.stageWidth - 550) / 2;
         inviteBtn.y = testBtn.y + 125;
         
         var ldImgBtn:LabelButton = new LabelButton();
         ldImgBtn.label = "Load IMG";
         ldImgBtn.width = 250;
         ldImgBtn.x = inviteBtn.x + 300;
         ldImgBtn.y = inviteBtn.y;         
         
         testBtn.addEventListener(MouseEvent.CLICK,testUUID);
         regBtn.addEventListener(MouseEvent.CLICK,callRegistration);
         inviteBtn.addEventListener(MouseEvent.CLICK,callSendDownload);
         ldImgBtn.addEventListener(MouseEvent.CLICK,callLoadImage);         
         
         addChild(testBtn);
         addChild(regBtn);
         addChild(inviteBtn);
         addChild(testLbl);
         addChild(ldImgBtn);
         addChild(imgHolder);
         
         stage.nativeWindow.visible = true;
         NativeApplication.nativeApplication.systemIdleMode = SystemIdleMode.KEEP_AWAKE;
         
         this.addEventListener(Event.ADDED_TO_STAGE,init);
      }
      
      private function init(e:Event):void {
         this.removeEventListener(Event.ADDED_TO_STAGE,init);
         if( bbmExtension == null ){
            bbmExtension = new BBMAne();
            bbmExtension.bbmspImages.addEventListener(ANEImageEvent.IMAGE_LOADED,retrieveImage);
            bbmExtension.bbmspImages.addEventListener(ANEImageEvent.IMAGE_RETRIEVED,displayImage);
         }
      }
      
      public function testUUID(e:MouseEvent):void {         
         var out:Object = bbmExtension.checkStatus();
         testLbl.text = out.toString();
      }
      public function callRegistration(e:MouseEvent):void {
         bbmExtension.bbmspRegistration.startRegistration(uuid);
      }
      public function callSendDownload(e:MouseEvent):void {
         bbmExtension.bbmspInvite.sendDownloadInvite();
      }
      public function callLoadImage(e:MouseEvent):void {
         bbmExtension.bbmspImages.loadImage();
      }
      
      private function retrieveImage(e:ANEImageEvent):void {
         trace("[Test.as] Image loaded. Now trying to retrieve image back from ANE: ["+e.id+"] "+e.filename);
         var id:Number = e.id;
         bbmExtension.bbmspImages.retrieveImage(id);
      }
      
      private function displayImage(e:ANEImageEvent):void {
         trace("[Test.as] Image retrieved and trying to display.");
      }
   }      
}




