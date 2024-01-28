import { useState } from "preact/hooks";
import plusIcon from "@assets/plus.svg?raw";
import { Alarm } from "@/types";
import { AlarmCard } from "@components/LightAlarm/AlarmCard";
import Icon from "@/components/Icon";

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
    <div class="container max-w-sm px-4 py-8 flex flex-col justify-center items-center">
      <div class="w-full px-6 flex justify-between items-center mb-8">
        <h1 class="text-2xl">Light Alarm</h1>
        <button class="btn btn-square btn-ghost">
          <Icon icon={plusIcon} />
        </button>
      </div>

      <div class="w-full">
        {alarms.map((alarm) => {
          return <AlarmCard {...alarm} />;
        })}
      </div>
      <div class="mt-8 px-6 w-full flex flex-col gap-4">
        <button class="btn w-full btn-primary btn-outline px-8">Save</button>
      </div>
    </div>
  );
}
