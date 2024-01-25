import { PropsWithChildren } from "preact/compat";
import { MutableRef, useEffect, useRef } from "preact/hooks";

interface VerticalCarouselProps extends PropsWithChildren {}

const isFullyVisibleOnYAxis = (item: DOMRect, container: DOMRect) => {
  return item.top >= container.top && item.bottom <= container.bottom;
};

export function VerticalCarousel({ children }: VerticalCarouselProps) {
  const ref = useRef<HTMLDivElement>(null);

  const onScroll = () => {
    const items = ref?.current.querySelectorAll(".carousel-item");

    if (!items) {
      return;
    }

    const container = ref?.current.getBoundingClientRect();

    items.forEach((item) => {
      const rect = item.getBoundingClientRect();

      const isVisible = isFullyVisibleOnYAxis(rect, container);

      item.setAttribute("data-visible", isVisible.toString());
    });
  };

  useEffect(() => {
    ref.current.addEventListener("scroll", onScroll);
    return () => {
      ref.current.removeEventListener("scroll", onScroll);
    };
  }, [ref.current]);

  return (
    <div ref={ref} class="h-32 space-y-4 carousel carousel-center carousel-vertical rounded-box">
      {children}
    </div>
  );
}
