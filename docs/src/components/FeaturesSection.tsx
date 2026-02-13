const FEATURES = [
  "Authentication",
  "Social Overlay (Win)",
  "Achievements",
  "Stats & Leaderboards",
  "Lobby & Sessions",
  "Multiplayer (P2P)",
  "Voice",
  "Metrics",
  "Mods",
  "Player/Title Storage",
  "Progression Snapshot",
  "Reports & Sanctions",
  "Ecommerce (EGS)",
  "AntiCheat",
];

const HIGH_LEVEL_CLASSES = [
  "HPlatform",
  "HAuth",
  "HAchievements",
  "HFriends",
  "HStats",
  "HLeaderboards",
  "HLobbies",
  "HP2P",
  "HLog",
];

export function FeaturesSection() {
  return (
    <section id="features" className="py-20 sm:py-24 bg-[#0a0f1e]">
      <div className="container mx-auto px-6">
        <h2 className="text-3xl font-bold mb-4 text-center text-white tracking-tight">
          Core Features
        </h2>
        <p className="text-gray-400 text-center mb-14 max-w-2xl mx-auto font-light">
          All the EOS features you need, wrapped in a clean GDScript API.
        </p>
        <div className="grid grid-cols-2 sm:grid-cols-3 md:grid-cols-4 lg:grid-cols-5 gap-3 max-w-6xl mx-auto">
          {FEATURES.map((feature, index) => (
            <div key={index} className="feature-card p-4 rounded-xl">
              <span className="flex items-center text-sm">
                <svg
                  className="w-4 h-4 mr-2 text-emerald-400 flex-shrink-0"
                  fill="currentColor"
                  viewBox="0 0 20 20"
                >
                  <path
                    fillRule="evenodd"
                    d="M10 18a8 8 0 100-16 8 8 0 000 16zm3.707-9.293a1 1 0 00-1.414-1.414L9 10.586 7.707 9.293a1 1 0 00-1.414 1.414l2 2a1 1 0 001.414 0l4-4z"
                    clipRule="evenodd"
                  ></path>
                </svg>
                <span className="text-gray-200 font-medium">{feature}</span>
              </span>
            </div>
          ))}
        </div>
      </div>
    </section>
  );
}

export function HighLevelSection() {
  return (
    <section id="high-level" className="py-16 sm:py-20 bg-gray-950">
      <div className="container mx-auto px-6">
        <h2 className="text-3xl font-bold mb-10 text-center text-white">
          High Level EOS Classes
        </h2>
        <p className="text-gray-300 mb-8 max-w-2xl mx-auto text-center">
          These classes simplify interaction with EOS services. They include
          built-in documentation accessible directly within the Godot Editor.
        </p>
        <div className="grid grid-cols-2 sm:grid-cols-3 md:grid-cols-4 lg:grid-cols-5 gap-4 max-w-5xl mx-auto">
          {HIGH_LEVEL_CLASSES.map((cls, index) => (
            <div
              key={index}
              className="bg-blue-900/80 hover:bg-blue-800/80 p-4 rounded-lg text-center shadow transition"
            >
              <span className="font-mono font-medium text-blue-100">{cls}</span>
            </div>
          ))}
        </div>
      </div>
    </section>
  );
}
