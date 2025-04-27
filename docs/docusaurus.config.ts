import { themes as prismThemes } from "prism-react-renderer";
import type { Config } from "@docusaurus/types";
import type * as Preset from "@docusaurus/preset-classic";

// This runs in Node.js - Don't use client-side code here (browser APIs, JSX...)

const config: Config = {
  title: "Epic Online Services Godot",
  tagline: "Easiest way to use Epic Online Services in Godot 4.2+",
  favicon: "img/favicon.ico",

  url: "https://3ddelano.github.io",
  baseUrl: "/epic-online-services-godot/",

  // GitHub pages deployment config
  organizationName: "3ddelano",
  projectName: "epic-online-services-godot",
  deploymentBranch: 'gh-pages',

  trailingSlash: false,

  onBrokenLinks: "throw",
  onBrokenMarkdownLinks: "warn",

  i18n: {
    defaultLocale: "en",
    locales: ["en"],
  },

  presets: [
    [
      "classic",
      {
        docs: {
          sidebarPath: "./sidebars.ts",
          editUrl:
            'https://github.com/3ddelano/epic-online-services-godot/tree/main/docs',
        },
        theme: {
          customCss: "./src/css/custom.css",
        },
      } satisfies Preset.Options,
    ],
  ],

  themeConfig: {
    colorMode: {
      defaultMode: "dark",
      disableSwitch: true,
      respectPrefersColorScheme: false,
    },
    image: "img/og-image.jpg",
    navbar: {
      title: "Epic Online Services Godot",
      logo: {
        alt: "EOSG Logo",
        src: "img/eosg-logo.svg",
      },
      items: [
        {
          type: "docSidebar",
          sidebarId: "docsSidebar",
          position: "left",
          label: "Docs",
        },
        {
          to: "showcase",
          position: "left",
          label: "Showcase",
        },
        {
          href: "https://github.com/3ddelano/epic-online-services-godot",
          label: "GitHub",
          position: "right",
        },
      ],
    },
    footer: {
      style: "dark",
      links: [
        {
          title: "Docs",
          items: [
            {
              label: "Docs",
              to: "/docs/introduction",
            },
          ],
        },
        {
          title: "Community",
          items: [
            {
              label: "Discord",
              href: "https://discord.gg/FZY9TqW",
            }
          ],
        },
        {
          title: "More",
          items: [
            {
              label: "GitHub",
              href: "https://github.com/facebook/docusaurus",
            },
            {
              label: "Sponsor",
              href: "https://github.com/sponsors/3ddelano",
            },
          ],
        },
      ],
      copyright: `Copyright © ${new Date().getFullYear()} Delano Lourenco`,
    },
    prism: {
      theme: prismThemes.github,
      darkTheme: prismThemes.dracula,
      additionalLanguages: ["gdscript", "gradle", "java"]
    },
  } satisfies Preset.ThemeConfig,

  plugins: [["./src/plugins/tailwind-config.ts", {}]],
};

export default config;
