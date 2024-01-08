import { render } from "preact";
import { LocationProvider, Router, Route } from "preact-iso";

import { Header } from "./components/Header.jsx";
import { RemoteControl } from "./pages/RemoteControl/index.js";
import { NotFound } from "./pages/_404.jsx";
import "./index.css";
import { LightAlarm } from "./pages/LightAlarm/index.js";

export function App() {
  return (
    <LocationProvider>
      <Header />
      <main>
        <Router>
          <Route path="/" component={RemoteControl} />
          <Route path="/light-alarm" component={LightAlarm} />
          <Route default component={NotFound} />
        </Router>
      </main>
    </LocationProvider>
  );
}

render(<App />, document.getElementById("app"));
