                                                                                                                                                    package {
   import ane.bbm.BBMAne;
   import ane.bbm.events.ANEImageEvent;
   import ane.bbm.events.ANEUserProfileEvent;
   
   import flash.desktop.NativeApplication;
   import flash.desktop.SystemIdleMode;
   import flash.display.Bitmap;
   import flash.display.Sprite;
   import flash.display.StageAlign;
   import flash.display.StageScaleMode;
   import flash.events.Event;
   import flash.events.MouseEvent;
   
   import qnx.fuse.ui.buttons.LabelButton;
   import qnx.fuse.ui.text.Label;
   import qnx.fuse.ui.text.TextAlign;
   import qnx.fuse.ui.text.TextFormat;
   
   public class BBM_ANE_Test extends Sprite {
      
      var testLbl:Label;
      var usrTag:Label;
      var imgHolder:Sprite;
      var bbmExtension:BBMAne;   
      var usrSelected:Boolean = false;
      
      var imgId:int;
      
      private var uuid:String = "99b1d456-46e6-4f75-aca0-dacb4b878927"; //87-88-89-27
      
      public function BBM_ANE_Test() {
         super();
         
         // support autoOrients
         stage.align = StageAlign.TOP_LEFT;
         stage.scaleMode = StageScaleMode.NO_SCALE;
         
         testLbl = new Label();
         testLbl.width = stage.stageWidth - 240;
         testLbl.height = 425;
         testLbl.x = 5;
         testLbl.y = 50;
         testLbl.maxLines = 12;
         testLbl.minLines = 8;
         
         imgHolder = new Sprite();
         imgHolder.x = stage.stageWidth - 230;
         imgHolder.y = 25;
         imgHolder.graphics.beginFill(0xAAAAAA,0.8);
         imgHolder.graphics.drawRoundRect(0,0,225,250,20);
         imgHolder.graphics.endFill();
         
         var tagHolder:Sprite = new Sprite();
         tagHolder.x = stage.stageWidth - 230;
         tagHolder.y = imgHolder.y + 265;
         tagHolder.graphics.beginFill(0xAAAAAA,0.8);
         tagHolder.graphics.drawRoundRect(0,0,225,50,20);
         tagHolder.graphics.endFill();
                 
         var txtFmt:TextFormat = new TextFormat();
         txtFmt.bold = true;
         txtFmt.size = 28;
         txtFmt.align = TextAlign.CENTER;         
         
         usrTag = new Label();
         usrTag.width = 205;
         usrTag.height = 40;
         usrTag.x = 10;
         usrTag.y = 5;
         usrTag.text = "User Handle";
         usrTag.format = txtFmt;
         tagHolder.addChild(usrTag);
         
         var testBtn:LabelButton = new LabelButton();
         testBtn.label = "Status";
         testBtn.width = 250;
         testBtn.x = (stage.stageWidth - 550) / 2;
         testBtn.y = testLbl.y + 450; 
      
         var regBtn:LabelButton = new LabelButton();
         regBtn.label = "Reg UUID";
         regBtn.width = 250;
         regBtn.x = testBtn.x + 300;
         regBtn.y = testBtn.y;
         
         var inviteBtn:LabelButton = new LabelButton();
         inviteBtn.label = "Send Invite";
         inviteBtn.width = 250;
         inviteBtn.x = (stage.stageWidth - 550) / 2;
         inviteBtn.y = testBtn.y + 125;
         
         var ldProfBtn:LabelButton = new LabelButton();
         ldProfBtn.label = "View Profile";
         ldProfBtn.width = 250;
         ldProfBtn.x = inviteBtn.x + 300;
         ldProfBtn.y = inviteBtn.y;         
         
         testBtn.addEventListener(MouseEvent.CLICK,testUUID);
         regBtn.addEventListener(MouseEvent.CLICK,callRegistration);
         inviteBtn.addEventListener(MouseEvent.CLICK,callSendDownload);
         //imgHolder.addEventListener(MouseEvent.CLICK,callLoadImage);    
         ldProfBtn.addEventListener(MouseEvent.CLICK,displayProfile);
         
         addChild(testBtn);
         addChild(regBtn);
         addChild(inviteBtn);
         addChild(testLbl);
         addChild(ldProfBtn);
         addChild(imgHolder);
         addChild(tagHolder);
         
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
            bbmExtension.bbmspUserProfile.addEventListener(ANEUserProfileEvent.PROFILE_LOADED,updateHandle);
            bbmExtension.bbmspUserProfile.addEventListener(ANEUserProfileEvent.PROFILE_UPDATED,updateHandle);
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
         usrSelected = true;
         bbmExtension.bbmspImages.loadImage();
      }
      
      public function displayProfile(e:MouseEvent):void {
         var profile:ProfileCard = new ProfileCard(bbmExtension,new Bitmap(Bitmap(imgHolder.getChildAt(0)).bitmapData.clone()));
         this.stage.addChild(profile);
      }
      
      private function retrieveImage(e:ANEImageEvent):void {
         var id:Number = e.id;
         imgId = id;
         bbmExtension.bbmspImages.retrieveImage(id);
      }
      
      private function displayImage(e:ANEImageEvent):void {
         var pic:Bitmap = e.image;
         pic.bitmapData = ImageResizer.bilinearIterative(pic.bitmapData,225,250,ResizeMath.METHOD_PAN_AND_SCAN);
         if( imgHolder.numChildren > 0 ) imgHolder.removeChildAt(0);
         imgHolder.addChild( new Bitmap(pic.bitmapData.clone()) );
         
         //if( usrSelected ){
         //   bbmExtension.bbmspUserProfile.setUserProfileDisplayPicture(e.id);
         //   usrSelected = false;
         //}
      }
      
      private function updateHandle(e:ANEUserProfileEvent):void {
         usrTag.text = bbmExtension.bbmspUserProfile.getUserDisplayName();
         var id:Number = bbmExtension.bbmspUserProfile.getUserProfilePicture();
         bbmExtension.bbmspImages.retrieveImage(id);
      }
   }      
}


import ane.bbm.BBMAne;
import ane.bbm.events.ANEImageEvent;

import flash.display.Bitmap;
import flash.display.Sprite;
import flash.events.Event;
import flash.events.EventDispatcher;
import flash.events.MouseEvent;
import flash.events.TouchEvent;
import flash.external.ExtensionContext;
import flash.text.TextFormatAlign;

import qnx.fuse.ui.buttons.LabelButton;
import qnx.fuse.ui.buttons.SegmentedControl;
import qnx.fuse.ui.buttons.ToggleSwitch;
import qnx.fuse.ui.core.Container;
import qnx.fuse.ui.skins.SkinAssets;
import qnx.fuse.ui.text.Label;
import qnx.fuse.ui.text.TextArea;
import qnx.fuse.ui.text.TextFormat;
import qnx.fuse.ui.text.TextInput;



class ProfileCard extends Sprite {
   private var _bbm:BBMAne;
   private var img:Sprite;
   private var _pic:Bitmap;
   private var statMsg:TextArea;
   private var persMsg:TextArea;
   private var status:ToggleSwitch;
   
   public function ProfileCard(bbm:BBMAne,pic:Bitmap) {
      _bbm = bbm;
      _pic = pic;
      this.addEventListener(Event.ADDED_TO_STAGE,init);  
      this.addEventListener(Event.REMOVED_FROM_STAGE,cleanup);
      _bbm.bbmspImages.addEventListener(ANEImageEvent.IMAGE_RETRIEVED,displayImage);
   }
   
   public function init(e:Event):void {
      this.removeEventListener(Event.ADDED_TO_STAGE,init);
      
      this.graphics.beginFill(0x999999,1.0);
      this.graphics.drawRoundRect(0,0,stage.stageWidth,stage.stageHeight/2,30);
      this.graphics.endFill();
      this.graphics.beginFill(0xDCDCDC,1.0);
      this.graphics.drawRoundRect(10,10,stage.stageWidth-20,(stage.stageHeight/2)-20,30);
      this.graphics.endFill();
      
      var cls:LabelButton = new LabelButton();
      cls.label = "CLOSE";
      cls.width = 250;
      cls.height = 80;
      cls.x = (stage.stageWidth - 250) / 2;
      cls.y = this.height-100;
      cls.addEventListener(MouseEvent.CLICK,closeProfile);
      addChild(cls);
      
      var profile:Container = new Container();
      profile.height = this.height-125;
      profile.width = this.width-30;      
      profile.y = 15;
      profile.x = 15;
      addChild(profile);
      
      img = new Sprite();
      img.x = this.width - 255;
      img.y = 0;
      img.graphics.beginFill(0xAAAAAA,0.8);
      img.graphics.drawRoundRect(0,0,225,250,20);
      img.graphics.endFill();
      img.addEventListener(MouseEvent.CLICK,callLoadImage);
      img.addChild(_pic);
      profile.addChild(img);
      
      var name:TextInput = new TextInput();
      name.editable = true;
      name.x = 0;
      name.y = 0;
      name.text = _bbm.bbmspUserProfile.getUserDisplayName();
      name.prompt = "Display Name";
      name.width = this.width - 265;
      name.height = 80;
      profile.addChild(name);
                
      status = new ToggleSwitch();
      status.defaultLabel = "BUSY";
      status.selectedLabel = "AVAILABLE";
      status.x = 0;
      status.y = 90;
      status.width = this.width - 265;
      status.height = 80;
      profile.addChild(status);
      if( _bbm.bbmspUserProfile.getUserStatus() != "BUSY" ) status.selected = true;
      
      var version:Label = new Label();
      version.x = 0;
      version.y = 180;
      version.width = this.width-265;
      version.height = 80;
      version.text = "App Version: "+_bbm.bbmspUserProfile.getUserAppVersion();
      var tf:TextFormat = new TextFormat();
      tf.bold = true;
      tf.align = TextFormatAlign.CENTER;
      version.format = tf;
      profile.addChild(version);
      
      statMsg = new TextArea();
      statMsg.x = 0;
      statMsg.y = 265;
      statMsg.width = this.width - 30;
      statMsg.height = 120;
      statMsg.maxLines = 3;
      statMsg.maxChars = 160;
      statMsg.text = _bbm.bbmspUserProfile.getUserStatusMessage();
      statMsg.prompt = "Status Message";
      profile.addChild(statMsg);
      
      persMsg = new TextArea();
      persMsg.x = 0;
      persMsg.y = 395;
      persMsg.width = this.width - 30;
      persMsg.height = 120;
      persMsg.maxLines = 3;
      persMsg.maxChars = 160;
      persMsg.text = _bbm.bbmspUserProfile.getUserPersonalMessage();
      persMsg.prompt = "Personal Message";
      profile.addChild(persMsg);
      
      this.y = stage.stageHeight/4;      
   }
   
   private function cleanup(e:Event){
      _bbm.bbmspImages.removeEventListener(ANEImageEvent.IMAGE_RETRIEVED,displayImage);   
   }
   
   private function closeProfile(e:MouseEvent):void {
      if( _bbm.bbmspUserProfile.getUserPersonalMessage() != persMsg.text ){
         _bbm.bbmspUserProfile.setUserProfilePersonalMessage( persMsg.text );
      }
      
      var cStat:String = "BUSY";
      if( status.selected ) cStat = "AVAILABLE";
      if( (_bbm.bbmspUserProfile.getUserStatusMessage() != statMsg.text) ||
          (_bbm.bbmspUserProfile.getUserStatus() != cStat) ){
         trace(cStat);         
         var code:int = 1;
         if( status.selected ) code = 0;
         _bbm.bbmspUserProfile.setUserProfileStatus( code, statMsg.text );
      }
      
      stage.removeChild(this);
   }
   
   public function callLoadImage(e:MouseEvent):void {
      _bbm.bbmspImages.loadImage();
   }
   
   private function displayImage(e:ANEImageEvent):void {
      var pic:Bitmap = e.image;
      pic.bitmapData = ImageResizer.bilinearIterative(pic.bitmapData,225,250,ResizeMath.METHOD_PAN_AND_SCAN);
      img.addChild( new Bitmap(pic.bitmapData.clone()) );      
      _bbm.bbmspUserProfile.setUserProfileDisplayPicture(e.id);      
   }
}

