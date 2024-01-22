interface IconProps {
  icon: string;
  size?: number;
}

export default function Icon({ icon, size }: IconProps) {
  return (
    <div
      dangerouslySetInnerHTML={{
        __html: icon,
      }}
    />
  );
}
