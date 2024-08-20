import { useLocation } from "preact-iso";
import sunRiseIcon from "@assets/sunrise.svg?raw";
import moreHorizontalIcon from "@assets/more-horizontal.svg?raw";
import Icon from "./Icon";
import { useAuth } from "@/hooks/auth";
import { Link } from "./Link";

export function Header() {
  const { user, signOut } = useAuth();

  return (
    <header class="">
      <div class="navbar rounded-b-xl shadow-xl overflow-visible">
        <div class="flex-1 ">
          <a href="/" class="btn btn-ghost text-xl gap-2">
            <Icon icon={sunRiseIcon} size={24} />
            SunRise
          </a>
        </div>
        <div class="flex gap-4">
          {!user?.username ? (
            <Link href="/login" class="btn">
              Log in
            </Link>
          ) : (
            <>
              <span>Hello, {user.username}!</span>
              <div class="flex-none dropdown dropdown-end">
                <div
                  class="btn btn-square btn-ghost"
                  role="button"
                  tabIndex={0}
                >
                  <Icon icon={moreHorizontalIcon} />
                </div>
                <ul class="menu menu-sm dropdown-content shadow rounded-box mt-4 bg-base-200">
                  <li>
                    <a href="/configs">Configs</a>
                  </li>
                  {user && (
                    <>
                      <li>
                        <button
                          type="button"
                          class="whitespace-nowrap"
                          onClick={() =>
                            confirm("Are you sure?") &&
                            fetch("/api/restart", {
                              method: "POST",
                            }).then(
                              (res) =>
                                res.ok &&
                                setTimeout(() => location.reload(), 2000)
                            )
                          }
                        >
                          Restart ESP
                        </button>
                      </li>
                      {!user.noAuth && (
                        <li>
                          <button type="button" onClick={signOut}>
                            Logout
                          </button>
                        </li>
                      )}
                    </>
                  )}
                </ul>
              </div>
            </>
          )}
        </div>
      </div>
    </header>
  );
}
