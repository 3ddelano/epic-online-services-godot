const SCREENSHOTS_DATA = [
  {
    category: "Windows",
    images: [
      {
        src: "img/screenshots/windows_auth_success.png",
        alt: "Windows Auth Success Screenshot",
      },
    ],
  },
  {
    category: "Android",
    images: [
      {
        src: "img/screenshots/android_auth_success.jpg",
        alt: "Android Auth Success Screenshot",
      },
      {
        src: "img/screenshots/android_p2p_game.jpg",
        alt: "Android P2P Game Screenshot",
      },
    ],
  },
  {
    category: "iOS / macOS",
    images: [
      {
        src: "img/screenshots/ios_simulator_auth_success.png",
        alt: "iOS Simulator Auth Success Screenshot",
      },
      {
        src: "img/screenshots/ios_simulator_in_lobby.png",
        alt: "iOS Simulator In Lobby Screenshot",
        subtitle: "Cross-Platform Lobbies",
      },
      {
        src: "img/screenshots/mac_in_lobby.png",
        alt: "macOS In Lobby Screenshot",
      },
    ],
  },
];

const TUTORIALS_DATA = {
  playlist_url:
    "https://www.youtube.com/playlist?list=PL5t0hR7ADzun5JYF4e2a2FtZEWYHxK83_",
  thumbnail_url: "https://img.youtube.com/vi/ENyvF4yVjKg/0.jpg",
  thumbnail_alt: "Epic Online Services Tutorial Series Thumbnail",
};

export function ScreenshotsSection() {
  return (
    <section id="screenshots" className="py-20 sm:py-24 bg-[#030712]">
      <div className="container mx-auto px-6">
        <h2 className="text-3xl font-bold mb-4 text-center text-white tracking-tight">
          Screenshots & Demos
        </h2>
        <p className="text-gray-400 text-center mb-14 max-w-2xl mx-auto font-light">
          See the plugin running on multiple platforms.
        </p>
        <div className="grid grid-cols-1 sm:grid-cols-2 lg:grid-cols-3 gap-8">
          {SCREENSHOTS_DATA.map((platform, i) => (
            <div key={i} className="flex items-center flex-col">
              <h3 className="text-lg font-bold mb-4 text-center text-gray-200 uppercase tracking-wider">
                {platform.category}
              </h3>
              {platform.images.map((image, j) => (
                <div key={j} className="flex flex-col items-center w-full">
                  {image.subtitle && (
                    <h4 className="text-sm font-bold my-3 text-center text-gray-400 uppercase tracking-wider">
                      {image.subtitle}
                    </h4>
                  )}
                  <img
                    src={image.src}
                    alt={image.alt}
                    className={`w-full max-w-[800px] h-auto rounded-xl glass border-white/10 hover:brightness-110 transition-all duration-300 ${
                      j < platform.images.length - 1 ? "mb-4" : ""
                    }`}
                  />
                </div>
              ))}
            </div>
          ))}
        </div>
      </div>
    </section>
  );
}

export function DemoSection() {
  return (
    <section id="demo" className="py-20 sm:py-24 bg-[#0a0f1e]">
      <div className="container mx-auto px-6 text-center">
        <h2 className="text-3xl font-bold mb-4 text-white tracking-tight">
          Video Tutorials
        </h2>
        <p className="text-gray-400 mb-10 max-w-2xl mx-auto font-light">
          Watch the tutorial playlist on YouTube for a visual guide and
          examples.
        </p>
        <div className="max-w-3xl mx-auto aspect-video rounded-2xl overflow-hidden glass">
          <a
            href={TUTORIALS_DATA.playlist_url}
            target="_blank"
            rel="noopener noreferrer"
            className="block relative group"
          >
            <img
              src={TUTORIALS_DATA.thumbnail_url}
              alt={TUTORIALS_DATA.thumbnail_alt}
              className="w-full h-full object-cover"
            />
            <div className="absolute inset-0 bg-black/50 flex items-center justify-center opacity-0 group-hover:opacity-100 transition-opacity duration-300">
              <div className="w-20 h-20 rounded-full bg-white/10 backdrop-blur-sm flex items-center justify-center border border-white/20">
                <svg
                  className="w-10 h-10 text-white ml-1"
                  fill="currentColor"
                  viewBox="0 0 20 20"
                >
                  <path d="M6.5 5.5v9l7-4.5-7-4.5z"></path>
                </svg>
              </div>
            </div>
          </a>
        </div>
        <a
          href={TUTORIALS_DATA.playlist_url}
          target="_blank"
          rel="noopener noreferrer"
          className="inline-block mt-8 text-blue-400 hover:text-blue-300 font-semibold text-sm no-underline transition-colors"
        >
          Watch the playlist on YouTube →
        </a>
      </div>
    </section>
  );
}
