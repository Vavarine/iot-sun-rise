// import "preact/debug";
import { render } from "preact";
import { LocationProvider, Router, Route } from "preact-iso";

import { Header } from "./components/Header.jsx";
import { RemoteControlPage } from "./pages/RemoteControl/index.js";
import { NotFoundPage } from "./pages/_404.jsx";
import { LightAlarmPage } from "./pages/LightAlarm/index.js";
import { Footer } from "./components/Footer.js";

import "./index.css";
import {ConfigPage} from "./pages/Config/index.js";
import { ToastProvider } from "./hooks/toast.js";
import { AuthProvider } from "./hooks/auth.js";
import { LoginPage } from "./pages/Login/index.js";

export function App() {
  return (
    <LocationProvider>
      <ToastProvider>
        <AuthProvider>
          <Header />
          <main class="flex flex-col items-center mb-auto">
            <Router>
              <Route path="/" component={RemoteControlPage} />
              <Route path="/light-alarm" component={LightAlarmPage} />
              <Route path="/configs" component={ConfigPage} />
              <Route path="/login" component={LoginPage} />
              <Route default component={NotFoundPage} />
            </Router>
          </main>
          <Footer />
        </AuthProvider>
      </ToastProvider>
    </LocationProvider>
  );
}

render(<App />, document.getElementById("app"));
