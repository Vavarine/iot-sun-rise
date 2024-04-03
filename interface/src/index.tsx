// import "preact/debug";
import { render } from "preact";
import { LocationProvider, Router, Route } from "preact-iso";

import { Header } from "./components/Header.jsx";
import { RemoteControl } from "./pages/RemoteControl/index.js";
import { NotFound } from "./pages/_404.jsx";
import { LightAlarm } from "./pages/LightAlarm/index.js";
import { Footer } from "./components/Footer.js";

import "./index.css";
import Config from "./pages/Config/index.js";

export function App() {
  return (
    <LocationProvider>
      <Header />
      <main class="flex flex-col items-center mb-auto">
        <Router>
          <Route path="/" component={RemoteControl} />
          <Route path="/light-alarm" component={LightAlarm} />
          <Route path="/configs" component={Config} />
          <Route default component={NotFound} />
        </Router>
      </main>
      <Footer />
    </LocationProvider>
  );
}

render(<App />, document.getElementById("app"));
