# bb10-ane-bbm

**My BBMID for my DevAlpha is CMY2 and the PIN is 29D9370E.**

This is an AIR Native Extension (ANE) to allow BlackBerry 10 AIR apps access to the BBM Social Platform (BBMSP). This repository contains all of the needed files including a pre-built copy of the ANE, and the source (if you want to adjust or tinker with it). There are three applications included in the bundle. There is a c/c++ library that contains the native code (bbmsp-ane), the corresponding AIR library (BBM-ane), and a test application to show how the features work (BBM_ANE_Test). Only the two library files are needed to gain access to the BBMSP; just import them into your workspace and add the references to your project(s). To simplify things, each file in the AIR library is matched to a file of the same name in the c/c++ library with corresponding matching functions. 

## The following features are enabled through this ANE:
* Registering with the BBMSP
* Sending download invites for your application
* Updating user profile (picture, status, status message)
* Updating user profile box

## The following features are not enabled/complete and why:
* Sending messages (this feature is not enabled in the BBMSP currently)
* User contacts (since you are unable to send messages or files the only useful purpose for this I saw was to get a list of contacts that have you app installed)


## Usage/Caveats:

Each time you start your application you need to register with your uuid. The ANE checks this value and uses it to start the other needed threads. Once you register, the user profile and profile box items are automatically loaded if your application has access to those features. When loading images, the ANE loads an original sized copy of the image as well as a smaller copy which is used for profile/profile box images. The profile and profile box image functions have the word profile in them. Because of the way the listeners are dispatched from the ANE, each one of the custom listeners recieves each event. So if you change the code or add your own custom listener, keep this in mind. Also with the way the callbacks are used, each listener gets a copy of each registered event; so you will need to track which image id is returned so that you know which image to update, if you are displaying more than one image at a time _(ie. in the sample app when the profile card is displayed, both the main view and the profile card receive notice that the image was loaded. So if you only want to update the profile card, you will need to track the image id that is returned after you load the image so that you only update the profile card when you recieve notice that that id was retrieved)_.

## The Test App
The status button has no purpose as the view was changed to update automatically. You will need to click the register button first before doing anything else. Once you have registered, you can click any of the buttons in any order. When viewing the profile, if you make any changes to the text or status, it will prompt you to accept the changes and _**WILL ACTUALLY CHANGE**_ your BBM profile. When adding a profile box item, move the slider under the icon to use and add the text (and cookie if desired) and select ok. Note that you are not prompted to accept changes to the profile box.

## Disclaimer
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
