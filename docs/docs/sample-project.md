# Sample Project

To run the sample project which showcases most of the EOS features, follow these steps:

1. Clone/Download the [plugin repo](https://github.com/3ddelano/epic-online-services-godot)

2. You cannot directly open the ./sample folder and run the project, because the plugin binaries are not included. To include the plugin binaries. Download the [latest release](https://github.com/3ddelano/epic-online-services-godot/releases/latest) from the Releases section and replace the existing `sample/addons/epic-online-services-godot` folder with the one from the Release (this includes the built shared libraries). See the [Installation](/docs/installation) section for more information on how to install the plugin.

3. Copy your credentials (`Product Id`, `Sandbox Id`, `Deployment Id`, `Client Id`, `Client Secret`) of your Epic Games "Product" from the Epic Games Dev Portal and paste them in `./sample/Main.gd` script in the relevant sections. The encryption key is a random 64 character long string. These credentials need to be kept as private as possible. One way is to make sure to encrypt all scripts when exporting the final game. (See [Compiling with script key encryption](https://docs.godotengine.org/en/stable/development/compiling/compiling_with_script_encryption_key.html))

4. Configure your Product on the [EOS Dev Portal](https://dev.epicgames.com/portal) with the following configuration:
  - In the `Client Policies` section in `Product Settings`, for the Client policy type choose `Custom policy`, enable the `User is required` and enable every features and action except `Connect` (Trusted Server Required). This will allow the sample to access the different services provided by Epic Online Services. In your actual game, the client policy is important and you should give minimal permissions to features.
  - In the `Permissions` section of `Epic Account Services`, enable all three: `Basic Profile`, `Online Presence` and `Friends`. 
  - (Optional if you want some pre-made achievements)
  In the `Achievements` section in `Game Services`, use the `Bulk Import` option and import the `HelloProduct.zip` file located at `res://HelloProduct.zip`

5. Finally open the `./sample` folder in Godot.