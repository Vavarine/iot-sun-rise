import { useState } from "preact/hooks";
import { AlarmTableRow } from "@/components/LightAlarm/AlarmTableRow";
import { Alarm } from "@/types";

export function LightAlarm() {
  const [alarms, setAlarms] = useState<Alarm[]>([
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
  ]);

  return (
    <div class="container py-8 flex flex-col justify-center items-center">
      <h1>Light Alarm</h1>
      <div className="overflow-x-auto">
        <table className="table">
          <tbody>
            {alarms.map((alarm) => {
              return <AlarmTableRow {...alarm} />;
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
