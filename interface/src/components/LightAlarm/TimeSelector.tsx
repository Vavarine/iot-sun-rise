import { useEffect, useRef } from "preact/hooks";
import { VerticalCarousel } from "./VerticalCarousel";

export function TimeSelector() {
  const container = useRef<HTMLDivElement>(null);

  return (
    <div class="flex items-center" ref={container}>
      <VerticalCarousel>
        {[...Array(24)].map((_, i) => {
          return (
            <div
              class="carousel-item opacity-20 data-[visible=true]:opacity-100 transition-opacity"
              data-value={i}
            >
              <div class="flex justify-center items-center h-full">
                <span class="text-5xl font-mono">{i.toString().padStart(2, "0")}</span>
              </div>
            </div>
          );
        })}
      </VerticalCarousel>
      <span class="text-5xl font-mono">:</span>
      <VerticalCarousel>
        {[...Array(60)].map((_, i) => {
          return (
            <div
              class="carousel-item opacity-20 data-[visible=true]:opacity-100 transition-opacity"
              data-value={i}
            >
              <div class="flex justify-center items-center h-full">
                <span class="text-5xl font-mono">{i.toString().padStart(2, "0")}</span>
              </div>
            </div>
          );
        })}
      </VerticalCarousel>
    </div>
  );
}
