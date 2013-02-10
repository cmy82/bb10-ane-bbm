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
   
   [SWF(frameRate="30")]
   public class BBM_ANE_Test extends Sprite {
      
      var testLbl:Label;
      var usrTag:Label;
      var imgHolder:Sprite;
      var bbmExtension:BBMAne;   
      private var profLoading:Boolean = true;      
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
         
         var ldProfBxItmBtn:LabelButton = new LabelButton();
         ldProfBxItmBtn.label = "Load Profile Box Item";
         ldProfBxItmBtn.width = 450;
         ldProfBxItmBtn.x = (stage.stageWidth - 450) / 2;
         ldProfBxItmBtn.y = inviteBtn.y+125;
         
         //testBtn.addEventListener(MouseEvent.CLICK,testUUID);
         regBtn.addEventListener(MouseEvent.CLICK,callRegistration);
         inviteBtn.addEventListener(MouseEvent.CLICK,callSendDownload);
         //imgHolder.addEventListener(MouseEvent.CLICK,callLoadImage);    
         ldProfBtn.addEventListener(MouseEvent.CLICK,displayProfile);
         ldProfBxItmBtn.addEventListener(MouseEvent.CLICK,displayProfileBox);
         
         addChild(testBtn);
         addChild(regBtn);
         addChild(inviteBtn);
         addChild(testLbl);
         addChild(ldProfBtn);
         addChild(imgHolder);
         addChild(tagHolder);
         addChild(ldProfBxItmBtn);
         
         stage.nativeWindow.visible = true;
         NativeApplication.nativeApplication.systemIdleMode = SystemIdleMode.KEEP_AWAKE;
         
         this.addEventListener(Event.ADDED_TO_STAGE,init);
         this.addEventListener(Event.ENTER_FRAME,testUUID);
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
      
      public function testUUID(e:Event):void {         
         var out:Object = bbmExtension.checkStatus();
         testLbl.text = out.toString();
         if( stage.numChildren == 1 ) profLoading = true;
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
      
      public function displayProfile(e:MouseEvent):void {
         var profile:ProfileCard = new ProfileCard(bbmExtension,new Bitmap(Bitmap(imgHolder.getChildAt(0)).bitmapData.clone()));
         this.stage.addChild(profile);
      }
      
      public function displayProfileBox(e:MouseEvent):void {
         profLoading = false;
         var profileBox:ProfileBoxCard = new ProfileBoxCard(bbmExtension);
         this.stage.addChild(profileBox);
      }
                
      private function retrieveImage(e:ANEImageEvent):void {         
         if( profLoading ){
            trace("[ANE Test] Calling retrieve image for loaded profile image "+e.id);
            var id:Number = e.id;
            imgId = id;
            bbmExtension.bbmspImages.retrieveImage(id);
         }
      }
      
      private function displayImage(e:ANEImageEvent):void {
         var pic:Bitmap = e.image;
         pic.bitmapData = ImageResizer.bilinearIterative(pic.bitmapData,225,250,ResizeMath.METHOD_PAN_AND_SCAN);
         if( imgHolder.numChildren > 0 ) imgHolder.removeChildAt(0);
         imgHolder.addChild( new Bitmap(pic.bitmapData.clone()) );
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
import qnx.fuse.ui.listClasses.ScrollDirection;
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
   private var imgID:Number;
   private var pb1:Container, pb2:Container, pb3:Container;
   
   public function ProfileCard(bbm:BBMAne,pic:Bitmap) {
      _bbm = bbm;
      _pic = pic;
      this.addEventListener(Event.ADDED_TO_STAGE,init);  
      this.addEventListener(Event.REMOVED_FROM_STAGE,cleanup);      
      _bbm.bbmspImages.addEventListener(ANEImageEvent.PROF_IMAGE_RETRIEVED,displayImage);   
      _bbm.bbmspImages.addEventListener(ANEImageEvent.IMAGE_LOADED,retrieveProfileImage);
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
      profile.setActualSize(this.width-30,this.height-115);
      
      profile.scrollDirection = ScrollDirection.VERTICAL;
      profile.vScrollVisible = true;
      addChild(profile);
      
      img = new Sprite();
      img.x = this.width - 255;
      img.y = 0;
      img.graphics.beginFill(0xAAAAAA,0.8);
      img.graphics.drawRoundRect(0,0,120,120,20);
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
      
      pb1 = new Container();
      pb1.height = 160;
      pb1.width = this.width-40;      
      pb1.y = 520;
      pb1.x = 5;
      
      pb2 = new Container();
      pb2.height = 160;
      pb2.width = this.width-40;      
      pb2.y = 680;
      pb2.x = 5;
      
      pb3 = new Container();
      pb3.height = 160;
      pb3.width = this.width-40;      
      pb3.y = 840;
      pb3.x = 5;
      
      for( var i:int = 0; i< _bbm.bbmspUserProfileBox.getProfileBoxSize(); ++i ){
         var pb:Container = null;
         if( i==0 ){ profile.addChild(pb1); pb = pb1; }
         if( i==1 ){ profile.addChild(pb2); pb = pb2; }
         if( i==2 ){ profile.addChild(pb3); pb = pb3; }
         
         var txt:Label = new Label();
         txt.width = pb.width - 160;
         txt.height = 80;
         txt.x = 160;
         txt.y = 0;
         txt.text = _bbm.bbmspUserProfileBox.getText(i);
         pb.addChild(txt);
         
         var cke:Label = new Label();
         cke.width = pb.width-160;
         cke.height = 80;
         cke.x = 160;
         cke.y = 80;
         cke.text = _bbm.bbmspUserProfileBox.getCookie(i);
         pb.addChild(cke);
         
      }
      
      
      this.y = stage.stageHeight/4;      
   }
   
   private function cleanup(e:Event):void {
      this.removeEventListener(Event.REMOVED_FROM_STAGE,cleanup);
      _bbm.bbmspImages.removeEventListener(ANEImageEvent.PROF_IMAGE_RETRIEVED,displayImage);   
      _bbm.bbmspImages.removeEventListener(ANEImageEvent.IMAGE_LOADED,retrieveProfileImage);
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
      trace("calling display image after image sent to ANE for caching: "+e.id);
      var pic:Bitmap = e.image;
      pic.bitmapData = ImageResizer.bilinearIterative(pic.bitmapData,225,250,ResizeMath.METHOD_PAN_AND_SCAN);
      img.addChild( new Bitmap(pic.bitmapData.clone()) );      
      trace("Calling set user profile image");
      _bbm.bbmspUserProfile.setUserProfileDisplayPicture(imgID);      
   }
   
   private function retrieveProfileImage(e:ANEImageEvent):void {
      trace("[ProfileCard] Retrieve profile image called for id "+e.id);
      var id:Number = e.id;
      imgID = id;
      _bbm.bbmspImages.retrieveProfileImage(id);      
   }
}


class ProfileBoxCard extends Sprite {
   private var _bbm:BBMAne;
   private var _pic1:Bitmap; //FFVII - 1001
   private var _pic2:Bitmap; //ZELDA - 1002
   private var _pic3:Bitmap; //HALO  - 1003
   private var txtMsg:TextArea;
   private var cookieMsg:TextArea; 
   private var choice:SegmentedControl;
   
   public function ProfileBoxCard(bbm:BBMAne) {
      _bbm = bbm;
      this.addEventListener(Event.ADDED_TO_STAGE,init);  
      this.addEventListener(Event.REMOVED_FROM_STAGE,cleanup);      
      _bbm.bbmspImages.addEventListener(ANEImageEvent.IMAGE_LOADED,registerIcon);
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
      cls.label = "ADD";
      cls.width = 250;
      cls.height = 80;
      cls.x = (stage.stageWidth - 550) / 2;
      cls.y = this.height-100;
      cls.addEventListener(MouseEvent.CLICK,add);
      addChild(cls);
      
      var cncl:LabelButton = new LabelButton();
      cncl.label = "CANCEL";
      cncl.width = 250;
      cncl.height = 80;
      cncl.x = cls.x+300;
      cncl.y = this.height-100;
      cncl.addEventListener(MouseEvent.CLICK,cancel);
      addChild(cncl);
      
      var profile:Container = new Container();
      profile.height = this.height-125;
      profile.width = this.width-30;      
      profile.y = 15;
      profile.x = 15;
      addChild(profile);
              
      var icnWdth:Number = (this.width-40)/4;
      
      _pic1 = new Resources.FFVII;
      _pic1.bitmapData = ImageResizer.bilinearIterative(_pic1.bitmapData,icnWdth-10,150,ResizeMath.METHOD_PAN_AND_SCAN);
      _pic1.x = 5;
      _pic1.y = 15;
      profile.addChild(_pic1);
      
      _pic2 = new Resources.ZELDA;
      _pic2.bitmapData = ImageResizer.bilinearIterative(_pic2.bitmapData,icnWdth-10,150,ResizeMath.METHOD_PAN_AND_SCAN);
      _pic2.x = (icnWdth)+5;
      _pic2.y = 15;
      profile.addChild(_pic2);
      
      _pic3 = new Resources.HALO;
      _pic3.bitmapData = ImageResizer.bilinearIterative(_pic3.bitmapData,icnWdth-10,150,ResizeMath.METHOD_PAN_AND_SCAN);
      _pic3.x = (icnWdth*2)+5;;
      _pic3.y = 15;
      profile.addChild(_pic3);
      
      choice = new SegmentedControl();
      choice.width = this.width-40;
      choice.height = 80;
      choice.x = 5;
      choice.y = 175;
      choice.addItem( {label: "Icon 1"} );
      choice.addItem( {label: "Icon 2"} );
      choice.addItem( {label: "Icon 3"} );
      choice.addItem( {label: "No Icon"} );
      choice.selectedIndex = 3;
      profile.addChild(choice);
      
      txtMsg = new TextArea();
      txtMsg.x = 0;
      txtMsg.y = 265;
      txtMsg.width = this.width - 30;
      txtMsg.height = 120;
      txtMsg.maxLines = 3;
      txtMsg.maxChars = 160;
      txtMsg.text = "";
      txtMsg.prompt = "Profile Box Text";
      profile.addChild(txtMsg);
      
      cookieMsg = new TextArea();
      cookieMsg.x = 0;
      cookieMsg.y = 395;
      cookieMsg.width = this.width - 30;
      cookieMsg.height = 120;
      cookieMsg.maxLines = 3;
      cookieMsg.maxChars = 160;
      cookieMsg.text = "";
      cookieMsg.prompt = "Profile Box Cookie";
      profile.addChild(cookieMsg);
      
      this.y = stage.stageHeight/4;      
      
      _bbm.bbmspImages.loadImageFromResource("ffvii_icon.jpg");
      _bbm.bbmspImages.loadImageFromResource("zelda_icon.png");
      _bbm.bbmspImages.loadImageFromResource("halo_icon.png");
   }
   
   private function cleanup(e:Event):void {
      _bbm.bbmspImages.removeEventListener(ANEImageEvent.IMAGE_LOADED,registerIcon);
   }
   
   private function registerIcon(e:ANEImageEvent):void {
      trace(e.filename);
      if( e.filename == "ffvii_icon.jpg" )
         _bbm.bbmspUserProfileBox.registerIcon(e.id,1001);
      if( e.filename == "zelda_icon.png" )
         _bbm.bbmspUserProfileBox.registerIcon(e.id,1002);
      if( e.filename == "halo_icon.png" )
         _bbm.bbmspUserProfileBox.registerIcon(e.id,1003);
   }
   
   private function cancel(e:MouseEvent):void {                       
      stage.removeChild(this);
   }
   
   private function add(e:MouseEvent):void {
      if( choice.selectedIndex == 0 ){
         _bbm.bbmspUserProfileBox.addItem(txtMsg.text,cookieMsg.text,1001);
      } else
      if( choice.selectedIndex == 1 ){
         _bbm.bbmspUserProfileBox.addItem(txtMsg.text,cookieMsg.text,1002);
      } else
      if( choice.selectedIndex == 2 ){
         _bbm.bbmspUserProfileBox.addItem(txtMsg.text,cookieMsg.text,1003);
      } else
      if( choice.selectedIndex == 3 ){
         _bbm.bbmspUserProfileBox.addItem(txtMsg.text,cookieMsg.text);
      }
      stage.removeChild(this);
   }
}

