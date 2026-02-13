import { HeroSection } from "../components/HeroSection";
import { MainSystemsSection } from "../components/MainSystemsSection";
import { FeaturesSection } from "../components/FeaturesSection";
import { ScreenshotsSection, DemoSection } from "../components/MediaSections";
import { SupportSection } from "../components/SupportSection";
import { Footer, NavBar } from "../components/Layout";

export default function App() {
  return (
    <div className="min-h-screen bg-[#030712] text-gray-200 antialiased font-[Outfit,sans-serif]">
      <NavBar />
      <main className="pt-16">
        <HeroSection />
        <MainSystemsSection />
        <FeaturesSection />
        <ScreenshotsSection />
        <DemoSection />
        <SupportSection />
      </main>
      <Footer />
    </div>
  );
}
