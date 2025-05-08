import ReactPlayer from 'react-player'

# Initialization

Follow the steps below to initialize and use the plugin.

Check out the below video for a quick walkthrough.

<ReactPlayer controls url='https://www.youtube.com/watch?v=ENyvF4yVjKg' />

<br></br>

1. You will need the following from the EOS Developer Portal:
- `PRODUCT_ID`
- `SANDBOX_ID`
- `DEPLOYMENT_ID`
- `CLIENT_ID`
- `CLIENT_SECRET`

2. You also need to provide these values yourself:
- `PRODUCT_NAME` - A name chosen by you for the project
- `PRODUCT_VERSION` - A version chosen by you for the project
- `ENCRYPTION_KEY` - A random 64 character long string

Now at the start of your game, or when you want to initialize EOS. Use the following script:

```gdscript
extends Node

func _ready() -> void:
	# This will control which logs you get from EOSG
	HLog.log_level = HLog.LogLevel.INFO

	var init_opts = EOS.Platform.InitializeOptions.new()
	init_opts.product_name = "PRODUCT_NAME_HERE" # Change this
	init_opts.product_version = "PRODUCT_VERSION_HERE" # Change this

	var create_opts = EOS.Platform.CreateOptions.new()
	create_opts.product_id = "PRODUCT_ID_HERE" # Change this
	create_opts.sandbox_id = "SANDBOX_ID_HERE" # Change this
	create_opts.deployment_id = "DEPLOYMENT_ID_HERE" # Change this
	create_opts.client_id = "CLIENT_ID_HERE" # Change this
	create_opts.client_secret = "CLIENT_SECRET_HERE" # Change this
	create_opts.encryption_key = "ENCRYPTION_KEY_HERE" # Change this

	# Enable Social Overlay on Windows
	if OS.get_name() == "Windows":
		HAuth.auth_login_flags = EOS.Auth.LoginFlags.None
		create_opts.flags = EOS.Platform.PlatformFlags.WindowsEnableOverlayOpengl

	# Initialize the SDK
	var init_res := await HPlatform.initialize_async(init_opts)
	if not EOS.is_success(init_res):
		printerr("Failed to initialize EOS SDK: ", EOS.result_str(init_res))
		return
	
	# Create platform
	var create_success := await HPlatform.create_platform_async(create_opts)
	if not create_success:
		printerr("Failed to create EOS Platform")
		return

	# Setup Logs from EOS
	HPlatform.log_msg.connect(_on_eos_log_msg)
	# This will control which logs you get from EOS SDK
	var log_res := HPlatform.set_eos_log_level(EOS.Logging.LogCategory.AllCategories, EOS.Logging.LogLevel.Info)
	if not EOS.is_success(log_res):
		printerr("Failed to set logging level")
		return

	HAuth.logged_in.connect(_on_logged_in)

	# During development use the devauth tool to login
	HAuth.login_devtool_async("localhost:4545", "CREDENTIAL_NAME_HERE")

	# Only on mobile device (Login without any credentials)
	# await HAuth.login_anonymous_async()


func _on_logged_in():
	print("Logged in successfully: product_user_id=%s" % HAuth.product_user_id)

# This method is called when we get a log message from EOS SDK
func _on_eos_log_msg(msg: EOS.Logging.LogMessage) -> void:
	print("SDK %s | %s" % [msg.category, msg.message])
```

## Initialization on Windows

To use the `Account Portal` login option on Windows, you need to bootstrap Godot with EOS. See the steps below.

### Bootstrapping Godot with EOS for Windows

Follow the tutorial at [Redistributable Installer](https://dev.epicgames.com/docs/services/en-US/EpicAccountServices/Crossplayacrossplatforms/RedistributableInstaller/index.html) and also see the below steps.

A sample of the generated `.ini` file for the Godot Editor is shown below (during game development):
```
ApplicationPath=APPLICATION_PATH_HERE
WorkingDirectory=
WaitForExit=0
NoOperation=0
```

In the above `.ini` file, the `APPLICATION_PATH_HERE` can either be the path to your Godot Editor executable (Eg. `Godot_v4.2.0-stable_win64.exe`) when you are in development or the path to the exported game (Eg. `MyAmazingGame.exe`)

## Initialization on Android

The plugin supports the Android platform, but needs a few one-time steps to initialize and configure android exports.

### Pre-requisites

1. Setup the `Android Build Template` in your Godot project by following the tutorial [Gradle builds for Andriod](https://docs.godotengine.org/en/stable/tutorials/export/android_gradle_build.html). This will create an android project in `res://android/build`.

2. Now with reference to the tutorial [Add the EOS SDK to Your Android Studio Project](https://dev.epicgames.com/docs/epic-online-services/platforms/android#4-add-the-eos-sdk-to-your-android-studio-project), perform the following steps.

3. In the `res://android/build/build.gradle` file, add the following lines after the implementations in the `dependencies` section.

    ```gradle
    dependencies {
        implementation libraries.kotlinStdLib
        implementation libraries.androidxFragment
        
        // highlight-start
        // Add these lines
        // EOS SDK dependencies
        implementation 'androidx.appcompat:appcompat:1.5.1'
        implementation 'androidx.constraintlayout:constraintlayout:2.1.4'
        implementation 'androidx.security:security-crypto:1.0.0'
        implementation 'androidx.browser:browser:1.4.0'
        // Update the path so that it points to eossdk-StaticSTDC-release.aar provided in addons/epic-online-services-godot/bin/android/
        implementation files('../../addons/epic-online-services-godot/bin/android/eossdk-StaticSTDC-release.aar')
        // highlight-end

        ...other code
    ```

4. In the `res://android/build/build.gradle` file, add the following lines after the `defaultConfig` in the `android` section.

    ```gradle
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

            // highlight-start
            // Add these lines
            // This is needed by EOS Android SDK
            String ClientId = "PUT YOUR EOS CLIENT_ID HERE"
            resValue("string", "eos_login_protocol_scheme", "eos." + ClientId.toLowerCase())
            // highlight-end
        }

        ... other code
    ```

5. In the `res://android/build/config.gradle` file, update the `minSdk` to `23` to match with the requirements of the `EOS Android SDK`.

    Before
    ```gradle
    minSdk             : 21,
    ```
    After
    ```gradle
    minSdk             : 23,
    ```

6. In the `res://android/build/src/com/godot/game/GodotGame.java` file, update it as follows.
    ```java
    package com.godot.game;

    // highlight-next-line
    import com.epicgames.mobile.eossdk.EOSSDK;     // added
    import org.godotengine.godot.GodotActivity;

    import android.os.Bundle;

    public class GodotApp extends GodotActivity {
        // highlight-next-line
        static {                                   // added
            // highlight-next-line
            System.loadLibrary("EOSSDK");          // added
        // highlight-next-line
        }                                          // added
        
        @Override
        public void onCreate(Bundle savedInstanceState) {
            EOSSDK.init(getActivity());  // added
            
            setTheme(R.style.GodotAppMainTheme);
            super.onCreate(savedInstanceState);

        }
    }

    ```

7. Now open your project in the Godot Editor, and goto `Project -> Export` and create a new Android export profile.

8. In the `Gradle Build` section, enable `Use Gradle Build`. In the `Architectures` section enable `arm64-v8a`. In the `Permissions` section ensure that `ACESSS_NETWORK_STATE`, `ACCESS_WIFI_STATE` and `INTERNET` are enabled. These permissions are needed for the EOS SDK to work. Fill in the other details such as package name, etc as needed.

9. You can now export the Android APK by clicking the `Export Project` button.


## Initialization on iOS

Export the project from Godot editor for iOS target. **You might get a build error during this process, ignore it.** Open the generated iOS project in XCode and build the project. The build should be successful. EOSG has support for `iOS arm64 device` and `iOS arm64 simulator`.
