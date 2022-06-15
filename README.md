Epic Online Services Godot (WIP)
=========================================
<img alt="Project Logo" src="./_media/logo.png" height="150">

### Unofficial Epic Online Services wrapper for Godot Engine 3.x (includes demo project)

<img alt="Godot3" src="https://img.shields.io/badge/-Godot 3.x-478CBF?style=for-the-badge&logo=godotengine&logoWidth=20&logoColor=white" />  <img alt="Epic Online Services" src="https://img.shields.io/badge/-Epic Online Services-313131?style=for-the-badge&logo=epic-games&logoWidth=20&logoColor=white" />

> Tested on: Windows and Linux

#### Disclaimer: This project is NOT affiliated with Epic Games Inc or Godot Engine. It doesn't endorse Epic Online Services. This project and sample Godot scenes is provided solely for educational purposes and may or may not comply with Epic Games' Design Guidelines, if you plan to release a game make sure you read the Guidelines and any other steps needed to release a public game like asking for user consent, option to delete user data, website with privacy policy and license, etc.

<a href="#current-status-link"><h2>Current Status</h2>
</a>

## [Demo Video (Coming Soon)](#)

## [Documentation (Coming Soon)](#)

How does it work
--------------

This project wraps the Epic Online Services C# SDK so that it can be easily used in Godot using GDScript, VisualScript, C#, etc with similar class hierarchy and type support. It makes use of signals for sending events like user login, logout, achievement unlock, etc.


Requirements
--------------

> You need to have the **Mono version of Godot** to make use of this project. This doesn't mean you have to code in C#. You can code in any language that Godot supports.

- Mono version (C# support) of Godot Engine (Get it here [Godot Engine Download](https://godotengine.org/download))
  
  Easily convert a GDScript project to Mono (Project menu -> Tools -> C# -> Create C# Solution)
- EOS C# SDK (Download from [Epic Developer Portal](https://dev.epicgames.com/portal/sdk))
- Make sure you have accepted the Terms for [Epic Online Services](https://www.epicgames.com/site/en-US/tos?lang=en-US)
- A product registered with Epic Games Services (Make one for free [Epic Developer Portal](https://dev.epicgames.com/portal))


Getting Started
--------------
1. Download/clone the repo and copy the `EOS` folder to your own Godot project folder.

2. Download the `EOS C# SDK`, extract it, and copy and paste the `Source` and `Bin` folders to the `EOS` folder in step 1. The EOS folder should look something like this:
<img src="./_media/eos_folder_structure.png">

3. Open the file, `res://EOS/Source/Core/Config.cs` and paste the following lines at the top and save the file 
   > You may get some permission warning saying that the file is Readonly. Make sure that the file gets saved/overwritten
   ```C
    #if GODOT_WINDOWS || GODOT_64
    #define EOS_PLATFORM_WINDOWS_64
    #elif GODOT_X11
    #define EOS_PLATFORM_LINUX
    #elif GODOT_OSX
    #define EOS_PLATFORM_OSX
    #elif GODOT_IOS
    #define EOS_PLATFORM_IOS
    #elif GODOT_ANDROID || __ANDROID__
    #define EOS_PLATFORM_ANDROID
    #endif
   ``` 

4. You can now use the methods/signals from the `EOS` class in Godot.
   
   Minimal Starting Boilerplate (See `Main.gd` and `LogsView.gd` for sample)
   ```GDScript
    func _ready():
        # -----
        # Initialize the SDK
        # -----
        var init_options = EOS.Platform.InitializeOptions.new()
        init_options.product_name = "PRODUCT NAME HERE"
        init_options.product_version = "PRODUCT VERSION HERE"

        var init_result: int = EOS.Platform.PlatformInterface.initialize(init_options)
        if init_result != EOS.Result.Success:
            print("Failed to initialize EOS SDK: ", EOS.print_result(init_result))
            return

        # -----
        # Create platform
        # -----
        var create_options = EOS.Platform.CreateOptions.new()
        create_options.product_id = "PRODUCT_ID HERE"
        create_options.sandbox_id = "SANDBOX_ID HERE"
        create_options.deployment_id = "DEPLOYMENT_ID HERE"
        create_options.client_id = "CLIENT_ID HERE"
        create_options.client_secret = "CLIENT_SECRET HERE"
        create_options.encryption_key = "ENCRYPTION_KEY HERE"

        # Show Social Overlay
        if OS.get_name() == "Windows":
            create_options.flags = EOS.Platform.PlatformFlags.WindowsEnableOverlayOpengl

        var create_success: bool = EOS.Platform.PlatformInterface.create(create_options)
        if not create_success:
            print("Failed to create EOS Platform")
            return

        # -----
        # Setup Logging
        # -----
        EOS.get_instance().connect("logging_interface_callback", self, "_on_logging_interface_callback")
        var res = EOS.Logging.set_log_level(EOS.Logging.LogCategory.AllCategories, EOS.Logging.LogLevel.Info)
        if res != EOS.Result.Success:
            print("Failed to set log level: ", EOS.print_result(res))
    
    func _on_logging_interface_callback(msg):
        msg = EOS.Logging.LogMessage.from(msg) as EOS.Logging.LogMessage
        
        if msg == null:
            return
        
        print("SDK %s | %s" % [msg.category, msg.message])
   ```


### How to run the sample scenes?
> The sample Godot project is located in the **Sample** folder

Copy the credentials (Product Id, Sandbox Id, Deployment Id, Client Id, Client Secret) for your product from the Dev Portal and paste them in `Main.gd` script in the relevant `EOS.Platform.CreateOptions`. The encryption key is a random 64 character long string. These credentials need to be kept as private as possible. One way is to make sure to encrypt all scripts when exporting the final game. To do this in the Export menu, in the Scripts tab, select the Encrypted mode for GDScript Export Mode and enter a strong encryption key.

Then configure your Product on the EOS Dev Portal with the following configuration:
- In the Client Policies section in Product Settings, enable all the features except Connect (Disabled by Epic)
- In the Permissions section of Epic Account Services, enable all three: Basic Profile, Online Presence and Friends. 
- (Optional if you want some pre-made achievements)
  
  In the Achievements section in Game Services, use the Bulk Import option and import the `HelloProduct.zip` file located at `res://HelloProduct.zip`




# Support Development

### Making this project took a lot of time and effort, reading the EOS documentation countless times and testing each method in Godot. I would really appreciate if you could support the project in any way.

<a href="https://www.buymeacoffee.com/3ddelano" target="_blank"><img height="41" width="174" src="https://cdn.buymeacoffee.com/buttons/v2/default-red.png" alt="Buy Me A Coffee" width="150" ></a>

#### Want to support in other ways? Contact me on Discord: `@3ddelano#6033`

Join the Discord server for discussing suggestions or bugs: [3ddelano Cafe](https://discord.gg/FZY9TqW)

<a id="current-status-link"><h2>Current Status</h2></a>

- Auth Interface
  - [x] Implementation
  - [x] Sample
- Achievements Interface
  - [x] Implementation
  - [x] Sample
- Connect Interface
  - [x] Implementation
  - [x] Sample
- AntiCheatServer Interface
  - [ ] Implementation
  - [ ] Sample
- AntiCheatClient Interface
  - [ ] Implementation
  - [ ] Sample
- CustomInvites Interface
  - [x] Implementation
  - [x] Sample
- Ecom Interface
  - [ ] Implementation
  - [ ] Sample
- Friends Interface
  - [ ] Implementation
  - [ ] Sample
- KWS Interface
  - [ ] Implementation
  - [ ] Sample
- Leaderboards Interface
  - [ ] Implementation
  - [ ] Sample
- Lobby Interface
  - [ ] Implementation
  - [ ] Sample
- Metrics Interface
  - [ ] Implementation
  - [ ] Sample
- Mods Interface
  - [ ] Implementation
  - [ ] Sample
- P2P Interface
  - [ ] Implementation
  - [ ] Sample
- PlayerDataStorage Interface
  - [ ] Implementation
  - [ ] Sample
- Presence Interface
  - [ ] Implementation
  - [ ] Sample
- ProgressionSnapshot Interface
  - [ ] Implementation
  - [ ] Sample
- Reports Interface
  - [ ] Implementation
  - [ ] Sample
- RTC Interface
  - [ ] Implementation
  - [ ] Sample
- Sanctions Interface
  - [ ] Implementation
  - [ ] Sample
- Sessions Interface
  - [ ] Implementation
  - [ ] Sample
- Stats Interface
  - [ ] Implementation
  - [ ] Sample
- TitleStorage Interface
  - [ ] Implementation
  - [ ] Sample
- UI Interface
  - [ ] Implementation
  - [ ] Sample
- UserInfo Interface
  - [ ] Implementation
  - [ ] Sample
