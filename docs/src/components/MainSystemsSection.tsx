const MAIN_SYSTEMS = [
  {
    title: "High Level EOS",
    tag: "Recommended",
    description:
      "Provides easy-to-use methods and signals to interact with EOS. Ideal for most cases and rapid development.",
    link: "docs/introduction",
    icon: (
      <svg
        className="w-6 h-6 text-blue-400"
        fill="none"
        stroke="currentColor"
        viewBox="0 0 24 24"
      >
        <path
          strokeLinecap="round"
          strokeLinejoin="round"
          strokeWidth="2"
          d="M13 10V3L4 14h7v7l9-11h-7z"
        ></path>
      </svg>
    ),
    color: "blue",
  },
  {
    title: "GDExtension EOS",
    tag: "Advanced",
    description:
      "Offers advanced EOS usage with direct access to the C SDK via GDExtension. Suitable for complex needs.",
    extra: (
      <>
        Provides{" "}
        <code className="bg-purple-500/10 px-2 py-0.5 rounded text-purple-300 font-mono text-xs">
          EOS
        </code>{" "}
        and{" "}
        <code className="bg-purple-500/10 px-2 py-0.5 rounded text-purple-300 font-mono text-xs">
          IEOS
        </code>{" "}
        classes.
      </>
    ),
    icon: (
      <svg
        className="w-6 h-6 text-purple-400"
        fill="none"
        stroke="currentColor"
        viewBox="0 0 24 24"
      >
        <path
          strokeLinecap="round"
          strokeLinejoin="round"
          strokeWidth="2"
          d="M10 20l4-16m4 4l4 4-4 4M6 16l-4-4 4-4"
        ></path>
      </svg>
    ),
    color: "purple",
  },
];

export function MainSystemsSection() {
  return (
    <section id="main-systems" className="py-20 sm:py-24 bg-[#030712] relative">
      <div className="container mx-auto px-6">
        <h2 className="text-3xl font-bold mb-4 text-center text-white tracking-tight">
          Two Main Systems
        </h2>
        <p className="text-gray-400 text-center mb-14 max-w-2xl mx-auto font-light">
          Choose the approach that fits your project's complexity.
        </p>
        <div className="grid md:grid-cols-2 gap-8 max-w-5xl mx-auto">
          {MAIN_SYSTEMS.map((system, i) => (
            <div
              key={i}
              className={`glass rounded-2xl p-8 transition-all duration-300 hover:-translate-y-1 hover:shadow-xl hover:shadow-${system.color}-500/5 border-${system.color}-500/20 hover:border-${system.color}-500/40`}
              style={{
                borderColor: `rgba(${
                  system.color === "blue" ? "59,130,246" : "168,85,247"
                },0.2)`,
              }}
            >
              <div
                className={`mb-5 w-12 h-12 rounded-xl bg-${system.color}-500/10 flex items-center justify-center`}
              >
                {system.icon}
              </div>
              <h3 className="text-xl font-bold mb-3 text-white">
                {system.title}
              </h3>
              <span
                className={`inline-block mb-4 text-xs font-bold uppercase tracking-widest text-${system.color}-400 bg-${system.color}-500/10 px-3 py-1 rounded-full`}
              >
                {system.tag}
              </span>
              <p className="text-gray-400 mb-4 font-light leading-relaxed">
                {system.description}
              </p>
              {system.link && (
                <a
                  href={system.link}
                  className={`inline-flex items-center text-${system.color}-400 hover:text-${system.color}-300 font-semibold text-sm no-underline transition-colors`}
                >
                  Learn more
                  <svg
                    className="w-4 h-4 ml-1"
                    fill="none"
                    stroke="currentColor"
                    viewBox="0 0 24 24"
                  >
                    <path
                      strokeLinecap="round"
                      strokeLinejoin="round"
                      strokeWidth="2"
                      d="M9 5l7 7-7 7"
                    ></path>
                  </svg>
                </a>
              )}
              {system.extra && (
                <p className="text-gray-500 text-sm">{system.extra}</p>
              )}
            </div>
          ))}
        </div>
      </div>
    </section>
  );
}
