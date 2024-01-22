import { useLocation } from "preact-iso";

export function Footer() {
  const { url } = useLocation();

  const menuItems = [
    {
      name: "Remote Control",
      url: "/",
    },
    {
      name: "Light Alarm",
      url: "/light-alarm",
    },
  ];

  return (
    <ul className="menu menu-horizontal bg-base-200 rounded-box w-max fixed bottom-4 left-1/2 -translate-x-1/2">
      {menuItems.map((item) => (
        <li>
          <a href={item.url} className={url === item.url ? "active" : ""}>
            {item.name}
          </a>
        </li>
      ))}
    </ul>
  );
}
