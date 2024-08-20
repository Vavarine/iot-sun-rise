import { useEffect, useState } from "preact/hooks";
import plusIcon from "@assets/plus.svg?raw";
import { Alarm } from "@/types";
import { AlarmCard } from "@components/LightAlarm/AlarmCard";
import Icon from "@/components/Icon";
import { useToast } from "@/hooks/toast";
import { useAuth } from "@/hooks/auth";

export function LightAlarmPage() {
  const [loading, setLoading] = useState(false);
  const [alarms, setAlarms] = useState<Alarm[]>([]);
  const { user } = useAuth();
  const { addToast } = useToast();

  const handleAdd = () => {
    if (!user) {
			addToast({
				description: 'This action requires authentication',
				type: 'error'
			})
			return
		}

    setAlarms([
      {
        id: crypto.randomUUID(),
        days: {
          sunday: false,
          monday: false,
          tuesday: false,
          wednesday: false,
          thursday: false,
          friday: false,
          saturday: false,
        },
        enabled: true,
        time: {
          hour: new Date().getHours(),
          minute: new Date().getMinutes(),
        },
      },
      ...alarms,
    ]);
  };

  const handleRemove = (id: string) => {
    if (!user) {
			addToast({
				description: 'This action requires authentication',
				type: 'error'
			})
			return
		}

    const updatedAlarms = alarms.filter((alarm) => alarm.id !== id);
    setAlarms(alarms.filter((alarm) => alarm.id !== id));
    saveAlarms(updatedAlarms);
  };

  const handleEdit = (alarm: Alarm) => {
    setAlarms(alarms.map((a) => (a.id === alarm.id ? alarm : a)));
  };

  const fetchAlarms = async () => {
    setLoading(true);
    const response = await fetch("/api/alarms");
    try {
      const data = await response.json();
      setAlarms(data.alarms ?? []);

    } catch (error) {
      console.error(error);
      setAlarms([]);
    }

    setLoading(false);
  };

  const handleSaveAlarms = async () => {
    saveAlarms();
  };

  const saveAlarms = async (updatedAlarms?: Alarm[]) => {
    if (!user) {
			addToast({
				description: 'This action requires authentication save',
				type: 'error'
			})
			return
		}

    setLoading(true);
    try {
      await fetch("/api/alarms", {
        method: "POST",
        body: JSON.stringify({ alarms: updatedAlarms ?? alarms }),
      });

      fetchAlarms();
    } finally {
      setLoading(false);
    }
  };

  useState(() => {
    fetchAlarms();
  });

  return (
    <div class="container max-w-sm px-4 py-8 flex flex-col justify-center items-center">
      <div class="w-full px-6 flex justify-between items-center mb-8">
        <h1 class="text-2xl">Light Alarm</h1>
        <button class="btn btn-square btn-ghost" onClick={handleAdd}>
          <Icon icon={plusIcon} />
        </button>
      </div>
      {loading ? (
        <div class="flex justify-center">
          <span class="loading loading-spinner loading-md" />
        </div>
      ) : (
        <>
          <div class="w-full">
            {alarms.map((alarm) => {
              return (
                <AlarmCard
                  {...alarm}
                  onSave={handleSaveAlarms}
                  onRemove={() => handleRemove(alarm.id)}
                  onEdit={handleEdit}
                />
              );
            })}
          </div>
          <div class="mt-8 px-6 w-full flex flex-col gap-4">
            <button
              class="btn w-full btn-primary btn-outline px-8"
              onClick={() => handleSaveAlarms()}
            >
              Save
            </button>
          </div>
        </>
      )}
    </div>
  );
}
