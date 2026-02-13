import CodeBlock from "@theme/CodeBlock";

const codeExample = `
# In main script
extends Node

func _ready() -> void:
    # Setup HEOS Logs
    HLog.log_level = HLog.LogLevel.INFO

    var init_opts = EOS.Platform.InitializeOptions.new()
    init_opts.product_name = "PRODUCT_NAME_HERE"
    init_opts.product_version = "PRODUCT_VERSION_HERE"

    var create_opts = EOS.Platform.CreateOptions.new()
    create_opts.product_id = "PRODUCT_ID_HERE"
    create_opts.sandbox_id = "SANDBOX_ID_HERE"
    create_opts.deployment_id = "DEPLOYMENT_ID_HERE"
    create_opts.client_id = "CLIENT_ID_HERE"
    create_opts.client_secret = "CLIENT_SECRET_HERE"
    create_opts.encryption_key = "ENCRYPTION_KEY_HERE"

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

    # Example: Get top records for a leaderboard
    var records := await HLeaderboards.get_leaderboard_records_async("LEADERBOARD_ID_HERE")
    print(records)


func _on_eos_log_msg(msg: EOS.Logging.LogMessage) -> void:
    print("SDK %s | %s" % [msg.category, msg.message])
`;

export function ExampleSection() {
  return (
    <section id="example" className="py-16 sm:py-20 bg-gray-900">
      <div className="container mx-auto px-6">
        <h2 className="text-3xl font-bold mb-8 text-center text-white">
          Basic Usage Example (GDScript)
        </h2>
        <div className="max-w-4xl mx-auto">
          <CodeBlock language="gdscript">{codeExample}</CodeBlock>
        </div>
      </div>
    </section>
  );
}
