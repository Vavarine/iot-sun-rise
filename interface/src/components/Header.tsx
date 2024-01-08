import { useLocation } from "preact-iso";

export function Header() {
  const { url } = useLocation();

  return (
    <header>
      <div className="navbar bg-base-100">
        <div className="flex-1">
          <a className="btn btn-ghost text-xl">Arduino SunRise</a>
        </div>
        <nav className="flex-none">
          <ul class="menu menu-horizontal gap-1 px-1">
            <li>
              <a href="/">RC</a>
            </li>
            <li>
              <a href="/light-alarm">Light Alarm</a>
            </li>
          </ul>
        </nav>
      </div>
    </header>
  );
}
