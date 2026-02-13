import { useState } from "react";

// Navigation links data
const NAV_LINKS = [
  {
    label: "Docs",
    href: "docs/introduction",
    external: false,
  },
  {
    label: "Showcase",
    href: "showcase",
    external: false,
  },
  {
    label: "GitHub",
    href: "https://github.com/3ddelano/epic-online-services-godot",
    external: true,
    icon: (
      <svg
        xmlns="http://www.w3.org/2000/svg"
        className="w-4 h-4"
        fill="#fff"
        viewBox="0 0 256 256"
      >
        <path
          d="M17.791 46.836A2 2 0 0 0 19 45v-5.4q.002-.297.041-.61L19 39h-3.6c-1.5 0-2.8-.6-3.4-1.8-.7-1.3-1-3.5-2.8-4.7-.3-.2-.1-.5.5-.5.6.1 1.9.9 2.7 2 .9 1.1 1.8 2 3.4 2 2.487 0 3.82-.125 4.622-.555C21.356 34.056 22.649 33 24 33v-.025c-5.668-.182-9.289-2.066-10.975-4.975-3.665.042-6.856.405-8.677.707a22 22 0 0 1-.151-.987c1.797-.296 4.843-.647 8.345-.714a8 8 0 0 1-.291-.849c-3.511-.178-6.541-.039-8.187.097-.02-.332-.047-.663-.051-.999 1.649-.135 4.597-.27 8.018-.111a10 10 0 0 1-.13-1.543c0-1.7.6-3.5 1.7-5-.5-1.7-1.2-5.3.2-6.6 2.7 0 4.6 1.3 5.5 2.1C21 13.4 22.9 13 25 13s4 .4 5.6 1.1c.9-.8 2.8-2.1 5.5-2.1 1.5 1.4.7 5 .2 6.6 1.1 1.5 1.7 3.2 1.6 5 0 .484-.045.951-.11 1.409 3.499-.172 6.527-.034 8.204.102-.002.337-.033.666-.051.999-1.671-.138-4.775-.28-8.359-.089a8 8 0 0 1-.325.98c3.546.046 6.665.389 8.548.689q-.064.498-.151.987c-1.912-.306-5.171-.664-8.879-.682-1.665 2.878-5.22 4.755-10.777 4.974V33c2.6 0 5 3.9 5 6.6V45c0 .823.498 1.53 1.209 1.836C41.37 43.804 48 35.164 48 25 48 12.318 37.683 2 25 2S2 12.318 2 25c0 10.164 6.63 18.804 15.791 21.836"
          fontFamily="none"
          fontSize="none"
          fontWeight="none"
          style={{ mixBlendMode: "normal" }}
          textAnchor="none"
          transform="scale(5.12)"
        ></path>
      </svg>
    ),
  },
];

export function NavBar() {
  const [isMobileMenuOpen, setIsMobileMenuOpen] = useState(false);

  return (
    <nav className="fixed top-0 left-0 right-0 z-[100] glass py-4 px-6 h-16 navbar">
      <div className="container mx-auto flex items-center justify-between">
        <a
          href="/epic-online-services-godot/"
          className="group flex items-center gap-3 no-underline"
        >
          <img
            src="img/eosg-logo.svg"
            alt="Logo"
            className="w-8 h-8 group-hover:rotate-12 transition-transform duration-500"
          />
          <span className="text-white text-sm font-bold tracking-tight uppercase group-hover:text-blue-400 transition-colors">
            EOS Godot
          </span>
        </a>

        {/* Desktop menu */}
        <div className="hidden sm:flex items-center gap-8">
          {NAV_LINKS.map((link) => (
            <a
              key={link.label}
              href={link.href}
              {...(link.external && {
                rel: "noopener noreferrer",
                target: "_blank",
              })}
              className={
                link.label === "GitHub"
                  ? "button-secondary px-4 py-2 rounded-lg text-xs font-bold uppercase tracking-widest no-underline flex items-center gap-2"
                  : "text-white hover:text-blue-400 active:text-blue-400 text-sm font-semibold uppercase tracking-wider transition-colors no-underline"
              }
            >
              {link.icon}
              {link.label}
            </a>
          ))}
        </div>

        {/* Mobile menu button */}
        <button
          className="sm:hidden p-2 -mr-2"
          onClick={() => setIsMobileMenuOpen(!isMobileMenuOpen)}
          aria-label="Toggle menu"
        >
          <svg
            className="w-6 h-6 text-white"
            fill="none"
            stroke="currentColor"
            viewBox="0 0 24 24"
          >
            {isMobileMenuOpen ? (
              <path
                strokeLinecap="round"
                strokeLinejoin="round"
                strokeWidth="2"
                d="M6 18L18 6M6 6l12 12"
              />
            ) : (
              <path
                strokeLinecap="round"
                strokeLinejoin="round"
                strokeWidth="2"
                d="M4 6h16M4 12h16m-7 6h7"
              />
            )}
          </svg>
        </button>
      </div>

      {/* Mobile menu dropdown */}
      {isMobileMenuOpen && (
        <div className="sm:hidden absolute top-16 left-0 right-0 backdrop-blur-xl bg-gray-900/95 border-t border-b border-white/10 p-4 shadow-lg">
          <div className="container mx-auto px-6 py-4 flex flex-col gap-4">
            {NAV_LINKS.map((link) => (
              <a
                key={link.label}
                href={link.href}
                {...(link.external && {
                  rel: "noopener noreferrer",
                  target: "_blank",
                })}
                className="text-white hover:text-blue-400 active:text-blue-400 text-sm font-semibold uppercase tracking-wider transition-colors no-underline flex items-center gap-2"
                onClick={() => setIsMobileMenuOpen(false)}
              >
                {link.icon}
                {link.label}
              </a>
            ))}
          </div>
        </div>
      )}
    </nav>
  );
}

export function Footer() {
  return (
    <footer className="bg-[#030712] border-t border-white/5 pt-12 pb-8">
      <div className="container px-6">
        <div className="glass rounded-xl text-yellow-300/80 text-xs p-5 max-w-3xl mb-10">
          <p className="leading-relaxed">
            <strong className="text-yellow-200">Disclaimer: </strong>This
            project Epic Online Services Godot (EOSG) is <strong>NOT</strong>{" "}
            affiliated with Epic Games Inc or Godot Engine. It does not endorse
            Epic Online Services. This project and the sample Godot scenes are
            provided solely for educational purposes and may or may not comply
            with Epic Games' Design Guidelines. If you plan to release a game
            using EOS, ensure you read the official{" "}
            <a
              href="https://dev.epicgames.com/docs/services/en-US/EpicAccountServices/DesignGuidelines/index.html"
              target="_blank"
              rel="noopener noreferrer"
              className="underline hover:text-yellow-100 transition-colors"
            >
              Guidelines
            </a>{" "}
            and fulfill all requirements (user consent, data deletion, privacy
            policy, etc.).
          </p>
        </div>

        <p className="text-gray-500 text-sm">
          Copyright &copy; {new Date().getFullYear()}{" "}
          <a
            href="https://github.com/3ddelano"
            target="_blank"
            rel="noopener noreferrer"
            className="text-blue-400/80 hover:text-blue-300 transition-colors"
          >
            Delano Lourenco
          </a>
          . All rights reserved.
        </p>
        <p className="text-gray-600 text-xs mt-2">
          Epic Games, Epic Online Services, Godot Engine, and their respective
          logos are trademarks or registered trademarks of their respective
          owners.
        </p>
      </div>
    </footer>
  );
}
