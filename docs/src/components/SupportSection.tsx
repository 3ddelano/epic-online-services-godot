const SUPPORT_DATA = {
  bmc_url: "https://www.buymeacoffee.com/3ddelano",
  github_sponsor_url: "https://github.com/sponsors/3ddelano",
  discord_url: "https://discord.gg/FZY9TqW",
  username: "@3ddelano",
};

export function SupportSection() {
  return (
    <section
      id="support"
      className="py-20 sm:py-24 bg-[#030712] relative overflow-hidden"
    >
      {/* Background glow */}
      <div className="absolute top-1/2 left-1/2 -translate-x-1/2 -translate-y-1/2 w-[600px] h-[400px] bg-blue-600/5 rounded-full blur-[120px]"></div>

      <div className="container mx-auto px-6 text-center relative z-10">
        <h2 className="text-3xl font-bold mb-4 text-white tracking-tight">
          Support Development
        </h2>
        <p className="text-gray-400 mb-10 max-w-2xl mx-auto font-light leading-relaxed">
          Creating and maintaining this plugin requires significant time and
          effort. If you find this project helpful, please consider supporting
          its development.
        </p>
        <div className="flex flex-wrap justify-center items-center gap-6 mb-10">
          <a
            href={SUPPORT_DATA.bmc_url}
            target="_blank"
            rel="noopener noreferrer"
            className="inline-block transition transform hover:scale-105"
          >
            <img
              height="45"
              width="180"
              src="https://cdn.buymeacoffee.com/buttons/v2/default-red.png"
              alt="Buy Me A Coffee"
            />
          </a>
          <a
            href={SUPPORT_DATA.github_sponsor_url}
            target="_blank"
            rel="noopener noreferrer"
            className="button-primary px-6 py-3 rounded-xl text-sm font-bold no-underline inline-flex items-center gap-2"
          >
            <svg className="w-5 h-5" fill="currentColor" viewBox="0 0 16 16">
              <path
                fillRule="evenodd"
                d="M8 1.314C12.438-3.248 23.534 4.735 8 15-7.534 4.736 3.562-3.248 8 1.314z"
              />
            </svg>
            GitHub Sponsor
          </a>
        </div>

        <div className="glass rounded-2xl p-8 max-w-xl mx-auto">
          <p className="text-gray-300 mb-4 font-medium">
            Need help, have suggestions, or found a bug?
          </p>
          <div className="flex flex-wrap justify-center items-center gap-4">
            <a
              href={SUPPORT_DATA.discord_url}
              target="_blank"
              rel="noopener noreferrer"
              className="button-secondary px-5 py-2.5 rounded-xl text-sm font-bold no-underline inline-flex items-center gap-2"
            >
              <svg className="w-5 h-5" fill="currentColor" viewBox="0 0 24 24">
                <path d="M19.27 5.33C17.94 4.71 16.5 4.26 15 4a.09.09 0 0 0-.07.03c-.18.33-.39.76-.53 1.09a16.09 16.09 0 0 0-4.8 0c-.14-.34-.35-.76-.54-1.09a.09.09 0 0 0-.07-.03c-1.5.26-2.93.71-4.27 1.33a.07.07 0 0 0-.05.08c-.67 1.9-.94 3.9-.94 6s.27 4.09.94 6a.07.07 0 0 0 .05.08c1.34.61 2.77 1.07 4.27 1.33a.09.09 0 0 0 .07-.03c.19-.34.4-.77.54-1.1a16.28 16.28 0 0 0 5.34-.01c.14.33.35.76.53 1.1a.09.09 0 0 0 .07.03c1.5-.26 2.93-.71 4.27-1.33a.07.07 0 0 0 .05-.08c.67-1.91.94-3.91.94-6s-.28-4.09-.94-6a.07.07 0 0 0-.05-.08zM8.5 13.5c-.83 0-1.5-.67-1.5-1.5s.67-1.5 1.5-1.5 1.5.67 1.5 1.5-.67 1.5-1.5 1.5zm7 0c-.83 0-1.5-.67-1.5-1.5s.67-1.5 1.5-1.5 1.5.67 1.5 1.5-.67 1.5-1.5 1.5z" />
              </svg>
              Join Discord
            </a>
            <span className="text-gray-500 text-sm">
              or DM{" "}
              <code className="glass px-2 py-0.5 rounded text-gray-300 font-mono text-xs">
                {SUPPORT_DATA.username}
              </code>
            </span>
          </div>
        </div>
      </div>
    </section>
  );
}
