export function HowItWorksSection() {
  return (
    <section id="how-it-works" className="py-16 sm:py-20 bg-gray-950">
      <div className="container mx-auto px-6">
        <div className="max-w-3xl mx-auto text-center">
          <h2 className="text-3xl font-bold mb-6 text-white">How It Works</h2>
          <p className="text-lg text-gray-300 leading-relaxed">
            This project utilizes Godot's{" "}
            <strong className="text-blue-400">GDExtension</strong> system to
            wrap the official Epic Online Services C SDK. This allows seamless
            integration and usage within Godot projects using GDScript, C#, or
            other supported languages. It provides a familiar class hierarchy
            and leverages Godot's signal system for handling asynchronous events
            like user login, achievement unlocks, lobby updates, and more.
          </p>
        </div>
      </div>
    </section>
  );
}

export function InstallationSection() {
  return (
    <section id="installation" className="py-16 sm:py-20 bg-gray-900">
      <div className="container mx-auto px-6">
        <h2 className="text-3xl font-bold mb-12 text-center text-white">
          Installation
        </h2>
        <div className="grid md:grid-cols-2 gap-10 max-w-5xl mx-auto">
          <div className="bg-gray-800 p-6 rounded-lg shadow-md border border-gray-700">
            <h3 className="text-xl font-semibold mb-4 text-white">
              Method 1: Asset Library (Recommended)
            </h3>
            <ol className="list-decimal list-inside space-y-2 text-gray-300">
              <li>
                Open the <strong className="text-white">AssetLib</strong> tab in
                the Godot editor.
              </li>
              <li>
                Search for{" "}
                <code className="bg-gray-700 px-1.5 py-0.5 rounded text-gray-100">
                  EOSG
                </code>{" "}
                or{" "}
                <code className="bg-gray-700 px-1.5 py-0.5 rounded text-gray-100">
                  Epic Online Services Godot
                </code>
                .
              </li>
              <li>
                Find the plugin by{" "}
                <strong className="text-white">3ddelano</strong> and click on
                it.
              </li>
              <li>
                Click <strong className="text-blue-400">Download</strong>, then{" "}
                <strong className="text-blue-400">Install</strong>.
              </li>
              <li>
                Go to{" "}
                <strong className="text-white">
                  Project {"->"} Project Settings {"->"} Plugins
                </strong>
                .
              </li>
              <li>
                Enable the{" "}
                <strong className="text-white">
                  Epic Online Services Godot (EOSG)
                </strong>{" "}
                plugin.
              </li>
              <li>Restart the Godot editor.</li>
            </ol>
            <a
              href="https://godotengine.org/asset-library/asset/2453"
              target="_blank"
              rel="noopener noreferrer"
              className="inline-block mt-4 text-blue-400 hover:text-blue-300 underline"
            >
              View on Godot Asset Library
            </a>
          </div>
          <div className="bg-gray-800 p-6 rounded-lg shadow-md border border-gray-700">
            <h3 className="text-xl font-semibold mb-4 text-white">
              Method 2: GitHub Release
            </h3>
            <ol className="list-decimal list-inside space-y-2 text-gray-300">
              <li>
                Download the latest release{" "}
                <code className="bg-gray-700 px-1.5 py-0.5 rounded text-gray-100">
                  .zip
                </code>{" "}
                file from GitHub.
              </li>
              <li>Extract the zip archive.</li>
              <li>
                Copy the{" "}
                <code className="bg-gray-700 px-1.5 py-0.5 rounded text-gray-100">
                  addons/epic-online-services-godot
                </code>{" "}
                folder into your project's{" "}
                <code className="bg-gray-700 px-1.5 py-0.5 rounded text-gray-100">
                  res://addons/
                </code>{" "}
                folder (create it if needed).
              </li>
              <li>
                Go to{" "}
                <strong className="text-white">
                  Project {"->"} Project Settings {"->"} Plugins
                </strong>
                .
              </li>
              <li>
                Enable the{" "}
                <strong className="text-white">
                  Epic Online Services Godot (EOSG)
                </strong>{" "}
                plugin.
              </li>
              <li>Restart the Godot editor.</li>
            </ol>
            <a
              href="https://github.com/3ddelano/epic-online-services-godot/releases/latest"
              target="_blank"
              rel="noopener noreferrer"
              className="inline-block mt-4 text-blue-400 hover:text-blue-300 underline"
            >
              Download from GitHub Releases
            </a>
          </div>
        </div>
        <p className="text-center mt-8 text-gray-400">
          After installation, you can start using the{" "}
          <code className="bg-gray-700 px-1.5 py-0.5 rounded text-gray-100">
            EOS
          </code>{" "}
          and{" "}
          <code className="bg-gray-700 px-1.5 py-0.5 rounded text-gray-100">
            H
          </code>{" "}
          classes in your scripts.
        </p>
      </div>
    </section>
  );
}
