import { Alarm } from "@/types";
import { TimeSelector } from "./TimeSelector";

interface EditAlarmModalProps {
  modalId: string;
  alarm: Alarm;
}

export function EditAlarmModal({
  modalId,
  alarm: { days, enabled, time },
}: EditAlarmModalProps) {
  return (
    <dialog id={modalId} className="modal modal-bottom sm:modal-middle">
      <div className="modal-box">
        <form method="dialog">
          <button className="btn btn-sm btn-circle btn-ghost absolute right-2 top-2">
            ✕
          </button>
        </form>
        <div class="flex items-center flex-col gap-6">
          <h3 class="text-xl mb-2">Alarm</h3>
          <TimeSelector
            hour={new Date().getHours()}
            minute={new Date().getMinutes()}
          />
          <div class="flex gap-2 mt-4">
            {Object.keys(days).map((day) => {
              const isActive = days[day as keyof typeof days];

              return (
                <button
                  class={`btn btn-circle btn-sm btn-outline ${
                    isActive ? "btn-active" : ""
                  }`}
                >
                  {day.slice(0, 1)}
                </button>
              );
            })}
          </div>
          <button class="btn btn-outline btn-error w-full">Remove</button>
        </div>
      </div>
      <form method="dialog" className="modal-backdrop">
        <button>close</button>
      </form>
    </dialog>
  );
}
