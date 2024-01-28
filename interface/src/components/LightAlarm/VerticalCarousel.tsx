import { PropsWithChildren } from "preact/compat";
import { useEffect, useRef, useState } from "preact/hooks";

interface VerticalSliderProps extends PropsWithChildren {
  defaultIndex?: number;
  onChange?: (index: number) => void;
}

const isFullyVisibleOnYAxis = (item: DOMRect, container: DOMRect) => {
  return item.top >= container.top && item.bottom <= container.bottom;
};

const isHTMLElement = (element: unknown): element is HTMLElement => {
  return element instanceof HTMLElement;
};

function throttle(callbackFn, limit) {
  let wait = false;
  return function () {
    if (!wait) {
      callbackFn.call();
      wait = true;
      setTimeout(function () {
        wait = false;
      }, limit);
    }
  };
}

export function VerticalCarousel({
  children,
  defaultIndex: activeIndex,
  onChange,
}: VerticalSliderProps) {
  const ref = useRef<HTMLDivElement>(null);
  const [carouselItems, setCarouselItems] = useState<Element[]>([]);

  const onScroll = () => {
    const container = ref?.current;

    if (!container) return;

    const containerRect = ref?.current.getBoundingClientRect();
    const currentItems = ref?.current.querySelectorAll(".carousel-item");

    currentItems.forEach((item) => {
      const rect = item.getBoundingClientRect();
      const isVisible = isFullyVisibleOnYAxis(rect, containerRect);

      item.setAttribute("data-visible", isVisible.toString());
      if (isVisible) onChange(Number(item.getAttribute("data-index")));
    });

    const currentItemHeight = currentItems[0].getBoundingClientRect().height;

    if (
      container.scrollTop >
      container.scrollHeight - (currentItemHeight * carouselItems.length) / 4
    ) {
      carouselItems.forEach((item) => {
        container.appendChild(item.cloneNode(true));
      });
    }

    if (container.scrollTop < (currentItemHeight * carouselItems.length) / 4) {
      [...carouselItems].reverse().forEach((item) => {
        container.prepend(item.cloneNode(true));
      });
    }
  };

  const scrollTo = (index: number) => {
    const items = ref?.current.querySelectorAll(".carousel-item");

    if (!items) {
      return;
    }

    const item = items[index];

    if (!isHTMLElement(item)) {
      return;
    }

    const itemRect = item.getBoundingClientRect();
    const container = ref?.current;
    const containerRect = container.getBoundingClientRect();

    const isVisible = isFullyVisibleOnYAxis(itemRect, containerRect);

    if (!isVisible) {
      ref.current.scrollTo({
        top: item.offsetTop - ref?.current.offsetTop - containerRect.height / 2,
        behavior: "smooth",
      });
    }
  };

  useEffect(() => {
    if (!ref?.current) return;

    onScroll();
    ref.current.addEventListener("scroll", throttle(onScroll, 50));

    setCarouselItems([...ref.current.querySelectorAll(".carousel-item")]);

    return () => {
      ref.current.removeEventListener("scroll", throttle(onScroll, 50));
    };
  }, [ref.current]);

  useEffect(() => {
    if (activeIndex !== undefined) {
      scrollTo(activeIndex);
    }
  }, [activeIndex]);

  return (
    <div
      ref={ref}
      class="h-52 space-y-4 p-4 py-5 carousel carousel-center carousel-vertical rounded-box"
    >
      {children}
    </div>
  );
}

export function VerticalCarouselItem({
  children,
  index,
}: PropsWithChildren<{ index: number }>) {
  return (
    <div
      class="carousel-item opacity-20 data-[visible=true]:opacity-100 transition-opacity"
      data-index={index}
    >
      <div class="flex justify-center items-center h-full">{children}</div>
    </div>
  );
}
