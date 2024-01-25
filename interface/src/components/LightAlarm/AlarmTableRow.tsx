import { useId } from "preact/hooks";
import { EditAlarmModal } from "./EditAlarmModal";
import { Alarm } from "@/types";

interface AlarmProps extends Alarm {}

export function AlarmTableRow({ time, days, enabled }: AlarmProps) {
  const id = useId();

  return (
    <tr
      class="hover:bg-base-200 font-mono"
      onClick={() => (document.getElementById(id) as HTMLDialogElement).showModal()}
    >
      <th>
        <span class="text-2xl">{time.hour}</span>h<span class="text-2xl">{time.minute}</span>m
      </th>
      <td>
        <div class="flex align-center">
          {Object.keys(days).map((day) => {
            const isActive = days[day as keyof typeof days];

            return (
              <div class="font-mono indicator h-5 w-5 flex items-center justify-center">
                <span class={isActive ? "opacity-100" : "opacity-20"}>{day.slice(0, 1)}</span>
                {isActive && (
                  <span className="indicator-item indicator-center badge p-0 w-1 h-1 badge-primary" />
                )}
              </div>
            );
          })}
        </div>
      </td>
      <td>
        <input
          type="checkbox"
          checked={enabled}
          class="checkbox checkbox-primary"
          onClick={(e) => e.stopPropagation()}
        />
      </td>
      <EditAlarmModal modalId={id} />
    </tr>
  );
}
