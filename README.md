Epic Online Services Godot (WIP)
=========================================
<img alt="Project Logo" src="./_media/logo.png" height="150">

### Unofficial Epic Online Services wrapper for Godot Engine 4.2 (includes demo project)

<img alt="Godot3" src="https://img.shields.io/badge/-Godot 4.2-478CBF?style=for-the-badge&logo=godotengine&logoWidth=20&logoColor=white" />&nbsp;&nbsp;&nbsp;<img alt="Epic Online Services 1.16.1" src="https://img.shields.io/badge/-Epic Online Services 1.16.1-313131?style=for-the-badge&logo=epic-games&logoWidth=20&logoColor=white" />

> Supports Windows x64, Linux x64 and Android

> Disclaimer: This project is NOT affiliated with Epic Games Inc or Godot Engine. It doesn't endorse Epic Online Services. This project and sample Godot scenes are provided solely for educational purposes and may or may not comply with Epic Games' Design Guidelines, if you plan to release a game make sure you read the [Guidelines](https://dev.epicgames.com/docs/services/en-US/EpicAccountServices/DesignGuidelines/index.html) and any other steps needed to release a public game like asking for user consent, option to delete user data, website with privacy policy and license, etc.


#### The [main](https://github.com/3ddelano/epic-online-services-godot/tree/main) branch is for Godot 4.2
#### The [godot3-mono](https://github.com/3ddelano/epic-online-services-godot/tree/godot3-mono) branch is for Godot 3 Mono (C#) (un maintained)


## Support Development

#### Making this project took a lot of time and effort, reading the Epic Online Services documentation countless times and testing each method in Godot. I would really appreciate if you could support the project in any way.

<a href="https://www.buymeacoffee.com/3ddelano" target="_blank"><img height="41" width="174" src="https://cdn.buymeacoffee.com/buttons/v2/default-red.png" alt="Buy Me A Coffee" width="150" ></a>
<br>
<a href="https://github.com/sponsors/3ddelano" target="_blank">Github Sponsor</a>

#### Want to support in other ways? Contact me on Discord: `@3ddelano`

Join the Discord server for discussing suggestions or bugs: [3ddelano Cafe](https://discord.gg/FZY9TqW)


## [View Current Project Status](#current-project-status)

## [Demo Video (Coming Soon)](#)

## [Documentation (Coming Soon)](#)

## Screenshots

- Windows
   <img src="./_media/windows_auth_success.png">

- Android
   <img src="./_media/android_auth_success.jpg">


## How does it work

This project uses GDExtension to wrap the Epic Online Services C SDK so that it can be easily used in Godot using GDScript, C#, etc with similar class hierarchy and static type support. It makes use of signals for sending events like user login, logout, achievement unlock, etc.


Installation
--------------

This is a regular plugin for `Godot 4.2`. To install the plugin follow the steps below:

1. Goto the Releases section and download the [latest release](https://github.com/3ddelano/epic-online-services-godot/releases/latest)
2. Extract the zip file and copy the `addons/epic-online-services-godot` folder into the `res://addons/` folder of your project. If the `res://addons` does not exist, create it.
3. In the Godot editor, goto `Project->Project Settings->Plugins` and enable the `Epic Online Services Godot 4.2` plugin.
4. You can now use the plugin. Head to the [Documentation](#) for more information on how to use the plugin. Use the below starter script.
    ```GDScript
    # In main script
    extends Node

    func _ready() -> void:
        # Initialize the SDK
        var init_options = EOS.Platform.InitializeOptions.new()
        init_options.product_name = "PRODUCT_NAME_HERE"
        init_options.product_version = "PRODUCT_VERSION_HERE"

        var init_result := EOS.Platform.PlatformInterface.initialize(init_options)
        if init_result != EOS.Result.Success:
            print("Failed to initialize EOS SDK: ", EOS.result_str(init_result))
            return

        # Create platform
        var create_options = EOS.Platform.CreateOptions.new()
        create_options.product_id = "PRODUCT_ID_HERE"
        create_options.sandbox_id = "SANDBOX_ID_HERE"
        create_options.deployment_id = "DEPLOYMENT_ID_HERE"
        create_options.client_id = "CLIENT_ID_HERE"
        create_options.client_secret = "CLIENT_SECRET_HERE"
        create_options.encryption_key = "ENCRYPTION_KEY_HERE"

        # Enable Social Overlay on Windows
        if OS.get_name() == "Windows":
            create_options.flags = EOS.Platform.PlatformFlags.WindowsEnableOverlayOpengl

        var create_success := EOS.Platform.PlatformInterface.create(create_options)
        if not create_success:
            print("Failed to create EOS Platform")
            return

        # Setup Logs from EOS
        EOS.get_instance().logging_interface_callback.connect(_on_logging_interface_callback)
        var res := EOS.Logging.set_log_level(EOS.Logging.LogCategory.AllCategories, EOS.Logging.LogLevel.Info)
        if res != EOS.Result.Success:
            print("Failed to set log level: ", EOS.result_str(res))

        _anonymous_login()


    func _on_logging_interface_callback(msg) -> void:
        msg = EOS.Logging.LogMessage.from(msg) as EOS.Logging.LogMessage
        print("SDK %s | %s" % [msg.category, msg.message])


    func _anonymous_login() -> void:
        # Login using Device ID (no user interaction/credentials required)
        var opts = EOS.Connect.CreateDeviceIdOptions.new()
        opts.device_model = OS.get_name() + " " + OS.get_model_name()
        EOS.Connect.ConnectInterface.create_device_id(opts)
        await EOS.get_instance().connect_interface_create_device_id_callback

        var credentials = EOS.Connect.Credentials.new()
        credentials.token = null
        credentials.type = EOS.ExternalCredentialType.DeviceidAccessToken

        var login_options = EOS.Connect.LoginOptions.new()
        login_options.credentials = credentials
        var user_login_info = EOS.Connect.UserLoginInfo.new()
        user_login_info.display_name = "Anon User"
        login_options.user_login_info = user_login_info
        EOS.Connect.ConnectInterface.login(login_options)
        EOS.get_instance().connect_interface_login_callback.connect(_on_connect_interface_login_callback)

    func _on_connect_interface_login_callback(data: Dictionary) -> void:
        if not data.success:
            print("Login failed")
            EOS.print_result(data)
            return

        print_rich("[b]Login successfull[/b]: local_user_id=", data.local_user_id)
    ```


Development Setup
--------------

#### Pre-requisites
- Godot Engine 4.2 (Get it here [Godot Engine Download](https://godotengine.org/download/))
- Epic Online Services C SDK (Download from [Epic Developer Portal](https://dev.epicgames.com/portal/sdk))
- Make sure you have accepted the Terms and Conditions for [Epic Online Services](https://www.epicgames.com/site/en-US/tos?lang=en-US)
- A product registered with Epic Games Services (Make one for free [Epic Developer Portal](https://dev.epicgames.com/portal))

To develop this plugin, follow the below steps:
1. Download/clone the repository.

2. Extract the `EOS C SDK` zip downloaded from Epic Games, rename it to `eos-sdk` and paste it in the `thirdparty/` folder. Refer to the below folder structure.
   
   <img src="./_media/eos_folder_structure.png">

3. Build the GDExtension plugin in debug mode (With debug symbols)
   ```shell
   # In root folder
   scons platform=<platform> target=template_debug dev_build=yes
   ```
   Eg. `scons platform=windows target=template_debug dev_build=yes`

4. Build the GDExtension plugin for release (Optimized)
   ```shell
   # In root folder
   scons platform=windows target=template_release
   ```

5. The built GDExtension library will be in the `res://addons/epic-online-services-godot/bin/` folder of the sample project.


### How to run the sample project?
> The sample Godot project is located in the **Sample** folder

1. Clone/Download the repo.
2. Download the [latest release](https://github.com/3ddelano/epic-online-services-godot/releases/latest) from the Releases section and replace the existing `/addons/epic-online-services-godot` with the one from the Release (this includes the built shared libraries).
3. Copy your credentials (`Product Id`, `Sandbox Id`, `Deployment Id`, `Client Id`, `Client Secret`) of your Epic Games "Product" from the Epic Games Dev Portal and paste them in `Main.gd` script in the relevant sections. The encryption key is a random 64 character long string. These credentials need to be kept as private as possible. One way is to make sure to encrypt all scripts when exporting the final game. (See [Compiling with script key encryption](https://docs.godotengine.org/en/stable/development/compiling/compiling_with_script_encryption_key.html))

4. Configure your Product on the EOS Dev Portal with the following configuration:
  - In the `Client Policies` section in `Product Settings`, for the Client policy type choose `Custom policy`, enable the `User is required` and enable every features and action except `Connect` (Trusted Server Required). This will allow the sample to access the different services provided by Epic Online Services. In your actual game, the client policy is important and you should give minimal permissions to features.
  - In the `Permissions` section of `Epic Account Services`, enable all three: `Basic Profile`, `Online Presence` and `Friends`. 
  - (Optional if you want some pre-made achievements)
  In the `Achievements` section in `Game Services`, use the `Bulk Import` option and import the `HelloProduct.zip` file located at `res://HelloProduct.zip`

### Bootstrapping Godot executable with Epic Online Services
If you want to use the `Account Portal` login option in Epic Online Services, you need to bootstrap the Godot/Game executable as needed by `EOS-SDK 1.15` and greater. See [Redistributable Installer](https://dev.epicgames.com/docs/services/en-US/EpicAccountServices/Crossplayacrossplatforms/RedistributableInstaller/index.html)

A sample of the generated `.ini` file for the Godot Editor is shown below (during game development):
```
ApplicationPath=Godot_v4.2.0-stable_win64.exe
WorkingDirectory=
WaitForExit=0
NoOperation=0
```
Follow the instructions in [Running the service for local development](https://dev.epicgames.com/docs/services/en-US/EpicAccountServices/Crossplayacrossplatforms/RedistributableInstaller/index.html#runningtheserviceforlocaldevelopment) and:
- During game development
  
  Bootstrap the Godot Editor executable (eg. `Godot_v4.2.0-stable_win64.exe`) to test the `Account Portal` login
- After exporting the game
  
  Bootstrap the exported game executable (eg. `My Amazing Game.exe`)


## Exporting for Android

### Pre-requisites

- EOS Android SDK (Download from [Epic Developer Portal](https://dev.epicgames.com/portal))

1. Extract the `EOS Android SDK` zip downloaded from Epic Games, rename it to `eos-sdk` and paste it in the `thirdparty/` folder.

2. Setup the `Android Build Template` in your Godot project by following the tutorial [Gradle builds for Andriod](https://docs.godotengine.org/en/stable/tutorials/export/android_gradle_build.html). This will create an android project in `res://android/build`.

3. Now with reference to the tutorial [Add the EOS SDK to Your Android Studio Project](https://dev.epicgames.com/docs/epic-online-services/platforms/android#4-add-the-eos-sdk-to-your-android-studio-project), perform the following steps.

4. In the `res://android/build/build.gradle` file, add the following lines after the implementations in the `dependencies` section.
	
	Before
	```
	dependencies {
    	implementation libraries.kotlinStdLib
    	implementation libraries.androidxFragment
		... other code
	```

	After
	```
	dependencies {
		implementation libraries.kotlinStdLib
		implementation libraries.androidxFragment
		
		// EOS SDK dependencies
		implementation 'androidx.appcompat:appcompat:1.0.0'
		implementation 'androidx.constraintlayout:constraintlayout:1.1.3'
		implementation 'androidx.security:security-crypto:1.0.0'
		implementation 'androidx.browser:browser:1.0.0'
		implementation files('../../../thirdparty/eos-sdk/SDK/Bin/Android/static-stdc++/aar/eos-sdk.aar')

		...other code
	```

4. In the `res://android/build/build.gradle` file, add the following lines after the `defaultConfig` in the `android` section.
	
	Before
	```
	android {

		... other code

		defaultConfig {
			... other code
		
			// Feel free to modify the application id to your own.
			applicationId getExportPackageName()
			versionCode getExportVersionCode()
			versionName getExportVersionName()
			minSdkVersion getExportMinSdkVersion()
			targetSdkVersion getExportTargetSdkVersion()

			missingDimensionStrategy 'products', 'template'
		}

		... other code
	```

	After
	```
	android {

		... other code

		defaultConfig {
			... other code
		
			// Feel free to modify the application id to your own.
			applicationId getExportPackageName()
			versionCode getExportVersionCode()
			versionName getExportVersionName()
			minSdkVersion getExportMinSdkVersion()
			targetSdkVersion getExportTargetSdkVersion()

			missingDimensionStrategy 'products', 'template'

			// This is needed by EOS Android SDK
			String ClientId = "PUT YOUR EOS CLIENT ID HERE"
			resValue("string", "eos_login_protocol_scheme", "eos." + ClientId.toLowerCase())
		}

		... other code
	```

5. In the `res://android/build/config.gradle` file, update the `minSdk` to `23` to match with the requirements of the `EOS Android SDK`.

	Before
	```
	minSdk             : 22,
	```
	After
	```
	minSdk             : 23,
	```

6. In the `res://android/build/src/com/godot/game/GodotGame.java` file, update it as follows.
	```java
	package com.godot.game;

	import com.epicgames.mobile.eossdk.EOSSDK;     // added
	import org.godotengine.godot.GodotActivity;

	import android.os.Bundle;

	public class GodotApp extends GodotActivity {
		static {                                   // added
			System.loadLibrary("EOSSDK");          // added
		}                                          // added
		
		@Override
		public void onCreate(Bundle savedInstanceState) {
			EOSSDK.init(getApplicationContext());  // added

			setTheme(R.style.GodotAppMainTheme);
			super.onCreate(savedInstanceState);
		}
	}

	```

7. Now open your project in the Godot Editor, and goto `Project -> Export` and create a new Android export profile.

8. In the `Gradle Build` section, enable `Use Gradle Build`. In the `Architectures` section enable `arm64-v8a`. In the `Permissions` section ensure that `ACESSS_NETWORK_STATE`, `ACCESS_WIFI_STATE` and `INTERNET` are enabled. These permissions are needed for the EOS SDK to work. Fill in the other details such as package name, etc as needed.

9. You can now export the Android APK by clicking the Export Project button.


## Current Project Status

- Auth Interface
  - [x] Implementation
  - [x] Sample
- Achievements Interface
  - [x] Implementation
  - [x] Sample
- Connect Interface
  - [x] Implementation
  - [x] Sample
- CustomInvites Interface
  - [x] Implementation
  - [x] Sample
- Friends Interface
  - [x] Implementation
  - [x] Sample
- Stats Interface
  - [x] Implementation
  - [x] Sample
- UserInfo Interface
  - [x] Implementation
  - [x] Sample
- Leaderboards Interface
  - [x] Implementation
  - [x] Sample
- KWS Interface
  - [x] Implementation
  - [ ] Sample (No general access yet)
- Lobby Interface
  - [x] Implementation
  - [ ] Sample
- Metrics Interface
  - [x] Implementation
  - [x] Sample
- Mods Interface
  - [x] Implementation
  - [x] Sample
- P2P Interface
  - [x] Implementation
  - [ ] Sample
- PlayerDataStorage Interface
  - [x] Implementation
  - [ ] Sample
- Presence Interface
  - [x] Implementation
  - [x] Sample
- ProgressionSnapshot Interface
  - [x] Implementation
  - [x] Sample
- Reports Interface
  - [x] Implementation
  - [x] Sample
- RTC Interface
  - [x] Implementation
  - [ ] Sample
- Sanctions Interface
  - [x] Implementation
  - [ ] Sample
- Sessions Interface
  - [x] Implementation
  - [ ] Sample
- TitleStorage Interface
  - [x] Implementation
  - [ ] Sample
- UI Interface
  - [x] Implementation
  - [x] Sample
- Ecom Interface
  - [x] Implementation
  - [ ] Sample (Needs Epic Games Store access)
- AntiCheatServer Interface
  - [ ] Implementation
  - [ ] Sample
- AntiCheatClient Interface
  - [ ] Implementation
  - [ ] Sample
- Version Interface
  - [x] Implementation
  - [x] Sample
- Integrated Platform Interface
  - [ ] Implementation
  - [ ] Sample