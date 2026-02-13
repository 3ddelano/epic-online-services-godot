import useDocusaurusContext from "@docusaurus/useDocusaurusContext";
import { GodotIcon, EpicIcon } from "../components/Icons";

const VERSIONS = [
  { label: "Godot", value: "4.2+", icon: GodotIcon },
  { label: "EOS SDK", value: "1.18.1.2", icon: EpicIcon },
];

const PLATFORMS = [
  { label: "Windows x64/arm64", icon: "🖥️" },
  { label: "Linux x64/arm64", icon: "🐧" },
  { label: "Android x64/arm64", icon: "🤖" },
  { label: "MacOS x64/arm64", icon: "🍎" },
  { label: "iOS arm64", icon: "🍏" },
];

const HERO_ACTIONS = [
  {
    label: "Download Latest",
    href: "https://github.com/3ddelano/epic-online-services-godot/releases/latest",
    primary: true,
    target: "_blank",
  },
  {
    label: "Documentation",
    href: "docs/introduction",
    primary: false,
  },
];

export function HeroSection() {
  const { siteConfig } = useDocusaurusContext();
  return (
    <section
      id="hero"
      className="relative bg-[#030712] py-28 sm:py-36 overflow-hidden"
    >
      <div className="absolute inset-0 hero-image opacity-30"></div>
      {/* Glow orbs */}
      <div className="absolute top-20 left-1/4 w-96 h-96 bg-blue-600/20 rounded-full blur-[120px]"></div>
      <div className="absolute bottom-20 right-1/4 w-80 h-80 bg-purple-600/15 rounded-full blur-[100px]"></div>

      <div className="container px-6 relative z-10 grid lg:grid-cols-2 gap-12 items-center">
        {/* Left col */}
        <div className="max-w-xl">
          <h1 className="text-4xl font-bold tracking-tight sm:text-5xl md:text-6xl mb-6 text-white leading-tight">
            {siteConfig.title}
          </h1>
          <p className="text-xl text-gray-400 mb-8 font-light leading-relaxed">
            {siteConfig.tagline}
          </p>
          <div className="flex flex-wrap gap-3 mb-8">
            {VERSIONS.map((v, i) => (
              <span
                key={i}
                className="glass px-4 py-2 rounded-full flex items-center text-sm font-semibold text-gray-200"
              >
                <v.icon className="w-4 h-4 mr-2" />
                {v.label} {v.value}
              </span>
            ))}
          </div>

          <div className="flex flex-wrap gap-4 mb-10">
            {HERO_ACTIONS.map((action, i) => (
              <a
                key={i}
                href={action.href}
                target={action.target}
                rel={action.target === "_blank" ? "noopener noreferrer" : ""}
                className={`${
                  action.primary ? "button-primary" : "button-secondary"
                } px-7 py-3.5 rounded-xl text-sm font-bold uppercase tracking-wider no-underline`}
              >
                {action.label}
              </a>
            ))}
          </div>

          <div className="text-gray-500">
            <p className="mb-3 font-semibold text-gray-400 text-sm uppercase tracking-wider">
              Supported Platforms
            </p>
            <div className="flex flex-wrap gap-2">
              {PLATFORMS.map((platform, i) => (
                <span
                  key={i}
                  className="glass px-3 py-1.5 rounded-full text-xs font-semibold text-gray-300 transition-transform duration-200 hover:scale-105"
                >
                  {platform.icon} {platform.label}
                </span>
              ))}
            </div>
          </div>
        </div>

        {/* Logo Column */}
        <div className="flex-1 flex justify-center items-center">
          <div className="relative">
            <div className="absolute inset-0 bg-blue-500/20 rounded-full blur-[60px] scale-110"></div>
            <img
              src="img/eosg-logo.svg"
              alt="Logo"
              className="w-64 h-64 object-contain rounded-md relative z-10 drop-shadow-2xl"
            />
          </div>
        </div>
      </div>
    </section>
  );
}
