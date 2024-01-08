import preactLogo from "../../assets/preact.svg";

interface IconProps {
  icon: string;
  size?: number;
}

export default function Icon({ icon, size }: IconProps) {
  return <img src={icon} height={size} width={size} />;
}
