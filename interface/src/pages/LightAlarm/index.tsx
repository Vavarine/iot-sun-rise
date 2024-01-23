import { EditAlarmModal } from "../../components/EditAlarmModal";

export function LightAlarm() {
  const alarms = [
    {
      time: {
        hour: 7,
        minute: 30,
      },
      days: {
        sunday: false,
        monday: true,
        tuesday: true,
        wednesday: true,
        thursday: true,
        friday: true,
        saturday: false,
      },
      enabled: true,
    },
    {
      time: {
        hour: 8,
        minute: 30,
      },
      days: {
        sunday: false,
        monday: true,
        tuesday: true,
        wednesday: false,
        thursday: true,
        friday: true,
        saturday: false,
      },
      enabled: false,
    },
    {
      time: {
        hour: 7,
        minute: 30,
      },
      days: {
        sunday: false,
        monday: true,
        tuesday: true,
        wednesday: true,
        thursday: true,
        friday: true,
        saturday: false,
      },
      enabled: false,
    },
    {
      time: {
        hour: 7,
        minute: 30,
      },
      days: {
        sunday: false,
        monday: true,
        tuesday: true,
        wednesday: true,
        thursday: true,
        friday: true,
        saturday: false,
      },
      enabled: true,
    },
  ];

  return (
    <div class="container py-8 flex flex-col justify-center items-center">
      <h1>Light Alarm</h1>
      <div className="overflow-x-auto">
        <table className="table">
          <tbody>
            {alarms.map(({ days, enabled, time }, i) => {
              return (
                <>
                  <tr
                    class="hover:bg-base-200 font-mono"
                    onClick={() =>
                      (document.getElementById(`modalId-${i}`) as HTMLDialogElement).showModal()
                    }
                  >
                    <th>
                      <span class="text-2xl">{time.hour}</span>h
                      <span class="text-2xl">{time.minute}</span>m
                    </th>
                    <td>
                      <div class="flex align-center">
                        {Object.keys(days).map((day) => {
                          const isActive = days[day as keyof typeof days];

                          return (
                            <div class="font-mono indicator h-5 w-5 flex items-center justify-center">
                              <span class={isActive ? "opacity-100" : "opacity-20"}>
                                {day.slice(0, 1)}
                              </span>
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
                  </tr>
                  <EditAlarmModal modalId={`modalId-${i}`} />
                </>
              );
            })}
          </tbody>
        </table>
        <div class="flex justify-center mt-6">
          <button class="btn btn-wide">Save</button>
        </div>
      </div>
    </div>
  );
}
